/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RT_ProcessorBase.h"

//==============================================================================
/**
*/
class RT_ProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    RT_ProcessorEditor (RT_ProcessorBase&);
    ~RT_ProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    RT_ProcessorBase& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RT_ProcessorEditor)
};
