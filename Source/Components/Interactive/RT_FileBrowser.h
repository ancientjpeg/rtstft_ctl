/*
  ==============================================================================

    RT_FileBrowser.h
    Created: 4 May 2022 12:49:09pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../../Utilities/RT_FileTree.h"
#include "../Utility/RT_Component.h"
#include "RT_ScrollableMenu.h"
#include <JuceHeader.h>

//==============================================================================
/*
 */
class RT_FileBrowser : public RT_Component {
public:
  RT_FileBrowser(RT_ProcessorInterface *inInterface, juce::File inBrowserRoot,
                 int columnWidth, juce::String inTargetSuffix = "");
  ~RT_FileBrowser() override;

  void resized() override;
  void paint(juce::Graphics &) override;

private:
  int                                 mColumnWidth;
  RT_FileTree                         mFileTree;
  juce::OwnedArray<RT_ScrollableMenu> mDirectoryMenus;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_FileBrowser)
};
