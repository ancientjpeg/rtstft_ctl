/*
  ==============================================================================

    RT_PropertyManager.h
    Created: 18 Mar 2022 1:57:34pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../Interface/RT_ProcessorInterface.h"

class RT_PropertyManager {
public:
  RT_PropertyManager(RT_ProcessorInterface *mInterface);
  ~RT_PropertyManager();
};
