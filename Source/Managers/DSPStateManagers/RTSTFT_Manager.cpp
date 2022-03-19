/*
  ==============================================================================

    RTSTFT_Manager.cpp
    Created: 19 Mar 2022 2:08:48pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RTSTFT_Manager.h"
#include "../../RT_AudioProcessor.h"

RTSTFT_Manager::RTSTFT_Manager(RT_ProcessorInterface *mInterface) : p(NULL) {}
RTSTFT_Manager::~RTSTFT_Manager()
{
  if (p != NULL) {
    rt_clean(p);
  }
}
rt_params RTSTFT_Manager::getParamsStruct() {}
void RTSTFT_Manager::prepareToPlay(double inSampleRate, int inSamplesPerBlock)
{
  if (!initialized || inSamplesPerBlock > mCurrentSamplesPerBlock
      || inSampleRate != mCurrentSampleRate) {
    int samplesPerBlock = inSamplesPerBlock;
    int powerOfTwoCheck = (samplesPerBlock & (samplesPerBlock - 1));
    if (powerOfTwoCheck != 0) {
      int  nextHighestPower = sizeof(samplesPerBlock) - 1;
      bool found;
      while (!found && nextHighestPower-- > 1) {
        if (samplesPerBlock & (1 << nextHighestPower) == 1) {
          nextHighestPower++;
          break;
        }
      }
      samplesPerBlock = 1 << nextHighestPower;
    }
    mCurrentSamplesPerBlock = samplesPerBlock;
    mCurrentSampleRate      = inSampleRate;

    int numChannels = mInterface->getProcessor()->getChannelCountOfBus(true, 0);
    p = rt_init(numChannels, 512, samplesPerBlock, 4, 0, mCurrentSampleRate);
  }
}
void RTSTFT_Manager::processBlock(juce::AudioBuffer<float> &buffer) {}