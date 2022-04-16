/*
  ==============================================================================

    ProcessorBase.h
    Created: 18 Mar 2022 1:17:27pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once
#include "Managers/DSPStateManagers/RTSTFT_Manager.h"
#include "Managers/StateManagers/RT_StateManager.h"
#include "Managers/Interface/RT_ProcessorInterface.h"
#include "Managers/LookAndFeelManagement/RT_LookAndFeelManagement.h"
#include "Managers/OSManagers/RT_FileManager.h"
#include "Managers/DSPStateManagers/RT_ParameterManager.h"
#include <JuceHeader.h>
#include <JucePluginDefines.h>

class RT_ProcessorBase : public juce::AudioProcessor,
                         public RT_ProcessorInterface {

protected:
  RT_LookAndFeel::Manager mLookAndFeelManager;
  RTSTFT_Manager          mRTSTFTManager;
  RT_ParameterManager     mParameterManager;
  RT_StateManager         mGUIStateManager;
  RT_FileManager          mFileManager;

  juce::MemoryBlock       mStateInformation;
  int                     mXMLOffset           = -1;
  bool                    mAwaitingStateUpdate = false;

public:
  RT_ProcessorBase();
  virtual ~RT_ProcessorBase();

  //==============================================================================
  juce::AudioProcessor    *getProcessor() override;
  RT_LookAndFeel::Manager *getLookAndFeelManager() override;
  RTSTFT_Manager          *getRTSTFTManager() override;
  RT_ParameterManager     *getParameterManager() override;
  RT_StateManager         *getGUIStateManager() override;
  RT_FileManager          *getFileManager() override;

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
  void        getStateInformation(juce::MemoryBlock &destData) override;
  void        setStateInformation(const void *data, int sizeInBytes) override;
  void        verifyStateIsUpToDate();
  const void *getManipsBinaryPointer();

  static const int ManipsBinaryMagicNumber = 0x3f2f4f51;

  //==============================================================================

#ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

private:
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_ProcessorBase)
};
