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
  mRTSTFTManager.prepareToPlay(sampleRate, samplesPerBlock);
  verifyStateIsUpToDate();
}

void RT_AudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                     juce::MidiBuffer         &midiMessages)
{
  juce::ScopedNoDenormals noDenormals;
  verifyStateIsUpToDate();
  if (!isSuspended()) {
    auto ms_buf = readBufferToMidSide(buffer);
    mRTSTFTManager.processBlock(ms_buf);
    writeMidSideToBuffer(buffer, ms_buf);
  }
}

juce::AudioBuffer<float>
RT_AudioProcessor::readBufferToMidSide(juce::AudioBuffer<float> &buffer)
{
  juce::AudioBuffer<float> midsideBuf(2, buffer.getNumSamples());
  for (int i = 0; i < buffer.getNumSamples(); i++) {
    float l = buffer.getSample(0, i);
    float r = buffer.getSample(1, i);
    float m = (l + r) * 0.5;
    float s = (l - r) * 0.5;
    midsideBuf.setSample(0, i, m);
    midsideBuf.setSample(1, i, s);
  }
  return midsideBuf;
}

void RT_AudioProcessor::writeMidSideToBuffer(
    juce::AudioBuffer<float> &buffer, juce::AudioBuffer<float> &midsideBuf)
{
  for (int i = 0; i < buffer.getNumSamples(); i++) {
    float m = midsideBuf.getSample(0, i);
    float s = midsideBuf.getSample(1, i);
    float l = m + s;
    float r = m - s;
    buffer.setSample(0, i, l);
    buffer.setSample(1, i, r);
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
