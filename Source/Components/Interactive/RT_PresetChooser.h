/*
  ==============================================================================

    RT_PresetChooser.h
    Created: 3 May 2022 8:28:30am
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once
#include "../../Managers/Interface/RT_ProcessorInterface.h"
#include "../Utility/RT_Component.h"
#include "RT_FileBrowser.h"
#include <JuceHeader.h>

class RT_PresetChooser : public RT_Component {
public:
  RT_PresetChooser(RT_ProcessorInterface *inInterface);
  void paint(juce::Graphics &) override;
  void resized() override;

  void loadPreset();
  void savePreset();

private:
  juce::Label                        mCurrentPresetLabel;
  juce::TextButton                   mLoadButton, mSaveButton;
  RT_FileBrowser                     mFileBrowser;
  std::unique_ptr<juce::FileChooser> mFileChooser;
};