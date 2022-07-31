/*
  ==============================================================================

    RT_FFTDisplay.cpp
    Created: 19 Mar 2022 1:15:45pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_FFTDisplay.h"
#include "../../Managers/DSPStateManagers/RTSTFT_Manager.h"
#include "../../Managers/LookAndFeelManagement/RT_LookAndFeelManagement.h"
#include "../../Managers/StateManagers/RT_PropertyManager.h"
#include <algorithm>

//==============================================================================
RT_FFTDisplay::RT_FFTDisplay(RT_ProcessorInterface *inInterface)
    : RT_Component(inInterface)
{
  mDbMaxAsAmp = rt_dbtoa(mDbMax);
#if JUCE_DEBUG
  startTimer(1000 / 15);
#else
  startTimer(1000 / 30);
#endif
}

RT_FFTDisplay::~RT_FFTDisplay() {}

void RT_FFTDisplay::paint(juce::Graphics &g)
{
  g.fillAll(mInterface->getLookAndFeelManager()->getUIColour(windowBackground));
  _paintBins(g);
}

// yes, I know this code could be a lot cleaner.
// I need a dedicated UI programmer ...
void RT_FFTDisplay::_paintBins(juce::Graphics &g)
{
  auto p               = mInterface->getRTSTFTManager()->getParamsStruct();
  auto prop_man        = mInterface->getPropertyManager();
  auto lafm            = mInterface->getLookAndFeelManager();
  int  active_chan     = prop_man->getActiveChannelIndex();
  int  num_amps_in_fft = rt_manip_len(p);
  int  bins_in_window = num_amps_in_fft < mMaxBins ? num_amps_in_fft : mMaxBins;
  int  active_manip_flavor = prop_man->getActiveManipFlavor();
  int  i_incr = num_amps_in_fft <= mMaxBins ? 1 : num_amps_in_fft / mMaxBins;

  juce::Rectangle<float> bin_rect, active_manip_rect;
  float                  width     = 1.f / bins_in_window * getWidth();
  float                  reduction = width * 0.1f;
  // paint channels
  for (int i = 0; i < bins_in_window; i++) {
    int   bin_idx = i * i_incr;
    float left    = manipsIndexToXPos(bin_idx);
    float active_amp, amp_max = 0.f;
    for (int c = 0; c < p->num_chans; c++) {
      float amp = 0.f;
      for (int b = 0; b < i_incr; b++) {
        amp = std::max(p->chans[c]->framebuf->amp_holder[bin_idx + b], amp);
      }
      amp_max = std::max(amp, amp_max);
      if (c == active_chan) {
        active_amp = amp;
      }
    }

    float amp_max_ypos    = scaleAmpToYPosNormDbScale(amp_max);
    float active_amp_ypos = scaleAmpToYPosNormDbScale(amp_max);
    bin_rect.setBounds(left, getHeight() - active_amp_ypos, width,
                       active_amp_ypos);
    bin_rect.reduce(reduction, 0);
    g.setColour(lafm->getUIColour(highlightedFill));
    g.fillRect(bin_rect);
    if (amp_max_ypos > active_amp_ypos) {
      g.setColour(lafm->getUIColour(defaultFill));
      g.fillRect(bin_rect.withBottom(getHeight() - active_amp_ypos)
                     .withTop(getHeight() - amp_max_ypos));
    }

    if (active_manip_flavor < 0) continue;
    rt_manip_flavor_t flavor  = (rt_manip_flavor_t)active_manip_flavor;
    float             mod_val = rt_get_manip_mod_val(p, flavor);
    bool              mono    = prop_man->getMultichannelMode() == 0;
    juce::Colour      active_manip_col;
    for (int c = 0; c < (mono ? 1 : p->num_chans); c++) {
      float manip_ypos = 0.f;
      for (int b = 0; b < i_incr; b++) {
        int manip_index = rt_manip_index(p, flavor, bin_idx + b);
        manip_ypos += p->chans[c]->manip->hold_manips[manip_index];
      }
      manip_ypos = scaleAmpToYPosNormDbScale(manip_ypos / i_incr * mod_val);
      auto col   = lafm->getUIColour(c == active_chan || mono ? highlightedFill
                                                              : defaultFill);
      if (manip_ypos < active_amp_ypos) {
        col = col.contrasting();
      }
      auto rect = bin_rect.withBottom(getHeight() - (manip_ypos - 1.f))
                      .withTop(getHeight() - (manip_ypos + 1.f));
      if (c == active_chan || mono) {
        active_manip_col  = col;
        active_manip_rect = rect;
      }
      g.setColour(col);
      g.fillRect(rect);
    }
    g.setColour(active_manip_col);
    g.fillRect(active_manip_rect);
  }
}

int RT_FFTDisplay::xPosToManipsIndex(float inXPos)
{
  int manip_len
      = rt_manip_len(mInterface->getRTSTFTManager()->getParamsStruct());
  int ret = inXPos * manip_len / getWidth();
  return std::clamp<int>(ret, 0, manip_len);
}
float RT_FFTDisplay::manipsIndexToXPos(int inIndex)
{
  return (float)getWidth()
         / rt_manip_len(mInterface->getRTSTFTManager()->getParamsStruct())
         * inIndex;
}

void RT_FFTDisplay::resized()
{
  // This method is where you should set the bounds of any child
  // components that your component contains..
}

void RT_FFTDisplay::timerCallback() { repaint(); }

void RT_FFTDisplay::mouseDown(const juce::MouseEvent &event)
{
  mLastDragPos = event.position;
  auto propMan = mInterface->getPropertyManager();
  mActiveChannelIndexForMouseDown
      = propMan->getMultichannelMode() == RT_MULTICHANNEL_MONO
            ? 0
            : mInterface->getPropertyManager()->getActiveChannelIndex();
}

void RT_FFTDisplay::mouseDrag(const juce::MouseEvent &event)
{
  // save last pos and account for any positions between last pos and this pos
  // recurse this
  auto p = mInterface->getRTSTFTManager()->getParamsStruct();
  int  activeManipFlavor
      = mInterface->getPropertyManager()->getActiveManipFlavor();
  if (activeManipFlavor == -1) {
    return;
  }
  rt_manip_flavor_t activeManip   = (rt_manip_flavor_t)activeManipFlavor;
  int               lastPosIndex  = xPosToManipsIndex(mLastDragPos.x);
  int               finalPosIndex = xPosToManipsIndex(event.position.x);
  int               indexDiff     = finalPosIndex - lastPosIndex;
  int               indexDiffSign = indexDiff >= 0 ? 1 : -1;

  float             manipVal
      = scaleYPosNormToManipAmp(yPosNorm(mLastDragPos.y), p, activeManip);
  rt_manip_set_bin_single(p, p->chans[mActiveChannelIndexForMouseDown],
                          activeManip, lastPosIndex, manipVal);
  if (indexDiff != 0) {
    float nextIndexXPos = manipsIndexToXPos(lastPosIndex + indexDiffSign);
    float nextIndexYPos
        = juce::jmap((float)lastPosIndex + indexDiffSign, (float)lastPosIndex,
                     (float)finalPosIndex, mLastDragPos.y, event.position.y);
    mLastDragPos.setXY(nextIndexXPos, nextIndexYPos);
    mouseDrag(event);
  }
  else {
    mLastDragPos = event.position;
  }
}

float RT_FFTDisplay::getDbValNormalized(float dB)
{
  return (dB - mDbMin) / mDbRange;
}

float RT_FFTDisplay::yPosNorm(float YPos) { return 1.f - (YPos / getHeight()); }

float RT_FFTDisplay::yPosDenorm(float YPosNormalized)
{
  return (1.f - YPosNormalized) * getHeight();
}

float RT_FFTDisplay::scaleAmpToYPosNormDbScale(float inAmp)
{
  if (inAmp <= 0.f) {
    return 0.f;
  }
  inAmp = std::clamp<float>(rt_atodb(inAmp), mDbMin, mDbMax) - mDbMin;
  inAmp = inAmp / mDbRange;
  return inAmp * getHeight();
}

float RT_FFTDisplay::scaleYPosNormToAmpDbScale(float inYPosNormalized)
{
  float ret = inYPosNormalized;
  if (ret <= 0.f) {
    return 0.f;
  }
  ret = std::min<float>(ret, mDbMaxAsAmp);
  ret = rt_dbtoa(ret * getDbRange() + mDbMin);
  return ret;
}

float RT_FFTDisplay::scaleManipAmpToYPosNorm(float inAmp, rt_params p,
                                             rt_manip_flavor_t activeManip)
{
  float ret = inAmp;
  ret += rt_get_manip_mod_val(p, activeManip);
  ret = scaleAmpToYPosNormDbScale(inAmp);
  return ret;
}

float RT_FFTDisplay::scaleYPosNormToManipAmp(float             inYPosNormalized,
                                             rt_params         p,
                                             rt_manip_flavor_t activeManip)
{
  float ret = inYPosNormalized;
  ret       = scaleYPosNormToAmpDbScale(inYPosNormalized);
  ret /= rt_get_manip_mod_val(p, activeManip);
  return ret;
}

float RT_FFTDisplay::getDbRange() { return mDbMax - mDbMin; }
