/*
  ==============================================================================

    RT_FFTDisplay.cpp
    Created: 19 Mar 2022 1:15:45pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_FFTDisplay.h"

//==============================================================================
RT_FFTDisplay::RT_FFTDisplay(RT_ProcessorInterface *inInterface)
    : RT_Component(inInterface)
{
  // In your constructor, you should add any child components, and
  // initialise any special settings that your component needs.

  const rt_params p        = mInterface->getRTSTFTManager()->getParamsStruct();

  auto            blocklen = rt_manip_block_len(p);
  mLocalManipCopies = std::make_unique<rt_real[]>(blocklen * p->num_chans);
  for (int i = 0; i < RT_MANIP_FLAVOR_COUNT; i++) {
    copyManips((rt_manip_flavor_t)i);
  }

  //  mInterface->getRTSTFTManager()->addListener(this);

  startTimer(1000 / 15);
}

RT_FFTDisplay::~RT_FFTDisplay() {}

void RT_FFTDisplay::paint(juce::Graphics &g)
{

  g.fillAll(getLookAndFeel_V4().getCurrentColourScheme().getUIColour(
      windowBackground));

  g.setColour(juce::Colours::lightgrey);
  const rt_params p = mInterface->getRTSTFTManager()->getParamsStruct();
  if (p != NULL || !p->initialized) {
    int   maxBars      = 128;
    int   numAmpsInFFT = rt_manip_len(p);
    int   barsInWindow = numAmpsInFFT < maxBars ? numAmpsInFFT : maxBars;
    float width        = (float)1 / barsInWindow * getWidth();
    int   i_incr       = numAmpsInFFT <= maxBars ? 1 : numAmpsInFFT / maxBars;
    for (int i = 0; i < barsInWindow; i++) {
      float height = p->hold->amp_holder[i * i_incr] * getHeight();
      float x      = (float)i / barsInWindow * getWidth();
      g.fillRect(x, (float)(getHeight() - height), width, height);
    }
  }
}

void RT_FFTDisplay::resized()
{
  // This method is where you should set the bounds of any child
  // components that your component contains..
}

void RT_FFTDisplay::timerCallback() { repaint(); }

void RT_FFTDisplay::onManipChanged(rt_manip_flavor_t inManipFlavor)
{
  copyManips(inManipFlavor);
}

void RT_FFTDisplay::copyManips(rt_manip_flavor_t inTargetManipFlavor)
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
