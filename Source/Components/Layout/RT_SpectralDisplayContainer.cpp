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
    RT_ProcessorInterface *inInterface,
    int                    inBorderSize)
    : RT_Component(inInterface), mFFTDisplay(mInterface),
      mManipSelector(mInterface,
                     mInterface->getPropertyManager()
                         ->getGUIStateTree()
                         .getPropertyAsValue("active_manip", nullptr),
                     RT_MANIP_GUI_IDS),
      mChannelModeSelector(
          mInterface,
          mInterface->getPropertyManager()
              ->getValueTreeRef()
              .getPropertyAsValue("manip_multichannel", nullptr),
          RT_MULTICHANNEL_MODE_IDS,
          false,
          true),
      mChannelSelector(mInterface,
                       mInterface->getPropertyManager()
                           ->getGUIStateTree()
                           .getPropertyAsValue("active_chan", nullptr),
                       {"0", "1"},
                       false,
                       true)
{
  addAndMakeVisible(mFFTDisplay);
  addAndMakeVisible(mManipSelector);
  addAndMakeVisible(mChannelModeSelector);
  addAndMakeVisible(mChannelSelector);
}

RT_SpectralDisplayContainer::~RT_SpectralDisplayContainer()
{
}

void RT_SpectralDisplayContainer::paint(juce::Graphics &g)
{
  auto lafm = mInterface->getLookAndFeelManager();
  g.setColour(lafm->getUIColour(outline));
  g.fillAll();
  auto  dbBounds       = mDbScaleTicks;
  float dbRange        = RT_DB_MAX - RT_DB_MIN;
  int   tickSeparation = 12;
  g.setFont(juce::Font(10));
  // can use any multiple of tickSeparation for first value - just to be sure we
  // get 0dB
  g.setColour(lafm->getUIColour(windowBackground));
  g.fillRect(dbBounds);
  g.setColour(lafm->getUIColour(outline));
  for (int i = RT_DB_MAX - tickSeparation; i > RT_DB_MIN; i -= tickSeparation) {
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
  int  border            = RT_LookAndFeel::PADDING_SMALL;
  auto bounds            = getLocalBounds();
  auto topBounds         = bounds.removeFromTop(40);
  mManipSelector.setBounds(
      topBounds.removeFromLeft(topBounds.getWidth() * 0.75f));
  topBounds.removeFromLeft(border);
  mChannelSelector.setBounds(topBounds.removeFromRight(rightSectionWidth));
  topBounds.removeFromRight(border);
  mChannelModeSelector.setBounds(topBounds);

  bounds.removeFromTop(border);

  mDbScaleTicks = bounds.removeFromRight(rightSectionWidth);
  bounds.removeFromRight(border);
  mFFTDisplay.setBounds(bounds);
}
