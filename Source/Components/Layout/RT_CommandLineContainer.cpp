/*
  ==============================================================================

    RT_CommandLineContainer.cpp
    Created: 19 Mar 2022 1:43:10pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_CommandLineContainer.h"
#include <JuceHeader.h>

//==============================================================================
RT_CommandLineContainer::RT_CommandLineContainer(
    RT_ProcessorInterface *mInterface)
{
  // In your constructor, you should add any child components, and
  // initialise any special settings that your component needs.
}

RT_CommandLineContainer::~RT_CommandLineContainer() {}

void RT_CommandLineContainer::paint(juce::Graphics &g)
{
  /* This demo code just fills the component's background and
     draws some placeholder text to get you started.

     You should replace everything in this method with your own
     drawing code..
  */
  using enum juce::LookAndFeel_V4::ColourScheme::UIColour;

  g.fillAll(RT_getLookAndFeel().getCurrentColourScheme().getUIColour(
      windowBackground)); // clear the background

  g.setColour(juce::Colours::green);
  g.drawRect(getLocalBounds(), 1); // draw an outline around the component

  g.setColour(juce::Colours::white);
  g.setFont(14.0f);
  g.drawText("RT_CommandLineContainer", getLocalBounds(),
             juce::Justification::centred, true); // draw some placeholder text
}

void RT_CommandLineContainer::resized()
{
  // This method is where you should set the bounds of any child
  // components that your component contains..
}
