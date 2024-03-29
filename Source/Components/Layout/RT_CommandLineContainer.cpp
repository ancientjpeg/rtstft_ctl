/*
  ==============================================================================

    RT_CommandLineContainer.cpp
    Created: 19 Mar 2022 1:43:10pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_CommandLineContainer.h"
#include "../../Managers/DSPStateManagers/RTSTFT_Manager.h"
#include "../../Managers/StateManagers/RT_PropertyManager.h"

//==============================================================================
RT_CommandLineContainer::RT_CommandLineContainer(
    RT_ProcessorInterface *inInterface, int inBorderSize)
    : RT_Component(inInterface),
      mCommandLinePrompt("RT_CMD_PROMPT", "user@rt_cmd$"),
      mErrorMessageContainer("RT_CMD_ERROR_BOX", "")
{

  mCommandLineEntry.setColour(juce::TextEditor::focusedOutlineColourId,
                              juce::Colours::transparentWhite);
  mCommandLineEntry.setColour(juce::TextEditor::outlineColourId,
                              juce::Colours::transparentWhite);
  mCommandLineEntry.setJustification(
      juce::Justification(juce::Justification::centredLeft));
  mCommandLineEntry.setMultiLine(false);
  mCommandLineEntry.setTextToShowWhenEmpty(mEntryPlaceholderText,
                                           juce::Colours::lightgrey);
  mCommandLineEntry.onReturnKey = [this]() {
    auto text = mCommandLineEntry.getText();
    auto hist = mInterface->getPropertyManager();
    hist->pushNewHistoryCommand(text);
    hist->resetCommandHistoryPos();
    mCommandLineEntry.clear();
    mCommandLineEntry.repaint();
    mInterface->getRTSTFTManager()->executeCMDCommand(text);
    mErrorMessageContainer.setText(
        mInterface->getRTSTFTManager()->getCMDMessage(),
        juce::NotificationType::dontSendNotification);
  };
  mCommandLineEntry.setColour(juce::TextEditor::backgroundColourId,
                              juce::Colours::transparentWhite);
  addAndMakeVisible(mCommandLineEntry);

  mCommandLinePrompt.setEditable(false);
  mCommandLinePrompt.setFont(juce::Font(16, juce::Font::bold));
  mCommandLinePrompt.setColour(juce::Label::outlineColourId,
                               juce::Colours::transparentWhite);
  mCommandLinePrompt.setJustificationType(
      juce::Justification(juce::Justification::centredRight));
  addAndMakeVisible(mCommandLinePrompt);
  mErrorMessageContainer.setFont(juce::Font(8));
  addAndMakeVisible(mErrorMessageContainer);

  mCommandLineEntry.addKeyListener(this);
}

void RT_CommandLineContainer::paint(juce::Graphics &g)
{
  g.fillAll(RT_LookAndFeel::WHITE);
}

void RT_CommandLineContainer::resized()
{
  auto bounds = getLocalBounds();

  int  strW   = mCommandLinePrompt.getFont().getStringWidth(
         mCommandLinePrompt.getText());
  auto promptBounds = bounds.removeFromLeft(strW + 15);
  auto errorBounds  = bounds.removeFromRight(300);
  mCommandLinePrompt.setBounds(promptBounds);

  mCommandLineEntry.setBounds(bounds);

  mErrorMessageContainer.setBounds(errorBounds);
}

bool RT_CommandLineContainer::keyPressed(const juce::KeyPress &kp,
                                         juce::Component      *orig)
{
  auto guiState = mInterface->getPropertyManager();
  if (!mCommandLineEntry.isTextInputActive()
      || guiState->commandHistoryIsEmpty()) {
    return false;
  }
  if (kp == juce::KeyPress::upKey) {
    mCommandLineEntry.setText(guiState->getNextStringInHistory(false),
                              juce::NotificationType::dontSendNotification);
  }
  else if (kp == juce::KeyPress::downKey) {
    mCommandLineEntry.setText(guiState->getNextStringInHistory(true),
                              juce::NotificationType::dontSendNotification);
  }
  return false;
}
