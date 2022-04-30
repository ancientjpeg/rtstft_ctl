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
    : RT_Component(mInterface)
{
}

RT_SettingsDisplay::~RT_SettingsDisplay() {}

void RT_SettingsDisplay::paint(juce::Graphics &g)
{
  g.fillAll(
      getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
  g.setColour(juce::Colours::grey);
  g.drawRect(getLocalBounds(), 1);
  g.setColour(juce::Colours::white);
  g.setFont(14.0f);
  g.drawText("RT_SettingsDisplay", getLocalBounds(),
             juce::Justification::centred, true);
}

void RT_SettingsDisplay::resized()
{
  // This method is where you should set the bounds of any child
  // components that your component contains..
}
