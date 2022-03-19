/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "RT_ProcessorEditor.h"
using RT_LookAndFeel::ComponentType;

//==============================================================================
RT_ProcessorEditor::RT_ProcessorEditor(RT_ProcessorInterface *inInterface)
    : AudioProcessorEditor(inInterface->getProcessor()),
      mInterface(inInterface), mMainWindow(10)
{
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(800, 600);
  addAndMakeVisible(mMainWindow);
  setLookAndFeel(mInterface->getLookAndFeelManager()->getSingleLookAndFeel(
      ComponentType::PLUGIN_DEFAULT));
}

RT_ProcessorEditor::~RT_ProcessorEditor() {}

//==============================================================================
void RT_ProcessorEditor::paint(juce::Graphics &g)
{
  // (Our component is opaque, so we must completely fill the background with a
  // solid colour)
  using enum juce::LookAndFeel_V4::ColourScheme::UIColour;
  juce::LookAndFeel_V4 &lookAndFeel
      = static_cast<juce::LookAndFeel_V4 &>(getLookAndFeel());
  g.fillAll(lookAndFeel.getCurrentColourScheme().getUIColour(outline));

  // setFont
}

void RT_ProcessorEditor::resized()
{
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..
  mMainWindow.setBounds(getLocalBounds());
}
