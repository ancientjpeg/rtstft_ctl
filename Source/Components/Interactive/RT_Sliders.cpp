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

//==============================================================================
RT_Sliders::LabelledRotaryKnob::LabelledRotaryKnob(
    const juce::NormalisableRange<float> *inRangePtr,
    juce::String inLabelString, float inLabelPortion)
    : mLabelString(inLabelString), mLabelPortion(inLabelPortion),
      mKnob(inRangePtr)
{
  mLabel.addListener(this);
  mLabel.setEditable(false, true, true);
  addAndMakeVisible(mKnob, 0);
  addAndMakeVisible(mLabel);
  mKnob.onValueChange = [this]() {
    mLabel.setText(juce::String(mKnob.getValue(), 2),
                   juce::dontSendNotification);
  };
  mLabel.setText(juce::String(mKnob.getValue(), 2), juce::dontSendNotification);
}

void RT_Sliders::LabelledRotaryKnob::resized()
{
  auto bounds   = getLocalBounds();
  mTitleBounds  = bounds.removeFromBottom(mLabelPortion * getHeight());
  int knobBound = std::min(bounds.getWidth(), bounds.getHeight());
  mKnob.setBounds(bounds.withSizeKeepingCentre(knobBound, knobBound));
  int h = 14;
  int w = 32;
  mLabel.setBounds(bounds.withSizeKeepingCentre(w, h));
  mLabel.setJustificationType(juce::Justification::centred);
  mLabel.setFont(juce::Font(h - 4));
}

void RT_Sliders::LabelledRotaryKnob::paint(juce::Graphics &g)
{
  g.setColour(juce::Colours::black);
  g.setFont(mTitleBounds.getHeight());
  g.drawText(mLabelString, mTitleBounds, juce::Justification::centred);
}
void RT_Sliders::LabelledRotaryKnob::labelTextChanged(
    juce::Label *labelThatHasChanged)
{
  mKnob.setValue(mNewKnobValue);
  labelThatHasChanged->setText(juce::String(mKnob.getValue(), 2),
                               juce::dontSendNotification);
}
void RT_Sliders::LabelledRotaryKnob::editorShown(juce::Label      *l,
                                                 juce::TextEditor &t)
{
  t.setColour(juce::TextEditor::focusedOutlineColourId,
              juce::Colours::transparentWhite);
  t.setJustification(juce::Justification(juce::Justification::centredTop));
}
void RT_Sliders::LabelledRotaryKnob::editorHidden(juce::Label      *l,
                                                  juce::TextEditor &t)
{
  mNewKnobValue = t.getText().getFloatValue();

  //  auto s = juce::String(mKnob.getValue(), 2);
  //  mLabel.setText(s, juce::NotificationType::dontSendNotification);
  // l->setText(mLabelString, juce::NotificationType::sendNotification);
}

juce::Slider *RT_Sliders::LabelledRotaryKnob::getKnobPointer()
{
  return &mKnob;
}
