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

#define RT_MOD_DB_RADIUS (36.f)
static const juce::StringArray RT_PARAM_IDS{
    "Pitch Ratio", "Retention", "Phase Mod", "Phase Chaos",
    "Gain Mod",    "Gate Mod",  "Limit Mod", "Dry/Wet"};
static const juce::StringArray RT_PARAM_NAMES{
    "pitch ratio", "retention", "phase mod", "phase chaos",
    "gain mod",    "gate mod",  "limit mod", "dry/wet"};

static juce::Array<juce::NormalisableRange<float>> RT_PARAM_RANGES{
    juce::NormalisableRange<float>(-2400.f, 2400.f, 0.f),
    juce::NormalisableRange<float>(0.f, 2.f, 0.f),
    juce::NormalisableRange<float>(0.f, 2.f, 0.f),
    juce::NormalisableRange<float>(0.f, 1.f, 0.f),
    juce::NormalisableRange<float>(-RT_MOD_DB_RADIUS, RT_MOD_DB_RADIUS, 0.f,
                                   1.f, true),
    juce::NormalisableRange<float>(-RT_MOD_DB_RADIUS, RT_MOD_DB_RADIUS, 0.f,
                                   1.f, true),
    juce::NormalisableRange<float>(-RT_MOD_DB_RADIUS, RT_MOD_DB_RADIUS, 0.f,
                                   1.f, true),
    juce::NormalisableRange<float>(0.f, 1.f, 0.f),
};

static const float
    RT_PARAM_DEFAULTS[RT_PARAM_FLAVOR_COUNT] // should be RT_PARAM_FLAVOR_COUNT
    = {0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f};

/* property defines */
static const juce::StringArray RT_MANIP_GUI_IDS = {"Gain", "Gate", "Limit"};

enum RT_FFT_MODIFIERS {
  RT_FFT_MODIFIER_FRAME_SIZE,
  RT_FFT_MODIFIER_OVERLAP_FACTOR,
  RT_FFT_MODIFIER_PAD_FACTOR,
  RT_FFT_MODIFIERS_COUNT,
};

static const juce::StringArray RT_FFT_MODIFIER_IDS
    = {"frame_size", "overlap_factor", "pad_factor"};

enum RT_MULTICHANNEL_MODES {
  RT_MULTICHANNEL_MONO,
  RT_MULTICHANNEL_LR,
  RT_MULTICHANNEL_MS,
  RT_MULTICHANNEL_MODE_COUNT,
};

static const juce::StringArray RT_MULTICHANNEL_MODE_IDS
    = {"mono", "left/right", "mid/side"};