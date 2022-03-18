/*
  ==============================================================================

    ProcessorInterface.h
    Created: 18 Mar 2022 1:16:54pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once
#include "../DSPStateManagers/RT_ParameterManager.h"
#include "../DSPStateManagers/RT_PropertyManager.h"
#include "../GUIManagers/RT_StyleManager.h"

class RT_ProcessorBase;

class RT_ProcessorInterface {
  
public:
  virtual ~RT_ProcessorInterface();
  virtual RT_ProcessorBase *getProcessor() = 0;
  virtual RT_StyleManager *getStyleManager() = 0;
};
