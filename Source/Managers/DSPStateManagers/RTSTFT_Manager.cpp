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
    : mInterface(inInterface), p(NULL), mFFTSetterThread(mInterface),
      mCurrentSamplesPerBlock(2048), mCurrentSampleRate(0)
{
  mNumChannels = mInterface->getProcessor()->getChannelCountOfBus(true, 0);
  resetParamsStruct();
}

RTSTFT_Manager::~RTSTFT_Manager()
{
  if (mInitialized) {
    rt_clean(p);
    p = NULL; // superfluous but always a good practice
  }
}

void RTSTFT_Manager::prepareToPlay(double inSampleRate, int inSamplesPerBlock)
{
  mCurrentSampleRate = inSampleRate;
  auto checkSamplesPerBlock
      = RT_Utilities::getNearestPowerOfTwo(inSamplesPerBlock);
  int numChannels = mInterface->getProcessor()->getChannelCountOfBus(true, 0);
  if (inSamplesPerBlock < mCurrentSamplesPerBlock) {
    mCurrentSamplesPerBlock = inSamplesPerBlock;
    mInitialized            = false;
    resetParamsStruct();
  }
  if (inSampleRate != p->sample_rate) {
    rt_set_sample_rate(p, inSampleRate);
  }
}

void RTSTFT_Manager::processBlock(juce::AudioBuffer<float> &buffer)
{
  rt_cycle(p, buffer.getArrayOfWritePointers(), buffer.getNumSamples());
}

void RTSTFT_Manager::releaseResources() { rt_flush(p); }

void RTSTFT_Manager::resetParamsStruct(int inFFTSize, int inOverlapFactor)
{
  if (!mInitialized || mNumChannels != p->num_chans) {
    if (p != NULL) {
      rt_clean(p);
    }
    p = rt_init(mNumChannels, inFFTSize, mCurrentSamplesPerBlock,
                inOverlapFactor, 0, mCurrentSampleRate);
  }
  p->listener  = {(void *)this, &RTSTFT_CMDListenerCallback};
  mInitialized = true;
}

const rt_params RTSTFT_Manager::getParamsStruct() { return p; }

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

void RTSTFT_Manager::executeCMDCommand(juce::String inCMDString)
{
  mCMDErrorState = rt_parse_and_execute(p, inCMDString.toRawUTF8());
  mCMDMessage    = juce::String(p->parser.error_msg_buffer);
}

int          RTSTFT_Manager::getCMDErrorState() { return mCMDErrorState; }
juce::String RTSTFT_Manager::getCMDMessage() { return mCMDMessage; }

void         RTSTFT_Manager::readManipsFromBinary()
{
  assert(mInitialized);
  auto        processor = (RT_ProcessorBase *)(mInterface->getProcessor());
  const void *manipsBinaryPtr = processor->getManipsBinaryPointer();
  assert(manipsBinaryPtr != nullptr);
  int magicNumber = *(int32_t *)manipsBinaryPtr;
  if (magicNumber != ManipsBinaryMagicNumber) {
    return;
  }
  void *ptr              = (float *)manipsBinaryPtr + 4;

  int   newFFTSize       = 2048; // TEST
  int   newPadFactor     = 0;
  int   newOverlapFactor = 8;
  // to prevent bad reads from older builds
  if (newFFTSize > p->fft_max) {
    return;
  }
  if (newFFTSize != p->fft_size) {
    changeFFTSize(newFFTSize, newOverlapFactor, newPadFactor);
  }
  rt_manip_overwrite_manips(p, p->chans[0], (rt_real *)ptr,
                            rt_manip_block_len(p));
  // for (int i = 0; i < p->num_chans; i++) {
  //   rt_manip_overwrite_manips(p, p->chans[i], );
  // }
}

void RTSTFT_Manager::writeManipsToFile(juce::MemoryOutputStream &stream)
{
  stream.writeInt(ManipsBinaryMagicNumber);
  rt_uint i;
  for (i = 0; i < p->num_chans; i++) {
    stream.write(p->chans[i]->manip->manips,
                 rt_manip_block_len(p) * sizeof(float));
  }
}

void RTSTFT_Manager::changeFFTSize(int inNewFFTSize, int inNewOverlapFactor,
                                   int inNewPadFactor)
{
  assert(mInitialized);
  mThreadFFTSize       = inNewFFTSize;
  mThreadOverlapFactor = inNewOverlapFactor;
  mThreadPadFactor     = inNewPadFactor;
  mInterface->getProcessor()->suspendProcessing(true);
  mFFTSetterThread.run();
}

void RTSTFT_Manager::changeFFTSizeInternal()
{
  if (!juce::isPowerOfTwo(mThreadFFTSize) || mThreadFFTSize > p->fft_max
      || !mInitialized) {
    mInterface->getProcessor()->suspendProcessing(false);
    return;
  }
  rt_set_fft_size(p, mThreadFFTSize, mThreadOverlapFactor, mThreadPadFactor);
  rt_update_fft_size(p);
  mInterface->getProcessor()->suspendProcessing(false);
}

//=============================================================================

void RTSTFT_Manager::RTSTFT_ManagerCMDCallback(rt_listener_return_t const info)
{
  if (info.param_flavor != RT_PARAM_FLAVOR_UNDEFINED) {
    mLastUpdateWasCMD = true;
    auto param
        = mInterface->getParameterManager()->getValueTreeState()->getParameter(
            RT_PARAM_IDS[info.param_flavor]);
    param->setValueNotifyingHost(info.param_value);
  }
  else if (info.manip_flavor != RT_MANIP_FLAVOR_UNDEFINED) {
    mListenerList.call(
        [&info](Listener &l) { l.onManipChanged(info.manip_flavor); });
  }
}

void RTSTFT_CMDListenerCallback(void                      *RTSTFTManagerPtr,
                                rt_listener_return_t const info)
{
  ((RTSTFT_Manager *)RTSTFTManagerPtr)->RTSTFT_ManagerCMDCallback(info);
}

void RTSTFT_Manager::addListener(Listener *l) { mListenerList.add(l); }
