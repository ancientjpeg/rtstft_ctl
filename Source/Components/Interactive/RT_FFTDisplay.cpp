/*
  ==============================================================================

    RT_FFTDisplay.cpp
    Created: 19 Mar 2022 1:15:45pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_FFTDisplay.h"
#include "../../Managers/DSPStateManagers/RTSTFT_Manager.h"
#include "../../Managers/StateManagers/RT_PropertyManager.h"

//==============================================================================
RT_FFTDisplay::RT_FFTDisplay(RT_ProcessorInterface *inInterface)
    : RT_Component(inInterface)
{
  const rt_params p        = mInterface->getRTSTFTManager()->getParamsStruct();

  auto            blocklen = rt_manip_block_len(p);
  mDbMaxAsAmp              = rt_dbtoa(mDbMax);
#if JUCE_DEBUG
  startTimer(1000 / 8);
#else
  startTimer(1000 / 30);
#endif
}

RT_FFTDisplay::~RT_FFTDisplay() {}

void RT_FFTDisplay::paint(juce::Graphics &g)
{
  g.fillAll(mInterface->getLookAndFeelManager()->getUIColour(windowBackground));
  auto propMan    = mInterface->getPropertyManager();
  bool mono       = propMan->getMultichannelMode() == RT_MULTICHANNEL_MONO;
  int  activeChan = mono ? 0 : propMan->getActiveChannelIndex();
  if (mono) {
    for (int i = 0;
         i < mInterface->getRTSTFTManager()->getParamsStruct()->num_chans;
         i++) {
      if (i != activeChan)
        paintChannel(g, i);
    }
  }
  paintChannel(g, activeChan, true);
}
void RT_FFTDisplay::paintChannel(juce::Graphics &g, int inChannelIndex,
                                 bool inIsActiveChannel)
{
  const rt_params p = mInterface->getRTSTFTManager()->getParamsStruct();
  if (p == NULL || !p->initialized) {
    return;
  }
  auto  lamf         = mInterface->getLookAndFeelManager();
  int   maxBars      = 128;
  int   numAmpsInFFT = rt_manip_len(p);
  int   barsInWindow = numAmpsInFFT < maxBars ? numAmpsInFFT : maxBars;
  float width        = (float)1 / barsInWindow * getWidth();
  int   i_incr       = numAmpsInFFT <= maxBars ? 1 : numAmpsInFFT / maxBars;

  for (int i = 0; i < barsInWindow; i++) {
    float ampCurr = p->hold->amp_holder[i * i_incr];
    float height  = scaleAmpToYPosNormDbScale(ampCurr);
    float x       = (float)i / barsInWindow * getWidth();
    g.setColour(lamf->getUIColour(highlightedFill));
    g.fillRect(x, (float)(getHeight() - height), width, height);
    x += width * 0.125;
    if (inIsActiveChannel) {
      float manipWidth = width * 0.75;
      paintManips(g, inChannelIndex, i, i_incr, x, manipWidth);
    }
  }
}

void RT_FFTDisplay::paintManips(juce::Graphics &g, int inActiveChannelIndex,
                                int i, int i_incr, float x, float manipWidth)
{
  const rt_params p = mInterface->getRTSTFTManager()->getParamsStruct();
  int             activeManipFlavor
      = mInterface->getPropertyManager()->getActiveManipFlavor();
  bool mono = mInterface->getPropertyManager()->getMultichannelMode()
              == RT_MULTICHANNEL_MONO;
  auto lamf = mInterface->getLookAndFeelManager();
  for (int c = 0; c < p->num_chans; c++) {
    for (int m = 0; m <= RT_MANIP_LIMIT; m++) {
      if (c != inActiveChannelIndex && (m != activeManipFlavor || mono)) {
        continue;
      }
      int   manip_index = rt_manip_index(p, (rt_manip_flavor_t)m, i * i_incr);
      float val         = p->chans[c]->manip->hold_manips[manip_index];
      // TEST
      // float val = p->chans[c]->manip->manips[manip_index];
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
      float height = scaleManipAmpToYPosNorm(val, p, (rt_manip_flavor_t)m);
      auto  col    = activeManipFlavor == m
                         ? (c == inActiveChannelIndex
                                ? juce::Colour(255, 220, 80)
                                : lamf->getUIColour(highlightedFill))
                         : lamf->getUIColour(defaultFill);
      g.setColour(col);
      float manipBarHeight = 2.f;
      g.fillRect(x, (float)(getHeight() - height) - manipBarHeight * 0.5f,
                 manipWidth, manipBarHeight);
    }
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
  ret += rt_get_manip_val(p, activeManip);
  ret = scaleAmpToYPosNormDbScale(inAmp);
  return ret;
}

float RT_FFTDisplay::scaleYPosNormToManipAmp(float             inYPosNormalized,
                                             rt_params         p,
                                             rt_manip_flavor_t activeManip)
{
  float ret = inYPosNormalized;
  ret       = scaleYPosNormToAmpDbScale(inYPosNormalized);
  ret       = ret -= rt_get_manip_val(p, activeManip);
  return ret;
}

float RT_FFTDisplay::getDbRange() { return mDbMax - mDbMin; }
