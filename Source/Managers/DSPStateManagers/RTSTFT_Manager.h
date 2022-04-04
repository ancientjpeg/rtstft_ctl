/*
  ==============================================================================

    RTSTFT_Manager.h
    Created: 19 Mar 2022 2:08:48pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once
#include "../../../RTSTFT/src/rtstft.h"
#include "../Interface/RT_ProcessorInterface.h"
#include <JuceHeader.h>

/* weird stuff with C struct typedefs idk */
typedef struct RTSTFT_Params_Listener_Return_Data rt_cpp_listener_return_t;

using rt_param_flavor_t = RT_PARAM_FLAVOR;

class RTSTFT_Manager : public juce::AudioProcessorValueTreeState::Listener {
  RT_ProcessorInterface *mInterface;
  rt_params              p;
  int                    mCurrentSamplesPerBlock;
  float                  mCurrentSampleRate = -1;
  int                    mNumChannels;
  bool                   mInitialized      = false;
  bool                   mLastUpdateWasCMD = false;
  juce::String           mCMDMessage       = "";
  int                    mCMDErrorState    = 0;

public:
  RTSTFT_Manager(RT_ProcessorInterface *inInterface);
  ~RTSTFT_Manager();
  const rt_params getParamsStruct();
  void            prepareToPlay(double inSampleRate, int inSamplesPerBlock);
  void            processBlock(juce::AudioBuffer<float> &buffer);
  void            releaseResources();
  void            parameterChanged(const juce::String &parameterID,
                                   float               newValue) override;
  void         RTSTFT_ManagerCMDCallback(rt_cpp_listener_return_t const info);
  
  void         executeCMDCommand(juce::String inCMDString);
  int          getCMDErrorState();
  juce::String getCMDMessage();

  struct Listener {
    virtual void onManipChanged(rt_manip_flavor_t inManipFlavor) {}
    virtual void onParamChanged(rt_param_flavor_t inParamFlavor,
                                rt_real           inNewVal)
    {
    }
    virtual ~Listener() {}
  };
  void addListener(Listener *l);

private:
  
  juce::ListenerList<Listener> mListenerList;
};

extern "C" void RTSTFT_CMDListenerCallback(void *RTSTFTManagerPtr,
                                           rt_cpp_listener_return_t const info);
