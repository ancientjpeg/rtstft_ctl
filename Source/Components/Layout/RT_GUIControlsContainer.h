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
#include "../Interactive/RT_InplaceComboBox.h"
#include "../Interactive/RT_Sliders.h"
#include <JuceHeader.h>

//==============================================================================

class RT_GUIControlsContainer : public juce::Component {
public:
  RT_GUIControlsContainer(RT_ProcessorInterface *inInterface);
  ~RT_GUIControlsContainer() = default;

  void paint(juce::Graphics &) override;
  void resized() override;

private:
  RT_ProcessorInterface  *mInterface;
  RT_InplaceComboBox<int> mFrameSizeSelector, mOverlapSelector;
  juce::OwnedArray<RT_Sliders::LabelledRotaryKnob> mKnobs;
  juce::OwnedArray<juce::AudioProcessorValueTreeState::SliderAttachment>
      mKnobAttachments;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_GUIControlsContainer)
};
