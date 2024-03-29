/*
  ==============================================================================

    RT_PresetManager.cpp
    Created: 9 Apr 2022 12:04:07pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_PresetManager.h"
#include "../../RT_AudioProcessor.h"
#include "../DSPStateManagers/RTSTFT_Manager.h"
#include "../DSPStateManagers/RT_ParameterDefines.h"
#include "../DSPStateManagers/RT_ParameterManager.h"
#include "../OSManagers/RT_FileManager.h"
#include "RT_PropertyManager.h"

RT_PresetManager::RT_PresetManager(RT_ProcessorInterface *inInterface)
    : mInterface(inInterface), mActivePresetRawData(0)
{
  _storeCurrentStateInMemory();
  auto presetRoot        = mInterface->getFileManager()->getPresetsDirectory();
  auto defaultPresetPath = genPresetPathInPresetsDir("default");
  if (!defaultPresetPath.existsAsFile()) {
    writePresetToDisk(defaultPresetPath);
  }
  else {
    _presetChange(defaultPresetPath);
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

/**
 * @brief Retrieve current state as a new preset in memory
 *
 * @param inDestData Block to copy the preset data to.
 * @param inOverwriteStoredState Normally this function will overwrite the
 * internal saved state in RT_PresetManager, but we can avoid this behavior if
 * need be by setting inOverwriteStoredState to False.
 *
 */
void RT_PresetManager::getPreset(juce::MemoryBlock &inDestData,
                                 bool               inOverwriteStoredState)
{
  if (inOverwriteStoredState) {
    _storeCurrentStateInMemory();
    inDestData = mActivePresetRawData;
  }
  else {
    _storeCurrentStateInMemoryBlock(inDestData);
  }
}

juce::File
RT_PresetManager::genPresetPathInPresetsDir(juce::String inPresetName)
{
  juce::String presetName = inPresetName + sc_PresetSuffix;
  auto         presetsDir = juce::File::addTrailingSeparator(
              mInterface->getFileManager()->getPresetsDirectory().getFullPathName());
  juce::File presetPath(presetsDir + presetName);
  return presetPath;
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
  _presetChange(inPresetPath);
  _refreshListeners();
}

bool RT_PresetManager::loadPresetFromDisk(juce::File inPresetFile)
{
  if (inPresetFile.getFileExtension() != ".rtstftpreset") {
    DBG(inPresetFile.getFileExtension());
    return false;
  }
  mActivePresetRawData.setSize(0);
  juce::FileInputStream istream(inPresetFile);
  if (istream.openedOk()) {
    istream.readIntoMemoryBlock(mActivePresetRawData);
    _presetChange(inPresetFile);
    loadPreset();
    return true;
  }
  return false;
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

juce::String RT_PresetManager::getCurrentPresetName()
{
  auto str = mActivePresetPath.getFileNameWithoutExtension();
  return str;
}

void RT_PresetManager::addListener(Listener *l, bool initialNotification)
{
  mListenerList.add(l);
  if (initialNotification) {
    l->onPresetChange();
  }
}

void RT_PresetManager::_storeCurrentStateInMemory()
{
  _storeCurrentStateInMemoryBlock(mActivePresetRawData);
}

void RT_PresetManager::_storeCurrentStateInMemoryBlock(juce::MemoryBlock &dest)
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

  juce::AudioProcessor::copyXmlToBinary(*state, dest);
  auto size          = ((uint32_t *)dest.getData())[1];
  auto manips_stream = juce::MemoryOutputStream(dest, true);
  manips_stream.setPosition(size + 9);
  mInterface->getRTSTFTManager()->writeManipsToBinary(manips_stream);
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

void RT_PresetManager::_presetChange(juce::File inNewPreset)
{
  mActivePresetPath = inNewPreset;
  mListenerList.call([](RT_PresetManager::Listener &l) { l.onPresetChange(); });
}

void RT_PresetManager::_refreshListeners()
{
  mListenerList.call(
      [](RT_PresetManager::Listener &l) { l.onPresetDirRefresh(); });
}