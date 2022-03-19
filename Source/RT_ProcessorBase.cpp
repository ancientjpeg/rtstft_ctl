/*
  ==============================================================================

    ProcessorBase.cpp
    Created: 18 Mar 2022 1:17:27pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_ProcessorBase.h"
#include "RT_ProcessorEditor.h"

//==============================================================================
RT_ProcessorBase::RT_ProcessorBase()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(
        BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
            .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
            .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
#endif
{
}

RT_ProcessorBase::~RT_ProcessorBase() {}
//==============================================================================
RT_ProcessorBase        *RT_ProcessorBase::getProcessor() { return this; }

RT_LookAndFeel::Manager *RT_ProcessorBase::getLookAndFeelManager()
{
  return &mLookAndFeelManager;
}

//==============================================================================
const juce::String RT_ProcessorBase::getName() const { return JucePlugin_Name; }

bool               RT_ProcessorBase::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool RT_ProcessorBase::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool RT_ProcessorBase::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double RT_ProcessorBase::getTailLengthSeconds() const { return 0.0; }

int    RT_ProcessorBase::getNumPrograms()
{
  return 1; // NB: some hosts don't cope very well if you tell them there are 0
            // programs, so this should be at least 1, even if you're not really
            // implementing programs.
}

int                RT_ProcessorBase::getCurrentProgram() { return 0; }

void               RT_ProcessorBase::setCurrentProgram(int index) {}

const juce::String RT_ProcessorBase::getProgramName(int index) { return {}; }

void RT_ProcessorBase::changeProgramName(int index, const juce::String &newName)
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool RT_ProcessorBase::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  // Some plugin hosts, such as certain GarageBand versions, will only
  // load plugins that support stereo bus layouts.
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
      && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
#endif
}
#endif

//==============================================================================
bool RT_ProcessorBase::hasEditor() const
{
  return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *RT_ProcessorBase::createEditor()
{
  return new RT_ProcessorEditor(this);
}

//==============================================================================
void RT_ProcessorBase::getStateInformation(juce::MemoryBlock &destData)
{
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
}

void RT_ProcessorBase::setStateInformation(const void *data, int sizeInBytes)
{
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.
}
