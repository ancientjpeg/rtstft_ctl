/*
  ==============================================================================

    RT_FFTDisplayContainer.cpp
    Created: 19 Mar 2022 1:15:45pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_FFTDisplayContainer.h"

//==============================================================================
RT_FFTDisplayContainer::RT_FFTDisplayContainer(
    RT_ProcessorInterface *inInterface, int inBorderSize)
    : RT_BorderedComponent(inInterface, inBorderSize)
{
  // In your constructor, you should add any child components, and
  // initialise any special settings that your component needs.
  
  const rt_params p        = mInterface->getRTSTFTManager()->getParamsStruct();

  auto            blocklen = rt_manip_block_len(p);
  mLocalManipCopies = std::make_unique<rt_real[]>(blocklen * p->num_chans);
  for (int i = 0; i < RT_MANIP_FLAVOR_COUNT; i++) {
    copyManips((rt_manip_flavor_t)i);
  }

  mInterface->getRTSTFTManager()->addListener(this);
  
  startTimer(1000 / 15);
}

RT_FFTDisplayContainer::~RT_FFTDisplayContainer() {}

void RT_FFTDisplayContainer::paintInBorder(juce::Graphics &g)
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
  float width        = (float)1 / barsInWindow * getWidthAdj();
  int   i_incr       = numAmpsInFFT <= maxBars ? 1 : numAmpsInFFT / maxBars;
  for (int i = 0; i < barsInWindow; i++) {
    float height = p->hold->amp_holder[i * i_incr] * getHeightAdj() * 32;
    float x      = (float)i / barsInWindow * getWidthAdj();
    g.fillRect(x, (float)getHeight() - height - mBorderSize / 2, width, height);
  }
}

void RT_FFTDisplayContainer::resized()
{
  // This method is where you should set the bounds of any child
  // components that your component contains..
}

void RT_FFTDisplayContainer::timerCallback() { repaint(); }

void RT_FFTDisplayContainer::onManipChanged(rt_manip_flavor_t inManipFlavor)
{
  copyManips(inManipFlavor);
}

void RT_FFTDisplayContainer::copyManips(rt_manip_flavor_t inTargetManipFlavor)
{
  const rt_params p         = mInterface->getRTSTFTManager()->getParamsStruct();
  auto            manip_len = rt_manip_len(p);
  auto            blocklen  = rt_manip_block_len(p);
  auto            num_chans = p->manip_multichannel ? p->num_chans : 1;
  for (int i = 0; i < num_chans; i++) {
    std::memcpy(mLocalManipCopies.get() + blocklen * i,
                rt_manip_read_buffer(p, p->chans[i], (rt_manip_flavor_t)0),
                manip_len * sizeof(rt_real));
  }
}
