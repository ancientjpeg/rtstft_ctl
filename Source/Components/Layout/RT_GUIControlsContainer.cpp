/*
  ==============================================================================

    RT_GUIControlsContainer.cpp
    Created: 19 Mar 2022 1:16:07pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_GUIControlsContainer.h"
#include "../../Managers/DSPStateManagers/RTSTFT_Manager.h"
#include "../../Managers/DSPStateManagers/RT_ParameterManager.h"
#include <JuceHeader.h>

//==============================================================================
RT_GUIControlsContainer::RT_GUIControlsContainer(
    RT_ProcessorInterface *inInterface)
    : mInterface(inInterface)
{
  for (int i = 0; i < RT_PARAM_FLAVOR_COUNT; i++) {
    mKnobs.add(std::make_unique<RT_Sliders::LabelledRotaryKnob>(
        RT_PARAM_RANGES.getRawDataPointer() + i, RT_PARAM_IDS[i]));
    mKnobAttachments.add(
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            *mInterface->getParameterManager()->getValueTreeState(), RT_PARAM_IDS[i],
            *mKnobs[i]->getKnobPointer()));
    addAndMakeVisible(*mKnobs[i]);
  }
}

RT_GUIControlsContainer::~RT_GUIControlsContainer() {}

void RT_GUIControlsContainer::paint(juce::Graphics &g)
{
  g.fillAll(getLookAndFeel().findColour(
      juce::ResizableWindow::backgroundColourId)); // clear the background
}

void RT_GUIControlsContainer::resized()
{
  // This method is where you should set the bounds of any child
  // components that your component contains..
  auto bounds           = getLocalBounds();
  int  knobAreaVertical = getHeight() / mKnobs.size();
  for (int i = 0; i < RT_PARAM_FLAVOR_COUNT; i++) {
    auto thisBound = bounds.removeFromTop(knobAreaVertical);
    thisBound.setWidth(thisBound.getHeight());
    if (i & 1) {
      thisBound.translate(getWidth() - thisBound.getRight(), 0);
    }

    mKnobs[i]->setBounds(thisBound);
  }
}
