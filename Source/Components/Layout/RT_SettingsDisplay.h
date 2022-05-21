/*
  ==============================================================================

    RT_SettingsDisplay.h
    Created: 29 Apr 2022 7:18:08pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../Interactive/RT_PresetChooser.h"
#include "../Utility/RT_BorderedComponent.h"
#include <JuceHeader.h>

//==============================================================================
class RT_SettingsDisplay : public RT_BorderedComponent {
public:
  RT_SettingsDisplay(RT_ProcessorInterface *inInterface);
  ~RT_SettingsDisplay() override;

  void paintInBorder(juce::Graphics &) override;
  void resized() override;

private:
  RT_PresetChooser mPresetChooser;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_SettingsDisplay)
};
