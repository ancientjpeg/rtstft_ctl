/*
  ==============================================================================

    RT_FFTDisplayContainer.h
    Created: 19 Mar 2022 1:15:45pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../../Managers/DSPStateManagers/RTSTFT_Manager.h"
#include "../../Managers/GUIStateManagers/RT_GUIStateManager.h"
#include "../../Managers/Interface/RT_ProcessorInterface.h"
#include "../Utility/RT_BorderedComponent.h"
#include <JuceHeader.h>

//==============================================================================
/*
 */
class RT_FFTDisplay : public RT_Component,
                      public juce::Timer,
                      public RTSTFT_Manager::Listener {
  std::unique_ptr<rt_real[]> mLocalManipCopies;

public:
  RT_FFTDisplay(RT_ProcessorInterface *inInterface);
  ~RT_FFTDisplay() override;

  void paint(juce::Graphics &) override;
  void resized() override;
  void timerCallback() override;
  void onManipChanged(rt_manip_flavor_t inManipFlavor);

private:
  void copyManips(rt_manip_flavor_t inTargetManipFlavor);
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_FFTDisplay)
};
