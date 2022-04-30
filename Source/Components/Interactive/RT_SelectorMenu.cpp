/*
  ==============================================================================

    RT_SelectorMenu.cpp
    Created: 9 Apr 2022 11:22:43am
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_SelectorMenu.h"
#include "../../Managers/LookAndFeelManagement/RT_LookAndFeelManagement.h"
#include "../../Managers/StateManagers/RT_PropertyManager.h"

RT_SelectorMenu::RT_SelectorMenu(RT_ProcessorInterface *inInterface,
                                 bool                   inIsVertical)
    : RT_Component(inInterface),
      mSelectorData(mInterface->getPropertyManager()->getSelectorData()),
      mVertical(inIsVertical)
{
}

void RT_SelectorMenu::paint(juce::Graphics &g)
{
  juce::Font font = juce::Font(12, 0);
  g.setFont(font);
  auto lamf = mInterface->getLookAndFeelManager();
  g.fillAll(lamf->getUIColour(defaultFill));
  for (int i = 0; i < mSelectorData->fields.size(); i++) {
    const auto &this_bound = mSelectionsBounds[i];
    const auto &d          = mSelectorData->fields[i];
    auto        color_id   = d.active ? defaultFill
                                      : (checkHover(&this_bound) ? highlightedFill
                                                                 : windowBackground);
    g.setColour(lamf->getUIColour(color_id));
    g.fillRect(this_bound);

    auto text_bounds = this_bound.withSizeKeepingCentre(
        font.getStringWidth(d.selectionID), 14);
    color_id = d.active ? highlightedText : defaultText;
    g.setColour(lamf->getUIColour(color_id));
    g.drawText(d.selectionID.toLowerCase(), text_bounds, 12);
  }
}
void RT_SelectorMenu::resized()
{
  mSelectionsBounds.clear();
  auto bounds    = getLocalBounds();
  auto numFields = mSelectorData->fields.size();
  // this is ugly and should be generalized...
  if (mVertical) {
    auto fieldHeight = bounds.getHeight() / numFields;
    for (int i = 0; i < numFields; i++) {
      auto curr = bounds.removeFromTop(fieldHeight);
      mSelectionsBounds.push_back(curr.toFloat());
      bounds.removeFromTop(RT_LookAndFeel::widgetBorderSize);
    }
  }
  else {
    auto fieldWidth = bounds.getWidth() / numFields;
    for (int i = 0; i < numFields; i++) {
      auto curr = bounds.removeFromLeft(fieldWidth);
      mSelectionsBounds.push_back(curr.toFloat());
      bounds.removeFromLeft(RT_LookAndFeel::widgetBorderSize);
    }
  }
}

void RT_SelectorMenu::mouseDown(const juce::MouseEvent &event)
{
  int numFields    = mSelectorData->fields.size();
  int newSelection = event.getMouseDownX() / (getWidth() / numFields);
  if (mSelectorData->activeField == newSelection) {
    mSelectorData->fields[mSelectorData->activeField].active = false;
    mSelectorData->activeField                               = -1;
  }
  else {
    if (mSelectorData->activeField != -1) {
      mSelectorData->fields[mSelectorData->activeField].active = false;
    }
    mSelectorData->activeField                               = newSelection;
    mSelectorData->fields[mSelectorData->activeField].active = true;
  }

  repaint();
}

void RT_SelectorMenu::mouseEnter(const juce::MouseEvent &event)
{
  mLastInternalMousePos = event.position;
}
void RT_SelectorMenu::mouseMove(const juce::MouseEvent &event)
{
  for (auto const &r : mSelectionsBounds) {
    if (r.contains(event.position) && (&r != mSelectionWithHover)) {
      mSelectionWithHover = &r;
      repaint();
      return;
    }
  }
  mSelectionWithHover = nullptr;
}
void RT_SelectorMenu::mouseExit(const juce::MouseEvent &event)
{
  mSelectionWithHover = nullptr;
  repaint();
}
bool RT_SelectorMenu::checkHover(const juce::Rectangle<float> *comp)
{
  return comp == mSelectionWithHover;
}