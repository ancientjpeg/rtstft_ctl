/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "RT_ProcessorEditor.h"
using namespace RT_LookAndFeel;
using enum juce::LookAndFeel_V4::ColourScheme::UIColour;

//==============================================================================
RT_ProcessorEditor::RT_ProcessorEditor(RT_ProcessorInterface *inInterface)
    : AudioProcessorEditor(inInterface->getProcessor()),
      mInterface(inInterface), mMainWindow(mInterface)
{
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.

  setLookAndFeel(mInterface->getLookAndFeelManager()->getSingleLookAndFeel(
      ComponentType::PLUGIN_DEFAULT));
  addAndMakeVisible(mMainWindow);
  addAndMakeVisible(mHeader);

  setSize(800, 600);
}

RT_ProcessorEditor::~RT_ProcessorEditor() {}

//==============================================================================
void RT_ProcessorEditor::paint(juce::Graphics &g)
{
  juce::LookAndFeel_V4::ColourScheme &colScheme
      = (static_cast<juce::LookAndFeel_V4 &>(getLookAndFeel()))
            .getCurrentColourScheme();
  g.fillAll(colScheme.getUIColour(defaultFill));
}

void RT_ProcessorEditor::resized()
{
  auto contentBounds = getBounds().reduced(mainBorderSize);
  int  headerHeight  = headerHeightRatio * contentBounds.getHeight();
  auto headerBounds  = contentBounds.removeFromTop(headerHeight);
  mHeader.setBounds(headerBounds);
  mMainWindow.setBounds(contentBounds);
}
