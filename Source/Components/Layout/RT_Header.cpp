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
    : RT_BorderedComponent(inInterface, inBorderSize)
{
  // In your constructor, you should add any child components, and
  // initialise any special settings that your component needs.
  addAndMakeVisible(mPluginTitle);
  mPluginTitle.setText("rtstft_ctl",
                       juce::NotificationType::dontSendNotification);
  mPluginTitle.setFont(juce::Font(24, juce::Font::bold));
  mPluginTitle.setEditable(false);
  mPluginSubtitle.setText("sound_ctl",
                          juce::NotificationType::dontSendNotification);
  mPluginSubtitle.setEditable(false);
  addAndMakeVisible(mPluginSubtitle);
  addAndMakeVisible(mSettingsMenuButton);
}

RT_Header::~RT_Header() {}

void RT_Header::paintInBorder(juce::Graphics &g)
{
  /* This demo code just fills the component's background and
     draws some placeholder text to get you started.

     You should replace everything in this method with your own
     drawing code..
  */
  using enum juce::LookAndFeel_V4::ColourScheme::UIColour;

  g.setColour((static_cast<juce::LookAndFeel_V4 &>(getLookAndFeel()))
                  .getCurrentColourScheme()
                  .getUIColour(windowBackground)); // clear the background
  g.fillRect(getBoundsAdj());
}

void RT_Header::resized()
{
  auto bounds = getBoundsAdj();
  mPluginTitle.setBounds(bounds.removeFromLeft(0.2 * getWidthAdj()));
  mPluginSubtitle.setBounds(bounds.removeFromLeft(0.7 * getWidthAdj()));
  mSettingsMenuButton.setBounds(bounds);
  // This method is where you should set the bounds of any child
  // components that your component contains..
}
