/*
  ==============================================================================

    RT_PresetManager.cpp
    Created: 9 Apr 2022 12:04:07pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_PresetManager.h"
#include "../DSPStateManagers/RTSTFT_Manager.h"
#include "../DSPStateManagers/RT_ParameterDefines.h"
#include "../DSPStateManagers/RT_ParameterManager.h"
#include "RT_PropertyManager.h"

RT_PresetManager::RT_PresetManager(RT_ProcessorInterface *inInterface)
    : mInterface(inInterface), mActivePresetValueTree(), mActivePresetRawData(0)
{
}

void RT_PresetManager::storePresetInMemory(juce::MemoryBlock &inMem)
{
  mActivePresetRawData = inMem;
}
bool RT_PresetManager::loadPreset(juce::MemoryBlock *inMemPtr)
{
  if (inMemPtr != nullptr) {
    storePresetInMemory(*inMemPtr);
  }
  void *data = mActivePresetRawData.getData();
  assert(data != nullptr);

  auto state = juce::AudioProcessor::getXmlFromBinary(
      data, (int)mActivePresetRawData.getSize());
  mInterface->getParameterManager()->getValueTreeState()->replaceState(
      juce::ValueTree::fromXml(*state->getChildByName("PARAMETER_TREE")));

  auto propertiesXml = state->getChildByName("rtstft_ctl_properties");
  if (!propertiesXml) {
    return false;
  }
  auto  propertyTree = juce::ValueTree::fromXml(*propertiesXml);

  auto *rt_man       = mInterface->getRTSTFTManager();
  auto *prop_man     = mInterface->getPropertyManager();

  if (!prop_man->assertTreeCanValidlyReplace(propertyTree)) {
    return false; // keep params, but don't overwrite manips or FFT size
  }
  prop_man->replaceState(propertyTree);

  rt_man->readManipsFromBinary(false);
  return true;
}
int RT_PresetManager::getXmlBlockSize()
{
  void *data    = mActivePresetRawData.getData();
  int xmlOffset = ((uint32_t *)data)[1] + 9; // includes magic number, size int,
  // and trailing nullterm in XML binary
  return xmlOffset;
}