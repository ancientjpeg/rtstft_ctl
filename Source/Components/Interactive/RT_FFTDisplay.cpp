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
  mDbMaxAsAmp = rt_dbtoa(mDbMax);
#if JUCE_DEBUG
  startTimer(1000 / 15);
#else
  startTimer(1000 / 15);
#endif
}

RT_FFTDisplay::~RT_FFTDisplay() {}

void RT_FFTDisplay::paint(juce::Graphics &g)
{
  g.fillAll(mInterface->getLookAndFeelManager()->getUIColour(windowBackground));
  auto propMan    = mInterface->getPropertyManager();
  int  activeChan = propMan->getActiveChannelIndex();
  // paint both even when in mono b/c incoming audio is still stereo
  for (int i = 0;
       i < mInterface->getRTSTFTManager()->getParamsStruct()->num_chans; i++) {
    if (i != activeChan)
      paintChannel(g, i);
  }
  paintChannel(g, activeChan, true);
}
void RT_FFTDisplay::paintChannel(juce::Graphics &g, int inChannelIndex,
                                 bool inIsActiveChannel)
{
  const rt_params p       = mInterface->getRTSTFTManager()->getParamsStruct();
  auto            propMan = mInterface->getPropertyManager();
  bool            mono = propMan->getMultichannelMode() == RT_MULTICHANNEL_MONO;
  int             activeManipFlavor = propMan->getActiveManipFlavor();
  if (p == NULL || !p->initialized) {
    return;
  }
  auto  lafm         = mInterface->getLookAndFeelManager();
  int   maxBars      = 128;
  int   numAmpsInFFT = rt_manip_len(p);
  int   barsInWindow = numAmpsInFFT < maxBars ? numAmpsInFFT : maxBars;
  float width        = 1.f / barsInWindow * getWidth();
  int   i_incr       = numAmpsInFFT <= maxBars ? 1 : numAmpsInFFT / maxBars;

  for (int i = 0; i < barsInWindow; i++) {
    float ampCurr = p->chans[inChannelIndex]->framebuf->amp_holder[i * i_incr];
    float height  = scaleAmpToYPosNormDbScale(ampCurr);
    float x       = (float)i / barsInWindow * getWidth();
    g.setColour(inIsActiveChannel ? lafm->getUIColour(highlightedFill)
                                  : lafm->getUIColour(defaultFill));
    g.fillRect(x, (float)(getHeight() - height), width, height);
    x += width * 0.125;
    if (inIsActiveChannel) {
      float        manipWidth = width * 0.75;
      float        val;
      juce::Colour manipCol;
      int          activeChan = mono ? 0 : inChannelIndex;
      for (int m = 0; m <= RT_MANIP_LIMIT; m++) {
        for (int c = 0; c < p->num_chans; c++) {
          if (m == activeManipFlavor) {
            if (c == activeChan) {
              manipCol = juce::Colour(255, 220, 60);
            }
            else if (!mono) {
              manipCol = lafm->getUIColour(highlightedFill);
            }
            else {
              continue;
            }
          }
          else if (c == activeChan) {
            manipCol = lafm->getUIColour(defaultFill);
          }
          else {
            continue;
          }
          int manip_index = rt_manip_index(p, (rt_manip_flavor_t)m, i * i_incr);
          val             = p->chans[c]->manip->hold_manips[manip_index];
          if (val <= ampCurr) {
            float contrast = c == activeChan ? .5f : 1.f;
            if (!(m == activeManipFlavor && c == activeChan)) {
              manipCol = lafm->getUIColour(highlightedFill).contrasting(contrast);
            }
          }
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
          float manipBarHeight = 2.f;
          g.setColour(manipCol);
          g.fillRect(x, (float)(getHeight() - height) - manipBarHeight * 0.5f,
                     manipWidth, manipBarHeight);
        }
      }
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
