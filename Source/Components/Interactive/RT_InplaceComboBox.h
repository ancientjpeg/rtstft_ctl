/*
  ==============================================================================

    RT_InplaceComboBox.h
    Created: 26 Apr 2022 9:41:17am
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once
#include "../../Managers/Interface/RT_ProcessorInterface.h"
#include "../Utility/RT_Component.h"

class RT_InplaceComboBox : public RT_Component {
public:
  RT_InplaceComboBox(RT_ProcessorInterface *inInterface);
  ~RT_InplaceComboBox();

  juce::Label     *getLabel();
  juce::Component *getMenu();

  class Listener {
  public:
    virtual void comboBoxChanged(RT_InplaceComboBox *inChangedComboBox) = 0;
  };

private:
  juce::Label     mLabel;
  juce::Component mMenuSection;

  void            onSelectionChange();

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_InplaceComboBox);
};