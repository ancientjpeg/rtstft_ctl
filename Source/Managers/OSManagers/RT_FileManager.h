/*
  ==============================================================================

    RT_FileManager.h
    Created: 18 Mar 2022 2:05:21pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once
#include "../Interface/RT_ProcessorInterface.h"
#include <JuceHeader.h>

class RT_FileManager {

  inline static const juce::String sc_AppSupportDirName
      = juce::String("/Library/Application Support/sound_ctl/rtstft_ctl");
  inline static const juce::String sc_PresetSuffix
      = juce::String(".rtstftpreset");

  RT_ProcessorInterface *mInterface;
  juce::File             mAppSupportDir, mAppPresetsDir;

  void                   validateSupportDirectory();

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_FileManager);

public:
  RT_FileManager() = delete;
  RT_FileManager(RT_ProcessorInterface *inInterface);
  ~RT_FileManager() = default;
};