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
      mFFTDisplayContainer(mInterface)
{
  addAndMakeVisible(mFFTDisplayContainer);
  addAndMakeVisible(mGUIControlsContainer);
}

void RT_MainWindow::paint(juce::Graphics &g)
{
  g.fillAll(RT_getLookAndFeel().getCurrentColourScheme().getUIColour(
      windowBackground));
}
void RT_MainWindow::resized()
{
  using namespace RT_LookAndFeel;
  auto bounds        = getBounds();
  int  cmdHeight     = cmdHeightRatio * getHeight();
  int  contentHeight = getHeight() - cmdHeight;
  auto cmdBounds     = bounds.removeFromBottom(cmdHeight);
}
