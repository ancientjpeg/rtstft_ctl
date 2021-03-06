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

  for (int i = 0; i < RT_PARAM_FLAVOR_COUNT; i++) {
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        RT_PARAM_IDS[i], RT_PARAM_NAMES[i], RT_PARAM_RANGES[i],
        RT_PARAM_DEFAULTS[i]));
  }
  mValueTreeState.reset(new juce::AudioProcessorValueTreeState(
      (juce::AudioProcessor &)*mInterface->getProcessor(), nullptr,
      "PARAMETER_TREE", {parameters.begin(), parameters.end()}));
}
RT_ParameterManager::~RT_ParameterManager() {}
juce::AudioProcessorValueTreeState *RT_ParameterManager::getValueTreeState()
{
  return mValueTreeState.get();
}
