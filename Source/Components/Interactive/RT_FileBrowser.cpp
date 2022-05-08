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
}

void RT_FileBrowser::resized()
{
  auto bounds       = getLocalBounds().withWidth(mColumnWidth);
  int  translationX = mColumnWidth + RT_LookAndFeel::widgetBorderSize;
  for (auto menu : mDirectoryMenus) {
    menu->component.setBounds(bounds);
    bounds.translate(translationX, 0);
  }
}

RT_FileBrowser::SubMenu::SubMenu(RT_FileBrowser         *inParent,
                                 RT_FileTree::Directory *inTreeDir)
    : parent(inParent), component(parent->mInterface), treeDir(inTreeDir)
{
  juce::StringArray files;
  for (const auto &f : *treeDir->getChildFileArrayPtr()) {
    juce::String s = parent->mShowExtensions ? f.getFileName()
                                             : f.getFileNameWithoutExtension();
    files.add(s);
  }
  component.setSelections(files);
  component.onSelection = [this](juce::String inNewSelection) {
    juce::File file = treeDir->getFileByBasename(inNewSelection);
    if (file.isDirectory()) {
      DBG("DIR");
    }
    else if (file.existsAsFile()) {
      DBG("FILE");
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
