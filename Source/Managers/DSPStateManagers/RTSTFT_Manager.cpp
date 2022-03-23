/*
  ==============================================================================

    RTSTFT_Manager.cpp
    Created: 19 Mar 2022 2:08:48pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RTSTFT_Manager.h"
#include "../../RT_AudioProcessor.h"
#include "../../Utilities/RT_MathUtilities.h"
#include "RT_ParameterDefines.h"
#include "RT_ParameterManager.h"

RTSTFT_Manager::RTSTFT_Manager(RT_ProcessorInterface *inInterface)
    : mInterface(inInterface), p(NULL), mCurrentSamplesPerBlock(0)
{
}
RTSTFT_Manager::~RTSTFT_Manager()
{
  if (initialized) {
    rt_clean(p);
  }
}
const rt_params RTSTFT_Manager::getParamsStruct() { return p; }
void RTSTFT_Manager::prepareToPlay(double inSampleRate, int inSamplesPerBlock)
{
  if (mCurrentSampleRate != inSampleRate) {
    mCurrentSampleRate = inSampleRate;
    if (initialized) {
      rt_set_sample_rate(p, mCurrentSampleRate);
    }
  }
  int numChannels = mInterface->getProcessor()->getChannelCountOfBus(true, 0);
  if (!initialized || inSamplesPerBlock >= mCurrentSamplesPerBlock
      || mNumChannels != numChannels) {
    if (initialized) {
      initialized = false;
      rt_clean(p);
      p = NULL;
    }
    mNumChannels        = numChannels;
    int samplesPerBlock = RT_Utilities::getNearestPowerOfTwo(inSamplesPerBlock);
    mCurrentSamplesPerBlock = mCurrentSamplesPerBlock < samplesPerBlock
                                  ? samplesPerBlock
                                  : mCurrentSamplesPerBlock;
    p           = rt_init(mNumChannels, 2048, mCurrentSamplesPerBlock, 4, 0,
                          mCurrentSampleRate);
    p->listener = {(void *)this, &RTSTFTListenerCallback};
    initialized = true;
  }
}
void RTSTFT_Manager::processBlock(juce::AudioBuffer<float> &buffer)
{
  rt_cycle(p, buffer.getArrayOfWritePointers(), buffer.getNumSamples());
//  for (int i = 0; i < buffer.getNumChannels(); i++) {
//    rt_cycle_chan(p, i, buffer.getWritePointer(i), buffer.getNumSamples());
//  }
}

void RTSTFT_Manager::releaseResources() { rt_flush(p); }

void RTSTFT_Manager::parameterChanged(const juce::String &parameterID,
                                      float               newValue)
{
  const juce::ScopedReadLock paramUpdateLockChec(mRTSTFTParamsUpdateLock);
  int                        paramFlavor = RT_PARAM_IDS.indexOf(parameterID);
  if (paramFlavor < 0 || paramFlavor >= RT_PARAM_FLAVOR_COUNT) {
    // error handling here...
  }
  rt_set_single_param(p, (rt_param_flavor)paramFlavor, newValue);
}

void RTSTFTListenerCallback(void *RTSTFTManagerPtr)
{
  RTSTFT_Manager             *ptr = (RTSTFT_Manager *)RTSTFTManagerPtr;
  const juce::ScopedWriteLock paramRealUpdateLock(ptr->mRTSTFTParamsUpdateLock);
  auto tree = ptr->mInterface->getParameterManager()->getValueTree();
  for (int i = 0; i < RT_PARAM_FLAVOR_COUNT; i++) {
    *(tree->getRawParameterValue(RT_PARAM_IDS[i])) = 1.f;
  }
}
