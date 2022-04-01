/*
  ==============================================================================

    RT_CommandLineContainer.cpp
    Created: 19 Mar 2022 1:43:10pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_CommandLineContainer.h"
#include <JuceHeader.h>

//==============================================================================
RT_CommandLineContainer::RT_CommandLineContainer(
    RT_ProcessorInterface *inInterface, int inBorderSize)
    : RT_BorderedComponent(inInterface, inBorderSize),
      mCommandLinePrompt("RT_CMD_PROMPT", "user@rt_cmd$"),
      mErrorMessageContainer("RT_CMD_ERROR_BOX", "eroors..")
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
  mCommandLineEntry.addListener(static_cast<juce::TextEditor::Listener *>(
      mInterface->getRTSTFTManager()));
  mCommandLineEntry.onReturnKey = [this]() {
    auto text = mCommandLineEntry.getText();
    mCommandLineEntry.clear();
    mCommandLineEntry.repaint();
  };
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
  
  mInterface->getRTSTFTManager()->addListener(this);
}

void RT_CommandLineContainer::paintInBorder(juce::Graphics &g)
{
  using enum juce::LookAndFeel_V4::ColourScheme::UIColour;
  g.setColour(
      getLookAndFeel_V4().getCurrentColourScheme().getUIColour(defaultFill));
}

void RT_CommandLineContainer::resized()
{
  auto bounds = getBoundsAdj();

  int  strW   = mCommandLinePrompt.getFont().getStringWidth(
         mCommandLinePrompt.getText());
  auto promptBounds = bounds.removeFromLeft(strW + 15);
  auto errorBounds  = bounds.removeFromRight(300);
  mCommandLinePrompt.setBounds(promptBounds);

  mCommandLineEntry.setBounds(bounds);

  mErrorMessageContainer.setBounds(errorBounds);
}

void RT_CommandLineContainer::onCMDReturn()
{
  auto rtm = mInterface->getRTSTFTManager();
  mErrorMessageContainer.setText(rtm->getCMDMessage(),
                                 juce::NotificationType::sendNotification);
}
