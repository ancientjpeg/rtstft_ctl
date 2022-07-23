/*
  ==============================================================================

    RT_MainDisplay.cpp
    Created: 18 Mar 2022 2:09:33pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_MainDisplay.h"
#include "../../Managers/LookAndFeelManagement/RT_LookAndFeelManagement.h"

using enum juce::LookAndFeel_V4::ColourScheme::UIColour;

RT_MainDisplay::RT_MainDisplay(RT_ProcessorInterface *inInterface)
    : RT_Component(inInterface), mGUIControlsContainer(mInterface),
      mSpectralDisplayContainer(mInterface, RT_LookAndFeel::PADDING_SMALL),
      mCommandLineContainer(mInterface, RT_LookAndFeel::PADDING_SMALL)
{
  addAndMakeVisible(mSpectralDisplayContainer);
  addAndMakeVisible(mGUIControlsContainer);
  addAndMakeVisible(mCommandLineContainer);
}

void RT_MainDisplay::paint(juce::Graphics &g)
{
  g.fillAll(getLookAndFeel_V4().getCurrentColourScheme().getUIColour(
      windowBackground));
  g.setColour(RT_LookAndFeel::BLACK);
  g.fillRect(getLocalBounds());
}

void RT_MainDisplay::resized()
{
  auto bounds    = getLocalBounds();

  auto cmdBounds = bounds.removeFromBottom(RT_LookAndFeel::cmdHeight);
  mCommandLineContainer.setBounds(cmdBounds);
  bounds.removeFromBottom(RT_LookAndFeel::PADDING_MAIN);

  auto controlsBounds = bounds.removeFromLeft(0.3f * getWidth())
                            .withTrimmedRight(RT_LookAndFeel::PADDING_MAIN);
  mGUIControlsContainer.setBounds(controlsBounds);

  bounds.removeFromRight(RT_LookAndFeel::PADDING_MAIN);
  mSpectralDisplayContainer.setBounds(bounds);
}
