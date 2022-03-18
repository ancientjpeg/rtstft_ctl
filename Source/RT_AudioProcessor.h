/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RT_ProcessorBase.h"


//==============================================================================
/**
*/
class RT_AudioProcessor  : public RT_ProcessorBase
{
public:
    //==============================================================================
    RT_AudioProcessor();
    ~RT_AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
  
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RT_AudioProcessor)
};
