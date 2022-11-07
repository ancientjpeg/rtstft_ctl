/*
  ==============================================================================

    RT_Sliders.h
    Created: 19 Mar 2022 7:40:01pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace RT_Sliders {
class RotaryKnob : public juce::Slider {
public:
  RotaryKnob(const juce::NormalisableRange<float> *inRangePtr);
  ~RotaryKnob() = default;
  void paint(juce::Graphics &g) override;

private:
  float mStartAngle = juce::MathConstants<float>::twoPi * 2.f / 3.f;
  float mEndAngle   = juce::MathConstants<float>::twoPi * 4.f / 3.f;
  const juce::NormalisableRange<float> *mRangePtr;
};

class LabelledRotaryKnob : public juce::Component,
                           juce::Label::Listener {

public:
  LabelledRotaryKnob(const juce::NormalisableRange<float> *inRangePtr,
                     juce::String                          inLabelString,
                     float inLabelPortion = 0.1f,
                     int   inSigFigs      = 2);
  void          resized() override;
  juce::Slider *getKnobPointer();
  void          labelTextChanged(juce::Label *labelThatHasChanged) override;
  void          editorShown(juce::Label *l, juce::TextEditor &t) override;
  void          editorHidden(juce::Label *l, juce::TextEditor &t) override;
  void          paint(juce::Graphics &g) override;

private:
  juce::String         mLabelString;
  float                mLabelPortion, mNewKnobValue;
  juce::Label          mLabel;
  juce::Rectangle<int> mTitleBounds;
  RotaryKnob           mKnob;
  int                  mSigFigs;
};
} // namespace RT_Sliders
