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
#include "../StateManagers/RT_PropertyManager.h"
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
  else if (mInitialized
           && (inFFTSize != p->fft_size
               || inOverlapFactor != p->overlap_factor)) {
    changeFFTSize(inFFTSize, inOverlapFactor);
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
  // else if (paramFlavor >= RT_PARAM_GAIN_MOD
  //          && paramFlavor <= RT_PARAM_LIMIT_MOD) {
  //   if (paramFlavor == RT_PARAM_GATE_MOD) {
  //     newValue -= 1.f;
  //   }
  // }
  rt_set_single_param(p, (rt_param_flavor_t)paramFlavor, newValue);
}

void RTSTFT_Manager::valueTreePropertyChanged(
    juce::ValueTree        &treeWhosePropertyHasChanged,
    const juce::Identifier &property)
{
  juce::var val = treeWhosePropertyHasChanged[property];

  // this is so so so dumb
  if (property
      == (juce::Identifier)RT_FFT_MODIFIER_IDS[RT_FFT_MODIFIER_FRAME_SIZE]) {
    changeFFTSize((int)val, p->overlap_factor, p->pad_factor);
  }
  else if (property
           == (juce::Identifier)
               RT_FFT_MODIFIER_IDS[RT_FFT_MODIFIER_OVERLAP_FACTOR]) {
    changeFFTSize(p->frame_size, (int)val, p->pad_factor);
  }
  else if (property
           == (juce::Identifier)
               RT_FFT_MODIFIER_IDS[RT_FFT_MODIFIER_PAD_FACTOR]) {
    changeFFTSize(p->frame_size, p->overlap_factor, (int)val);
  }
}

void RTSTFT_Manager::executeCMDCommand(juce::String inCMDString)
{
  mCMDErrorState = rt_parse_and_execute(p, inCMDString.toRawUTF8());
  mCMDMessage    = juce::String(p->parser.error_msg_buffer);
}

int          RTSTFT_Manager::getCMDErrorState() { return mCMDErrorState; }
juce::String RTSTFT_Manager::getCMDMessage() { return mCMDMessage; }

void         RTSTFT_Manager::readManipsFromBinary(bool inThreadedFFTUpdate)
{
  assert(mInitialized);
  auto        processor = (RT_ProcessorBase *)(mInterface->getProcessor());
  const void *manipsBinaryPtr = processor->getManipsBinaryPointer();
  assert(manipsBinaryPtr != nullptr);
  int magicNumber = *(int32_t *)manipsBinaryPtr;
  if (magicNumber != ManipsBinaryMagicNumber) {
    return;
  }
  void *ptr = (char *)manipsBinaryPtr + 4;

  rt_manip_overwrite_manips(p, p->chans[0], (rt_real *)ptr,
                            rt_manip_block_len(p));
  int to_chan   = p->manip_multichannel ? p->num_chans : 1;
  int block_len = rt_manip_block_len(p);
  for (int i = 0; i < to_chan; i++) {
    rt_manip_overwrite_manips(p, p->chans[0], (rt_real *)ptr + block_len * i,
                              block_len);
  }
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

void RTSTFT_Manager::changeFFTSize(int inNewFrameSize, int inNewOverlapFactor,
                                   int inNewPadFactor, bool threaded)
{
  assert(juce::isPowerOfTwo(inNewFrameSize)
         && inNewFrameSize * (1 << inNewPadFactor) <= p->fft_max
         && mInitialized);

  rt_set_fft_size(p, inNewFrameSize, inNewOverlapFactor, inNewPadFactor);
  if (threaded) {
    mFFTSetterThread.run();
  }
  else {
    changeFFTSizeInternal();
  }
}

void RTSTFT_Manager::changeFFTSizeInternal()
{
  mInterface->getProcessor()->suspendProcessing(true);
  rt_update_fft_size(p);
  mInterface->getProcessor()->suspendProcessing(false);
}

void RTSTFT_Manager::awaitFFTSizeChange()
{
  // just crash  out on an unexpectedly long wait time
  assert(mFFTSetterThread.waitForThreadToExit(5000));
}

//=============================================================================

void RTSTFT_Manager::RTSTFT_ManagerCMDCallback(rt_listener_return_t const info)
{
  if (info.param_flavor != RT_PARAM_FLAVOR_UNDEFINED) {
    //    auto param
    //        =
    //        mInterface->getParameterManager()->getValueTreeState()->getParameter(
    //            RT_PARAM_IDS[info.param_flavor]);
    //    param->setValueNotifyingHost(info.param_value); // only needed if i
    //    ever put the knob params in rt_cmd
  }
  else if (info.manip_flavor != RT_MANIP_FLAVOR_UNDEFINED) {
    //    mListenerList.call(
    //        [&info](Listener &l) { l.onManipChanged(info.manip_flavor); }); //
    //        also unecessary now
  }
}

void RTSTFT_CMDListenerCallback(void                      *RTSTFTManagerPtr,
                                rt_listener_return_t const info)
{
  ((RTSTFT_Manager *)RTSTFTManagerPtr)->RTSTFT_ManagerCMDCallback(info);
}

void RTSTFT_Manager::addListener(Listener *l) { mListenerList.add(l); }
