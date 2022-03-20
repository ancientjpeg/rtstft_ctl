/*
  ==============================================================================

    RT_Header.h
    Created: 19 Mar 2022 1:29:47pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
 */
class RT_Header : public juce::Component {
public:
  RT_Header();
  ~RT_Header() override;

  void paint(juce::Graphics &) override;
  void resized() override;

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_Header)
};
