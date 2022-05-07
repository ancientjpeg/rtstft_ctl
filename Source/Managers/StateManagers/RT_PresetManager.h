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

class RT_PresetManager {
public:
  RT_PresetManager(RT_ProcessorInterface *inInterface);
  void        storePresetInMemory(juce::MemoryBlock &inMem);
  void        storePresetInMemory(const void *inData, int inSize);
  void        writePresetToDisk(juce::String inPresetName);
  bool        loadPreset(juce::MemoryBlock *inMemPtr = nullptr);
  void        getPreset(juce::MemoryBlock &inDestData);

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

private:
  RT_ProcessorInterface *mInterface;
  juce::MemoryBlock      mActivePresetRawData;
  Tree                   mPresetsTree;
  void                   _storeCurrentStateInMemory();
};