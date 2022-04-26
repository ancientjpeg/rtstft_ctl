/*
  ==============================================================================

    RT_LookAndFeelManager.h
    Created: 18 Mar 2022 2:13:06pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#ifndef RT_LOOK_AND_FEEL_MANAGEMENT_H
#define RT_LOOK_AND_FEEL_MANAGEMENT_H

#include <JuceHeader.h>

namespace RT_LookAndFeel {

using enum juce::LookAndFeel_V4::ColourScheme::UIColour;

enum ComponentType { PLUGIN_DEFAULT, NUM_COMPONENT_TYPES };
static const juce::StringArray ComponentNames = {"Default", "Slider", "Button"};
static const int               mainBorderSize = 10;
static const int               mainPadding    = 5;
static const int               widgetBorderSize      = 2;
static const float             headerHeight          = 80;
static const float             cmdHeight             = 50;
static const int               menuItemDefaultHeight = 40;

static const juce::LookAndFeel_V4::ColourScheme DefaultColourScheme(
    juce::Colours::white, juce::Colours::black, juce::Colours::white,
    juce::Colours::black, juce::Colours::black, juce::Colours::black,
    juce::Colours::white, juce::Colours::grey, juce::Colours::black);

class Manager {
  std::vector<std::unique_ptr<juce::LookAndFeel_V4::ColourScheme>> mSkins;
  std::unique_ptr<juce::LookAndFeel_V4>
      mLookAndFeelCollection[NUM_COMPONENT_TYPES];

public:
  Manager();
  juce::LookAndFeel_V4 *getSingleLookAndFeel(ComponentType inComponentTypeID);
  juce::Colour
  getUIColour(juce::LookAndFeel_V4::ColourScheme::UIColour inColorToGet)
  {
    return DefaultColourScheme.getUIColour(inColorToGet); // change this
  }
};

class Default : public juce::LookAndFeel_V4 {
public:
  Default(juce::LookAndFeel_V4::ColourScheme inColourScheme
          = DefaultColourScheme);
  ~Default() = default;
  void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height,
                        float sliderPosProportional, float rotaryStartAngle,
                        float rotaryEndAngle, juce::Slider &s) override;
  juce::Label *createSliderTextBox(juce::Slider &s) override;
  virtual void drawButtonBackground(juce::Graphics &g, juce::Button &b,
                                    const juce::Colour &backgroundColour,
                                    bool shouldDrawButtonAsHighlighted,
                                    bool shouldDrawButtonAsDown) override;
  virtual void drawComboBox(juce::Graphics &g, int width, int height,
                            bool isButtonDown, int buttonX, int buttonY,
                            int buttonW, int buttonH,
                            juce::ComboBox &box) override;

  // virtual void
  // drawPopupMenuBackgroundWithOptions(juce::Graphics &g, int width, int
  // height,
  //                                    const juce::PopupMenu::Options &opts);
  virtual int getPopupMenuBorderSize() override;
  // virtual juce::PopupMenu::Options
  //             getOptionsForComboBoxPopupMenu(juce::ComboBox &box,
  //                                            juce::Label    &label) override;
  // virtual void getIdealPopupMenuItemSizeWithOptions(
  //     const juce::String &text, bool isSeparator, int standardMenuItemHeight,
  //     int &idealWidth, int &idealHeight,
  //     const juce::PopupMenu::Options &) override;

  virtual int getMenuWindowFlags() override { return 1; }
};

} // namespace RT_LookAndFeel

#endif
