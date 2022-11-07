/*
  ==============================================================================

    RT_FFTDisplayContainer.h
    Created: 19 Mar 2022 1:15:45pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../../Managers/DSPStateManagers/RTSTFT_Manager.h"
#include "../../Managers/DSPStateManagers/RT_ParameterDefines.h"
#include "../../Managers/Interface/RT_ProcessorInterface.h"
#include "../Utility/RT_Component.h"
#include <JuceHeader.h>

//==============================================================================
/*
 */
class RT_FFTDisplay : public RT_Component,
                      public juce::Timer {

public:
  RT_FFTDisplay(RT_ProcessorInterface *inInterface);
  ~RT_FFTDisplay() override;

  void  paint(juce::Graphics &g) override;
  void  resized() override;
  void  timerCallback() override;
  void  mouseDrag(const juce::MouseEvent &event) override;
  void  mouseDown(const juce::MouseEvent &event) override;

  int   xPosToManipsIndex(float inXPos);
  float manipsIndexToXPos(int inIndex);

  //==============================================================================

  static constexpr float mDbMin = RT_DB_MIN, mDbMax = RT_DB_MAX,
                         mDbRange = mDbMax - mDbMin;
  float getDbValNormalized(float dB);
  float yPosNorm(float YPos);
  float yPosDenorm(float YPosNormalized);
  float scaleAmpToYPosNormDbScale(float inAmp);
  float scaleYPosNormToAmpDbScale(float inYPos);
  float scaleManipAmpToYPosNorm(float             inAmp,
                                rt_params         p,
                                rt_manip_flavor_t activeManip);
  float scaleYPosNormToManipAmp(float             inYPosNormalized,
                                rt_params         p,
                                rt_manip_flavor_t activeManip);

private:
  static const int      mMaxBins = 128;
  juce::CriticalSection mPaintLock;
  float                 mDbMaxAsAmp;
  juce::Point<float>    mLastDragPos;
  float                 getDbRange();
  int                   mActiveChannelIndexForMouseDown;

  void                  _paintBins(juce::Graphics &g);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_FFTDisplay)
};
