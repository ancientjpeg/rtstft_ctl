/*
  ==============================================================================

    RT_ParameterDefines.h
    Created: 19 Mar 2022 5:00:35pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../../../RTSTFT/src/rtstft.h"
#include <JuceHeader.h>

static const juce::StringArray RT_PARAM_IDS{
    "Pitch Ratio", "Retention",  "Phase Mod",  "Phase Chaos",
    "Gain Nudge",  "Gate Nudge", "Limit Nudge"};
static const juce::StringArray RT_PARAM_NAMES{
    "pitch ratio", "retention",  "phase mod",  "phase chaos",
    "gain nudge",  "gate nudge", "limit nudge"};
static const juce::Array<juce::NormalisableRange<float>> RT_PARAM_RANGES{
    juce::NormalisableRange<float>(0.5f, 2.f, 0.f,
                                   std::log(0.5) / std::log(1. / 3.)),
    juce::NormalisableRange<float>(0.f, 2.f, 0.f),
    juce::NormalisableRange<float>(0.f, 2.f, 0.f),
    juce::NormalisableRange<float>(0.f, 1.f, 0.f),
    juce::NormalisableRange<float>(-1.0f, 1.0f, 0.f),
    juce::NormalisableRange<float>(-1.0f, 1.0f, 0.f),
    juce::NormalisableRange<float>(-1.0f, 1.0f, 0.f),
};

static const float
    RT_PARAM_DEFAULTS[RT_PARAM_FLAVOR_COUNT] // should be RT_PARAM_FLAVOR_COUNT
    = {1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f};

/* property defines */
static const juce::StringArray RT_MANIP_GUI_IDS = {"Gain", "Gate", "Limit"};

enum RT_FFT_MODIFIERS {
  RT_FFT_MODIFIER_FFT_SIZE,
  RT_FFT_MODIFIER_OVERLAP_FACTOR,
  RT_FFT_MODIFIER_PAD_FACTOR,
  RT_FFT_MODIFIERS_COUNT,
};
static const juce::StringArray RT_FFT_MODIFIER_IDS
= {"fft_size", "overlap_factor", "pad_factor"};
