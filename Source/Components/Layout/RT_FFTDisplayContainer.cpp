/*
  ==============================================================================

    RT_FFTDisplayContainer.cpp
    Created: 19 Mar 2022 1:15:45pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_FFTDisplayContainer.h"
#include "../../Managers/DSPStateManagers/RTSTFT_Manager.h"

//==============================================================================
RT_FFTDisplayContainer::RT_FFTDisplayContainer(
    RT_ProcessorInterface *inInterface)
    : mInterface(inInterface)
{
  // In your constructor, you should add any child components, and
  // initialise any special settings that your component needs.
  startTimer(1000 / 5);
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

  g.setColour(juce::Colours::black);
  g.drawRect(getLocalBounds(), 1); // draw an outline around the component

  g.setColour(juce::Colours::black);
  const rt_params p = mInterface->getRTSTFTManager()->getParamsStruct();
  for (int i = 0; i < p->hold->frame_size / 4; i++) {
    float height = p->hold->amp_holder[i * 4] * getHeight();
    float x      = (float)i / (p->hold->frame_size / 4) * getWidth();
    float width  = (float)1 / (p->hold->frame_size / 4) * getWidth();
    g.fillRect(x, (float)getHeight() - height, width, (float)getHeight());
  }
}

void RT_FFTDisplayContainer::resized()
{
  // This method is where you should set the bounds of any child
  // components that your component contains..
}

void RT_FFTDisplayContainer::timerCallback() { repaint(); }
