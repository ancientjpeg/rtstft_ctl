/*
  ==============================================================================

    RT_ParameterDefines.h
    Created: 19 Mar 2022 5:00:35pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

enum RT_ParameterFlavor { PITCH_RATIO, RETENTION_MOD, PHASE_MOD, NUM_PARAMS };
static const juce::StringArray RT_PARAM_IDS{"Pitch Ratio", "Retention",
                                            "Phase Mod"};
static const juce::Array<juce::NormalisableRange<float>> RT_PARAM_RANGES{
    juce::NormalisableRange<float>(0.5f, 2.f, 0.f, 0.430676558073f),
    juce::NormalisableRange<float>(0.f, 2.f, 0.f),
    juce::NormalisableRange<float>(0.f, 2.f, 0.f),
};

static const float RT_PARAM_DEFAULTS[NUM_PARAMS] = {1.f, 1.f, 1.f};
