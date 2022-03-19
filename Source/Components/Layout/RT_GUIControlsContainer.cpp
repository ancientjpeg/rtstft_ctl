/*
  ==============================================================================

    RT_GUIControlsContainer.cpp
    Created: 19 Mar 2022 1:16:07pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_GUIControlsContainer.h"
#include <JuceHeader.h>

//==============================================================================
RT_GUIControlsContainer::RT_GUIControlsContainer(
    RT_ProcessorInterface *inInterface)
{
  // In your constructor, you should add any child components, and
  // initialise any special settings that your component needs.
}

RT_GUIControlsContainer::~RT_GUIControlsContainer() {}

void RT_GUIControlsContainer::paint(juce::Graphics &g)
{
  /* This demo code just fills the component's background and
     draws some placeholder text to get you started.

     You should replace everything in this method with your own
     drawing code..
  */

  g.fillAll(getLookAndFeel().findColour(
      juce::ResizableWindow::backgroundColourId)); // clear the background

  g.setColour(juce::Colours::blue);
  g.drawRect(getLocalBounds(), 1); // draw an outline around the component

  g.setColour(juce::Colours::white);
  g.setFont(14.0f);
  g.drawText("RT_GUIControlsContainer", getLocalBounds(),
             juce::Justification::centred, true); // draw some placeholder text
}

void RT_GUIControlsContainer::resized()
{
  // This method is where you should set the bounds of any child
  // components that your component contains..
}
