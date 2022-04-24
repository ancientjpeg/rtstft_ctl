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
  setDefaultLookAndFeel(this);
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
  int        lineLength_2     = radiusFromCenter * 0.1f;
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

void RT_LookAndFeel::Default::drawComboBox(juce::Graphics &g, int width,
                                           int height, bool isButtonDown,
                                           int buttonX, int buttonY,
                                           int buttonW, int buttonH,
                                           juce::ComboBox &box)
{
  auto scheme = getCurrentColourScheme();
  g.setColour(scheme.getUIColour(defaultFill));
  auto bounds = juce::Rectangle<float>(buttonX, buttonY, buttonW, buttonH);
  juce::Rectangle<float> triBounds
      = bounds.removeFromRight(0.9f * bounds.getWidth());
  int bound = std::min(triBounds.getWidth(), triBounds.getHeight());
  triBounds = triBounds.withSizeKeepingCentre(bound / 3, bound / 3);
  juce::Path path;
  path.startNewSubPath(triBounds.getBottomLeft());
  path.lineTo(triBounds.getBottomRight());
  path.lineTo(triBounds.getTopLeft().withX(triBounds.getCentreX()));
  path.closeSubPath();
  if (box.isPopupActive()) {
    path.applyTransform(juce::AffineTransform::rotation(
        juce::MathConstants<float>::pi, triBounds.getCentreX(),
        triBounds.getCentreY()));
  }
  g.strokePath(path, juce::PathStrokeType(widgetBorderSize));
}

// void RT_LookAndFeel::Default::drawPopupMenuBackgroundWithOptions(
//     juce::Graphics &g, int width, int height,
//     const juce::PopupMenu::Options &opts)
// {
//   g.fillAll(getCurrentColourScheme().getUIColour(windowBackground));
//   g.fillAll(juce::Colours::grey);
// }

int RT_LookAndFeel::Default::getPopupMenuBorderSize()
{
  return widgetBorderSize;
}

// juce::PopupMenu::Options
// RT_LookAndFeel::Default::getOptionsForComboBoxPopupMenu(juce::ComboBox &box,
//                                                         juce::Label &label)
// {
//   return juce::PopupMenu::Options()
//       .withTargetComponent(&box)
//       .withItemThatMustBeVisible(box.getSelectedId())
//       .withInitiallySelectedItem(box.getSelectedId())
//       .withMinimumWidth(box.getWidth())
//       .withMaximumNumColumns(1)
//       .withStandardItemHeight(label.getHeight());
// }

// void RT_LookAndFeel::Default::getIdealPopupMenuItemSizeWithOptions(
//     const juce::String &text, bool isSeparator, int standardMenuItemHeight,
//     int &idealWidth, int &idealHeight, const juce::PopupMenu::Options &opts)
// {
//   juce::LookAndFeel_V2::getIdealPopupMenuItemSize(
//       text, isSeparator, standardMenuItemHeight, idealWidth, idealHeight);
//   idealWidth = opts.getMinimumWidth();
// }
