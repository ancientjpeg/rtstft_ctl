/*
  ==============================================================================

    ProcessorInterface.h
    Created: 18 Mar 2022 1:16:54pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

class RT_ProcessorBase;
class RT_PropertyManager;
class RT_ParameterManager;
class RTSTFT_Manager;
namespace RT_LookAndFeel {
class Manager;
}

class RT_ProcessorInterface {

public:
  virtual ~RT_ProcessorInterface() {}
  virtual RT_ProcessorBase        *getProcessor()          = 0;
  virtual RT_LookAndFeel::Manager *getLookAndFeelManager() = 0;
  virtual RTSTFT_Manager          *getRTSTFTManager()      = 0;
};
