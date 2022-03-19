/*
  ==============================================================================

    RT_LookAndFeelManager.h
    Created: 18 Mar 2022 2:13:06pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class RT_LookAndFeelManager {
  std::vector<std::unique_ptr<juce::LookAndFeel_V4::ColourScheme>> mSkins;
  juce::LookAndFeel_V4::ColourScheme                              *mCurrentSkin;
  std::unique_ptr<juce::LookAndFeel_V4>                            mLookAndFeel;

public:
  RT_LookAndFeelManager();
  juce::LookAndFeel_V4::ColourScheme *getCurrentSkin();
  juce::LookAndFeel_V4               *getCurrentLookAndFeel();
};
