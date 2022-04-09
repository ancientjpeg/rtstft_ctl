/*
  ==============================================================================

    RT_SelectorMenu.cpp
    Created: 9 Apr 2022 11:22:43am
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_SelectorMenu.h"

RT_SelectorMenu::RT_SelectorMenu(RT_ProcessorInterface *inInterface)
    : RT_Component(inInterface)
{
}
void RT_SelectorMenu::paint(juce::Graphics &g)
{
  juce::Font font = juce::Font(12, juce::Font::bold);
  g.setFont(font);
  using enum juce::LookAndFeel_V4::ColourScheme::UIColour;
  auto cols       = getLookAndFeel_V4().getCurrentColourScheme();
  auto bounds     = getLocalBounds();
  auto fieldWidth = bounds.getWidth() / mSelectorData->fields.size();
  for (SelectorData::Datum d : mSelectorData->fields) {
    auto this_bound = bounds.removeFromLeft(fieldWidth);
    auto color_id   = d.active ? defaultFill : highlightedFill;
    g.setColour(cols.getUIColour(color_id));
    g.fillRect(this_bound);
    auto text_bounds = this_bound.withSizeKeepingCentre(
        font.getStringWidth(d.selectionID), 14);
    g.drawText(d.selectionID, text_bounds, 12);
  }
}
void RT_SelectorMenu::resized() {}
void RT_SelectorMenu::mouseDown(const juce::MouseEvent &event)
{
  if (!(event.eventComponent == this)) {
    DBG("SELECTOR CALL MOUSE OUTSIDE SELECTOR");
    return;
  }
  else {
    DBG("OKAY IT WAS INSIDE THAT TIME");
  }
  int numFields    = mSelectorData->fields.size();
  int newSelection = event.getMouseDownX() / (getWidth() / numFields);
  if (mSelectorData->activeField != -1) {
    mSelectorData->fields[mSelectorData->activeField].active = false;
  }
  mSelectorData->activeField                               = newSelection;
  mSelectorData->fields[mSelectorData->activeField].active = true;
}