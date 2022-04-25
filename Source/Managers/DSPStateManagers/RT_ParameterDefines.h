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

#define RT_DB_MIN (-110.f)
#define RT_DB_MAX (18.f)
#define RT_DB_SLIDER_RADIUS (.5f)
static const juce::StringArray RT_PARAM_IDS{
    "Pitch Ratio", "Retention",  "Phase Mod",  "Phase Chaos",
    "Gain Nudge",  "Gate Nudge", "Limit Nudge"};
static const juce::StringArray RT_PARAM_NAMES{
    "pitch ratio", "retention",  "phase mod",  "phase chaos",
    "gain nudge",  "gate nudge", "limit nudge"};

static const std::function from_0_1_log
    = [](float start, float end, float val_0_1) {
        float ret = val_0_1;
        ret       = ret * 2.f - 1.f;
        ret       = ret * RT_DB_SLIDER_RADIUS;
        return ret;
      };

static const std::function to_0_1_log = [](float start, float end, float val) {
  float ret = val / RT_DB_SLIDER_RADIUS;
  ret       = val * 0.5f + 0.5f;
  return ret;
};

static const std::function snap_legal = [](float start, float end, float val) {
  if (val < start) {
    return start;
  }
  if (val > end) {
    return end;
  }
  return val;
};

static juce::Array<juce::NormalisableRange<float>> RT_PARAM_RANGES{
    juce::NormalisableRange<float>(0.5f, 2.f, 0.f,
                                   std::log(0.5) / std::log(1. / 3.)),
    juce::NormalisableRange<float>(0.f, 2.f, 0.f),
    juce::NormalisableRange<float>(0.f, 2.f, 0.f),
    juce::NormalisableRange<float>(0.f, 1.f, 0.f),
    juce::NormalisableRange<float>(-RT_DB_SLIDER_RADIUS, RT_DB_SLIDER_RADIUS,
                                   0.f),
    juce::NormalisableRange<float>(-RT_DB_SLIDER_RADIUS, RT_DB_SLIDER_RADIUS,
                                   0.f),
    juce::NormalisableRange<float>(-RT_DB_SLIDER_RADIUS, RT_DB_SLIDER_RADIUS,
                                   0.f),
    // juce::NormalisableRange<float>(-RT_DB_SLIDER_RADIUS, RT_DB_SLIDER_RADIUS,
    //                                from_0_1_log, to_0_1_log, snap_legal),
    // juce::NormalisableRange<float>(-RT_DB_SLIDER_RADIUS, RT_DB_SLIDER_RADIUS,
    //                                from_0_1_log, to_0_1_log, snap_legal),
    // juce::NormalisableRange<float>(-RT_DB_SLIDER_RADIUS, RT_DB_SLIDER_RADIUS,
    //                                from_0_1_log, to_0_1_log, snap_legal),
    // juce::NormalisableRange<float>(RT_DB_MIN, RT_DB_MAX, from_0_1_log,
    //                                to_0_1_log, snap_legal),
    // juce::NormalisableRange<float>(RT_DB_MIN, RT_DB_MAX, from_0_1_log,
    //                                to_0_1_log, snap_legal),
    // juce::NormalisableRange<float>(RT_DB_MIN, RT_DB_MAX, from_0_1_log,
    //                                to_0_1_log, snap_legal),
};

static const float
    RT_PARAM_DEFAULTS[RT_PARAM_FLAVOR_COUNT] // should be RT_PARAM_FLAVOR_COUNT
    = {1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f};

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
