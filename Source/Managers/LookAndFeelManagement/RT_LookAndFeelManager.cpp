/*
  ==============================================================================

    RT_LookAndFeelManager.cpp
    Created: 18 Mar 2022 2:13:06pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_LookAndFeelManager.h"

RT_LookAndFeelManager::RT_LookAndFeelManager()
{
  mSkins.push_back(std::make_unique<juce::LookAndFeel_V4::ColourScheme>(
      juce::Colours::white, juce::Colours::white, juce::Colours::white,
      juce::Colours::black, juce::Colours::black, juce::Colours::white,
      juce::Colours::blue, juce::Colours::blue, juce::Colours::black));
  mCurrentSkin = mSkins[0].get();
  mLookAndFeel.reset(new juce::LookAndFeel_V4(*mCurrentSkin));
}

juce::LookAndFeel_V4::ColourScheme *RT_LookAndFeelManager::getCurrentSkin()
{
  return mCurrentSkin;
}

juce::LookAndFeel_V4 *RT_LookAndFeelManager::getCurrentLookAndFeel()
{
  return mLookAndFeel.get();
}