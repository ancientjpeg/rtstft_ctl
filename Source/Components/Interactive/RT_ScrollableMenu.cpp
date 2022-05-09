/*
  ==============================================================================

    RT_ScrollableMenu.cpp
    Created: 4 May 2022 12:49:09pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_ScrollableMenu.h"
#include "../../Managers/LookAndFeelManagement/RT_LookAndFeelManagement.h"
#include <JuceHeader.h>

//==============================================================================
RT_ScrollableMenu::RT_ScrollableMenu(RT_ProcessorInterface *inInterface,
                                     juce::StringArray      inSelections,
                                     int                    inFontSize)
    : RT_Component(inInterface), mSelections(inSelections),
      mFontSize(inFontSize), mSelectionIndex(-1), mMousePos(-1.f, -1.f)
{
}

RT_ScrollableMenu::~RT_ScrollableMenu() {}

void RT_ScrollableMenu::resized()
{
  auto bounds     = getLocalBounds().withHeight(mFontSize);
  int  nextHeight = bounds.getHeight() + RT_LookAndFeel::widgetBorderSize;
  mSelectionBounds.clearQuick();
  for (int i = 0; i < mSelections.size(); i++) {
    mSelectionBounds.add(bounds);
    bounds.translate(0, nextHeight);
  }
}

void RT_ScrollableMenu::paint(juce::Graphics &g)
{
  auto lafm = mInterface->getLookAndFeelManager();
  g.fillAll(lafm->getUIColour(windowBackground));
  auto height = mSelectionBounds.getLast().getBottom()
                - mSelectionBounds[0].getY() + RT_LookAndFeel::widgetBorderSize;
  auto bg = mSelectionBounds[0].withHeight(height);
  g.setColour(lafm->getUIColour(widgetBackground));
  g.fillRect(bg);
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

void RT_ScrollableMenu::setSelections(juce::StringArray inSelections)
{
  mSelections = inSelections;
  resized();
  repaint();
}

int RT_ScrollableMenu::select(juce::String inNewSelection)
{
  mSelectionIndex = mSelections.indexOf(inNewSelection);
  if (mSelectionIndex > -1) {
    onSelection(inNewSelection);
  }
  return mSelectionIndex;
}

void         RT_ScrollableMenu::deselect() { mSelectionIndex = -1; }

juce::String RT_ScrollableMenu::getCurrentSelection()
{
  if (mSelectionIndex >= 0) {
    return mSelections[mSelectionIndex];
  }
  return juce::String();
}

int RT_ScrollableMenu::getSelectionFromPosition(juce::Point<float> inPos)
{
  for (int i = 0; i < mSelectionBounds.size(); i++) {
    const auto &selection = mSelectionBounds[i];
    if (selection.contains(inPos.roundToInt())) {
      return i;
    }
  }
  return -1;
}
void RT_ScrollableMenu::mouseDown(const juce::MouseEvent &e)
{
  mMousePos = e.position;
  int sel   = getSelectionFromPosition(mMousePos);
  if (sel > -1) {
    select(mSelections[sel]);
  }
}