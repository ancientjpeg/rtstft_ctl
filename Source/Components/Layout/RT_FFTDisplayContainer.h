/*
  ==============================================================================

    RT_FFTDisplayContainer.h
    Created: 19 Mar 2022 1:15:45pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../../Managers/Interface/RT_ProcessorInterface.h"
#include <JuceHeader.h>

//==============================================================================
/*
 */
class RT_FFTDisplayContainer : public juce::Component {
public:
  RT_FFTDisplayContainer(RT_ProcessorInterface *inInterface);
  ~RT_FFTDisplayContainer() override;

  void paint(juce::Graphics &) override;
  void resized() override;

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_FFTDisplayContainer)
};
