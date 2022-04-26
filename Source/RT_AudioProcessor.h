/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "RT_ProcessorBase.h"
#include <JuceHeader.h>

//==============================================================================
/**
 */
class RT_AudioProcessor : public RT_ProcessorBase {
public:
  //==============================================================================
  RT_AudioProcessor();
  ~RT_AudioProcessor() override;

  //==============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

  void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

  juce::AudioBuffer<float>
       readBufferToMidSide(juce::AudioBuffer<float> &buffer);
  void writeMidSideToBuffer(juce::AudioBuffer<float> &buffer,
                            juce::AudioBuffer<float> &midsideBuf);

private:
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_AudioProcessor)
};
