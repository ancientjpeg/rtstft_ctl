/*
  ==============================================================================

    RT_Bordered_Container.h
    Created: 18 Mar 2022 2:16:34pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../../Managers/Interface/RT_ProcessorInterface.h"
#include <JuceHeader.h>

class RT_BorderedContainer : juce::Component {
  unsigned int           mBorderSize;
  RT_ProcessorInterface *mInterface;

public:
  RT_BorderedContainer(RT_ProcessorInterface *inInterface, int inBorderWidth);
  void paint(juce::Graphics &g) override;
};
