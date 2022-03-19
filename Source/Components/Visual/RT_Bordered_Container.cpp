/*
  ==============================================================================

    RT_Bordered_Container.cpp
    Created: 18 Mar 2022 2:16:34pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_Bordered_Container.h"
#include "../../Managers/GUIManagers/RT_StyleManager.h"

RT_BorderedContainer::RT_BorderedContainer(int inBorderSize)
    : mBorderSize(inBorderSize)
{
}

void RT_BorderedContainer::paint(juce::Graphics &g)
{
  using juce::LookAndFeel_V4::ColourScheme::UIColour::defaultFill;
  using juce::LookAndFeel_V4::ColourScheme::UIColour::outline;
  juce::LookAndFeel_V4::ColourScheme skin
      = RT_getLookAndFeel().getCurrentColourScheme();
  g.fillAll(skin.getUIColour(defaultFill));
  g.setColour(skin.getUIColour(outline));
  g.drawRect(0, 0, getWidth(), mBorderSize);
  g.drawRect(0, 0, mBorderSize, getHeight());
  g.drawRect(0, getWidth() - mBorderSize, getWidth(), mBorderSize);
  g.drawRect(0, getHeight() - mBorderSize, mBorderSize, getHeight());
}

void RT_BorderedContainer::resized() {}