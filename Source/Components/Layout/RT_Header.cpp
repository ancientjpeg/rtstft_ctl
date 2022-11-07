/*
  ==============================================================================

    RT_Header.cpp
    Created: 19 Mar 2022 1:29:47pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_Header.h"
#include <JuceHeader.h>

//==============================================================================
RT_Header::RT_Header(RT_ProcessorInterface *inInterface, int inBorderSize)
    : RT_Component(inInterface), mSettingsMenuButton(mInterface)
{
  // In your constructor, you should add any child components, and
  // initialise any special settings that your component needs.
  addAndMakeVisible(mPluginTitle);
#ifdef JUCE_DEBUG
  juce::String header = "rtstft_ctl - DEBUG";
#else
  juce::String header = "rtstft_ctl";
#endif
  mPluginTitle.setText(header, juce::NotificationType::dontSendNotification);
  mPluginTitle.setFont(juce::Font(24, juce::Font::bold));
  mPluginTitle.setEditable(false);
  mPluginSubtitle.setText("sound_ctl",
                          juce::NotificationType::dontSendNotification);
  mPluginSubtitle.setEditable(false);
  addAndMakeVisible(mPluginSubtitle);
  addAndMakeVisible(mSettingsMenuButton);
}

RT_Header::~RT_Header()
{
}

void RT_Header::paint(juce::Graphics &g)
{
  using enum juce::LookAndFeel_V4::ColourScheme::UIColour;

  g.setColour((static_cast<juce::LookAndFeel_V4 &>(getLookAndFeel()))
                  .getCurrentColourScheme()
                  .getUIColour(windowBackground)); // clear the background
  g.fillRect(getLocalBounds());
}

void RT_Header::resized()
{
  auto bounds = getLocalBounds();
  mPluginTitle.setBounds(bounds.removeFromLeft(0.2 * getWidth()));
  mPluginSubtitle.setBounds(bounds.removeFromLeft(0.7 * getWidth()));
  mSettingsMenuButton.setBounds(bounds.withSizeKeepingCentre(20, 20));
}

void RT_Header::addSettingsMenuButtonListener(RT_GearButton::Listener *l)
{
  mSettingsMenuButton.addListener(l);
}