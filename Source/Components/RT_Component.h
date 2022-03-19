/*
  ==============================================================================

    RT_Component.h
    Created: 18 Mar 2022 6:20:20pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class RT_Component : public juce::Component {
public:
  juce::LookAndFeel_V4 &RT_getLookAndFeel();
};
