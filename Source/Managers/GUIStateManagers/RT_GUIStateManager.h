/*
  ==============================================================================

    RT_GUIStateManager.h
    Created: 9 Apr 2022 12:03:40pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../../Components/Interactive/RT_SelectorMenu.h"
#include "../Interface/RT_ProcessorInterface.h"

static const juce::StringArray RT_MANIP_GUI_IDS = {"Gate", "Gain", "Limit"};

class RT_GUIStateManager {
  RT_ProcessorInterface            *mInterface;

  std::list<juce::String>           mCommandHistory;
  std::list<juce::String>::iterator mHistoryIterator;
  int                               mCommandHistoryMax;

  RT_SelectorMenu::SelectorData     mManipSelectorData;

public:
  RT_GUIStateManager(RT_ProcessorInterface              *inInterface,
                     std::initializer_list<juce::String> inManipSelectorFields,
                     int inCommandHistoryMaxSize = 5);
  ~RT_GUIStateManager();

  juce::String                   getNextStringInHistory(bool reverse);
  void                           pushNewHistoryCommand(juce::String &s);
  bool                           commandHistoryIsEmpty();
  RT_SelectorMenu::SelectorData *getSelectorData();
};