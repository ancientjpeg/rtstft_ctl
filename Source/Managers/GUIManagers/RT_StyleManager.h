/*
  ==============================================================================

    RT_LookAndFeelManager.h
    Created: 18 Mar 2022 2:13:06pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class RT_StyleManager {
  std::vector<std::unique_ptr<juce::LookAndFeel_V4::ColourScheme>> mSkins;
  juce::LookAndFeel_V4::ColourScheme                              *mCurrentSkin;
  std::unique_ptr<juce::LookAndFeel_V4>                            mLookAndFeel;

public:
  RT_StyleManager();
  juce::LookAndFeel_V4::ColourScheme *getCurrentSkin();
};
