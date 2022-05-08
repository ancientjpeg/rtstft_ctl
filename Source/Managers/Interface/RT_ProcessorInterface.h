/*
  ==============================================================================

    ProcessorInterface.h
    Created: 18 Mar 2022 1:16:54pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class RT_AudioProcessor;
class RT_ProcessorBase;
class RT_ParameterManager;
class RT_PresetManager;
class RT_PropertyManager;
class RT_FileManager;
class RTSTFT_Manager;

namespace RT_LookAndFeel {
class Manager;
}

class RT_ProcessorInterface {

public:
  virtual ~RT_ProcessorInterface() {}
  virtual juce::AudioProcessor    *getProcessor()          = 0;
  virtual RT_AudioProcessor       *getRTProcessor()        = 0;
  virtual RT_LookAndFeel::Manager *getLookAndFeelManager() = 0;
  virtual RT_FileManager          *getFileManager()        = 0;
  virtual RT_ParameterManager     *getParameterManager()   = 0;
  virtual RT_PresetManager        *getPresetManager()      = 0;
  virtual RT_PropertyManager      *getPropertyManager()    = 0;
  virtual RTSTFT_Manager          *getRTSTFTManager()      = 0;
};
