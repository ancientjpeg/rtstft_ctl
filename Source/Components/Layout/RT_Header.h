/*
  ==============================================================================

    RT_Header.h
    Created: 19 Mar 2022 1:29:47pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../Interactive/RT_GearButton.h"
#include "../Utility/RT_Component.h"
#include <JuceHeader.h>

//==============================================================================
/*
 */
class RT_Header : public RT_Component {
public:
  RT_Header(RT_ProcessorInterface *inInterface, int inBorderSize);
  ~RT_Header() override;

  void paint(juce::Graphics &) override;
  void resized() override;

  void addSettingsMenuButtonListener(RT_GearButton::Listener *l);

private:
  juce::Label   mPluginTitle, mPluginSubtitle;
  RT_GearButton mSettingsMenuButton;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_Header)
};
