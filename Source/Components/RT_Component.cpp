/*
  ==============================================================================

    RT_Component.cpp
    Created: 18 Mar 2022 6:20:20pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_Component.h"

juce::LookAndFeel_V4 &RT_Component::RT_getLookAndFeel()
{
  return (juce::LookAndFeel_V4 &)juce::Component::getLookAndFeel();
}