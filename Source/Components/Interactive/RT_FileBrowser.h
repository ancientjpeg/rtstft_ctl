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
                 int columnWidth, bool inShowExtensions = true,
                 juce::String inTargetSuffix = "");
  ~RT_FileBrowser() override;

  void                            resized() override;
  void                            paint(juce::Graphics &) override;

  std::function<void(juce::File)> onFileClick = [](juce::File f) {};

private:
  class SubMenu {
  public:
    SubMenu(RT_FileBrowser *inParent, RT_FileTree::Directory *inTreeDir);
    ~SubMenu();
    void                    onFileClick();
    RT_FileBrowser         *parent;
    RT_ScrollableMenu       component;
    RT_FileTree::Directory *treeDir;
  };

  int                       mColumnWidth;
  bool                      mShowExtensions;
  RT_FileTree               mFileTree;
  juce::OwnedArray<SubMenu> mDirectoryMenus;

  juce::Rectangle<int>      mBackgroundBounds;

  void pushNewDirToStack(juce::File inNewDeepestDir, int inParentIndex);
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_FileBrowser)
};
