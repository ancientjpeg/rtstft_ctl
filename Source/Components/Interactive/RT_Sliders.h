/*
  ==============================================================================

    RT_Slider.h
    Created: 19 Mar 2022 7:40:01pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace RT_Sliders {
class RotaryKnob : public juce::Slider {
  float mStartAngle = juce::MathConstants<float>::pi;
  float mEndAngle   = juce::MathConstants<float>::pi;

public:
  RotaryKnob();
  ~RotaryKnob() = default;
  void paint(juce::Graphics &g) override;
};
} // namespace RT_Sliders