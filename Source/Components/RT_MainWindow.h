/*
  ==============================================================================

    RT_MainWindow.h
    Created: 18 Mar 2022 2:09:33pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../Managers/Interface/RT_ProcessorInterface.h"
#include "RT_Component.h"
#include <JuceHeader.h>

class RT_MainWindow : public RT_Component {

public:
  RT_MainWindow();
  void paint(juce::Graphics &g) override;
  void resized() override;
};
