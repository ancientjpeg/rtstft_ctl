/*
  ==============================================================================

    RT_SpectralDisplayContainer.h
    Created: 9 Apr 2022 2:00:38pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../Interactive/RT_FFTDisplay.h"
#include "../Interactive/RT_ValueLinkedSelectorMenu.h"
#include "../Utility/RT_Component.h"
#include <JuceHeader.h>

//==============================================================================
/*
 */
class RT_SpectralDisplayContainer : public RT_Component {

  RT_FFTDisplay              mFFTDisplay;
  RT_ValueLinkedSelectorMenu mManipSelector, mChannelModeSelector,
      mChannelSelector;
  juce::Rectangle<int> mDbScaleTicks;

public:
  RT_SpectralDisplayContainer(RT_ProcessorInterface *inInterface,
                              int                    inBorderSize);
  ~RT_SpectralDisplayContainer() override;

  void paint(juce::Graphics &) override;
  void resized() override;

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_SpectralDisplayContainer)
};
