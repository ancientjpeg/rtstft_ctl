/*
  ==============================================================================

    RT_SpectralDisplayContainer.cpp
    Created: 9 Apr 2022 2:00:38pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_SpectralDisplayContainer.h"
#include "../../Managers/LookAndFeelManagement/RT_LookAndFeelManagement.h"
#include "../Interactive/RT_FFTDisplay.h"

//==============================================================================
RT_SpectralDisplayContainer::RT_SpectralDisplayContainer(
    RT_ProcessorInterface *inInterface, int inBorderSize)
    : RT_BorderedComponent(inInterface, inBorderSize), mFFTDisplay(mInterface),
      mManipSelectorMenu(mInterface)
{
  addAndMakeVisible(mFFTDisplay);
  addAndMakeVisible(mManipSelectorMenu);
}

RT_SpectralDisplayContainer::~RT_SpectralDisplayContainer() {}

void RT_SpectralDisplayContainer::paintInBorder(juce::Graphics &g)
{
  g.setColour(
      getLookAndFeel_V4().getCurrentColourScheme().getUIColour(defaultFill));
  g.fillRect(mDisplaySeparation);
}

void RT_SpectralDisplayContainer::resized()
{
  auto bounds = getBoundsAdj();
  mManipSelectorMenu.setBounds(bounds.removeFromTop(40));
  mDisplaySeparation = bounds.removeFromTop(RT_LookAndFeel::widgetBorderSize);
  mFFTDisplay.setBounds(bounds);
}
