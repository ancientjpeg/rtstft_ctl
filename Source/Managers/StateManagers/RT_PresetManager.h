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

const static juce::ValueTree scDefaultPresetXML = {
    "",
    {},
    {
        {"rtstft_ctl_properties",
         {
             {RT_FFT_MODIFIER_IDS[RT_FFT_MODIFIER_FRAME_SIZE], (int)1024},
             {RT_FFT_MODIFIER_IDS[RT_FFT_MODIFIER_OVERLAP_FACTOR], (int)8},
             {RT_FFT_MODIFIER_IDS[RT_FFT_MODIFIER_PAD_FACTOR], (int)0},
             {"manip_multichannel",
              RT_MULTICHANNEL_MODE_IDS[RT_MULTICHANNEL_MONO]},
         },
         {juce::ValueTree("rt_chans", {}, {}),
          juce::ValueTree("rt_gui_state",
                          {{"active_manip", ""}, {"active_chan", "0"}}, {})}},

    }};

class RT_PresetManager {
public:
  RT_PresetManager(RT_ProcessorInterface *inInterface);
  void storePresetInMemory(juce::MemoryBlock &inMem);
  bool loadPreset(juce::MemoryBlock *inMemPtr = nullptr);

  int  getXmlBlockSize();

private:
  RT_ProcessorInterface *mInterface;
  juce::ValueTree        mActivePresetValueTree;
  juce::MemoryBlock      mActivePresetRawData;
};