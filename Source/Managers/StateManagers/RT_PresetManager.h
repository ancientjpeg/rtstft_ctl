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

  class Tree {
  public:
    Tree(juce::File inPresetsRoot);
    juce::File findPresetFile(juce::String inPresetName);
    bool       getPresetData(juce::MemoryBlock &inWriteableBlock);

    class Comparator {
    public:
      static int compareElements(juce::File &f0, juce::File &f1);
    };

  private:
    juce::Array<juce::File> mPresetPaths;
  };

  friend class RT_ProcessorBase;

private:
  RT_ProcessorInterface *mInterface;
  juce::MemoryBlock      mActivePresetRawData;
  Tree                   mPresetsTree;
  void                   _storeCurrentStateInMemory();
  void                   _loadPresetInternal();
};