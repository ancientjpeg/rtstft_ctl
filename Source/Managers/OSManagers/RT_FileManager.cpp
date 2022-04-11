/*
  ==============================================================================

    RT_FileManager.cpp
    Created: 18 Mar 2022 2:05:21pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_FileManager.h"

RT_FileManager::RT_FileManager(RT_ProcessorInterface *inInterface)
    : mInterface(inInterface), mAppSupportDir(sc_AppSupportDirName)
{
  validateSupportDirectory();
}

void RT_FileManager::validateSupportDirectory()
{
  if (!mAppSupportDir.exists()) {
    auto result = mAppSupportDir.createDirectory();
    assert(result.wasOk());
  }
}