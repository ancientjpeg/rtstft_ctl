/*
  ==============================================================================

    RT_PresetChooser.cpp
    Created: 3 May 2022 8:28:30am
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_PresetChooser.h"
#include "../../Managers/OSManagers/RT_FileManager.h"
#include "../../Managers/StateManagers/RT_PresetManager.h"

RT_PresetChooser::RT_PresetChooser(RT_ProcessorInterface *inInterface)
    : RT_Component(inInterface),
      mFileBrowser(mInterface,
                   mInterface->getFileManager()->getPresetsDirectory(), 100,
                   false, "*" + sc_PresetSuffix)
{
  addAndMakeVisible(mSaveButton);
  mSaveButton.setButtonText("save preset");
  mSaveButton.onClick = [this]() { savePreset(); };
  addAndMakeVisible(mLoadButton);
  mLoadButton.setButtonText("load preset");
  mLoadButton.onClick = [this]() { loadPreset(); };

  addAndMakeVisible(mCurrentPresetLabel);
  mCurrentPresetLabel.setText("Default", juce::dontSendNotification);
  addAndMakeVisible(mFileBrowser);
  mFileBrowser.onFileClick = [this](juce::File inClickedFile) {
    mInterface->getPresetManager()->loadPresetFromDisk(inClickedFile);
  };
}

void RT_PresetChooser::paint(juce::Graphics &g) {}
void RT_PresetChooser::resized()
{
  auto bounds    = getLocalBounds();
  auto topBounds = bounds.removeFromTop(20);
  mSaveButton.setBounds(topBounds.removeFromRight(80));
  mLoadButton.setBounds(topBounds.removeFromRight(80));
  mCurrentPresetLabel.setBounds(topBounds);
  mFileBrowser.setBounds(bounds);
}

void RT_PresetChooser::loadPreset()
{
  mFileChooser.reset(new juce::FileChooser(
      "Find ur file", mInterface->getFileManager()->getPresetsDirectory(),
      "*" + sc_PresetSuffix));
  auto flags = juce::FileBrowserComponent::openMode
               | juce::FileBrowserComponent::canSelectFiles;
  mFileChooser->launchAsync(flags, [this](const juce::FileChooser &fc) {
    juce::File preset = fc.getResult();
    if (preset.existsAsFile()) {
      mInterface->getPresetManager()->loadPresetFromDisk(preset);
    }
  });
}

void RT_PresetChooser::savePreset()
{
  mFileChooser.reset(new juce::FileChooser(
      "Save a file", mInterface->getFileManager()->getPresetsDirectory(),
      "*" + sc_PresetSuffix));
  auto flags = juce::FileBrowserComponent::saveMode;
  mFileChooser->launchAsync(flags, [this](const juce::FileChooser &fc) {
    juce::File preset = fc.getResult();
    if (preset.getFileExtension() != sc_PresetSuffix) {
      preset = preset.withFileExtension(sc_PresetSuffix);
    }
    mInterface->getPresetManager()->writePresetToDisk(preset);
  });
}
