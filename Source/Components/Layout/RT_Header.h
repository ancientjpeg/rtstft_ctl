/*
  ==============================================================================

    RT_Header.h
    Created: 19 Mar 2022 1:29:47pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../Utility/RT_BorderedComponent.h"
#include <JuceHeader.h>

//==============================================================================
/*
 */
class RT_Header : public RT_BorderedComponent {
public:
  RT_Header(RT_ProcessorInterface *inInterface, int inBorderSize);
  ~RT_Header() override;

  void paintInBorder(juce::Graphics &) override;
  void resized() override;

private:
  juce::Label        mPluginTitle, mPluginSubtitle;
  juce::ToggleButton mSettingsMenuButton;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_Header)
};
