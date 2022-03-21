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
  auto bounds     = s.getLocalBounds();
  int  knobDim    = std::min(bounds.getHeight(), bounds.getWidth()) / 2;
  auto dialBounds = bounds.withSizeKeepingCentre(knobDim, knobDim);
  g.drawEllipse(dialBounds.toFloat(), 2.f);
  const auto parameters = s.getRotaryParameters();
  auto radians = juce::jmap<float>(s.valueToProportionOfLength(s.getValue()),
                                   parameters.startAngleRadians,
                                   parameters.endAngleRadians);
  // this is likely not quite right and very unfinished
}
