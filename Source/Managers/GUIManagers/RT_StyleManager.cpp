/*
  ==============================================================================

    RT_LookAndFeelManager.cpp
    Created: 18 Mar 2022 2:13:06pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_StyleManager.h"

RT_StyleManager::RT_StyleManager()
{
  mSkins.push_back(std::make_unique<juce::LookAndFeel_V4::ColourScheme>(
      juce::Colours::white, juce::Colours::white, juce::Colours::white,
      juce::Colours::black, juce::Colours::black, juce::Colours::white,
      juce::Colours::blue, juce::Colours::blue));
  mCurrentSkin = mSkins[0].get();
  mLookAndFeel.reset(new juce::LookAndFeel_V4(*mCurrentSkin));
}

juce::LookAndFeel_V4::ColourScheme *RT_StyleManager::getCurrentSkin()
{
  return mCurrentSkin;
}