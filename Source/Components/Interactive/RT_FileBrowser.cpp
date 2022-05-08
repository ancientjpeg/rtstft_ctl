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
                               juce::String inTargetSuffix)

    : RT_Component(inInterface), mColumnWidth(inColumnWidth),
      mFileTree(inBrowserRoot, inTargetSuffix)

{
}

RT_FileBrowser::~RT_FileBrowser() {}

void RT_FileBrowser::paint(juce::Graphics &g) {}

void RT_FileBrowser::resized() {}
