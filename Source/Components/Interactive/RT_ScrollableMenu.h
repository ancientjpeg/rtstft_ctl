/*
  ==============================================================================

    RT_ScrollableMenu.h
    Created: 4 May 2022 12:49:09pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../Utility/RT_Component.h"
#include <JuceHeader.h>

//==============================================================================
/*
 */
class RT_ScrollableMenu : public RT_Component {
public:
  RT_ScrollableMenu(RT_ProcessorInterface *inInterface,
                    juce::StringArray inSelections = {}, int inFontSize = 20);
  ~RT_ScrollableMenu() override;

  void         resized() override;
  void         paint(juce::Graphics &) override;

  void         setSelections(juce::StringArray inSelections);
  int          select(juce::String inNewSelection);
  void         deselect();
  juce::String getCurrentSelection();

  std::function<void(juce::String)> onSelection = [](juce::String s) {};
  void mouseDown(const juce::MouseEvent &e) override;
  void mouseMove(const juce::MouseEvent &e) override;
  void mouseExit(const juce::MouseEvent &e) override;

private:
  juce::StringArray                 mSelections;
  juce::Array<juce::Rectangle<int>> mSelectionBounds;
  int                               mFontSize;
  int                               mSelectionIndex;
  int                               mTotalHeight;
  juce::Point<float>                mMousePos;
  bool                              mHasHover = false;
  int getSelectionFromPosition(juce::Point<float> inPos);
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_ScrollableMenu)
};
