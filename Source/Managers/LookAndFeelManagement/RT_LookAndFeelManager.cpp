/*
  ==============================================================================

    RT_LookAndFeelManager.cpp
    Created: 18 Mar 2022 2:13:06pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_LookAndFeelManagement.h"

RT_LookAndFeel::Manager::Manager()
{
  mLookAndFeelCollection[PLUGIN_DEFAULT]
      = std::unique_ptr<juce::LookAndFeel_V4>(new Default);
}

juce::LookAndFeel_V4 *
RT_LookAndFeel::Manager::getSingleLookAndFeel(ComponentType inComponentTypeID)
{
  return mLookAndFeelCollection[inComponentTypeID].get();
}

juce::Colour RT_LookAndFeel::Manager::getUIColour(
    juce::LookAndFeel_V4::ColourScheme::UIColour inColorToGet)
{
  return DefaultColourScheme.getUIColour(inColorToGet); // change this
}