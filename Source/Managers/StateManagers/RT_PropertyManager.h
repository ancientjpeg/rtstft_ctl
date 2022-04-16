/*
  ==============================================================================

    RT_PropertyManager.h
    Created: 9 Apr 2022 12:03:40pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../../Components/Interactive/RT_SelectorMenu.h"
#include "../Interface/RT_ProcessorInterface.h"

static const juce::StringArray RT_MANIP_GUI_IDS = {"Gain", "Gate", "Limit"};

class RT_PropertyManager {
  RT_ProcessorInterface            *mInterface;

  juce::ValueTree                   mValueTree;

  std::list<juce::String>           mCommandHistory;
  std::list<juce::String>::iterator mHistoryIterator;
  int                               mCommandHistoryMax;

  RT_SelectorMenu::SelectorData     mManipSelectorData;

public:
  RT_PropertyManager(RT_ProcessorInterface              *inInterface,
                     std::initializer_list<juce::String> inManipSelectorFields,
                     int inCommandHistoryMaxSize = 5);
  ~RT_PropertyManager();

  juce::String                   getNextStringInHistory(bool reverse);
  void                           pushNewHistoryCommand(juce::String &s);
  bool                           commandHistoryIsEmpty();
  void                           resetCommandHistoryPos();
  RT_SelectorMenu::SelectorData *getSelectorData();
};
