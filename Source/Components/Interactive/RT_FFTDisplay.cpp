/*
  ==============================================================================

    RT_FFTDisplay.cpp
    Created: 19 Mar 2022 1:15:45pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_FFTDisplay.h"
#include "../../Managers/DSPStateManagers/RTSTFT_Manager.h"
#include "../../Managers/GUIStateManagers/RT_GUIStateManager.h"

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

  auto cs          = getLookAndFeel_V4().getCurrentColourScheme();
  auto manip_state = mInterface->getGUIStateManager()->getSelectorData();
  g.fillAll(cs.getUIColour(windowBackground));

  const rt_params p = mInterface->getRTSTFTManager()->getParamsStruct();
  if (p != NULL && p->initialized) {
    int   maxBars      = 128;
    int   numAmpsInFFT = rt_manip_len(p);
    int   barsInWindow = numAmpsInFFT < maxBars ? numAmpsInFFT : maxBars;
    float width        = (float)1 / barsInWindow * getWidth();
    int   i_incr       = numAmpsInFFT <= maxBars ? 1 : numAmpsInFFT / maxBars;

    for (int i = 0; i < barsInWindow; i++) {
      float height = p->hold->amp_holder[i * i_incr] * getHeight();
      float x      = (float)i / barsInWindow * getWidth();
      g.setColour(cs.getUIColour(highlightedFill));
      g.fillRect(x, (float)(getHeight() - height), width, height);
      x += width * 0.25;
      float manip_width = width * 0.75;
      for (int m = 0; m <= RT_MANIP_LIMIT; m++) {
        int   manip_index = rt_manip_index(p, (rt_manip_flavor_t)m, i * i_incr);
        float val         = p->chans[0]->manip->hold_manips[manip_index];
        switch ((rt_manip_flavor_t)m) {
        case RT_MANIP_GAIN:
          val += p->hold->gain_mod;
          break;
        case RT_MANIP_GATE:
          val += p->hold->gate_mod;
          break;
        case RT_MANIP_LIMIT:
          val += p->hold->limit_mod;
          break;
        default:
          break;
        }
        height   = val * getHeight();
        auto col = cs.getUIColour(defaultFill);
        if (manip_state->activeField == m) {
          col = juce::Colours::goldenrod;
        }
        g.setColour(col);
        g.fillRect(x, (float)(getHeight() - height), manip_width, 2.f);
      }
    }
  }
}

int RT_FFTDisplay::xPosToManipsIndex(int inXPos)
{
  return inXPos
         * rt_manip_len(mInterface->getRTSTFTManager()->getParamsStruct())
         / getWidth();
}
int RT_FFTDisplay::manipsIndexToXPos(int inIndex)
{
  return getWidth()
         / rt_manip_len(mInterface->getRTSTFTManager()->getParamsStruct())
         * inIndex;
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

void RT_FFTDisplay::mouseDrag(const juce::MouseEvent &event)
{
  auto p          = mInterface->getRTSTFTManager()->getParamsStruct();
  int  numManips  = rt_manip_len(p);
  int  manipIndex = xPosToManipsIndex(event.x);
  int  activeField
      = mInterface->getGUIStateManager()->getSelectorData()->activeField;
  if (manipIndex >= 0 && manipIndex < numManips && activeField >= 0) {
    rt_manip_flavor_t active_manip = (rt_manip_flavor_t)activeField;
    float             newVal       = (float)event.y / getHeight() + rt_get_manip_val(p, active_manip);
    newVal                         = std::clamp<float>(1.f - newVal, 0.f, 1.f);
    rt_manip_set_bin_single(p, p->chans[0], active_manip, manipIndex, newVal);
  }
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
