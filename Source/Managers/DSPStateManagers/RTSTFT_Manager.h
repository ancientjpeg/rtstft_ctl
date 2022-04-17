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

class RTSTFT_Manager : public juce::AudioProcessorValueTreeState::Listener,
                       public juce::ValueTree::Listener {

public:
  RTSTFT_Manager(RT_ProcessorInterface *inInterface);
  ~RTSTFT_Manager();
  void resetParamsStruct(int inFFTSize = 1024, int inOverlapFactor = 8);
  const rt_params getParamsStruct();
  void            prepareToPlay(double inSampleRate, int inSamplesPerBlock);
  void            processBlock(juce::AudioBuffer<float> &buffer);
  void            releaseResources();
  void            parameterChanged(const juce::String &parameterID,
                                   float               newValue) override;
  void valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged,
                                const juce::Identifier &property) override;
  void changeFFTSize(int inNewFrameSize, int inNewOverlapFactor,
                     int inNewPadFactor = 0, bool threaded = false);
  void awaitFFTSizeChange();
  void executeCMDCommand(juce::String inCMDString);
  int  getCMDErrorState();
  juce::String     getCMDMessage();

  void             writeManipsToFile(juce::MemoryOutputStream &stream);
  void             readManipsFromBinary(bool inThreadedFFTUpdate = false);
  static const int ManipsBinaryMagicNumber = 0xBEEFED05;

  void RTSTFT_ManagerCMDCallback(rt_cpp_listener_return_t const info);

  struct Listener {
    virtual void onManipChanged(rt_manip_flavor_t inManipFlavor) {}
    virtual void onParamChanged(rt_param_flavor_t inParamFlavor,
                                rt_real           inNewVal)
    {
    }
    virtual ~Listener() {}
  };
  void addListener(Listener *l);

  class FFTSetterThread : public juce::Thread {
    RT_ProcessorInterface *mInterface;

  public:
    FFTSetterThread(RT_ProcessorInterface *inInterface)
        : juce::Thread("FFTSetterThread"), mInterface(inInterface)
    {
      setCurrentThreadPriority(8);
    }
    ~FFTSetterThread() = default;
    void run() override
    {
      mInterface->getRTSTFTManager()->changeFFTSizeInternal();
    }
  };

private:
  RT_ProcessorInterface       *mInterface;
  rt_params                    p;
  FFTSetterThread              mFFTSetterThread;
  int                          mCurrentSamplesPerBlock;
  float                        mCurrentSampleRate;
  int                          mNumChannels;
  bool                         mInitialized   = false;
  juce::String                 mCMDMessage    = "";
  int                          mCMDErrorState = 0;
  juce::ListenerList<Listener> mListenerList;
  void                         changeFFTSizeInternal();
};

extern "C" void RTSTFT_CMDListenerCallback(void *RTSTFTManagerPtr,
                                           rt_cpp_listener_return_t const info);
