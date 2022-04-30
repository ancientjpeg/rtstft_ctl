/*
  ==============================================================================

    RT_SettingsDisplay.h
    Created: 29 Apr 2022 7:18:08pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../Utility/RT_Component.h"
#include <JuceHeader.h>

//==============================================================================
class RT_SettingsDisplay : public RT_Component {
public:
  RT_SettingsDisplay(RT_ProcessorInterface *inInterface);
  ~RT_SettingsDisplay() override;

  void paint(juce::Graphics &) override;
  void resized() override;

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_SettingsDisplay)
};
