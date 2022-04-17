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
    : mInterface(inInterface), mValueTree("rtstft_ctl_properties"),
      mCommandHistoryMax(inCommandHistoryMaxSize),
      mManipSelectorData(inManipSelectorFields)
{
  const rt_params p = mInterface->getRTSTFTManager()->getParamsStruct();

  mValueTree.appendChild(
      juce::ValueTree(
          "rt_params",
          {
              {RT_FFT_MODIFIER_IDS[RT_FFT_MODIFIER_FFT_SIZE], (int)p->fft_size},
              {RT_FFT_MODIFIER_IDS[RT_FFT_MODIFIER_OVERLAP_FACTOR],
               (int)p->overlap_factor},
              {RT_FFT_MODIFIER_IDS[RT_FFT_MODIFIER_PAD_FACTOR],
               (int)p->pad_factor},
              {"manip_multichannel", p->manip_multichannel},
          },
          {juce::ValueTree("rt_chans", {}, {})

          }),
      nullptr);
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

void RT_PropertyManager::comboBoxChanged(juce::ComboBox *inChangedComboBox)
{
  DBG("Got ComboBox Notif");
  auto rt_man = mInterface->getRTSTFTManager();
  auto p      = rt_man->getParamsStruct();
  auto text   = inChangedComboBox->getText();
  if (inChangedComboBox->getName() == "FrameSizeSelector") {
    rt_man->changeFFTSize(text.getIntValue(), p->overlap_factor);
  }
  if (inChangedComboBox->getName() == "OverlapSelector") {
    rt_man->changeFFTSize(p->frame_size, text.getIntValue());
  }
}
