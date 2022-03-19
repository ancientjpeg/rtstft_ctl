/*
  ==============================================================================

    RT_MainWindow.cpp
    Created: 18 Mar 2022 2:09:33pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_MainWindow.h"

RT_MainWindow::RT_MainWindow(int inBorderSize) : mBorderSize(inBorderSize) {}

void RT_MainWindow::paint(juce::Graphics &g)
{
  g.fillAll(RT_getLookAndFeel().getCurrentColourScheme().getUIColour(
      juce::LookAndFeel_V4::ColourScheme::UIColour::defaultFill));
  g.setColour(RT_getLookAndFeel().getCurrentColourScheme().getUIColour(
      juce::LookAndFeel_V4::ColourScheme::UIColour::outline));
  g.drawRect(0, 0, getWidth(), mBorderSize);
  g.drawRect(0, 0, mBorderSize, getHeight());
  g.drawRect(0, getWidth() - mBorderSize, getWidth(), mBorderSize);
  g.drawRect(0, getHeight() - mBorderSize, mBorderSize, getHeight());
}
void RT_MainWindow::resized() {}
