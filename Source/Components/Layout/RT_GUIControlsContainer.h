/*
  ==============================================================================

    RT_GUIControlsContainer.h
    Created: 19 Mar 2022 1:16:07pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../../Managers/DSPStateManagers/RT_ParameterDefines.h"
#include "../../Managers/Interface/RT_ProcessorInterface.h"
#include "../RT_Component.h"
#include <JuceHeader.h>

//==============================================================================

class RT_GUIControlsContainer : public RT_Component {
public:
  RT_GUIControlsContainer(RT_ProcessorInterface *inInterface);
  ~RT_GUIControlsContainer() override;

  void paint(juce::Graphics &) override;
  void resized() override;

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_GUIControlsContainer)
};
