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

#if defined(__APPLE__)
#define RT_SUPPORT_FOLDER "Application Support/sound_ctl/rtstft_ctl"
#elif defined(__WIN32)
#define RT_SUPPORT_FOLDER "sound_ctl\\rtstft_ctl"
#else
#define RT_SUPPORT_FOLDER "sound_ctl/rtstft_ctl"
#endif

class RT_FileManager {

public:
  inline static const juce::String sc_RTSTFTAppSupportDirName
      = juce::String(RT_SUPPORT_FOLDER);
  inline static const juce::String sc_PresetSuffix
      = juce::String(".rtstftpreset");

  RT_ProcessorInterface *mInterface;

  RT_FileManager() = delete;
  RT_FileManager(RT_ProcessorInterface *inInterface);
  ~RT_FileManager() = default;

  void       savePreset(juce::String      inPresetName,
                        juce::MemoryBlock inPreparedMemoryBlock);
  void       loadPreset(juce::String inPresetName);
  juce::File getAppSupportDir();
  juce::File getPresetsDirectory();

private:
  juce::File mAppSupportDir, mAppPresetsDir;
  void       validateDirectoryStructure();

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_FileManager);
};
