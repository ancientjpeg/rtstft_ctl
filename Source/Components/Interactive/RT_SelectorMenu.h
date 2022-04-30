/*
  ==============================================================================

    RT_SelectorMenu.h
    Created: 9 Apr 2022 11:22:43am
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

// #include "../../Managers/DSPStateManagers/RTSTFT_Manager.h"
#include "../../Managers/Interface/RT_ProcessorInterface.h"
#include "../Utility/RT_Component.h"
#include <JuceHeader.h>

class RT_SelectorMenu : public RT_Component, public juce::Value::Listener {
public:
  RT_SelectorMenu(RT_ProcessorInterface              *inInterface,
                  const juce::Value                  &inValueToLink,
                  std::initializer_list<juce::String> inPossibleSelections,
                  bool inUseNullSelection = true, bool inIsVertical = false);
  RT_SelectorMenu(RT_ProcessorInterface  *inInterface,
                  const juce::Value      &inValueToLink,
                  const juce::StringArray inPossibleSelections,
                  bool inUseNullSelection = true, bool inIsVertical = false);

  ~RT_SelectorMenu()                       = default;

  std::function<void(void)> onNewSelection = []() {};
  juce::String              getActiveSelection();
  int                       getActiveSelectionIndex();
  void                      setNullSelection(juce::String inNewNull);

  void                      paint(juce::Graphics &g) override;
  void                      resized() override;
  void                      mouseDown(const juce::MouseEvent &event) override;
  void                      mouseMove(const juce::MouseEvent &event) override;
  void                      mouseExit(const juce::MouseEvent &event) override;

  void                      valueChanged(juce::Value &value) override;

private:
  juce::Point<float>                  mLastInternalMousePos;
  const juce::StringArray             mPossibleSelections;
  std::vector<juce::Rectangle<float>> mSelectionsBounds;
  const juce::Rectangle<float>       *mSelectionWithHover = nullptr;
  juce::Value                         mLinkedValue;
  bool                                mUseNullSelection;
  bool                                mVertical;

  bool checkHover(const juce::Rectangle<float> *comp);
};