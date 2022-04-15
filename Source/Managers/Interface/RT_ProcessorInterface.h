/*
  ==============================================================================

    ProcessorInterface.h
    Created: 18 Mar 2022 1:16:54pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class RT_ProcessorBase;
class RT_ParameterManager;
class RTSTFT_Manager;
class RT_GUIStateManager;
class RT_FileManager;

namespace RT_LookAndFeel {
class Manager;
}

class RT_ProcessorInterface {

public:
  virtual ~RT_ProcessorInterface() {}
  virtual RT_ProcessorBase        *getProcessor()          = 0;
  virtual RT_LookAndFeel::Manager *getLookAndFeelManager() = 0;
  virtual RT_ParameterManager     *getParameterManager()   = 0;
  virtual RTSTFT_Manager          *getRTSTFTManager()      = 0;
  virtual RT_GUIStateManager      *getGUIStateManager()    = 0;
  virtual RT_FileManager          *getFileManager()        = 0;
};
