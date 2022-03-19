/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "RT_ProcessorEditor.h"
using RT_LookAndFeel::ComponentType;
using enum juce::LookAndFeel_V4::ColourScheme::UIColour;

//==============================================================================
RT_ProcessorEditor::RT_ProcessorEditor(RT_ProcessorInterface *inInterface)
    : AudioProcessorEditor(inInterface->getProcessor()),
      mInterface(inInterface), mBorderSize(RT_LookAndFeel::mainBorderSize)
{
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.

  setLookAndFeel(mInterface->getLookAndFeelManager()->getSingleLookAndFeel(
      ComponentType::PLUGIN_DEFAULT));
  addAndMakeVisible(mMainWindow);

  setSize(800, 600);
}

RT_ProcessorEditor::~RT_ProcessorEditor() {}

//==============================================================================
void RT_ProcessorEditor::paint(juce::Graphics &g)
{
  juce::LookAndFeel_V4 &lookAndFeel
      = static_cast<juce::LookAndFeel_V4 &>(getLookAndFeel());
  g.fillAll(lookAndFeel.getCurrentColourScheme().getUIColour(defaultFill));
}

void RT_ProcessorEditor::resized()
{
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..
  juce::BorderSize<int> borderSizeObject(mBorderSize);
  mMainWindow.setBoundsInset(borderSizeObject);
}
