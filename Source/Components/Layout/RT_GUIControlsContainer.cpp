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
    : RT_Component(inInterface),
      mFrameSizeSelector(
          mInterface, RT_FFT_MODIFIER_IDS[RT_FFT_MODIFIER_FRAME_SIZE],
          {32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536},
          RT_LookAndFeel::menuItemDefaultHeight),
      mOverlapSelector(mInterface,
                       RT_FFT_MODIFIER_IDS[RT_FFT_MODIFIER_OVERLAP_FACTOR],
                       {2, 4, 8, 16}, RT_LookAndFeel::menuItemDefaultHeight)
{

  for (int i = 0; i < RT_PARAM_FLAVOR_COUNT; i++) {

    int numSigFigs = i == RT_SCALE_FACTOR_MOD ? 0 : 2;
    mKnobs.add(std::make_unique<RT_Sliders::LabelledRotaryKnob>(
        RT_PARAM_RANGES.getRawDataPointer() + i, RT_PARAM_IDS[i].toLowerCase(),
        0.2f, numSigFigs));
    auto *k_ptr = mKnobs[i]->getKnobPointer();
    k_ptr->setVelocityModeParameters(5, 1);
    mKnobAttachments.add(
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            *mInterface->getParameterManager()->getValueTreeState(),
            RT_PARAM_IDS[i], *mKnobs[i]->getKnobPointer()));
    addAndMakeVisible(*mKnobs[i]);
  }

  mFrameSizeSelector.addAndMakeVisibleWithParent(this);
  mOverlapSelector.addAndMakeVisibleWithParent(this);
}

void RT_GUIControlsContainer::paint(juce::Graphics &g)
{
  g.setColour(
      mInterface->getLookAndFeelManager()->getUIColour(windowBackground));
  g.fillRect(getBounds()); // clear the background
}

void RT_GUIControlsContainer::resized()
{
  auto bounds = getLocalBounds();
  auto comboBoxLabelsBounds
      = bounds.removeFromTop(40 + RT_LookAndFeel::widgetBorderSize);
  auto comboBoxMenusBounds
      = bounds.withTop(bounds.getY() - RT_LookAndFeel::widgetBorderSize);

  mFrameSizeSelector.getLabel()->setBounds(comboBoxLabelsBounds.removeFromRight(
      comboBoxLabelsBounds.getWidth() / 2));
  mFrameSizeSelector.getMenuSection()->setBounds(
      comboBoxMenusBounds.removeFromRight(comboBoxMenusBounds.getWidth() / 2));

  mOverlapSelector.getLabel()->setBounds(comboBoxLabelsBounds.withWidth(
      comboBoxLabelsBounds.getWidth() + RT_LookAndFeel::widgetBorderSize));
  mOverlapSelector.getMenuSection()->setBounds(comboBoxMenusBounds.withWidth(
      comboBoxLabelsBounds.getWidth() + RT_LookAndFeel::widgetBorderSize));

  bounds = getLocalBounds()
               .withTrimmedTop(40 + RT_LookAndFeel::widgetBorderSize
                               + RT_LookAndFeel::mainPadding * 2)
               .withTrimmedBottom(RT_LookAndFeel::mainPadding * 3);
  int knobAreaVertical = bounds.getHeight() / mKnobs.size();
  for (int i = 0; i < RT_PARAM_FLAVOR_COUNT; i++) {
    auto thisBound = bounds.removeFromTop(knobAreaVertical)
                         .withWidth(bounds.getWidth() / 2);
    if (i & 1) {
      thisBound.translate(bounds.getWidth() / 2, 0);
    }
    else {
    }
    mKnobs[i]->setBounds(thisBound);
  }
}
