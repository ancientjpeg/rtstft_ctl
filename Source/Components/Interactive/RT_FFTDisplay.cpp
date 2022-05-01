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
  // In your constructor, you should add any child components, and
  // initialise any special settings that your component needs.

  const rt_params p        = mInterface->getRTSTFTManager()->getParamsStruct();

  auto            blocklen = rt_manip_block_len(p);
  mLocalManipCopies = std::make_unique<rt_real[]>(blocklen * p->num_chans);
  //  for (int i = 0; i < RT_MANIP_FLAVOR_COUNT; i++) {
  //    copyManips((rt_manip_flavor_t)i);
  //  }

  //  mInterface->getRTSTFTManager()->addListener(this);

  mDbMaxAsAmp = rt_dbtoa(mDbMax);
  startTimer(1000 / 15);
}

RT_FFTDisplay::~RT_FFTDisplay() {}

void RT_FFTDisplay::paint(juce::Graphics &g)
{

  auto cs = getLookAndFeel_V4().getCurrentColourScheme();
  g.fillAll(cs.getUIColour(windowBackground));
  int activeManipIndex
      = mInterface->getPropertyManager()->getActiveManipFlavor();

  const rt_params p = mInterface->getRTSTFTManager()->getParamsStruct();
  if (p != NULL && p->initialized) {
    int   maxBars      = 128;
    int   numAmpsInFFT = rt_manip_len(p);
    int   barsInWindow = numAmpsInFFT < maxBars ? numAmpsInFFT : maxBars;
    float width        = (float)1 / barsInWindow * getWidth();
    int   i_incr       = numAmpsInFFT <= maxBars ? 1 : numAmpsInFFT / maxBars;

    for (int i = 0; i < barsInWindow; i++) {
      float ampCurr = p->hold->amp_holder[i * i_incr];
      float height  = scaleAmpToYPosNormDbScale(ampCurr);
      float x       = (float)i / barsInWindow * getWidth();
      g.setColour(cs.getUIColour(highlightedFill));
      g.fillRect(x, (float)(getHeight() - height), width, height);
      x += width * 0.125;
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
        height   = scaleManipAmpToYPosNorm(val, p, (rt_manip_flavor_t)m);
        auto col = cs.getUIColour(defaultFill);
        if (activeManipIndex == m) {
          col = juce::Colour(255, 225, 53);
        }
        g.setColour(col);
        float manipBarHeight = 2.f;
        g.fillRect(x, (float)(getHeight() - height) - manipBarHeight * 0.5f,
                   manip_width, manipBarHeight);
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
}

void RT_FFTDisplay::mouseDrag(const juce::MouseEvent &event)
{
  // save last pos and account for any positions between last pos and this pos
  // recurse this
  auto p = mInterface->getRTSTFTManager()->getParamsStruct();
  int  activeManipIndex
      = mInterface->getPropertyManager()->getActiveManipFlavor();
  if (activeManipIndex == -1) {
    return;
  }
  rt_manip_flavor_t activeManip   = (rt_manip_flavor_t)activeManipIndex;
  int               lastPosIndex  = xPosToManipsIndex(mLastDragPos.x);
  int               finalPosIndex = xPosToManipsIndex(event.position.x);
  int               indexDiff     = finalPosIndex - lastPosIndex;
  int               indexDiffSign = indexDiff >= 0 ? 1 : -1;

  float             manipVal
      = scaleYPosNormToManipAmp(yPosNorm(mLastDragPos.y), p, activeManip);
  rt_manip_set_bin_single(p, p->chans[0], activeManip, lastPosIndex, manipVal);
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
