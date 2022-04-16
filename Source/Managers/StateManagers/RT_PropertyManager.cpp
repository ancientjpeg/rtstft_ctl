/*
  ==============================================================================

    RT_PropertyManager.cpp
    Created: 9 Apr 2022 12:03:40pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_PropertyManager.h"

RT_PropertyManager::RT_PropertyManager(
    RT_ProcessorInterface              *inInterface,
    std::initializer_list<juce::String> inManipSelectorFields,
    int                                 inCommandHistoryMaxSize)
    : mInterface(inInterface), mValueTree("rt_properties"),

      mCommandHistoryMax(inCommandHistoryMaxSize),
      mManipSelectorData(inManipSelectorFields)
{
  mHistoryIterator = mCommandHistory.begin();
}

RT_PropertyManager::~RT_PropertyManager() {}

juce::String RT_PropertyManager::getNextStringInHistory(bool reverse)
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

void RT_PropertyManager::resetCommandHistoryPos()
{
  mHistoryIterator = mCommandHistory.begin();
}

bool RT_PropertyManager::commandHistoryIsEmpty()
{
  return mCommandHistory.empty();
}
void RT_PropertyManager::pushNewHistoryCommand(juce::String &s)
{
  mCommandHistory.push_front(s);
  if (mCommandHistory.size() > mCommandHistoryMax) {
    mCommandHistory.pop_back();
  }
}

RT_SelectorMenu::SelectorData *RT_PropertyManager::getSelectorData()
{
  return &mManipSelectorData;
}