/*
  ==============================================================================

    RT_AppWindow.cpp
    Created: 30 Mar 2022 4:17:34pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_AppWindow.h"
#include "../Managers/LookAndFeelManagement/RT_LookAndFeelManagement.h"
#include "Layout/RT_MainDisplay.h"
#include "Layout/RT_SettingsDisplay.h"

RT_AppWindow::RT_AppWindow(RT_ProcessorInterface *inInterface, int inBorderSize)
    : RT_Component(inInterface),
      mHeader(inInterface, RT_LookAndFeel::widgetBorderSize),
      mMainDisplay(inInterface), mSettingsDisplay(inInterface)

{
  addAndMakeVisible(mHeader);
  addAndMakeVisible(mMainDisplay);
  addChildComponent(mSettingsDisplay);
  mHeader.addSettingsMenuButtonListener(this);

  setLookAndFeel(&mLookAndFeel);
}

RT_AppWindow::~RT_AppWindow() { setLookAndFeel(nullptr); }

void RT_AppWindow::paint(juce::Graphics &g) {}

void RT_AppWindow::resized()
{
  mBaseBounds       = getBounds().reduced(RT_LookAndFeel::mainBorderSize);
  auto bounds       = mBaseBounds;
  auto headerBounds = bounds.removeFromTop(RT_LookAndFeel::headerHeight);
  auto displayPad   = juce::BorderSize<int>(RT_LookAndFeel::mainPadding);
  auto headerPad    = displayPad;
  headerPad.setBottom(0);
  mHeader.setBounds(headerPad.subtractedFrom(headerBounds));
  bounds = displayPad.subtractedFrom(bounds);
  mMainDisplay.setBounds(bounds);
  mSettingsDisplay.setBounds(bounds);
}

void RT_AppWindow::gearButtonChanged(bool inButtonState)
{
  mMainDisplay.setVisible(!inButtonState);
  mSettingsDisplay.setVisible(inButtonState);
}