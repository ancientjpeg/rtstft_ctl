/*
  ==============================================================================

    RT_SpectralDisplayContainer.cpp
    Created: 9 Apr 2022 2:00:38pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_SpectralDisplayContainer.h"
#include "../../Managers/LookAndFeelManagement/RT_LookAndFeelManagement.h"
#include "../../Managers/StateManagers/RT_PropertyManager.h"
#include "../Interactive/RT_FFTDisplay.h"

//==============================================================================
RT_SpectralDisplayContainer::RT_SpectralDisplayContainer(
    RT_ProcessorInterface *inInterface, int inBorderSize)
    : RT_BorderedComponent(inInterface, inBorderSize), mFFTDisplay(mInterface),
      mManipSelector(mInterface,
                     mInterface->getPropertyManager()
                         ->getGUIStateTree()
                         .getPropertyAsValue("active_manip", nullptr),
                     RT_MANIP_GUI_IDS),
      mChannelSelector(mInterface,
                       mInterface->getPropertyManager()
                           ->getValueTreeRef()
                           .getPropertyAsValue("manip_multichannel", nullptr),
                       {"multichannel", "mono"}, false, true)
{
  addAndMakeVisible(mFFTDisplay);
  addAndMakeVisible(mManipSelector);
}

RT_SpectralDisplayContainer::~RT_SpectralDisplayContainer() {}

void RT_SpectralDisplayContainer::paintInBorder(juce::Graphics &g)
{
  auto lafm = mInterface->getLookAndFeelManager();
  g.setColour(lafm->getUIColour(defaultFill));
  g.fillRect(mHorizontalSeparator);
  g.fillRect(mVerticalSeparator);
  auto  dbBounds       = mDbScaleTicks;
  float dbRange        = RT_DB_MAX - RT_DB_MIN;
  int   tickSeparation = 12;
  g.setFont(juce::Font(10));
  // can use any multiple of tickSeparation for first value - just to be sure we
  // get 0dB
  for (int i = tickSeparation; i > mFFTDisplay.mDbMin; i -= tickSeparation) {
    juce::String s(i);
    float yPos = (1.f - ((i - RT_DB_MIN) / dbRange)) * dbBounds.getHeight()
                 + dbBounds.getY();
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
  int  rightSectionWidth = 30;
  int  border            = RT_LookAndFeel::widgetBorderSize;
  auto bounds            = getBoundsAdj();
  auto topBounds         = bounds.removeFromTop(40);
  mChannelSelector.setBounds(topBounds.removeFromRight(rightSectionWidth));
  topBounds.removeFromRight(border);
  mManipSelector.setBounds(topBounds);
  mHorizontalSeparator = bounds.removeFromTop(border);
  mDbScaleTicks        = bounds.removeFromRight(rightSectionWidth);
  bounds.removeFromRight(border);
  mFFTDisplay.setBounds(bounds);

  mVerticalSeparator = getBoundsAdj().withX(mDbScaleTicks.getX() - border).withWidth(border);
}
