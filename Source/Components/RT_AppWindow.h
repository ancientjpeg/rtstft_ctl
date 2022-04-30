/*
  ==============================================================================

    RT_AppWindow.h
    Created: 30 Mar 2022 4:17:34pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../Managers/Interface/RT_ProcessorInterface.h"
#include "Layout/RT_Header.h"
#include "Layout/RT_MainDisplay.h"
#include "Layout/RT_SettingsDisplay.h"
#include "Utility/RT_BorderedComponent.h"
#include <JuceHeader.h>

class RT_AppWindow : public RT_BorderedComponent,
                     public RT_GearButton::Listener {
  RT_Header          mHeader;
  RT_MainDisplay     mMainDisplay;
  RT_SettingsDisplay mSettingsDisplay;

public:
  RT_AppWindow(RT_ProcessorInterface *inInterface, int inBorderSize);
  ~RT_AppWindow() override = default;
  void paintInBorder(juce::Graphics &g) override;
  void resized() override;
  void gearButtonChanged(bool inButtonState) override;
};