/*
  ==============================================================================

    RT_FileBrowser.cpp
    Created: 4 May 2022 12:49:09pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_FileBrowser.h"
#include "../../Managers/LookAndFeelManagement/RT_LookAndFeelManagement.h"
#include "../../Managers/StateManagers/RT_PresetManager.h"
#include <JuceHeader.h>

//==============================================================================
RT_FileBrowser::RT_FileBrowser(RT_ProcessorInterface *inInterface,
                               juce::File             inBrowserRoot,
                               int                    inColumnWidth,
                               bool                   inShowExtensions,
                               juce::String           inTargetSuffix)

    : RT_Component(inInterface), mColumnWidth(inColumnWidth),
      mShowExtensions(inShowExtensions)
{
  setNewTailDirectory(inBrowserRoot);
}

RT_FileBrowser::~RT_FileBrowser()
{
}

void RT_FileBrowser::paint(juce::Graphics &g)
{
  g.fillAll(RT_LookAndFeel::BLACK);

  auto &last_dir   = mOpenDirectories.back();
  auto  empty_left = last_dir.file_buttons[0].button->getBounds().getRight()
                    + RT_LookAndFeel::PADDING_SMALL;
  g.setColour(RT_LookAndFeel::WHITE);
  g.fillRect(getLocalBounds().withX(empty_left));
  for (auto &dir : mOpenDirectories) {
    g.fillRect(dir.empty_space);
  }
}

void RT_FileBrowser::resized()
{
  removeAllChildren();
  auto dir_bounds = juce::Rectangle<int>(0, 0, mColumnWidth, getWidth());
  for (auto &dir : mOpenDirectories) {
    auto button_height = RT_LookAndFeel::BUTTON_FONT_HEIGHT + 2;
    auto button_bounds = dir_bounds.withHeight(button_height);

    for (auto &fb : dir.file_buttons) {
      addAndMakeVisible(fb.button.get());
      fb.button->setBounds(button_bounds);
      button_bounds.translate(0, button_height + RT_LookAndFeel::PADDING_SMALL);
    }

    dir.empty_space = dir_bounds.withY(button_bounds.getY());
    dir_bounds.translate(mColumnWidth + RT_LookAndFeel::PADDING_SMALL, 0);
  }
  repaint();
}

void RT_FileBrowser::setNewTailDirectory(juce::File inDirectory)
{
  auto files = inDirectory.findChildFiles(juce::File::findFilesAndDirectories
                                              | juce::File::ignoreHiddenFiles,
                                          false);

  if (files.size() == 0) {
    return;
  }

  size_t count = mOpenDirectories.size();
  while (count-- > 0) {
    if (mOpenDirectories[count].path == inDirectory.getParentDirectory()) {
      break;
    }
    mOpenDirectories.pop_back();
  }
  mOpenDirectories.push_back(Directory());
  auto &new_dir = mOpenDirectories.back();
  new_dir.path  = inDirectory;

  for (auto f : files) {
    new_dir.file_buttons.push_back(FileButton());
    auto &new_fb = new_dir.file_buttons.back();
    new_fb.file  = f;
    new_fb.button.reset(new juce::TextButton());
    auto name
        = mShowExtensions ? f.getFileName() : f.getFileNameWithoutExtension();
    if (f.isDirectory()) {
      name += juce::String(" [DIR]");
    }

    auto *button = new_fb.button.get();
    button->setButtonText(name);
    new_dir.file_buttons.back().button->onClick = [this, f]() {
      if (f.isDirectory()) {
        setNewTailDirectory(f);
      }
      else {
        mInterface->getPresetManager()->loadPresetFromDisk(f);
      }
      resized();
    };

    addAndMakeVisible(button);
  }
}
