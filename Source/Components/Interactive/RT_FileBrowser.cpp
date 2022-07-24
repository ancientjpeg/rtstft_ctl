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

void RT_FileBrowser::resized() {}

RT_FileBrowser::DirectorySelector::DirectorySelector(RT_FileBrowser *inParent,
                                                     juce::File inDirectory)
    : mParent(inParent), mDirectory(inDirectory)
{
  auto files = mDirectory.findChildFiles(
      juce::File::findFiles | juce::File::ignoreHiddenFiles, false);

  for (auto f : files) {
    mFileButtons.emplace_back(juce::Label(), f);
  }
}

RT_FileBrowser::DirectorySelector::~DirectorySelector() {}