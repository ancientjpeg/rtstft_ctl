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
class RT_LookAndFeelManager;

class RT_ProcessorInterface {

public:
  virtual ~RT_ProcessorInterface() {}
  virtual RT_ProcessorBase      *getProcessor()          = 0;
  virtual RT_LookAndFeelManager *getLookAndFeelManager() = 0;
};
