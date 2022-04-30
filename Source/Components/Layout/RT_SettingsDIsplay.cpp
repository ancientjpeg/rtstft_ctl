/*
  ==============================================================================

    RT_SettingsDisplay.cpp
    Created: 29 Apr 2022 7:18:08pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_SettingsDisplay.h"
#include <JuceHeader.h>

//==============================================================================
RT_SettingsDisplay::RT_SettingsDisplay(RT_ProcessorInterface *inInterface)
    : RT_BorderedComponent(mInterface)
{
}

RT_SettingsDisplay::~RT_SettingsDisplay() {}

void RT_SettingsDisplay::paintInBorder(juce::Graphics &g) {}

void RT_SettingsDisplay::resized()
{
  // This method is where you should set the bounds of any child
  // components that your component contains..
}
