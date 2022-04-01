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
  if (mInitialized) {
    rt_clean(p);
  }
}
const rt_params RTSTFT_Manager::getParamsStruct() { return p; }
void RTSTFT_Manager::prepareToPlay(double inSampleRate, int inSamplesPerBlock)
{
  if (mCurrentSampleRate != inSampleRate) {
    mCurrentSampleRate = inSampleRate;
    if (mInitialized) {
      rt_set_sample_rate(p, mCurrentSampleRate);
    }
  }
  int numChannels = mInterface->getProcessor()->getChannelCountOfBus(true, 0);
  if (!mInitialized || inSamplesPerBlock >= mCurrentSamplesPerBlock
      || mNumChannels != numChannels) {
    if (mInitialized) {
      mInitialized = false;
      rt_clean(p);
      p = NULL;
    }
    mNumChannels        = numChannels;
    int samplesPerBlock = RT_Utilities::getNearestPowerOfTwo(inSamplesPerBlock);
    mCurrentSamplesPerBlock = mCurrentSamplesPerBlock < samplesPerBlock
                                  ? samplesPerBlock
                                  : mCurrentSamplesPerBlock;
    p            = rt_init(mNumChannels, 2048, mCurrentSamplesPerBlock, 4, 0,
                           mCurrentSampleRate);
    p->listener  = {(void *)this, &RTSTFT_CMDListenerCallback};
    mInitialized = true;
  }
}
void RTSTFT_Manager::processBlock(juce::AudioBuffer<float> &buffer)
{
  rt_cycle(p, buffer.getArrayOfWritePointers(), buffer.getNumSamples());
}

void RTSTFT_Manager::releaseResources() { rt_flush(p); }

void RTSTFT_Manager::parameterChanged(const juce::String &parameterID,
                                      float               newValue)
{
  int paramFlavor = RT_PARAM_IDS.indexOf(parameterID);
  if (paramFlavor < 0 || paramFlavor >= RT_PARAM_FLAVOR_COUNT) {
    // error handling here...
  }
  if (!mLastUpdateWasCMD) {
    // this is stupid and dumb as it's not guarunteed threadsafe (i.e. incoming
    // value could be different from the CMD-derived value) I just have it here
    // as a reminder in case things go wrong that I might need a more foolproof
    // method of setting the Valuetree i.e., DELET THIS
    rt_set_single_param(p, (rt_param_flavor_t)paramFlavor, newValue);
  }
  else {
    mLastUpdateWasCMD = false;
  }
}

void RTSTFT_Manager::addListener(Listener *l) { mListenerList.add(l); }

void RTSTFT_Manager::textEditorReturnKeyPressed(juce::TextEditor &t)
{
  executeCMDCommand(t.getText());
}

void RTSTFT_Manager::executeCMDCommand(juce::String inCMDString)
{
  mCMDErrorState = rt_parse_and_execute(p, inCMDString.toRawUTF8());
  mCMDMessage    = p->parser.error_msg_buffer;
}

void RTSTFT_Manager::RTSTFT_ManagerCMDCallback(rt_listener_return_t const info)
{
  mLastUpdateWasCMD = true;
  if (info.param_flavor != RT_PARAM_FLAVOR_UNDEFINED) {
    auto param
        = mInterface->getParameterManager()->getValueTree()->getParameter(
            RT_PARAM_IDS[info.param_flavor]);
    param->setValueNotifyingHost(info.param_value);
  }
  else if (info.manip_flavor != RT_MANIP_FLAVOR_UNDEFINED) {
    // notify FFT window
  }
}

void RTSTFT_CMDListenerCallback(void                      *RTSTFTManagerPtr,
                                rt_listener_return_t const info)
{
  ((RTSTFT_Manager *)RTSTFTManagerPtr)->RTSTFT_ManagerCMDCallback(info);
}
