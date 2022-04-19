/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "RT_AudioProcessor.h"

//==============================================================================
RT_AudioProcessor::RT_AudioProcessor()
{
  assert(getChannelCountOfBus(true, 0) == getChannelCountOfBus(false, 0));
}

RT_AudioProcessor::~RT_AudioProcessor() {}

//==============================================================================
void RT_AudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
  DBG(rt_dbtoa(-6));
  mRTSTFTManager.prepareToPlay(sampleRate, samplesPerBlock);
  verifyStateIsUpToDate();
}

void RT_AudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                     juce::MidiBuffer         &midiMessages)
{
  juce::ScopedNoDenormals noDenormals;
  verifyStateIsUpToDate();
  if (!isSuspended()) {
    mRTSTFTManager.processBlock(buffer);
  }
}

void RT_AudioProcessor::releaseResources()
{
  mRTSTFTManager.releaseResources();
}
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
  return new RT_AudioProcessor();
}
