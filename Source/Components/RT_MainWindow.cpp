/*
  ==============================================================================

    RT_MainWindow.cpp
    Created: 18 Mar 2022 2:09:33pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_MainWindow.h"
#include "../Managers/LookAndFeelManagement/RT_LookAndFeelManagement.h"

using enum juce::LookAndFeel_V4::ColourScheme::UIColour;

RT_MainWindow::RT_MainWindow(RT_ProcessorInterface *inInterface)
    : mInterface(inInterface), mGUIControlsContainer(mInterface),
      mFFTDisplayContainer(mInterface), mCommandLineContainer(mInterface)
{
  addAndMakeVisible(mFFTDisplayContainer);
  addAndMakeVisible(mGUIControlsContainer);
  addAndMakeVisible(mCommandLineContainer);
}

void RT_MainWindow::paint(juce::Graphics &g)
{
  g.fillAll(((juce::LookAndFeel_V4)getLookAndFeel())
                .getCurrentColourScheme()
                .getUIColour(windowBackground));
}
void RT_MainWindow::resized()
{
  using namespace RT_LookAndFeel;
  auto bounds    = getLocalBounds();
  int  cmdHeight = cmdHeightRatio * getHeight();
  auto cmdBounds = bounds.removeFromBottom(cmdHeight);
  mCommandLineContainer.setBounds(cmdBounds);
  auto controlsBounds = bounds.removeFromLeft(0.3 * getWidth());
  mGUIControlsContainer.setBounds(controlsBounds);
  mFFTDisplayContainer.setBounds(bounds);
}
