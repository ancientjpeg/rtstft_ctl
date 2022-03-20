/*
  ==============================================================================

    RT_CommandLineContainer.h
    Created: 19 Mar 2022 1:43:10pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../../Managers/Interface/RT_ProcessorInterface.h"
#include <JuceHeader.h>

//==============================================================================
/*
 */
class RT_CommandLineContainer : public juce::Component {
public:
  RT_CommandLineContainer(RT_ProcessorInterface *mInterface);
  ~RT_CommandLineContainer() override;

  void paint(juce::Graphics &) override;
  void resized() override;

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_CommandLineContainer)
};
