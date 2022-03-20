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
RT_Header::RT_Header()
{
  // In your constructor, you should add any child components, and
  // initialise any special settings that your component needs.
}

RT_Header::~RT_Header() {}

void RT_Header::paint(juce::Graphics &g)
{
  /* This demo code just fills the component's background and
     draws some placeholder text to get you started.

     You should replace everything in this method with your own
     drawing code..
  */
  using enum juce::LookAndFeel_V4::ColourScheme::UIColour;

  g.fillAll(((juce::LookAndFeel_V4)getLookAndFeel())
                .getCurrentColourScheme()
                .getUIColour(windowBackground)); // clear the background

  g.setColour(juce::Colours::red);
  g.drawRect(getLocalBounds(), 1); // draw an outline around the component

  g.setColour(juce::Colours::white);
  g.setFont(14.0f);
  g.drawText("RT_Header", getLocalBounds(), juce::Justification::centred,
             true); // draw some placeholder text
}

void RT_Header::resized()
{
  // This method is where you should set the bounds of any child
  // components that your component contains..
}
