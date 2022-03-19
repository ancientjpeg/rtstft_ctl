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
      = std::make_unique<juce::LookAndFeel_V4>(DefaultColourScheme);
}

juce::LookAndFeel_V4 *
RT_LookAndFeel::Manager::getSingleLookAndFeel(ComponentType inComponentTypeID)
{
  return mLookAndFeelCollection[inComponentTypeID].get();
}
