/*
  ==============================================================================

    RT_SettingsDIsplay.h
    Created: 29 Apr 2022 7:18:08pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class RT_SettingsDIsplay  : public juce::Component
{
public:
    RT_SettingsDIsplay();
    ~RT_SettingsDIsplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RT_SettingsDIsplay)
};
