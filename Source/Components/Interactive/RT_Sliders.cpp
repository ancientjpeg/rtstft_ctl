/*
  ==============================================================================

    RT_Sliders.cpp
    Created: 19 Mar 2022 7:40:26pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_Sliders.h"

RT_Sliders::RotaryKnob::RotaryKnob(
    const juce::NormalisableRange<float> *inRangePtr)
    : juce::Slider(juce::Slider::RotaryVerticalDrag, juce::Slider::NoTextBox),
      mRangePtr(inRangePtr)
{
  setRotaryParameters({mStartAngle, mEndAngle, true});
  setColour(ColourIds::textBoxTextColourId, juce::Colours::black);
  setColour(ColourIds::textBoxOutlineColourId, juce::Colour());
}

void RT_Sliders::RotaryKnob::paint(juce::Graphics &g)
{
  auto  rots            = getRotaryParameters();
  auto &laf             = static_cast<juce::LookAndFeel_V4 &>(getLookAndFeel());
  auto  normalizedValue = mRangePtr->convertTo0to1(getValue());
  laf.drawRotarySlider(g, 0, 0, getWidth(), getHeight() - getTextBoxHeight(),
                       normalizedValue, rots.startAngleRadians,
                       rots.endAngleRadians, *this);
}

RT_Sliders::LabelledRotaryKnob::LabelledRotaryKnob(
    const juce::NormalisableRange<float> *inRangePtr,
    juce::String inLabelString, float inLabelPortion)
    : mLabelString(inLabelString), mLabelPortion(inLabelPortion),
      mKnob(inRangePtr)
{
  mLabel.addListener(this);
  mLabel.setText(inLabelString, juce::NotificationType::dontSendNotification);
  mLabel.setEditable(false, true, true);
  addAndMakeVisible(mKnob);
  addAndMakeVisible(mLabel);
}

juce::Slider *RT_Sliders::LabelledRotaryKnob::getKnobPointer()
{
  return &mKnob;
}
void RT_Sliders::LabelledRotaryKnob::resized()
{
  auto bounds = getLocalBounds();
  mLabel.setBounds(bounds.removeFromBottom(mLabelPortion * getHeight()));
  mLabel.setJustificationType(juce::Justification::centred);
  mKnob.setBounds(bounds);
}
void RT_Sliders::LabelledRotaryKnob::labelTextChanged(
    juce::Label *labelThatHasChanged)
{
  mKnob.setValue(mNewKnobValue);
}
void RT_Sliders::LabelledRotaryKnob::editorShown(juce::Label      *l,
                                                 juce::TextEditor &t)
{
  t.setText("newValue",
            juce::NotificationType::dontSendNotification);
  t.setColour(juce::TextEditor::focusedOutlineColourId, juce::Colours::transparentWhite);
  t.setJustification(juce::Justification(juce::Justification::centredTop));
}
void RT_Sliders::LabelledRotaryKnob::editorHidden(juce::Label      *l,
                                                  juce::TextEditor &t)
{
  mNewKnobValue = t.getText().getFloatValue();
  l->setText(mLabelString, juce::NotificationType::sendNotification);
}
