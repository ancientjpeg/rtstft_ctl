/*
  ==============================================================================

    RT_GearButton.h
    Created: 29 Apr 2022 7:18:20pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../Utility/RT_Component.h"
class RT_GearButton : public RT_Component {

public:
  RT_GearButton(RT_ProcessorInterface *inInterface);
  ~RT_GearButton() = default;

  void paint(juce::Graphics &g) override;
  void resized() override;

  void mouseDown(const juce::MouseEvent &e) override;

  class Listener {
  public:
    Listener()                                             = default;
    virtual ~Listener()                                    = default;
    virtual void gearButtonChanged(bool inGearButtonState) = 0;
  };

  void addListener(Listener *inListener);

private:
  bool                         mActive = false;
  juce::ListenerList<Listener> mListenerList;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_GearButton)
};