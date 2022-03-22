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
  float currentKnobAngle
      = rotaryStartAngle
        + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

  auto       bounds    = s.getLocalBounds();
  auto       center    = bounds.getCentre();
  int        knobDim   = std::min(bounds.getHeight(), bounds.getWidth()) / 2;
  int        radius    = knobDim / 3;
  int        radiusInv = (bounds.getHeight() - 2 * radius) / 2;
  juce::Path knobPath;
  float      knobSpread = .2;
  knobPath.addCentredArc(center.getX(), center.getY(), radius, radius, 0,
                         knobSpread, juce::MathConstants<float>::twoPi-knobSpread, true);

  knobPath.startNewSubPath(center.getX(), radiusInv);
  knobPath.lineTo(center.getX(), radiusInv * 1.1);
  knobPath.closeSubPath();
  g.strokePath(
      knobPath, juce::PathStrokeType(2.f),
      juce::AffineTransform::rotation(currentKnobAngle, center.x, center.y));
}

juce::Label *RT_LookAndFeel::Default::createSliderTextBox(juce::Slider &s) {}
