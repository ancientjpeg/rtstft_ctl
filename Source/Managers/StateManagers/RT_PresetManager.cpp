/*
  ==============================================================================

    RT_PresetManager.cpp
    Created: 9 Apr 2022 12:04:07pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_PresetManager.h"
#include "../../RT_AudioProcessor.h"
#include "../../Utilities/RT_FileTree.h"
#include "../DSPStateManagers/RTSTFT_Manager.h"
#include "../DSPStateManagers/RT_ParameterDefines.h"
#include "../DSPStateManagers/RT_ParameterManager.h"
#include "../OSManagers/RT_FileManager.h"
#include "RT_PropertyManager.h"

RT_PresetManager::RT_PresetManager(RT_ProcessorInterface *inInterface)
    : mInterface(inInterface), mActivePresetRawData(0)
{
  _storeCurrentStateInMemory();
  auto presetRoot = mInterface->getFileManager()->getPresetsDirectory();
  if (presetRoot.getNumberOfChildFiles(7, "default.rtstftpreset") <= 0) {
    writePresetToDisk("default.rtstftpreset");
  }
}

void RT_PresetManager::storePresetInMemory(juce::MemoryBlock &inMem)
{
  mActivePresetRawData = inMem;
}

void RT_PresetManager::storePresetInMemory(const void *inData, int inSize)
{
  mActivePresetRawData.setSize(inSize);
  mActivePresetRawData.copyFrom(inData, 0, inSize);
}
void RT_PresetManager::loadPreset(juce::MemoryBlock *inMemPtr)
{
  if (inMemPtr != nullptr) {
    storePresetInMemory(*inMemPtr);
  }
  mInterface->getRTProcessor()->notifyOfStateChange();
}
void RT_PresetManager::_loadPresetInternal()
{
  void *data = mActivePresetRawData.getData();
  assert(data != nullptr);

  auto state = juce::AudioProcessor::getXmlFromBinary(
      data, (int)mActivePresetRawData.getSize());
  mInterface->getParameterManager()->getValueTreeState()->replaceState(
      juce::ValueTree::fromXml(*state->getChildByName("PARAMETER_TREE")));

  auto propertiesXml = state->getChildByName("rtstft_ctl_properties");
  juce::ValueTree propertyTree;
  if (propertiesXml) {
    propertyTree = juce::ValueTree::fromXml(*propertiesXml);
  }

  auto *prop_man = mInterface->getPropertyManager();

  if (prop_man->assertTreeCanValidlyReplace(propertyTree)) {
    // return false; // keep params, but don't overwrite manips or FFT size
    prop_man->replaceState(propertyTree);
  }

  mInterface->getRTSTFTManager()->readManipsFromBinary();
}

void RT_PresetManager::getPreset(juce::MemoryBlock &inDestData)
{
  _storeCurrentStateInMemory();
  inDestData = mActivePresetRawData;
}
void RT_PresetManager::_storeCurrentStateInMemory()
{
  auto paramState = mInterface->getParameterManager()
                        ->getValueTreeState()
                        ->copyState()
                        .createXml();
  auto propertyState
      = mInterface->getPropertyManager()->getXMLSerializedProperties();
  auto state = std::make_unique<juce::XmlElement>("rtstft_ctl_state");

  state->addChildElement(paramState.release());
  state->addChildElement(propertyState.release());

  juce::AudioProcessor::copyXmlToBinary(*state, mActivePresetRawData);
  auto size          = ((uint32_t *)mActivePresetRawData.getData())[1];
  auto manips_stream = juce::MemoryOutputStream(mActivePresetRawData, true);
  manips_stream.setPosition(size + 9);
  mInterface->getRTSTFTManager()->writeManipsToBinary(manips_stream);
}

void RT_PresetManager::writePresetToDisk(juce::String inPresetName)
{
  juce::String presetName = inPresetName + ".rtstftpreset";
  auto         presetsDir = juce::File::addTrailingSeparator(
              mInterface->getFileManager()->getPresetsDirectory().getFullPathName());
  juce::File presetPath(presetsDir + inPresetName);
  writePresetToDisk(presetPath);
}
void RT_PresetManager::writePresetToDisk(juce::File inPresetPath)
{
  _storeCurrentStateInMemory();
  if (inPresetPath.exists()) {
    inPresetPath.deleteFile();
  }
  juce::FileOutputStream ostream(inPresetPath);
  if (ostream.openedOk()) {
    ostream.write(mActivePresetRawData.getData(),
                  mActivePresetRawData.getSize());
  }
}

bool RT_PresetManager::loadPresetFromDisk(juce::File presetFile)
{
  if (presetFile.getFileExtension() != ".rtstftpreset") {
    DBG(presetFile.getFileExtension());
    return false;
  }
  mActivePresetRawData.setSize(0);
  juce::FileInputStream istream(presetFile);
  istream.readIntoMemoryBlock(mActivePresetRawData);
  loadPreset();
  return true;
}

const void *RT_PresetManager::getManipsBinaryPointer()
{
  const char *data = (const char *)mActivePresetRawData.getData();
  return (const void *)(data + getXmlBlockSize());
}

int RT_PresetManager::getXmlBlockSize()
{
  void *data    = mActivePresetRawData.getData();
  int xmlOffset = ((uint32_t *)data)[1] + 9; // includes magic number, size int,
  // and trailing nullterm in XML binary
  return xmlOffset;
}
