/*
  ==============================================================================

    RT_FileBrowser.h
    Created: 4 May 2022 12:49:09pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../Utility/RT_Component.h"
#include "RT_ScrollableMenu.h"
#include <JuceHeader.h>
#include <vector>

//==============================================================================
/*
 */
class RT_FileBrowser : public RT_Component {
public:
  RT_FileBrowser(RT_ProcessorInterface *inInterface, juce::File inBrowserRoot,
                 int columnWidth, bool inShowExtensions = true,
                 juce::String inTargetSuffix = "");
  ~RT_FileBrowser() override;

  void                            resized() override;
  void                            paint(juce::Graphics &) override;
  void                            setNewTailDirectory(juce::File inDirectory);

  std::function<void(juce::File)> onFileClick = [](juce::File) {};

private:
  struct Directory;
  struct FileButton {
    Directory                        *parent_dir;
    std::unique_ptr<juce::TextButton> button;
    juce::File                        file;
  };

  struct Directory {
    juce::File              path;
    std::vector<FileButton> file_buttons;
    juce::Rectangle<int>    empty_space;
  };

  int                    mColumnWidth;
  bool                   mShowExtensions;
  std::vector<Directory> mOpenDirectories;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_FileBrowser)
};
