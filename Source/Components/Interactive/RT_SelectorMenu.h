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

class RT_SelectorMenu : public RT_Component {
public:
  RT_SelectorMenu(RT_ProcessorInterface *inInterface);
  ~RT_SelectorMenu() = default;
  void paint(juce::Graphics &g) override;
  void resized() override;
};