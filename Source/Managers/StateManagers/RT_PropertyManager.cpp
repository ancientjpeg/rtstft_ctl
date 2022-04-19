/*
  ==============================================================================

    RT_PropertyManager.cpp
    Created: 9 Apr 2022 12:03:40pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_PropertyManager.h"
#include "../DSPStateManagers/RTSTFT_Manager.h"

RT_PropertyManager::RT_PropertyManager(
    RT_ProcessorInterface              *inInterface,
    std::initializer_list<juce::String> inManipSelectorFields,
    int                                 inCommandHistoryMaxSize)
    : mInterface(inInterface), mCommandHistoryMax(inCommandHistoryMaxSize),
      mManipSelectorData(inManipSelectorFields)
{
  const rt_params p = mInterface->getRTSTFTManager()->getParamsStruct();

  mValueTree        = juce::ValueTree(
             "rtstft_ctl_properties",
             {
                 {RT_FFT_MODIFIER_IDS[RT_FFT_MODIFIER_FRAME_SIZE], (int)p->fft_size},
                 {RT_FFT_MODIFIER_IDS[RT_FFT_MODIFIER_OVERLAP_FACTOR],
                  (int)p->overlap_factor},
                 {RT_FFT_MODIFIER_IDS[RT_FFT_MODIFIER_PAD_FACTOR], (int)p->pad_factor},
                 {"manip_multichannel", p->manip_multichannel},
      },
             {juce::ValueTree("rt_chans", {}, {})});

  mHistoryIterator = mCommandHistory.begin();
  mValueTree.addListener(mInterface->getRTSTFTManager());
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

juce::ValueTree &RT_PropertyManager::getValueTreeRef() {
  return mValueTree;
}

void             RT_PropertyManager::replaceState(juce::ValueTree &inNewState)
{
  setAllAttachmentsIgnoreCallbacks(true);
  mValueTree.copyPropertiesAndChildrenFrom(inNewState, nullptr);
  setAllAttachmentsIgnoreCallbacks(false);
}

std::unique_ptr<juce::XmlElement>
RT_PropertyManager::getXMLSerializedProperties()
{
  auto el = mValueTree.createXml();
  return el;
}

bool RT_PropertyManager::assertTreeCanValidlyReplace(
    juce::ValueTree &inComparisonTree)
{
  return assertValueTreesHaveCompatibleLayout(mValueTree, inComparisonTree);
}

bool RT_PropertyManager::assertValueTreesHaveCompatibleLayout(
    juce::ValueTree &inTemplateTree, juce::ValueTree &inComparisonTree)
{
  for (int i = 0; i < inTemplateTree.getNumProperties(); i++) {
    if (!inComparisonTree.hasProperty(inTemplateTree.getPropertyName(i))) {
      return false;
    }
  }
  for (auto it = inTemplateTree.begin(); it != inTemplateTree.end(); ++it) {
    auto templateSubTree = (*it);
    auto comparisonSubTree
        = inComparisonTree.getChildWithName(templateSubTree.getType());
    if (!comparisonSubTree.isValid()) {
      return false;
    }
    if (!assertValueTreesHaveCompatibleLayout(templateSubTree,
                                              comparisonSubTree)) {
      return false;
    }
  }
  return true;
}

void RT_PropertyManager::addComboBoxAttachment(juce::Identifier inTargetValueID,
                                               juce::ComboBox *inTargetComboBox)
{
  mComboBoxAttachments.add(std::make_unique<ComboBoxAttachment>(
      this, inTargetValueID, inTargetComboBox));
}

void RT_PropertyManager::setAllAttachmentsIgnoreCallbacks(
    bool inIgnoreCallbacks)
{
  for (auto a : mComboBoxAttachments) {
    a->mIgnoreCallbacks = inIgnoreCallbacks;
  }
}

RT_PropertyManager::ComboBoxAttachment::ComboBoxAttachment(
    RT_PropertyManager *inPropertyManager, juce::Identifier inValueIDToAttach,
    juce::ComboBox *inComboBoxToAttach)
    : mPropertyManager(inPropertyManager), mAttachedValueID(inValueIDToAttach),
      mAttachedValue(mPropertyManager->mValueTree.getPropertyAsValue(
          mAttachedValueID, nullptr)),
      mAttachedComboBox(inComboBoxToAttach)
{
  mAttachedComboBox->addListener(this);
  selectNewID((int)mAttachedValue.getValue());
}

void RT_PropertyManager::ComboBoxAttachment::comboBoxChanged(
    juce::ComboBox *inChangedComboBox)
{
  if (mIgnoreCallbacks) {
    return;
  }
  auto rt_man   = mPropertyManager->mInterface->getRTSTFTManager();
  auto prop_man = mPropertyManager->mInterface->getPropertyManager();
  auto p        = rt_man->getParamsStruct();
  auto text     = inChangedComboBox->getText();
  mAttachedValue.setValue(text.getIntValue());
  //  mPropertyManager->mValueTree.setProperty(mAttachedValueID, text.getIntValue(),
//                                           nullptr);
}

void RT_PropertyManager::ComboBoxAttachment::selectNewID(int newID)
{
  const juce::ScopedValueSetter<bool> svs(mIgnoreCallbacks, true);
  mAttachedComboBox->setSelectedId(newID, juce::sendNotificationSync);
}

void RT_PropertyManager::ComboBoxAttachment::selectNewIDWithoutNotification(
    int newID)
{
  mAttachedComboBox->setSelectedId(newID, juce::dontSendNotification);
}