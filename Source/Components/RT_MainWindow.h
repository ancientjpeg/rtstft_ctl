/*
  ==============================================================================

    RT_MainWindow.h
    Created: 18 Mar 2022 2:09:33pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once

#include "../Managers/Interface/RT_ProcessorInterface.h"
#include "RT_Component.h"
#include "Visual/RT_Bordered_Container.h"
#include <JuceHeader.h>

class RT_MainWindow : public RT_Component {

  RT_ProcessorInterface *mInterface;

public:
  RT_MainWindow();
};
