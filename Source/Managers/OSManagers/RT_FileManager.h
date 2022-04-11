/*
  ==============================================================================

    RT_FileManager.h
    Created: 18 Mar 2022 2:05:21pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once
#include "../Interface/RT_ProcessorInterface.h"
#include <JuceHeader.h>

class RT_FileManager {
  RT_ProcessorInterface *mInterface;

public:
  RT_FileManager(RT_ProcessorInterface *inInterface);
};