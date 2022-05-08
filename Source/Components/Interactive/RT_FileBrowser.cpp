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
                               juce::StringArray inSelections, int inFontSize)
    : RT_Component(inInterface), mSelections(inSelections),
      mFontSize(inFontSize), mSelectionIndex(-1), mMousePos(-1.f, -1.f)
{
}

RT_FileBrowser::~RT_FileBrowser() {}

void RT_FileBrowser::resized()
{
  auto bounds     = getLocalBounds().withHeight(mFontSize);
  int  nextHeight = bounds.getHeight() + RT_LookAndFeel::widgetBorderSize;
  mSelectionBounds.clearQuick();
  for (int i = 0; i < mSelections.size(); i++) {
    mSelectionBounds.add(bounds);
    bounds.translate(0, nextHeight);
  }
}

void RT_FileBrowser::paint(juce::Graphics &g)
{
  auto lafm = mInterface->getLookAndFeelManager();
  for (int i = 0; i < mSelections.size(); i++) {
    auto bounds   = mSelectionBounds[i];
    bool hasHover = mHasHover && bounds.contains(mMousePos.toInt());
    auto col      = i == mSelectionIndex
                        ? lafm->getUIColour(highlightedFill)
                        : (hasHover ? lafm->getUIColour(defaultFill)
                                    : lafm->getUIColour(windowBackground));
    g.setColour(col);
    g.fillRect(bounds);
    g.setColour(i == mSelectionIndex ? lafm->getUIColour(highlightedText)
                                     : lafm->getUIColour(defaultText));
    g.drawText(mSelections[i], bounds.toFloat(),
               juce::Justification::centredLeft);
  }
}

void RT_FileBrowser::setSelections(juce::StringArray inSelections)
{
  mSelections = inSelections;
  resized();
  repaint();
}

int RT_FileBrowser::select(juce::String inNewSelection)
{
  mSelectionIndex = mSelections.indexOf(inNewSelection);
  return mSelectionIndex;
}

int RT_FileBrowser::getSelectionFromPosition(juce::Point<float> inPos) {}