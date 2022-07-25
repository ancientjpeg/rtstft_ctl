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

  std::function<void(juce::File)> onFileClick = [](juce::File) {};

private:
  class DirectorySelector {
  public:
    DirectorySelector(RT_FileBrowser *inParent, juce::File inDirectory);
    ~DirectorySelector();

    void addLabelsAsChildren();
    void setBounds(juce::Rectangle<int> inBounds);

    struct FileButton {
      std::unique_ptr<juce::TextButton> button;
      juce::File                        file;
    };

  private:
    RT_FileBrowser         *mParent;
    std::vector<FileButton> mFileButtons;
    juce::File              mDirectory;
  };

  int                          mColumnWidth;
  bool                         mShowExtensions;
  std::list<DirectorySelector> mOpenDirectories;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_FileBrowser)
};
