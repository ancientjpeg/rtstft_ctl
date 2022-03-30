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


class RT_ValueTreeState : public juce::AudioProcessorValueTreeState {
public:
  RT_ValueTreeState(juce::AudioProcessor &processorToConnectTo, juce::UndoManager *undoManagerToUse, const juce::Identifier &valueTreeType, std::vector<std::unique_ptr<juce::RangedAudioParameter>> layoutPtrs) :
  juce::AudioProcessorValueTreeState (processorToConnectTo, undoManagerToUse, valueTreeType, {layoutPtrs.begin(), layoutPtrs.end()}) {}
  ~RT_ValueTreeState() = default;
private:
  void valueTreePropertyChanged (juce::ValueTree&, const juce::Identifier&) override {
    
  }
};


class RT_ParameterManager {
  RT_ProcessorInterface                              *mInterface;
  std::unique_ptr<juce::AudioProcessorValueTreeState> mValueTreeState;

public:
  RT_ParameterManager(RT_ProcessorInterface *inInterface);
  juce::AudioProcessorValueTreeState *getValueTree();
  ~RT_ParameterManager();
};
