/*
  ==============================================================================

    RT_ParameterManager.h
    Created: 18 Mar 2022 1:57:25pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../Interface/RT_ProcessorInterface.h"
#include "RT_ParameterDefines.h"

class RT_ParameterManager {
  RT_ProcessorInterface                              *mInterface;
  std::unique_ptr<juce::AudioProcessorValueTreeState> mValueTreeState;

public:
  RT_ParameterManager(RT_ProcessorInterface *inInterface);
  juce::AudioProcessorValueTreeState *getValueTreeState();
  ~RT_ParameterManager();
};
