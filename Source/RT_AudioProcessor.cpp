/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "RT_AudioProcessor.h"

//==============================================================================
RT_AudioProcessor::RT_AudioProcessor()
{
  int numChannels = getChannelCountOfBus(true, 0);
  if (numChannels != getChannelCountOfBus(false, 0)) {
    DBG("BAD CHANNEL COUNTS!");
    exit(1);
  }
}

RT_AudioProcessor::~RT_AudioProcessor() {}

//==============================================================================
void RT_AudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
  getRTSTFTManager()->prepareToPlay(sampleRate, samplesPerBlock);
}

void RT_AudioProcessor::releaseResources()
{
  getRTSTFTManager()->releaseResources();
}

void RT_AudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                     juce::MidiBuffer         &midiMessages)
{
  juce::ScopedNoDenormals noDenormals;
  getRTSTFTManager()->processBlock(buffer);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
  return new RT_AudioProcessor();
}
