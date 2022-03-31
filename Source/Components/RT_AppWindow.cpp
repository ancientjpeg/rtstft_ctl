/*
  ==============================================================================

    RT_AppWindow.cpp
    Created: 30 Mar 2022 4:17:34pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_AppWindow.h"
#include "Layout/RT_MainDisplay.h"

RT_AppWindow::RT_AppWindow(RT_ProcessorInterface *inInterface, int inBorderSize)
    : RT_BorderedComponent(inInterface, inBorderSize),
      mHeader(inInterface, RT_COMPONENT_BORDER_SIZE), mMainDisplay(inInterface)

{
  addAndMakeVisible(mHeader);
  addAndMakeVisible(mMainDisplay);
}
void RT_AppWindow::paintInBorder(juce::Graphics &g) {}

void RT_AppWindow::resized()
{
  auto bounds       = getBoundsAdj();
  auto headerBounds = bounds.removeFromTop(0.1f * getHeightAdj());
  mHeader.setBounds(headerBounds.reduced(RT_MAIN_PADDING));
  mMainDisplay.setBounds(bounds);
}