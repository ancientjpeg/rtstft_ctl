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
                                public RTSTFT_Manager::Listener,
                                public juce::KeyListener {
public:
  RT_CommandLineContainer(RT_ProcessorInterface *inInterface, int inBorderSize);
  ~RT_CommandLineContainer() override = default;

  void paintInBorder(juce::Graphics &) override;
  void resized() override;
  void onCMDReturn() override;
  bool keyPressed(const juce::KeyPress &kp, juce::Component *orig) override;

private:
  int          mBorderSize;
  juce::String mEntryPlaceholderText = "Enter your rt_cmd commands here.";
  std::list<juce::String>           mCommandHistory;
  std::list<juce::String>::iterator mHistoryIterator;
  juce::Label                       mCommandLinePrompt, mErrorMessageContainer;
  juce::TextEditor                  mCommandLineEntry;

  void showNextStringInHistory(bool reverse = false);
  void newHistoryCommand(juce::String &s);
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_CommandLineContainer)
};
