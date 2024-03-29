/*
  ==============================================================================

    RT_CommandLineContainer.h
    Created: 19 Mar 2022 1:43:10pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../../Managers/Interface/RT_ProcessorInterface.h"
#include "../Utility/RT_Component.h"
#include <JuceHeader.h>

//==============================================================================
/*
 */
class RT_CommandLineContainer : public RT_Component,
                                public juce::KeyListener {
public:
  RT_CommandLineContainer(RT_ProcessorInterface *inInterface, int inBorderSize);
  ~RT_CommandLineContainer() override = default;

  void paint(juce::Graphics &) override;
  void resized() override;
  bool keyPressed(const juce::KeyPress &kp, juce::Component *orig) override;

private:
  int              mBorderSize;
  juce::String     mEntryPlaceholderText = "Enter your rt_cmd commands here.";
  juce::Label      mCommandLinePrompt, mErrorMessageContainer;
  juce::TextEditor mCommandLineEntry;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_CommandLineContainer)
};
