/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "Components/RT_AppWindow.h"
#include "RT_ProcessorBase.h"
#include <JuceHeader.h>

//==============================================================================
/**
 */
class RT_ProcessorEditor : public juce::AudioProcessorEditor {

public:
  RT_ProcessorEditor(RT_ProcessorInterface *inInterface);
  ~RT_ProcessorEditor() override;

  //==============================================================================
  void paint(juce::Graphics &) override;
  void resized() override;

private:
  RT_ProcessorInterface *mInterface;
  RT_AppWindow           mAppWindow;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_ProcessorEditor)
};
