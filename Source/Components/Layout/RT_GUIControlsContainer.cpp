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
#include "../../Managers/StateManagers/RT_PropertyManager.h"
#include <JuceHeader.h>

//==============================================================================
RT_GUIControlsContainer::RT_GUIControlsContainer(
    RT_ProcessorInterface *inInterface)
    : mInterface(inInterface), mFrameSizeSelector("FrameSizeSelector"),
      mOverlapSelector("OverlapSelector")
{
  auto p        = mInterface->getRTSTFTManager()->getParamsStruct();
  auto prop_man = mInterface->getPropertyManager();

  for (int i = RT_FFT_MIN_POW; i <= RT_FFT_MAX_POW; i++) {
    mFrameSizeSelector.addItem(juce::String(1 << i), 1 << i);
  }
  mFrameSizeSelector.setSelectedId(
      p->frame_size, juce::NotificationType::dontSendNotification);
  prop_man->addComboBoxAttachment(
      RT_FFT_MODIFIER_IDS[RT_FFT_MODIFIER_FRAME_SIZE], &mFrameSizeSelector);
  addAndMakeVisible(mFrameSizeSelector);

  for (int i = 1; i <= 4; i++) {
    mOverlapSelector.addItem(juce::String(1 << i), 1 << i);
  }
  mOverlapSelector.setSelectedId(p->overlap_factor,
                                 juce::NotificationType::dontSendNotification);
  prop_man->addComboBoxAttachment(
      RT_FFT_MODIFIER_IDS[RT_FFT_MODIFIER_OVERLAP_FACTOR], &mOverlapSelector);
  addAndMakeVisible(mOverlapSelector);

  for (int i = 0; i < RT_PARAM_FLAVOR_COUNT; i++) {
    mKnobs.add(std::make_unique<RT_Sliders::LabelledRotaryKnob>(
        RT_PARAM_RANGES.getRawDataPointer() + i, RT_PARAM_IDS[i].toLowerCase(),
        0.2f));
    // if (i <= RT_PARAM_GAIN_MOD && i >= RT_PARAM_LIMIT_MOD) {
    auto *k_ptr = mKnobs[i]->getKnobPointer();
    k_ptr->setVelocityBasedMode(true);
    k_ptr->setVelocityModeParameters(5, 2);
    // }
    mKnobAttachments.add(
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            *mInterface->getParameterManager()->getValueTreeState(),
            RT_PARAM_IDS[i], *mKnobs[i]->getKnobPointer()));
    addAndMakeVisible(*mKnobs[i]);
  }
}

void RT_GUIControlsContainer::paint(juce::Graphics &g)
{
  g.fillAll(getLookAndFeel().findColour(
      juce::ResizableWindow::backgroundColourId)); // clear the background
}

void RT_GUIControlsContainer::resized()
{
  // This method is where you should set the bounds of any child
  // components that your component contains..
  auto bounds         = getLocalBounds();
  auto dropdownBounds = bounds.removeFromTop(40);
  mFrameSizeSelector.setBounds(
      dropdownBounds.removeFromRight(dropdownBounds.getWidth() / 2));
  mOverlapSelector.setBounds(dropdownBounds);
  int knobAreaVertical = bounds.getHeight() / mKnobs.size();
  for (int i = 0; i < RT_PARAM_FLAVOR_COUNT; i++) {
    auto thisBound = bounds.removeFromTop(knobAreaVertical);
    thisBound.setWidth(thisBound.getHeight() * 2);
    if (i & 1) {
      thisBound.translate(getWidth() - thisBound.getRight(), 0);
    }

    mKnobs[i]->setBounds(thisBound);
  }
}

std::unique_ptr<juce::XmlElement> getXMLSerializedProperties();