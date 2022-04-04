/*
  ==============================================================================

    RT_CommandLineContainer.h
    Created: 19 Mar 2022 1:43:10pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../../Managers/DSPStateManagers/RTSTFT_Manager.h"
#include "../../Managers/Interface/RT_ProcessorInterface.h"
#include "../Utility/RT_BorderedComponent.h"
#include <JuceHeader.h>

//==============================================================================
/*
 */
class RT_CommandLineContainer : public RT_BorderedComponent,
                                public RTSTFT_Manager::Listener {
public:
  RT_CommandLineContainer(RT_ProcessorInterface *inInterface, int inBorderSize);
  ~RT_CommandLineContainer() override = default;

  void paintInBorder(juce::Graphics &) override;
  void resized() override;
  void onCMDReturn() override;

private:
  int          mBorderSize;
  juce::String mEntryPlaceholderText = "Enter your rt_cmd commands here.";
  std::queue<juce::String> mCommandHistory;
  juce::Label              mCommandLinePrompt, mErrorMessageContainer;
  juce::TextEditor         mCommandLineEntry;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_CommandLineContainer)
};
