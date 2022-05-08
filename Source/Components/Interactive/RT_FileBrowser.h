/*
  ==============================================================================

    RT_FileBrowser.h
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
class RT_FileBrowser : public RT_Component {
public:
  RT_FileBrowser(RT_ProcessorInterface *inInterface,
                 juce::StringArray inSelections = {}, int inFontSize = 12);
  ~RT_FileBrowser() override;

  void                  resized() override;
  void                  paint(juce::Graphics &) override;

  void                  setSelections(juce::StringArray inSelections);
  int                   select(juce::String inNewSelection);

  std::function<void()> onSelection = []() {};

private:
  juce::StringArray                 mSelections;
  juce::Array<juce::Rectangle<int>> mSelectionBounds;
  int                               mFontSize;
  int                               mSelectionIndex;
  juce::Point<float>                mMousePos;
  int                               mTotalHeight;
  bool                              mHasHover = false;
  int getSelectionFromPosition(juce::Point<float> inPos);
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_FileBrowser)
};
