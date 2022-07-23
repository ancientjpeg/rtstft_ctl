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
      mSpectralDisplayContainer(mInterface, RT_LookAndFeel::widgetBorderSize),
      mCommandLineContainer(mInterface, RT_LookAndFeel::widgetBorderSize)
{
  addAndMakeVisible(mSpectralDisplayContainer);
  addAndMakeVisible(mGUIControlsContainer);
  addAndMakeVisible(mCommandLineContainer);
}

void RT_MainDisplay::paint(juce::Graphics &g)
{
  g.fillAll(getLookAndFeel_V4().getCurrentColourScheme().getUIColour(
      windowBackground));
}
void RT_MainDisplay::resized()
{
  using namespace RT_LookAndFeel;
  auto bounds    = getLocalBounds();
  int  cmdHeight = RT_LookAndFeel::cmdHeight;

  auto cmdBounds = bounds.removeFromBottom(cmdHeight)
                       .withTrimmedTop(RT_LookAndFeel::mainPadding)
                       .reduced(RT_LookAndFeel::mainPadding);
  mCommandLineContainer.setBounds(cmdBounds);

  auto controlsBounds = bounds.removeFromLeft(0.3 * getWidth())
                            .withTrimmedRight(RT_LookAndFeel::mainPadding);
  mGUIControlsContainer.setBounds(controlsBounds);

  mSpectralDisplayContainer.setBounds(bounds);
}
