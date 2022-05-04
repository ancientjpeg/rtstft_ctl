/*
  ==============================================================================

    RT_ScrollableMenu.cpp
    Created: 4 May 2022 12:49:09pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_ScrollableMenu.h"
#include "../../Managers/LookAndFeelManagement/RT_LookAndFeelManagement.h"
#include <JuceHeader.h>

//==============================================================================
RT_ScrollableMenu::RT_ScrollableMenu(RT_ProcessorInterface *inInterface,
                                     juce::StringArray      inSelections,
                                     int                    inFontSize)
    : RT_Component(inInterface), mSelections(inSelections),
      mFontSize(inFontSize)
{
}

RT_ScrollableMenu::~RT_ScrollableMenu() {}

void RT_ScrollableMenu::paint(juce::Graphics &g) {}

void RT_ScrollableMenu::resized()
{
  auto bounds     = getLocalBounds().withHeight(mFontSize);
  int  nextHeight = bounds.getHeight() + RT_LookAndFeel::widgetBorderSize;
  for (int i = 0; i < mSelections.size(); i++) {
    mSelectionBounds.add(bounds);
    bounds.translate(0, nextHeight);
  }
}
