/*
  ==============================================================================

    ProcessorBase.h
    Created: 18 Mar 2022 1:17:27pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Managers/Interface/RT_ProcessorInterface.h"

class RT_ProcessorBase : public juce::AudioProcessor, public RT_ProcessorInterface {
  
public:
  RT_ProcessorBase();
  virtual ~RT_ProcessorBase();
  //==============================================================================
  juce::AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;

  //==============================================================================
  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;
  

  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram (int index) override;
  const juce::String getProgramName (int index) override;
  void changeProgramName (int index, const juce::String& newName) override;

  //==============================================================================
  void getStateInformation (juce::MemoryBlock& destData) override;
  void setStateInformation (const void* data, int sizeInBytes) override;
  
  //==============================================================================
  
#ifndef JucePlugin_PreferredChannelConfigurations
 bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif
  
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RT_ProcessorBase)
};
