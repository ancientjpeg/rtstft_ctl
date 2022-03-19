/*
  ==============================================================================

    RT_Bordered_Container.h
    Created: 18 Mar 2022 2:16:34pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../RT_Component.h"
#include <JuceHeader.h>

class RT_BorderedContainer : public RT_Component {
  unsigned int mBorderSize;

public:
  RT_BorderedContainer(int inBorderSize);
  void paint(juce::Graphics &g) override;
  void resized() override;
};
