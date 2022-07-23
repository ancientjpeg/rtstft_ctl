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
  mFrameSizeSelector.onMenuVisibilityChange = [this]() { repaint(); };
  mOverlapSelector.addAndMakeVisibleWithParent(this);
  mOverlapSelector.onMenuVisibilityChange = [this]() { repaint(); };
}

void RT_GUIControlsContainer::paint(juce::Graphics &g)
{
  g.fillAll(RT_LookAndFeel::BLACK);
  g.setColour(RT_LookAndFeel::WHITE);
  g.fillRect(mBodyBounds);
}

void RT_GUIControlsContainer::paintOverChildren(juce::Graphics &g)
{
  if (mFrameSizeSelector.isMenuSectionVisible()
      || mOverlapSelector.isMenuSectionVisible()) {
    g.setColour(RT_LookAndFeel::BLACK);
    g.fillRect(mVertDivider);
  }
}

void RT_GUIControlsContainer::resized()
{
  auto bounds        = getLocalBounds();
  auto header_height = 40;
  auto body_y        = header_height + RT_LookAndFeel::PADDING_SMALL;
  auto body_height   = bounds.getHeight() - body_y;
  /* 2x small padding for column separator */
  auto divider_width  = RT_LookAndFeel::PADDING_SMALL;
  auto column_0_width = bounds.getWidth() / 2 - divider_width / 2;
  auto column_1_left  = column_0_width + divider_width;
  auto column_1_width = bounds.getWidth() - column_1_left;

  mFrameSizeSelector.getLabel()->setBounds(column_1_left, 0, column_1_width,
                                           header_height);
  mFrameSizeSelector.getMenuSection()->setBounds(column_1_left, body_y,
                                                 column_1_width, body_height);

  mOverlapSelector.getLabel()->setBounds(0, 0, column_0_width, header_height);
  mOverlapSelector.getMenuSection()->setBounds(0, body_y, column_0_width,
                                               body_height);

  mVertDivider = juce::Rectangle<int>(column_0_width, body_y, divider_width,
                                      body_height);

  mBodyBounds  = getLocalBounds().withY(body_y).withHeight(body_height);
  int  knobAreaVertical = mBodyBounds.getHeight() / mKnobs.size();

  auto body_bounds = mBodyBounds.withTrimmedTop(RT_LookAndFeel::PADDING_MAIN)
                         .withTrimmedBottom(2 * RT_LookAndFeel::PADDING_MAIN);

  for (int i = 0; i < RT_PARAM_FLAVOR_COUNT; i++) {
    auto thisBound = body_bounds.removeFromTop(knobAreaVertical)
                         .withWidth(body_bounds.getWidth() / 2);
    if (i & 1) {
      thisBound.translate(body_bounds.getWidth() / 2, 0);
    }
    else {
    }
    mKnobs[i]->setBounds(thisBound);
  }
}
