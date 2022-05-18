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
    : mInterface(inInterface), mCommandHistoryMax(inCommandHistoryMaxSize)
{
  mValueTree
      = {"rtstft_ctl_properties",
         {
             {RT_FFT_MODIFIER_IDS[RT_FFT_MODIFIER_FRAME_SIZE], (int)1024},
             {RT_FFT_MODIFIER_IDS[RT_FFT_MODIFIER_OVERLAP_FACTOR], (int)8},
             {RT_FFT_MODIFIER_IDS[RT_FFT_MODIFIER_PAD_FACTOR], (int)0},
             {"manip_multichannel",
              RT_MULTICHANNEL_MODE_IDS[RT_MULTICHANNEL_MONO]},
         },
         {juce::ValueTree("rt_chans", {}, {}),
          juce::ValueTree("rt_gui_state",
                          {{"active_manip", ""}, {"active_chan", "0"}}, {})}};

  mHistoryIterator = mCommandHistory.begin();
}

RT_PropertyManager::~RT_PropertyManager() {}

juce::ValueTree &RT_PropertyManager::getValueTreeRef() { return mValueTree; }
juce::ValueTree  RT_PropertyManager::getChansTree()
{
  return mValueTree.getChild(0);
}
juce::ValueTree RT_PropertyManager::getGUIStateTree()
{
  return mValueTree.getChild(1);
}

int RT_PropertyManager::getActiveManipFlavor()
{
  auto active = (juce::String)getGUIStateTree().getProperty("active_manip").toString();
  return RT_MANIP_GUI_IDS.indexOf(active);
}
int RT_PropertyManager::getMultichannelMode()
{
  auto active = (juce::String)mValueTree.getProperty("manip_multichannel").toString();
  return RT_MULTICHANNEL_MODE_IDS.indexOf(active);
}
int RT_PropertyManager::getActiveChannelIndex()
{
  return getGUIStateTree().getProperty("active_chan").toString().getIntValue();
}

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

std::unique_ptr<juce::XmlElement>
RT_PropertyManager::getXMLSerializedProperties()
{
  auto tree = mValueTree.createCopy();
  tree.removeChild(tree.getChildWithName("rt_gui_state"), nullptr);
  auto el = tree.createXml();
  return el;
}

void RT_PropertyManager::replaceState(juce::ValueTree &inNewState)
{
  auto guiState = mValueTree.getChildWithName("rt_gui_state");
  mValueTree.removeChild(guiState, nullptr);
  mValueTree.copyPropertiesAndChildrenFrom(inNewState, nullptr);
  mValueTree.addChild(guiState, 1, nullptr);
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
      if (templateSubTree.getType().toString() == "rt_gui_state")
        continue;
      return false;
    }
    if (!assertValueTreesHaveCompatibleLayout(templateSubTree,
                                              comparisonSubTree)) {
      return false;
    }
  }
  return true;
}
