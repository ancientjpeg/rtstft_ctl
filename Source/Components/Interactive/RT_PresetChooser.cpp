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
                "*.rtstftpreset"),
      mScrollableMenu(mInterface)
{
  addAndMakeVisible(mSaveButton);
  mSaveButton.setButtonText("save preset");
  addAndMakeVisible(mLoadButton);
  mLoadButton.setButtonText("load preset");
  mLoadButton.onClick = [this]() {};
  addAndMakeVisible(mCurrentPresetLabel);
  mCurrentPresetLabel.setText("Default", juce::dontSendNotification);
  addAndMakeVisible(mScrollableMenu);

  mScrollableMenu.setSelections(mFileTree.getFilenames());
}

void RT_PresetChooser::paint(juce::Graphics &g) {}
void RT_PresetChooser::resized()
{
  auto bounds    = getLocalBounds();
  auto topBounds = bounds.removeFromTop(20);
  mSaveButton.setBounds(topBounds.removeFromRight(80));
  mLoadButton.setBounds(topBounds.removeFromRight(80));
  mCurrentPresetLabel.setBounds(topBounds);
  mScrollableMenu.setBounds(bounds);
}
