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

enum ComponentType { PLUGIN_DEFAULT, SLIDER, BUTTON, NUM_COMPONENT_TYPES };
static const juce::StringArray ComponentNames = {"Default", "Slider", "Button"};
static const int               mainBorderSize = 10;
static const int               widgetBorderSize = 4;

class Manager {
  std::vector<std::unique_ptr<juce::LookAndFeel_V4::ColourScheme>> mSkins;
  std::unique_ptr<juce::LookAndFeel_V4>
      mLookAndFeelCollection[NUM_COMPONENT_TYPES];

public:
  Manager();
  juce::LookAndFeel_V4 *getSingleLookAndFeel(ComponentType inComponentTypeID);
};

static const juce::LookAndFeel_V4::ColourScheme DefaultColourScheme(
    juce::Colours::white, juce::Colours::white, juce::Colours::white,
    juce::Colours::black, juce::Colours::black, juce::Colours::black,
    juce::Colours::blue, juce::Colours::blue, juce::Colours::black);

} // namespace RT_LookAndFeel

#endif