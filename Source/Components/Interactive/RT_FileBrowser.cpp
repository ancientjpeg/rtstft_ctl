/*
  ==============================================================================

    RT_FileBrowser.cpp
    Created: 4 May 2022 12:49:09pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_FileBrowser.h"
#include "../../Managers/LookAndFeelManagement/RT_LookAndFeelManagement.h"
#include <JuceHeader.h>

//==============================================================================
RT_FileBrowser::RT_FileBrowser(RT_ProcessorInterface *inInterface,
                               juce::File inBrowserRoot, int inColumnWidth,
                               bool         inShowExtensions,
                               juce::String inTargetSuffix)

    : RT_Component(inInterface), mColumnWidth(inColumnWidth),
      mShowExtensions(inShowExtensions),
      mFileTree(inBrowserRoot, inTargetSuffix)

{
  mDirectoryMenus.add(
      std::make_unique<SubMenu>(this, mFileTree.getDirFromDepth(0)));
}

RT_FileBrowser::~RT_FileBrowser() {}

void RT_FileBrowser::paint(juce::Graphics &g)
{
  g.fillAll(mInterface->getLookAndFeelManager()->getUIColour(windowBackground));
  g.setColour(
      mInterface->getLookAndFeelManager()->getUIColour(widgetBackground));
  g.fillRect(mBackgroundBounds);
}

void RT_FileBrowser::resized()
{
  auto bounds       = getLocalBounds().withWidth(mColumnWidth);
  int  translationX = mColumnWidth + RT_LookAndFeel::widgetBorderSize;
  int  finalWidth   = 0;
  for (auto menu : mDirectoryMenus) {
    menu->component.setBounds(bounds);
    bounds.translate(translationX, 0);
    finalWidth += translationX;
  }
  mBackgroundBounds = mDirectoryMenus[0]->component.getLocalBounds();
  mBackgroundBounds.setWidth(finalWidth);
}

RT_FileBrowser::SubMenu::SubMenu(RT_FileBrowser         *inParent,
                                 RT_FileTree::Directory *inTreeDir)
    : parent(inParent), component(parent->mInterface, {}, 14),
      treeDir(inTreeDir)
{
  juce::StringArray files;
  for (const auto &f : *treeDir->getChildFileArrayPtr()) {
    juce::String s = parent->mShowExtensions ? f.getFileName()
                                             : f.getFileNameWithoutExtension();
    if (f.isDirectory()) {
      files.add(s + "/");
    }
    else {
      files.add(s);
    }
  }
  component.setSelections(files);
  component.onSelection = [this](juce::String inNewSelection) {
    if (inNewSelection.getLastCharacter() == '/') {
      inNewSelection = inNewSelection.dropLastCharacters(1);
    }
    juce::File file  = treeDir->getFileByBasename(inNewSelection);
    bool       found = false;
    if (file.isDirectory()) {
      int depth = 0;
      do {
        if (file.getParentDirectory()
            == parent->mFileTree.getDirFromDepth(depth)->getDir()) {
          found = true;
          break;
        }
      } while (++depth <= parent->mFileTree.getCurrentDepth());
      assert(found);
      parent->pushNewDirToStack(file, depth);
      parent->resized();
      parent->repaint();
    }
    else if (file.existsAsFile()) {
      parent->onFileClick(file);
    }
    component.deselect();
  };
  parent->addAndMakeVisible(component);
}
RT_FileBrowser::SubMenu::~SubMenu()
{
  parent->removeChildComponent(&component);
}

void RT_FileBrowser::pushNewDirToStack(juce::File inNewDeepestDir,
                                       int        inParentDepth)
{
  assert(inNewDeepestDir.isDirectory());
  auto *parent = mFileTree.getDirFromDepth(inParentDepth);
  if (parent == nullptr) {
    return;
  }
  juce::File parentDir = parent->getDir();
  assert(inNewDeepestDir.isAChildOf(parentDir));
  if (parentDir
      != mFileTree.getDirFromDepth(mDirectoryMenus.size() - 1)->getDir()) {
    mFileTree.traverseUpToDepth(inParentDepth);
    mDirectoryMenus.removeRange(inParentDepth + 1, mDirectoryMenus.size());
  }
  auto *dir = mFileTree.traverseDown(inNewDeepestDir);
  mDirectoryMenus.add(std::make_unique<SubMenu>(this, dir));
}
