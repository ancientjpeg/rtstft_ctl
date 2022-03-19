/*
  ==============================================================================

    RT_FFTDisplayContainer.cpp
    Created: 19 Mar 2022 1:15:45pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_FFTDisplayContainer.h"
#include <JuceHeader.h>


//==============================================================================
RT_FFTDisplayContainer::RT_FFTDisplayContainer(
    RT_ProcessorInterface *inInterface)
{
  // In your constructor, you should add any child components, and
  // initialise any special settings that your component needs.
}

RT_FFTDisplayContainer::~RT_FFTDisplayContainer() {}

void RT_FFTDisplayContainer::paint(juce::Graphics &g)
{
  /* This demo code just fills the component's background and
     draws some placeholder text to get you started.

     You should replace everything in this method with your own
     drawing code..
  */

  g.fillAll(getLookAndFeel().findColour(
      juce::ResizableWindow::backgroundColourId)); // clear the background

  g.setColour(juce::Colours::grey);
  g.drawRect(getLocalBounds(), 1); // draw an outline around the component

  g.setColour(juce::Colours::white);
  g.setFont(14.0f);
  g.drawText("RT_FFTDisplayContainer", getLocalBounds(),
             juce::Justification::centred, true); // draw some placeholder text
}

void RT_FFTDisplayContainer::resized()
{
  // This method is where you should set the bounds of any child
  // components that your component contains..
}
