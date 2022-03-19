/*
  ==============================================================================

    RT_ParameterManager.h
    Created: 18 Mar 2022 1:57:25pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../Interface/RT_ProcessorInterface.h"

class RT_ParameterManager {
public:
  RT_ParameterManager(RT_ProcessorInterface *mInterface);
  ~RT_ParameterManager();
};
