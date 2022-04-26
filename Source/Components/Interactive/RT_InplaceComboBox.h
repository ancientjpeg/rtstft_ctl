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
#include "../../Managers/LookAndFeelManagement/RT_LookAndFeelManagement.h"
#include "../../Managers/StateManagers/RT_PropertyManager.h"
#include "../Utility/RT_Component.h"

template <typename T>
class RT_InplaceComboBox : public juce::MouseListener,
                           public juce::Value::Listener {
public:
  RT_InplaceComboBox(RT_ProcessorInterface *inInterface,
                     juce::Identifier       inTargetValueID,
                     juce::Array<juce::var> inOptions,
                     int                    inIdealMenuItemHeight);
  ~RT_InplaceComboBox() = default;

  void addAndMakeVisibleWithParent(juce::Component *parent);
  // void setSelectedValue(juce::var inNewValue);

  class MenuSection : public juce::Component {
  public:
    MenuSection(RT_InplaceComboBox *parent) : mParent(parent) {}
    void paint(juce::Graphics &g) override { g.fillAll(juce::Colours::black); }
    void resized() override;

  private:
    RT_InplaceComboBox *mParent;
  };

  juce::Component *getMenuSection() { return (juce::Component *)&mMenuSection; }
  juce::Label     *getLabel() { return &mMainLabel; }

  void             valueChanged(juce::Value &v) override;
  void             mouseDown(const juce::MouseEvent &event) override;

private:
  RT_ProcessorInterface        *mInterface;
  int                           mNumOptions, mIdealMenuItemHeight;
  juce::Label                   mMainLabel, *mCurrentMenuSelection = nullptr;
  juce::OwnedArray<juce::Label> mMenuLabels;
  MenuSection                   mMenuSection;
  juce::Value                   mTargetValue;
  bool                          mIgnoreValueCallbacks = false;

  void                          onSelectionChange();

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_InplaceComboBox);
};

template <typename T>
RT_InplaceComboBox<T>::RT_InplaceComboBox(RT_ProcessorInterface *inInterface,
                                          juce::Identifier inTargetValueID,
                                          juce::Array<juce::var> inOptions,
                                          int inIdealMenuItemHeight)
    : mInterface(inInterface), mNumOptions(0),
      mIdealMenuItemHeight(inIdealMenuItemHeight), mMenuSection(this)
{
  mTargetValue
      = inInterface->getPropertyManager()->getValueTreeRef().getPropertyAsValue(
          inTargetValueID, nullptr);
  mTargetValue.addListener(this);
  juce::var currentVal = mTargetValue.getValue();
  auto      lafm       = mInterface->getLookAndFeelManager();
  for (auto option : inOptions) {
    auto label_ptr = std::make_unique<juce::Label>(juce::String(),
                                                   juce::String((T)option));
    label_ptr->addMouseListener(this, false);
    label_ptr->setText(juce::String((T)option), juce::dontSendNotification);
    label_ptr->setColour(juce::Label::backgroundColourId,
                         lafm->getUIColour(windowBackground));
    mMenuSection.addAndMakeVisible(label_ptr.get());
    mMenuLabels.add(label_ptr.release());
    mNumOptions++;
  }

  assert(inOptions.contains(currentVal));

  mMainLabel.setText(juce::String((T)currentVal), juce::dontSendNotification);
  mMainLabel.setColour(juce::Label::backgroundColourId,
                       lafm->getUIColour(windowBackground));
  mMainLabel.addMouseListener(this, false);
  valueChanged(mTargetValue);
}

template <typename T>
void RT_InplaceComboBox<T>::addAndMakeVisibleWithParent(juce::Component *parent)
{
  parent->addAndMakeVisible(mMainLabel);
  parent->addAndMakeVisible(mMenuSection);
  mMenuSection.setVisible(false);
}

// template <typename T>
// void RT_InplaceComboBox<T>::setSelectedValue(juce::var inNewValue)
// {
//   mInterface juce::ScopedValueSetter<bool> svs(mIgnoreValueCallbacks, true);
// }

template <typename T>
void RT_InplaceComboBox<T>::valueChanged(juce::Value &v)
{
  if (mIgnoreValueCallbacks) {
    return;
  }
  auto lafm = mInterface->getLookAndFeelManager();
  auto str  = juce::String((T)mTargetValue.getValue());
  mMainLabel.setText(str, juce::dontSendNotification);
  if (mCurrentMenuSelection) {
    mCurrentMenuSelection->setColour(juce::Label::backgroundColourId,
                                     lafm->getUIColour(windowBackground));
  }
  for (auto l : mMenuLabels) {
    if (l->getText() == str) {
      mCurrentMenuSelection = l;
      mCurrentMenuSelection->setColour(juce::Label::backgroundColourId,
                                       lafm->getUIColour(highlightedFill));
      break;
    }
  }
}

template <typename T>
void RT_InplaceComboBox<T>::MenuSection::resized()
{
  int  menuItemHeight = std::min<int>(mParent->mIdealMenuItemHeight,
                                     getHeight() / mParent->mNumOptions);
  auto bounds         = getLocalBounds().withHeight(menuItemHeight);
  for (juce::Label *l : mParent->mMenuLabels) {
    l->setBounds(bounds);
    bounds.translate(0, menuItemHeight);
  }
}

template <typename T>
void RT_InplaceComboBox<T>::mouseDown(const juce::MouseEvent &event)
{
  if (event.eventComponent == &mMainLabel) {
    mMenuSection.setVisible(!mMenuSection.isVisible());
    return;
  }
  for (juce::Label *l : mMenuLabels) {
    if (event.eventComponent == l) {
      mTargetValue.setValue(juce::var(l->getText().getIntValue()));
      return;
    }
  }
}

#endif
