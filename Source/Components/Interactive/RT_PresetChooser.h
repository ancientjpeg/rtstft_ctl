/*
  ==============================================================================

    RT_PresetChooser.h
    Created: 3 May 2022 8:28:30am
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once
#include "../../Managers/Interface/RT_ProcessorInterface.h"
#include "../../Managers/OSManagers/RT_FileManager.h"
#include "../../Managers/StateManagers/RT_PresetManager.h"
#include <JuceHeader.h>

class RT_PresetChooser {
public:
  RT_PresetChooser(RT_ProcessorInterface *inInterface);

private:
  juce::Label            mCurrentPreset;
  juce::TextButton       mLoadButton, mSaveButton;
  juce::FileChooser      mPresetFileChooser;
  RT_ProcessorInterface *mInterface;
};