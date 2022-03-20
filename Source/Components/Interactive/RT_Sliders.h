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
class RotarySlider : public juce::Slider {
public:
  RotarySlider()  = default;
  ~RotarySlider() = default;
  void paint(juce::Graphics &g) override;
};
} // namespace RT_Sliders