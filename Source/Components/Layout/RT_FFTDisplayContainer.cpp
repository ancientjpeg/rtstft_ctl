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
  g.drawRect(getLocalBounds(), 2); // draw an outline around the component

  g.setColour(juce::Colours::lightgrey);
  const rt_params p       = mInterface->getRTSTFTManager()->getParamsStruct();
  int             maxBars = 128;
  int             numAmpsInFFT = rt_manip_len(p);
  int   barsInWindow = numAmpsInFFT < maxBars ? numAmpsInFFT : maxBars;
  float width        = (float)1 / barsInWindow * getWidth();
  int   i_incr       = numAmpsInFFT <= maxBars ? 1 : numAmpsInFFT / maxBars;
  for (int i = 0; i < barsInWindow; i++) {
    float height = p->hold->amp_holder[i * i_incr] * getHeight() / 32;
    float x      = (float)i / barsInWindow * getWidth();
    g.fillRect(x, (float)getHeight() - height, width, height);
  }
}

void RT_FFTDisplayContainer::resized()
{
  // This method is where you should set the bounds of any child
  // components that your component contains..
}

void RT_FFTDisplayContainer::timerCallback() { repaint(); }
