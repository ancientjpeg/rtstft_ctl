/*
  ==============================================================================

    RT_ParameterManager.cpp
    Created: 18 Mar 2022 1:57:25pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_ParameterManager.h"
#include "RTSTFT_Manager.h"

RT_ParameterManager::RT_ParameterManager(RT_ProcessorInterface *inInterface)
    : mInterface(inInterface)
{
  std::vector<std::unique_ptr<juce::AudioParameterFloat>> parameters;

  RTSTFT_Manager *rtstft_obj = mInterface->getRTSTFTManager();
  for (int i = 0; i < NUM_PARAMS; i++) {
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        RT_PARAM_IDS[i], RT_PARAM_IDS[i], RT_PARAM_RANGES[i],
        RT_PARAM_DEFAULTS[i]));
  }
  mValueTreeState.reset(new juce::AudioProcessorValueTreeState(
      *mInterface->getProcessor(), nullptr, "PARAMETER_TREE",
      {parameters.begin(), parameters.end()}));
  for (int i = 0; i < NUM_PARAMS; i++) {
    mValueTreeState->addParameterListener(RT_PARAM_IDS[i], rtstft_obj);
  }
}
RT_ParameterManager::~RT_ParameterManager() {}
juce::AudioProcessorValueTreeState *RT_ParameterManager::getValueTree()
{
  return mValueTreeState.get();
}
