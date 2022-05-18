/*
  ==============================================================================

    RT_ValueLinkedSelectorMenu.cpp
    Created: 9 Apr 2022 11:22:43am
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_ValueLinkedSelectorMenu.h"
#include "../../Managers/LookAndFeelManagement/RT_LookAndFeelManagement.h"
#include "../../Managers/StateManagers/RT_PropertyManager.h"

RT_ValueLinkedSelectorMenu::RT_ValueLinkedSelectorMenu(
    RT_ProcessorInterface *inInterface, const juce::Value &inValueToLink,
    std::initializer_list<juce::String> inPossibleSelections,
    bool inUseNullSelection, bool inIsVertical)
    : RT_ValueLinkedSelectorMenu(inInterface, inValueToLink,
                                 juce::StringArray(inPossibleSelections),
                                 inUseNullSelection, inIsVertical)
{
}

RT_ValueLinkedSelectorMenu::RT_ValueLinkedSelectorMenu(
    RT_ProcessorInterface *inInterface, const juce::Value &inValueToLink,
    const juce::StringArray inPossibleSelections, bool inUseNullSelection,
    bool inIsVertical)
    : RT_Component(inInterface), mPossibleSelections(inPossibleSelections),
      mLinkedValue(inValueToLink), mUseNullSelection(inUseNullSelection),
      mVertical(inIsVertical)
{
  mLinkedValue.addListener(this);
  valueChanged(mLinkedValue);
}

juce::String RT_ValueLinkedSelectorMenu::getActiveSelection()
{
  return mLinkedValue.getValue().toString();
}

int RT_ValueLinkedSelectorMenu::getActiveSelectionIndex()
{
  return mPossibleSelections.indexOf(getActiveSelection());
}

void RT_ValueLinkedSelectorMenu::paint(juce::Graphics &g)
{
  juce::Font font = juce::Font(12, 0);
  g.setFont(font);
  auto lamf = mInterface->getLookAndFeelManager();
  g.fillAll(lamf->getUIColour(outline));
  for (int i = 0; i < mPossibleSelections.size(); i++) {
    const auto &boundCurr         = mSelectionsBounds[i];
    const auto &str               = mPossibleSelections[i];
    bool        isActiveSelection = str == getActiveSelection();
    auto        color_id
        = isActiveSelection
              ? outline
              : (checkHover(&boundCurr) ? defaultFill : windowBackground);
    g.setColour(lamf->getUIColour(color_id));
    g.fillRect(boundCurr);

    auto text_bounds
        = boundCurr.withSizeKeepingCentre(font.getStringWidth(str), 14);
    color_id = isActiveSelection ? highlightedText : defaultText;
    g.setColour(lamf->getUIColour(color_id));
    g.drawText(str.toLowerCase(), text_bounds, 12);
  }
}

void RT_ValueLinkedSelectorMenu::resized()
{
  mSelectionsBounds.clear();
  auto bounds    = getLocalBounds();
  auto numFields = mPossibleSelections.size();
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

void RT_ValueLinkedSelectorMenu::mouseDown(const juce::MouseEvent &event)
{
  juce::String newSelection("");
  for (int i = 0; i < mSelectionsBounds.size(); i++) {
    if (mSelectionsBounds[i].contains(event.position)) {
      newSelection = mPossibleSelections[i];
      break;
    }
  }
  if (getActiveSelection() == newSelection && mUseNullSelection) {
    mLinkedValue.setValue("");
  }
  else if (mPossibleSelections.contains(newSelection)) {
    mLinkedValue.setValue(newSelection);
  }
  else {
    return; // border click
  }

  repaint();
}

void RT_ValueLinkedSelectorMenu::mouseMove(const juce::MouseEvent &event)
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
void RT_ValueLinkedSelectorMenu::mouseExit(const juce::MouseEvent &event)
{
  mSelectionWithHover = nullptr;
  repaint();
}
bool RT_ValueLinkedSelectorMenu::checkHover(const juce::Rectangle<float> *comp)
{
  return comp == mSelectionWithHover;
}

void RT_ValueLinkedSelectorMenu::valueChanged(juce::Value &value)
{
  assert(value.refersToSameSourceAs(mLinkedValue));
  repaint();
}
