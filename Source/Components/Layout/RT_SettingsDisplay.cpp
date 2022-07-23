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
    : RT_Component(inInterface), mPresetChooser(mInterface)
{
  addAndMakeVisible(mPresetChooser);
}

RT_SettingsDisplay::~RT_SettingsDisplay() {}

void RT_SettingsDisplay::paint(juce::Graphics &g) {}

void RT_SettingsDisplay::resized()
{
  auto bounds = getBounds();
  mPresetChooser.setBounds(bounds);
}
