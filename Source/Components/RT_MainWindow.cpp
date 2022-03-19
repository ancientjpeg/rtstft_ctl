/*
  ==============================================================================

    RT_MainWindow.cpp
    Created: 18 Mar 2022 2:09:33pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_MainWindow.h"
using enum juce::LookAndFeel_V4::ColourScheme::UIColour;

RT_MainWindow::RT_MainWindow() {}

void RT_MainWindow::paint(juce::Graphics &g)
{
  g.fillAll(RT_getLookAndFeel().getCurrentColourScheme().getUIColour(
      windowBackground));
}
void RT_MainWindow::resized() {}
