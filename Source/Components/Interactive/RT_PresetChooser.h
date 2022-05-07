/*
  ==============================================================================

    RT_PresetChooser.h
    Created: 3 May 2022 8:28:30am
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once
#include "../../Managers/Interface/RT_ProcessorInterface.h"
#include "../../Managers/OSManagers/RT_FileManager.h"
#include "../../Managers/StateManagers/RT_PresetManager.h"
#include "../../Utilities/RT_FileTree.h"
#include "../Utility/RT_Component.h"
#include "RT_ScrollableMenu.h"
#include <JuceHeader.h>

class RT_PresetChooser : public RT_Component {
public:
  RT_PresetChooser(RT_ProcessorInterface *inInterface);
  void paint(juce::Graphics &) override;
  void resized() override;

private:
  juce::Label       mCurrentPresetLabel;
  juce::TextButton  mLoadButton, mSaveButton;
  RT_FileTree       mFileTree;
  RT_ScrollableMenu mScrollableMenu;
};