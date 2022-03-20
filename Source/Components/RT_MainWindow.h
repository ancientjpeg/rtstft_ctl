/*
  ==============================================================================

    RT_MainWindow.h
    Created: 18 Mar 2022 2:09:33pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../Managers/Interface/RT_ProcessorInterface.h"
#include "Layout/RT_CommandLineContainer.h"
#include "Layout/RT_FFTDisplayContainer.h"
#include "Layout/RT_GUIControlsContainer.h"
#include <JuceHeader.h>

class RT_MainWindow : public juce::Component {

  RT_ProcessorInterface  *mInterface;
  RT_GUIControlsContainer mGUIControlsContainer;
  RT_FFTDisplayContainer  mFFTDisplayContainer;
  RT_CommandLineContainer mCommandLineContainer;

public:
  RT_MainWindow(RT_ProcessorInterface *inInterface);
  void paint(juce::Graphics &g) override;
  void resized() override;
};
