/*
  ==============================================================================

    RT_FFTDisplayContainer.h
    Created: 19 Mar 2022 1:15:45pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../../Managers/DSPStateManagers/RTSTFT_Manager.h"
#include "../../Managers/Interface/RT_ProcessorInterface.h"
#include "../Utility/RT_BorderedComponent.h"
#include <JuceHeader.h>

//==============================================================================
/*
 */
class RT_FFTDisplay : public RT_Component,
                      public juce::Timer,
                      public RTSTFT_Manager::Listener {

public:
  RT_FFTDisplay(RT_ProcessorInterface *inInterface);
  ~RT_FFTDisplay() override;

  void  paint(juce::Graphics &) override;
  void  resized() override;
  void  timerCallback() override;
  void  mouseDrag(const juce::MouseEvent &event) override;
  void  mouseDown(const juce::MouseEvent &event) override;

  int   xPosToManipsIndex(float inXPos);
  float manipsIndexToXPos(int inIndex);
  void  onManipChanged(rt_manip_flavor_t inManipFlavor) override;

  //==============================================================================

  static constexpr float mDbMin = -90.f, mDbMax = 18.f,
                         mDbRange = mDbMax - mDbMin;
  float getDbValNormalized(float dB);
  float yPosNorm(float YPos);
  float yPosDenorm(float YPosNormalized);
  float scaleAmpToYPosNormDbScale(float inAmp);
  float scaleYPosNormToAmpDbScale(float inYPos);
  float scaleManipAmpToYPosNorm(float inAmp, rt_params p,
                                rt_manip_flavor_t activeManip);
  float scaleYPosNormToManipAmp(float inYPosNormalized, rt_params p,
                                rt_manip_flavor_t activeManip);

private:
  float                      mDbMaxAsAmp;
  std::unique_ptr<rt_real[]> mLocalManipCopies;
  juce::Point<float>         mLastDragPos;
  float                      getDbRange();

  void                       copyManips(rt_manip_flavor_t inTargetManipFlavor);
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_FFTDisplay)
};
