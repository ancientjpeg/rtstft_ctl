/*
  ==============================================================================

    RT_Bordered_Container.h
    Created: 18 Mar 2022 2:16:34pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../Managers/Interface/RT_ProcessorInterface.h"

class RT_BorderedContainer : juce::Component {
public:
  RT_BorderedContainer(RT_ProcessorInterface)
  void paint(juce::Graphics &g) override;
};
