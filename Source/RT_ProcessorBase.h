/*
  ==============================================================================

    ProcessorBase.h
    Created: 18 Mar 2022 1:17:27pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once
#include "Managers/DSPStateManagers/RT_ParameterManager.h"
#include "Managers/DSPStateManagers/RT_PropertyManager.h"
#include "Managers/GUIManagers/RT_StyleManager.h"
#include "Managers/Interface/RT_ProcessorInterface.h"
#include <JuceHeader.h>
#include <JucePluginDefines.h>

class RT_ProcessorBase : public juce::AudioProcessor,
                         public RT_ProcessorInterface {

  RT_StyleManager mStyleManager;

public:
  RT_ProcessorBase();
  virtual ~RT_ProcessorBase();

  //==============================================================================
  RT_ProcessorBase *getProcessor() override;
  RT_StyleManager  *getStyleManager() override;

  //==============================================================================
  juce::AudioProcessorEditor *createEditor() override;
  bool                        hasEditor() const override;

  //==============================================================================
  const juce::String getName() const override;

  bool               acceptsMidi() const override;
  bool               producesMidi() const override;
  bool               isMidiEffect() const override;
  double             getTailLengthSeconds() const override;

  //==============================================================================
  int                getNumPrograms() override;
  int                getCurrentProgram() override;
  void               setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String &newName) override;

  //==============================================================================
  void getStateInformation(juce::MemoryBlock &destData) override;
  void setStateInformation(const void *data, int sizeInBytes) override;

  //==============================================================================

#ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

private:
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_ProcessorBase)
};
