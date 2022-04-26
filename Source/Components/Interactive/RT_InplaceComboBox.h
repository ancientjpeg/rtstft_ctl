/*
  ==============================================================================

    RT_InplaceComboBox.h
    Created: 26 Apr 2022 9:41:17am
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once
#ifndef RT_INPLACE_COMBO_BOX_H
#define RT_INPLACE_COMBO_BOX_H

#include "../../Managers/Interface/RT_ProcessorInterface.h"
#include "../../Managers/StateManagers/RT_PropertyManager.h"
#include "../Utility/RT_Component.h"

template <typename T>
class RT_InplaceComboBox : public juce::MouseListener {
public:
  RT_InplaceComboBox(RT_ProcessorInterface *inInterface,
                     juce::Value inTargetValue, const T inValuesRange[],
                     int inNumOptions, int inIdealMenuItemHeight);
  ~RT_InplaceComboBox();

  void selectNewID(int newID);
  void addAndMakeVisibleWithParent(juce::Component *parent);

  class Listener {
  public:
    virtual void inplaceComboBoxChanged(RT_InplaceComboBox *inChangedComboBox)
        = 0;
  };
  class MenuSection : juce::Component {
  public:
    MenuSection(RT_InplaceComboBox *parent) : mParent(parent) {}
    void paint(juce::Graphics &g) override {}
    void resized() override
    {
      int  menuItemHeight = std::min<int>(mParent->mIdealMenuItemHeight,
                                         getHeight() / mParent->mNumOptions);
      auto bounds         = getBounds().withHeight(menuItemHeight);
      for (juce::Label &l : mMenuLabels) {
        l.setBounds(bounds);
        bounds.translate(0, menuItemHeight);
      }
    }

  private:
    RT_InplaceComboBox *mParent;
  };

  juce::Component *getMenu();
  juce::Label     *getLabel();
  void             setListener(Listener *l) { mPropertyManagerAsListener = l }

private:
  juce::Label                   mMainLabel;
  juce::Component               mMenuSection;
  juce::OwnedArray<juce::Label> mMenuLabels;
  juce::Value                   mTargetValue;
  T                            *mValuesRange;
  Listener                     *mPropertyManagerAsListener;
  int                           mNumOptions, mIdealMenuItemHeight;

  void                          onSelectionChange();

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_InplaceComboBox);
};

template <typename T>
RT_InplaceComboBox<T>::RT_InplaceComboBox(RT_ProcessorInterface *inInterface,
                                          juce::Value            inTargetValue,
                                          const T inValuesRange[],
                                          int     inNumOptions,
                                          int     inIdealMenuItemHeight)
    : RT_Component(inInterface),
      mPropertyManagerAsListener(mInterface->getPropertyManager()),
      mTargetValue(inTargetValue), mNumOptions(inNumOptions),
      mIdealMenuItemHeight(inIdealMenuItemHeight)
{
  mValuesRange = new T[mNumOptions];
  std::memcpy(mValuesRange, inValuesRange, mNumOptions * sizeof(T));
}

template <typename T>
RT_InplaceComboBox<T>::~RT_InplaceComboBox()
{
  delete[] mValuesRange;
}
template <typename T>
void RT_InplaceComboBox<T>::selectNewID(T newID)
{
}

template <typename T>
void RT_InplaceComboBox<T>::onSelectionChange()
{
  mMainLabel.setText(juce::String((T)mTrackedValue.getValue()),
                     juce::dontSendNotification);
  mPropertyManagerAsListener->comboBoxChanged(this);
}
template <typename T>
void RT_InplaceComboBox<T>::addAndMakeVisibleWithParent(juce::Component *parent)
{
  parent->addAndMakeVisible(mMainLabel);
  parent->addAndMakeVisible(mMenuSection);
}

#endif