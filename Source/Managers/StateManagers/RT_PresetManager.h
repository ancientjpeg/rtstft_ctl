/*
  ==============================================================================

    RT_PresetManager.h
    Created: 9 Apr 2022 12:04:07pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../Interface/RT_ProcessorInterface.h"
#include <JuceHeader.h>
class RT_PresetManager {
public:
  RT_PresetManager(RT_ProcessorInterface *inInterface);

private:
  RT_ProcessorInterface *mInterface;
  juce::ValueTree        mActivePreset;
};