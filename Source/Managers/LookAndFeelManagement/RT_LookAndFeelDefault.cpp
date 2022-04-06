/*
  ==============================================================================

    RT_DefaultLookAndFeel.cpp
    Created: 19 Mar 2022 7:28:48pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_LookAndFeelManagement.h"
using enum juce::LookAndFeel_V4::ColourScheme::UIColour;

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
  float currentKnobAngle
      = rotaryStartAngle
        + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

  float strokeThickness = 2.f;
  auto  bounds          = s.getLocalBounds();
  auto  center          = bounds.getCentre();
  int   radiusFromCenter
      = std::min(bounds.getHeight(), bounds.getWidth()) / 2 * 0.8f;
  int        radiusFromBounds = (bounds.getHeight() - 2 * radiusFromCenter) / 2;
  int        lineLength_2     = radiusFromCenter * 0.16f;
  juce::Path knobPath;
  float      knobSpread = .2;
  knobPath.addCentredArc(center.getX(), center.getY(), radiusFromCenter,
                         radiusFromCenter, 0, knobSpread,
                         juce::MathConstants<float>::twoPi - knobSpread, true);

  knobPath.startNewSubPath(center.getX(), radiusFromBounds - lineLength_2
                                              + strokeThickness * 0.5f);
  knobPath.lineTo(center.getX(),
                  radiusFromBounds + lineLength_2 + strokeThickness * 0.5f);
  knobPath.closeSubPath();
  g.strokePath(
      knobPath, juce::PathStrokeType(strokeThickness),
      juce::AffineTransform::rotation(currentKnobAngle, center.x, center.y));
}

juce::Label *RT_LookAndFeel::Default::createSliderTextBox(juce::Slider &s)
{
  juce::Label *label_ptr = new juce::Label;
  juce::String str;
  str << s.getValue();
  label_ptr->setText(str, juce::NotificationType::dontSendNotification);
  label_ptr->setColour(juce::Label::textColourId,
                       getCurrentColourScheme().getUIColour(defaultText));
  return label_ptr;
}

void RT_LookAndFeel::Default::drawButtonBackground(
    juce::Graphics &g, juce::Button &b, const juce::Colour &backgroundColour,
    bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
  if (shouldDrawButtonAsDown) {
    g.fillAll(juce::Colour(220, 232, 255));
  }
  else if (shouldDrawButtonAsHighlighted) {
    g.fillAll(juce::Colours::aliceblue);
  }
  else
    g.fillAll(backgroundColour);
}
