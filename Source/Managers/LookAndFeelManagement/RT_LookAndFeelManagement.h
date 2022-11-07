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
static const int               PADDING_APP    = 10;
static const int               PADDING_MAIN   = 6;
static const int               PADDING_SMALL  = 2;
static const float             headerHeight   = 80;
static const float             cmdHeight      = 50;
static const int               menuItemDefaultHeight = 40;

const juce::Colour             BLACK  = juce::Colour::fromString("FF000000");
const juce::Colour             GREY_4 = juce::Colour::fromString("FF333333");
const juce::Colour             GREY_3 = juce::Colour::fromString("FF666666");
const juce::Colour             GREY_2 = juce::Colour::fromString("FF999999");
const juce::Colour             GREY_1 = juce::Colour::fromString("FFCCCCCC");
const juce::Colour             WHITE  = juce::Colour::fromString("FFFFFFFF");

const juce::Colour DEBUG_BLUE         = juce::Colour::fromString("FF0000FF");
const juce::Colour DEBUG_GREEN        = juce::Colour::fromString("FF00FF00");
const juce::Colour DEBUG_RED          = juce::Colour::fromString("FFFF0000");

const float        BUTTON_FONT_HEIGHT = 12.f;

// UIColour :
//   windowBackground,  widgetBackground, menuBackground,
//   outline,           defaultText,      defaultFill,
//   highlightedText,   highlightedFill,  menuText

static const juce::LookAndFeel_V4::ColourScheme DefaultColourScheme(WHITE,
                                                                    BLACK,
                                                                    WHITE,
                                                                    BLACK,
                                                                    BLACK,
                                                                    GREY_2,
                                                                    WHITE,
                                                                    GREY_4,
                                                                    BLACK);

class Manager {
  std::vector<std::unique_ptr<juce::LookAndFeel_V4::ColourScheme>> mSkins;
  std::unique_ptr<juce::LookAndFeel_V4>
      mLookAndFeelCollection[NUM_COMPONENT_TYPES];

public:
  Manager();
  juce::LookAndFeel_V4 *getSingleLookAndFeel(ComponentType inComponentTypeID);
  juce::Colour
  getUIColour(juce::LookAndFeel_V4::ColourScheme::UIColour inColorToGet);
};

class Default : public juce::LookAndFeel_V4 {
public:
  Default(juce::LookAndFeel_V4::ColourScheme inColourScheme
          = DefaultColourScheme);
  ~Default() = default;
  void         drawRotarySlider(juce::Graphics &g,
                                int             x,
                                int             y,
                                int             width,
                                int             height,
                                float           sliderPosProportional,
                                float           rotaryStartAngle,
                                float           rotaryEndAngle,
                                juce::Slider   &s) override;
  juce::Label *createSliderTextBox(juce::Slider &s) override;
  virtual void drawButtonBackground(juce::Graphics     &g,
                                    juce::Button       &b,
                                    const juce::Colour &backgroundColour,
                                    bool shouldDrawButtonAsHighlighted,
                                    bool shouldDrawButtonAsDown) override;
  virtual void drawComboBox(juce::Graphics &g,
                            int             width,
                            int             height,
                            bool            isButtonDown,
                            int             buttonX,
                            int             buttonY,
                            int             buttonW,
                            int             buttonH,
                            juce::ComboBox &box) override;
  virtual int  getPopupMenuBorderSize() override;
  virtual int  getMenuWindowFlags() override;
  virtual void drawButtonText(juce::Graphics   &g,
                              juce::TextButton &b,
                              bool              shouldDrawButtonAsHighlighted,
                              bool shouldDrawButtonAsDown) override;
};

} // namespace RT_LookAndFeel

#endif
