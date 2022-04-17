/*
  ==============================================================================

    RT_PropertyManager.h
    Created: 9 Apr 2022 12:03:40pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../../Components/Interactive/RT_SelectorMenu.h"
#include "../DSPStateManagers/RT_ParameterDefines.h"
#include "../Interface/RT_ProcessorInterface.h"

class RT_PropertyManager {

public:
  RT_PropertyManager(RT_ProcessorInterface              *inInterface,
                     std::initializer_list<juce::String> inManipSelectorFields,
                     int inCommandHistoryMaxSize = 5);
  ~RT_PropertyManager();

  juce::String                      getNextStringInHistory(bool reverse);
  void                              pushNewHistoryCommand(juce::String &s);
  bool                              commandHistoryIsEmpty();
  void                              resetCommandHistoryPos();

  RT_SelectorMenu::SelectorData    *getSelectorData();

  juce::ValueTree                  &getValueTreeRef();
  std::unique_ptr<juce::XmlElement> getXMLSerializedProperties();
  void                              replaceState(juce::ValueTree &inNewState);
  bool assertTreeCanValidlyReplace(juce::ValueTree &inComparisonTree);
  static bool
       assertValueTreesHaveCompatibleLayout(juce::ValueTree &inTemplateTree,
                                            juce::ValueTree &inComparisonTree);

  void addComboBoxAttachment(juce::Identifier inTargetValueID,
                             juce::ComboBox  *inTargetComboBox);
  void setAllAttachmentsIgnoreCallbacks(bool inIgnoreCallbacks);

  class ComboBoxAttachment : public juce::ComboBox::Listener {
    RT_PropertyManager *mPropertyManager;
    juce::Identifier    mAttachedValueID;
    juce::Value         mAttachedValue;
    juce::ComboBox     *mAttachedComboBox;
    bool                mIgnoreCallbacks = false;

  public:
    ComboBoxAttachment(RT_PropertyManager *inPropertyManager,
                       juce::Identifier    inValueIDToAttach,
                       juce::ComboBox     *inComboBoxToAttach);
    ~ComboBoxAttachment() = default;
    void comboBoxChanged(juce::ComboBox *inChangedComboBox) override;
    void selectNewID(int newID);
    void selectNewIDWithoutNotification(int newID);
    friend class RT_PropertyManager;
  };

private:
  juce::OwnedArray<ComboBoxAttachment> mComboBoxAttachments;
  RT_ProcessorInterface               *mInterface;

  juce::ValueTree                      mValueTree;

  std::list<juce::String>              mCommandHistory;
  std::list<juce::String>::iterator    mHistoryIterator;
  int                                  mCommandHistoryMax;

  RT_SelectorMenu::SelectorData        mManipSelectorData;
};
