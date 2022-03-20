/*
  ==============================================================================

    RT_Slider.cpp
    Created: 19 Mar 2022 7:40:26pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_Sliders.h"

RT_Sliders::RotaryKnob::RotaryKnob()
    : juce::Slider(juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow)
{
  float rotAngle     = (2.f / 3.f) * juce::MathConstants<float>::pi;
  float rotAngleLeft = juce::MathConstants<float>::twoPi - rotAngle;
  setRotaryParameters({rotAngleLeft, rotAngle, true});
  setColour(ColourIds::textBoxTextColourId, juce::Colours::black);
}

void RT_Sliders::RotaryKnob::paint(juce::Graphics &g)
{
  auto  rots = getRotaryParameters();
  auto &laf  = static_cast<juce::LookAndFeel_V4 &>(getLookAndFeel());
  laf.drawRotarySlider(g, 0, 0, getWidth(), getHeight() - getTextBoxHeight(), 0,
                       rots.startAngleRadians, rots.endAngleRadians, *this);
}