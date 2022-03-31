/*
  ==============================================================================

    RT_BorderedComponent.h
    Created: 30 Mar 2022 3:57:16pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../../Managers/Interface/RT_ProcessorInterface.h"
#include "RT_Component.h"
#include <JuceHeader.h>

//==============================================================================
/*
 */
class RT_BorderedComponent : public RT_Component {

public:
  RT_BorderedComponent(RT_ProcessorInterface *inInterface, int inBorderSize,
                       juce::Colour inBorderColour = juce::Colours::black)
      : RT_Component(inInterface), mBorderSize(inBorderSize),
        mBorderColour(inBorderColour)
  {
  }
  virtual ~RT_BorderedComponent() override = default;

  void paint(juce::Graphics &g) final
  {
    using enum juce::LookAndFeel_V4::ColourScheme::UIColour;
    auto scheme = getLookAndFeel_V4().getCurrentColourScheme();
    g.fillAll(mBorderColour);
    g.setColour(scheme.getUIColour(windowBackground));
    g.fillRect(getBoundsAdj());
    paintInBorder(g);
  }
  virtual void         paintInBorder(juce::Graphics &g) = 0;

  juce::Rectangle<int> getBoundsAdj()
  {
    return getLocalBounds().reduced(mBorderSize);
  }
  int getWidthAdj() { return getBoundsAdj().getWidth(); }
  int getHeightAdj() { return getBoundsAdj().getHeight(); }

protected:
  int mBorderSize;

private:
  juce::Colour mBorderColour;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_BorderedComponent)
};
