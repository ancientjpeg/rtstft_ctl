/*
  ==============================================================================

    RTSTFT_Manager.h
    Created: 19 Mar 2022 2:08:48pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once
#include "../../../RTSTFT/src/rtstft.h"
#include "../Interface/RT_ProcessorInterface.h"
#include <JuceHeader.h>
class RTSTFT_Manager : juce::AudioProcessorValueTreeState::Listener {
  RT_ProcessorInterface *mInterface;
  rt_params              p;
  const int              mDefaultMaxSamplesPerBlock = 4096;
  int                    mCurrentSamplesPerBlock;
  float                  mCurrentSampleRate;
  bool                   initialized = false;

public:
  RTSTFT_Manager(RT_ProcessorInterface *mInterface);
  ~RTSTFT_Manager();
  rt_params getParamsStruct();
  void      prepareToPlay(double inSampleRate, int inSamplesPerBlock);
  void      processBlock(juce::AudioBuffer<float> &buffer);
  void      parameterChanged(const juce::String &parameterID,
                             float               newValue) override;
};
