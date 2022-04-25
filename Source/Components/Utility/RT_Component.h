/*
  ==============================================================================

    RT_Component.h
    Created: 30 Mar 2022 4:28:51pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once
#include "../../Managers/Interface/RT_ProcessorInterface.h"
#include <JuceHeader.h>


using enum juce::LookAndFeel_V4::ColourScheme::UIColour;

class RT_Component : public juce::Component {

protected:
  RT_ProcessorInterface *mInterface;
  juce::LookAndFeel_V4  &getLookAndFeel_V4()
  {
    return static_cast<juce::LookAndFeel_V4 &>(getLookAndFeel());
  }

public:
  RT_Component(RT_ProcessorInterface *inInterface) : mInterface(inInterface) {}
  virtual ~RT_Component() override = default;

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_Component)
};
