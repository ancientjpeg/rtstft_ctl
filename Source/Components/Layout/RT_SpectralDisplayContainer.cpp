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
      mManipSelector(mInterface), mChannelSelector(mInterface, true)
{
  addAndMakeVisible(mFFTDisplay);
  addAndMakeVisible(mManipSelector);
}

RT_SpectralDisplayContainer::~RT_SpectralDisplayContainer() {}

void RT_SpectralDisplayContainer::paintInBorder(juce::Graphics &g)
{
  g.setColour(
      getLookAndFeel_V4().getCurrentColourScheme().getUIColour(defaultFill));
  g.fillRect(mDisplaySeparation);
  auto dbBounds    = mDbScaleTicks;
  auto dbScaleLine = dbBounds.removeFromLeft(RT_LookAndFeel::widgetBorderSize);
  g.fillRect(dbScaleLine);
  int tickSeparation = 12;
  g.setFont(juce::Font(10));
  // can use any multiple of tickSeparation for first value - just to be sure we
  // get 0dB
  for (int i = tickSeparation; i > mFFTDisplay.mDbMin; i -= tickSeparation) {
    juce::String s(i);
    float        yPos = getHeightAdj()
                 - (mFFTDisplay.getDbValNormalized(i)) * dbBounds.getHeight();
    auto tickTextBounds = dbBounds.withY(yPos - 5).withHeight(10);
    auto dbTickBounds = tickTextBounds.removeFromLeft(dbBounds.getWidth() / 4);
    dbTickBounds
        = dbTickBounds.withSizeKeepingCentre(dbTickBounds.getWidth(), 2);
    g.fillRect(dbTickBounds);
    g.drawText(s, tickTextBounds, juce::Justification::centredRight);
  }
}

void RT_SpectralDisplayContainer::resized()
{
  auto bounds = getBoundsAdj();
  mManipSelector.setBounds(bounds.removeFromTop(40));
  mDisplaySeparation = bounds.removeFromTop(RT_LookAndFeel::widgetBorderSize);
  mDbScaleTicks      = bounds.removeFromRight(30);
  mFFTDisplay.setBounds(bounds);
}
