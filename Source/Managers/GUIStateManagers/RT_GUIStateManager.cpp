/*
  ==============================================================================

    RT_GUIStateManager.cpp
    Created: 9 Apr 2022 12:03:40pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_GUIStateManager.h"

RT_GUIStateManager::RT_GUIStateManager(
    RT_ProcessorInterface              *inInterface,
    std::initializer_list<juce::String> inManipSelectorFields,
    int                                 inCommandHistoryMaxSize)
    : mInterface(inInterface), mCommandHistoryMax(inCommandHistoryMaxSize),
      mManipSelectorData(inManipSelectorFields)
{
}

RT_GUIStateManager::~RT_GUIStateManager() {}

juce::String RT_GUIStateManager::getNextStringInHistory(bool reverse)
{
  juce::String str;
  if (commandHistoryIsEmpty()) {
    return str;
  }

  if (reverse) {
    if (mHistoryIterator != mCommandHistory.begin()) {
      str = *--mHistoryIterator;
    }
    return str;
  }
  else {
    if (mHistoryIterator == mCommandHistory.end()) {
      --mHistoryIterator;
    }
    str = *mHistoryIterator++;
    return str;
  }
}

bool RT_GUIStateManager::commandHistoryIsEmpty()
{
  return mCommandHistory.empty();
}
void RT_GUIStateManager::pushNewHistoryCommand(juce::String &s)
{
  mCommandHistory.push_front(s);
  if (mCommandHistory.size() > mCommandHistoryMax) {
    mCommandHistory.pop_back();
  }
}

RT_SelectorMenu::SelectorData *RT_GUIStateManager::getSelectorData()
{
  return &mManipSelectorData;
}