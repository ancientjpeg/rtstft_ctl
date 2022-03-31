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
      mCommandLinePrompt("RT_CMD_PROMPT", "rt_cmd:"),
      mCommandLineEntry("RT_CMD_ENTRY", "enter your rt_cmd commands here")
{
  addAndMakeVisible(mCommandLineEntry);
  // mCommandLineEntry.setColour(juce::Label::ColourIds::textColourId,
  //                             juce::Colours::blue);
  addAndMakeVisible(mCommandLinePrompt);
  mCommandLinePrompt.setEditable(false);
  mCommandLinePrompt.setFont(juce::Font(18, juce::Font::bold));
}

RT_CommandLineContainer::~RT_CommandLineContainer() {}

void RT_CommandLineContainer::paintInBorder(juce::Graphics &g)
{
  using enum juce::LookAndFeel_V4::ColourScheme::UIColour;
  g.setColour(
      getLookAndFeel_V4().getCurrentColourScheme().getUIColour(defaultFill));
}

void RT_CommandLineContainer::resized()
{
  auto bounds       = getBoundsAdj();
  auto promptBounds = bounds.removeFromLeft(80);
  mCommandLinePrompt.setBounds(promptBounds);
  mCommandLineEntry.setBounds(bounds);
}
