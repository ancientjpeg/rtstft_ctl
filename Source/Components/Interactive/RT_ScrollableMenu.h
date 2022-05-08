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
                    juce::StringArray inSelections = {}, int inFontSize = 12);
  ~RT_ScrollableMenu() override;

  void                  resized() override;
  void                  paint(juce::Graphics &) override;

  void                  setSelections(juce::StringArray inSelections);
  int                   select(juce::String inNewSelection);

  std::function<void()> onSelection = []() {};
  class Listener {
    virtual void onNewSelection(juce::String inNewSelection);
  };

private:
  juce::StringArray                 mSelections;
  juce::Array<juce::Rectangle<int>> mSelectionBounds;
  int                               mFontSize;
  int                               mSelectionIndex;
  juce::Point<float>                mMousePos;
  int                               mTotalHeight;
  bool                              mHasHover = false;
  int getSelectionFromPosition(juce::Point<float> inPos);
  juce::ListenerList<Listener> mListeners;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_ScrollableMenu)
};
