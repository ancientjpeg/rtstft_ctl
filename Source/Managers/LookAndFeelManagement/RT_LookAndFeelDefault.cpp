/*
  ==============================================================================

    RT_DefaultLookAndFeel.cpp
    Created: 19 Mar 2022 7:28:48pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_LookAndFeelManagement.h"

RT_LookAndFeel::Default::Default(
    juce::LookAndFeel_V4::ColourScheme inColourScheme)
    : juce::LookAndFeel_V4(inColourScheme)
{
}

void RT_LookAndFeel::Default::drawRotarySlider(juce::Graphics &g, int x, int y,
                                               int width, int height,
                                               float sliderPosProportional,
                                               float rotaryStartAngle,
                                               float rotaryEndAngle,
                                               juce::Slider &s)
{
  using enum juce::LookAndFeel_V4::ColourScheme::UIColour;
  juce::Rectangle<float> bounds(width, height);
  g.drawEllipse(bounds, 2.f);
}
