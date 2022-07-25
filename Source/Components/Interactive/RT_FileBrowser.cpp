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
      mShowExtensions(inShowExtensions)
{
}

RT_FileBrowser::~RT_FileBrowser() {}

void RT_FileBrowser::paint(juce::Graphics &g)
{
  g.fillAll(mInterface->getLookAndFeelManager()->getUIColour(windowBackground));
  g.setColour(
      mInterface->getLookAndFeelManager()->getUIColour(widgetBackground));
}

void RT_FileBrowser::resized()
{
  removeAllChildren();
  auto dirBounds = juce::Rectangle<int>(0, 0, mColumnWidth, getWidth());
  for (auto &dir : mOpenDirectories) {
    dir.setBounds(dirBounds);
    dir.addLabelsAsChildren();
    dirBounds.translate(mColumnWidth + RT_LookAndFeel::PADDING_SMALL, 0);
  }
}

RT_FileBrowser::DirectorySelector::DirectorySelector(RT_FileBrowser *inParent,
                                                     juce::File inDirectory)
    : mParent(inParent), mDirectory(inDirectory)
{
  auto files = mDirectory.findChildFiles(
      juce::File::findFiles | juce::File::ignoreHiddenFiles, false);

  for (auto f : files) {
    mFileButtons.push_back(FileButton());
    mFileButtons.back().file = f;
    mFileButtons.back().button.reset(new juce::TextButton());
    auto name = mParent->mShowExtensions ? f.getFileName()
                                         : f.getFileNameWithoutExtension();
    mFileButtons.back().button->setButtonText(name);
  }
}

RT_FileBrowser::DirectorySelector::~DirectorySelector() {}

void RT_FileBrowser::DirectorySelector::addLabelsAsChildren()
{
  for (auto &fb : mFileButtons) {
    mParent->addAndMakeVisible(fb.button.get());
  }
}
void RT_FileBrowser::DirectorySelector::setBounds(juce::Rectangle<int> inBounds)
{
  for (auto &fb : mFileButtons) {
    auto *b = fb.button.get();
  }
}