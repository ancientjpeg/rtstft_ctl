/*
  ==============================================================================

    RT_PresetChooser.cpp
    Created: 3 May 2022 8:28:30am
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_PresetChooser.h"

RT_PresetChooser::RT_PresetChooser(RT_ProcessorInterface *inInterface)
    : RT_Component(inInterface),
      mFileTree(mInterface->getFileManager()->getPresetsDirectory(),
                "*.rtstftpreset")
{
  addAndMakeVisible(mSaveButton);
  mSaveButton.setButtonText("save preset");
  addAndMakeVisible(mLoadButton);
  mLoadButton.setButtonText("load preset");
  mLoadButton.onClick = [this]() {};
  addAndMakeVisible(mCurrentPresetLabel);
  mCurrentPresetLabel.setText("Default", juce::dontSendNotification);
}

void RT_PresetChooser::paint(juce::Graphics &g) {}
void RT_PresetChooser::resized()
{
  auto bounds = getLocalBounds();
  mSaveButton.setBounds(bounds.removeFromRight(80));
  mLoadButton.setBounds(bounds.removeFromRight(80));
  mCurrentPresetLabel.setBounds(bounds);
}
