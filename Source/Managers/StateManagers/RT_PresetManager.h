/*
  ==============================================================================

    RT_PresetManager.h
    Created: 9 Apr 2022 12:04:07pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../DSPStateManagers/RT_ParameterDefines.h"
#include "../Interface/RT_ProcessorInterface.h"
#include <JuceHeader.h>

static const juce::String sc_PresetSuffix{".rtstftpreset"};

class RT_PresetManager {
public:
  RT_PresetManager(RT_ProcessorInterface *inInterface);
  void        writePresetToDisk(juce::String inPresetName);
  void        writePresetToDisk(juce::File inPresetPath);
  void        getPreset(juce::MemoryBlock &inDestData);
  void        storePresetInMemory(juce::MemoryBlock &inMem);
  void        storePresetInMemory(const void *inData, int inSize);

  void        loadPreset(juce::MemoryBlock *inMemPtr = nullptr);
  bool        loadPresetFromDisk(juce::File presetFile);

  int         getXmlBlockSize();
  const void *getManipsBinaryPointer();

  friend class RT_ProcessorBase;

private:
  RT_ProcessorInterface *mInterface;
  juce::MemoryBlock      mActivePresetRawData;
  juce::File             mActivePresetPath;
  void                   _storeCurrentStateInMemory();
  void                   _loadPresetInternal();
};