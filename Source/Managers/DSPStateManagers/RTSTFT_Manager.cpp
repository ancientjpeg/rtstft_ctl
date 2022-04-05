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
  resetParamsStruct();
}
RTSTFT_Manager::~RTSTFT_Manager()
{
  if (mInitialized) {
    rt_clean(p);
    p = NULL;
  }
}
void RTSTFT_Manager::resetParamsStruct(int inNumChans, float inSampleRate,
                                       int inSamplesPerBlock, int inFFTSize,
                                       int inOverlapFactor)
{
  // this needs some work
  if (mInitialized) {
    mInitialized = false;
    rt_clean(p);
    p = NULL;
  }
  mCurrentSampleRate  = inSampleRate;
  mNumChannels        = inNumChans;
  int samplesPerBlock = mCurrentSamplesPerBlock
      = RT_Utilities::getNearestPowerOfTwo(inSamplesPerBlock);
  p = rt_init(mNumChannels, inFFTSize, mCurrentSamplesPerBlock, inOverlapFactor,
              0, mCurrentSampleRate);
  p->listener  = {(void *)this, &RTSTFT_CMDListenerCallback};
  mInitialized = true;
}

const rt_params RTSTFT_Manager::getParamsStruct() { return p; }

void RTSTFT_Manager::prepareToPlay(double inSampleRate, int inSamplesPerBlock)
{
  int numChannels = mInterface->getProcessor()->getChannelCountOfBus(true, 0);
  if (!mInitialized) {
    resetParamsStruct(numChannels, inSampleRate, inSamplesPerBlock);
  }
  assert(inSamplesPerBlock < mCurrentSamplesPerBlock);
  if (inSampleRate != p->sample_rate) {
    p->sample_rate = inSampleRate;
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

void RTSTFT_Manager::executeCMDCommand(juce::String inCMDString)
{
  mCMDErrorState = rt_parse_and_execute(p, inCMDString.toRawUTF8());
  mCMDMessage    = juce::String(p->parser.error_msg_buffer);
}

int          RTSTFT_Manager::getCMDErrorState() { return mCMDErrorState; }
juce::String RTSTFT_Manager::getCMDMessage() { return mCMDMessage; }

std::unique_ptr<juce::XmlElement> RTSTFT_Manager::serializeParamsStruct()
{
  assert(p != NULL);
  std::unique_ptr<juce::XmlElement> p_xml
      = std::make_unique<juce::XmlElement>("rt_params");
  juce::XmlElement *chans_xml = new juce::XmlElement("chans");
  p_xml->addChildElement(chans_xml);
  for (int i = 0; i < p->num_chans; i++) {
    rt_chan           c = p->chans[i];
    juce::XmlElement *c_xml
        = new juce::XmlElement(juce::String("c") + juce::String(i));
    chans_xml->addChildElement(c_xml);
    c_xml->setAttribute(
        "manips", RT_Utilities::serializeFloats(c->manip->hold_manips,
                                                rt_manip_block_len(p) * 4));
  }
  return p_xml;
}

void RTSTFT_Manager::deserializeParamsStruct(juce::XmlElement *p_xml)
{
  assert(p != NULL);
  for (int i = 0; i < p->num_chans; i++) {
    rt_chan c     = p->chans[i];
    auto    c_xml = p_xml->getChildByName("chans")->getChildByName(
           juce::String("c") + juce::String(i));
    juce::String manips_b64 = c_xml->getStringAttribute("manips");
    auto         mem
        = RT_Utilities::deserializeFloats(manips_b64, rt_manip_block_len(p));
    rt_manip_overwrite_manips(p, c, (rt_real *)mem.getData(),
                              rt_manip_block_len(p));
  }
}

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
