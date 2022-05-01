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

RT_SelectorMenu::RT_SelectorMenu(
    RT_ProcessorInterface *inInterface, const juce::Value &inValueToLink,
    std::initializer_list<juce::String> inPossibleSelections,
    bool inUseNullSelection, bool inIsVertical)
    : RT_SelectorMenu(inInterface, inValueToLink,
                      juce::StringArray(inPossibleSelections),
                      inUseNullSelection, inIsVertical)
{
}

RT_SelectorMenu::RT_SelectorMenu(RT_ProcessorInterface  *inInterface,
                                 const juce::Value      &inValueToLink,
                                 const juce::StringArray inPossibleSelections,
                                 bool inUseNullSelection, bool inIsVertical)
    : RT_Component(inInterface), mPossibleSelections(inPossibleSelections),
      mLinkedValue(inValueToLink), mUseNullSelection(inUseNullSelection),
      mVertical(inIsVertical)
{
  mLinkedValue.addListener(this);
  valueChanged(mLinkedValue);
}

juce::String RT_SelectorMenu::getActiveSelection()
{
  return (juce::String)mLinkedValue.getValue();
}

int RT_SelectorMenu::getActiveSelectionIndex()
{
  return mPossibleSelections.indexOf(getActiveSelection());
}

void RT_SelectorMenu::paint(juce::Graphics &g)
{
  juce::Font font = juce::Font(12, 0);
  g.setFont(font);
  auto lamf = mInterface->getLookAndFeelManager();
  g.fillAll(lamf->getUIColour(defaultFill));
  for (int i = 0; i < mPossibleSelections.size(); i++) {
    const auto &boundCurr         = mSelectionsBounds[i];
    const auto &str               = mPossibleSelections[i];
    bool        isActiveSelection = str == getActiveSelection();
    auto        color_id
        = isActiveSelection
              ? defaultFill
              : (checkHover(&boundCurr) ? highlightedFill : windowBackground);
    g.setColour(lamf->getUIColour(color_id));
    g.fillRect(boundCurr);

    auto text_bounds
        = boundCurr.withSizeKeepingCentre(font.getStringWidth(str), 14);
    color_id = isActiveSelection ? highlightedText : defaultText;
    g.setColour(lamf->getUIColour(color_id));
    g.drawText(str.toLowerCase(), text_bounds, 12);
  }
}

void RT_SelectorMenu::resized()
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

void RT_SelectorMenu::mouseDown(const juce::MouseEvent &event)
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
    assert(false);
  }

  repaint();
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

void RT_SelectorMenu::valueChanged(juce::Value &value)
{
  assert(value.refersToSameSourceAs(mLinkedValue));
  repaint();
}
