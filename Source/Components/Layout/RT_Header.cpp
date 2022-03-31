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
  // This method is where you should set the bounds of any child
  // components that your component contains..
}
