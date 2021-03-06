/*
  ==============================================================================

    RT_FileManager.cpp
    Created: 18 Mar 2022 2:05:21pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_FileManager.h"

RT_FileManager::RT_FileManager(RT_ProcessorInterface *inInterface)
    : mInterface(inInterface)
{
  validateDirectoryStructure();
}

void RT_FileManager::validateDirectoryStructure()
{
  juce::File appSupportDir;
#ifdef _WIN32
  appSupportDir = juce::File(
      juce::File::getSpecialLocation(juce::File::userDocumentsDirectory)
          .getFullPathName());
#else
  appSupportDir = juce::File::getSpecialLocation(
      juce::File::commonApplicationDataDirectory);
#endif
  mAppSupportDir = appSupportDir.getChildFile(sc_RTSTFTAppSupportDirName);
  auto checkDir  = [](juce::File dir) {
    if (!dir.exists()) {
      auto result = dir.createDirectory();
      return result.wasOk();
    }
    return true;
  };
  assert(checkDir(mAppSupportDir)); // this might be a ticking time bomb lol
  mAppPresetsDir = mAppSupportDir.getChildFile("presets");
  assert(checkDir(mAppPresetsDir));
  if (!checkDir(mAppPresetsDir)) {
    exit(1);
  }
}

void RT_FileManager::savePreset(juce::String      inPresetName,
                                juce::MemoryBlock inPreparedMemoryBlock)
{
  auto fileDest = mAppPresetsDir.getChildFile(inPresetName + sc_PresetSuffix);
  auto writeStatus = fileDest.replaceWithData(inPreparedMemoryBlock.getData(),
                                              inPreparedMemoryBlock.getSize());
  jassert(writeStatus);
}

void       RT_FileManager::loadPreset(juce::String inPresetName) {}

juce::File RT_FileManager::getAppSupportDir() { return mAppSupportDir; }
juce::File RT_FileManager::getPresetsDirectory() { return mAppPresetsDir; }
