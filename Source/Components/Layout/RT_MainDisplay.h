/*
  ==============================================================================

    RT_MainDisplay.h
    Created: 18 Mar 2022 2:09:33pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../../Managers/Interface/RT_ProcessorInterface.h"
#include "../Utility/RT_Component.h"
#include "RT_CommandLineContainer.h"
#include "RT_FFTDisplayContainer.h"
#include "RT_GUIControlsContainer.h"
#include <JuceHeader.h>

class RT_MainDisplay : public RT_Component {

  RT_GUIControlsContainer mGUIControlsContainer;
  RT_FFTDisplayContainer  mFFTDisplayContainer;
  RT_CommandLineContainer mCommandLineContainer;

public:
  RT_MainDisplay(RT_ProcessorInterface *inInterface);
  ~RT_MainDisplay() override = default;
  void paint(juce::Graphics &g) override;
  void resized() override;
};
