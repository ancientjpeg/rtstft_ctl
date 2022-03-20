/*
  ==============================================================================

    RT_GUIControlsContainer.cpp
    Created: 19 Mar 2022 1:16:07pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_GUIControlsContainer.h"
#include "../../Managers/DSPStateManagers/RTSTFT_Manager.h"
#include <JuceHeader.h>

//==============================================================================
RT_GUIControlsContainer::RT_GUIControlsContainer(
    RT_ProcessorInterface *inInterface)
    : mInterface(inInterface)
{ 
  mPitchScaleKnob.setRange(0.5f, 2.f, 0);
  mPitchScaleKnob.setSkewFactorFromMidPoint(1.f);
  mPitchScaleKnob.setValue(1.0f);
  addAndMakeVisible(mPitchScaleKnob);
  mPitchScaleKnob.onValueChange = [this] {
    rt_set_scale_factor(mInterface->getRTSTFTManager()->getParamsStruct(),
                        mPitchScaleKnob.getValue());
  };
}

RT_GUIControlsContainer::~RT_GUIControlsContainer() {}

void RT_GUIControlsContainer::paint(juce::Graphics &g)
{
  /* This demo code just fills the component's background and
     draws some placeholder text to get you started.

     You should replace everything in this method with your own
     drawing code..
  */

  g.fillAll(getLookAndFeel().findColour(
      juce::ResizableWindow::backgroundColourId)); // clear the background

  g.setColour(juce::Colours::red);
  g.drawRect(getLocalBounds(), 1); // draw an outline around the component

  g.setColour(juce::Colours::black);
  g.setFont(14.0f);
  g.drawText("RT_GUIControlsContainer", getLocalBounds(),
             juce::Justification::centredBottom,
             true); // draw some placeholder text
}

void RT_GUIControlsContainer::resized()
{
  // This method is where you should set the bounds of any child
  // components that your component contains..
  auto knobBounds = getLocalBounds().removeFromTop(0.5 * getHeight());
  mPitchScaleKnob.setBounds(knobBounds);
}
