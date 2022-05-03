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

  class MenuSection : public juce::Component {
  public:
    MenuSection(RT_InplaceComboBox *parent) : mParent(parent) {}
    void paint(juce::Graphics &g) override;
    void resized() override;

  private:
    RT_InplaceComboBox *mParent;
  };

  class Label : public juce::Label {
  public:
    Label(RT_InplaceComboBox *inParent, bool isTitle = false,
          juce::String inID = {}, juce::String inText = {})
        : juce::Label(inID, inText), mParent(inParent), mIsTitle(isTitle)
    {
    }
    void setSelected(bool isSelected);
    void mouseEnter(const juce::MouseEvent &e) override;
    void mouseExit(const juce::MouseEvent &e) override;
    void paint(juce::Graphics &g) override;

  private:
    RT_InplaceComboBox *mParent;
    bool                mSelected = false, mHovered = false;
    bool                mIsTitle;
  };

  juce::Component *getMenuSection() { return (juce::Component *)&mMenuSection; }
  juce::Label     *getLabel() { return &mMainLabel; }

  void             valueChanged(juce::Value &v) override;
  void             mouseDown(const juce::MouseEvent &event) override;

private:
  RT_ProcessorInterface  *mInterface;
  int                     mNumOptions, mIdealMenuItemHeight;
  Label                   mMainLabel;
  Label                  *mCurrentMenuSelection = nullptr;
  juce::OwnedArray<Label> mMenuLabels;
  MenuSection             mMenuSection;
  juce::Value             mTargetValue;
  bool mIgnoreValueCallbacks = false, mMenuSectionVisible = false;

  void onSelectionChange();

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RT_InplaceComboBox);
};

template <typename T>
RT_InplaceComboBox<T>::RT_InplaceComboBox(RT_ProcessorInterface *inInterface,
                                          juce::Identifier inTargetValueID,
                                          juce::Array<juce::var> inOptions,
                                          int inIdealMenuItemHeight)
    : mInterface(inInterface), mNumOptions(0),
      mIdealMenuItemHeight(inIdealMenuItemHeight), mMainLabel(this, true),
      mMenuSection(this)
{
  mTargetValue
      = inInterface->getPropertyManager()->getValueTreeRef().getPropertyAsValue(
          inTargetValueID, nullptr);
  mTargetValue.addListener(this);
  juce::var currentVal = mTargetValue.getValue();
  auto      lafm       = mInterface->getLookAndFeelManager();
  for (auto option : inOptions) {
    auto label_ptr = std::make_unique<Label>(this, false, juce::String(),
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

template <typename T>
void RT_InplaceComboBox<T>::valueChanged(juce::Value &v)
{
  if (mIgnoreValueCallbacks) {
    return;
  }
  auto str = juce::String((T)mTargetValue.getValue());
  mMainLabel.setText(str, juce::dontSendNotification);
  if (mCurrentMenuSelection) {
    mCurrentMenuSelection->setSelected(false);
  }
  for (auto l : mMenuLabels) {
    if (l->getText() == str) {
      mCurrentMenuSelection = l;
      mCurrentMenuSelection->setSelected(true);
      break;
    }
  }
}

template <typename T>
void RT_InplaceComboBox<T>::MenuSection::paint(juce::Graphics &g)
{
  auto lafm = mParent->mInterface->getLookAndFeelManager();
  g.fillAll(lafm->getUIColour(outline));
  auto border      = juce::BorderSize<int>(RT_LookAndFeel::widgetBorderSize);
  auto innerBounds = border.subtractedFrom(getLocalBounds());
  g.setColour(lafm->getUIColour(windowBackground));
  g.fillRect(innerBounds);
}

template <typename T>
void RT_InplaceComboBox<T>::MenuSection::resized()
{
  int  menuItemHeight = std::min<int>(mParent->mIdealMenuItemHeight,
                                     getHeight() / mParent->mNumOptions);
  auto bounds         = getLocalBounds().withHeight(menuItemHeight);
  for (juce::Label *l : mParent->mMenuLabels) {
    l->setBounds(bounds);
    bounds.translate(0, menuItemHeight - RT_LookAndFeel::widgetBorderSize);
  }
}

template <typename T>
void RT_InplaceComboBox<T>::mouseDown(const juce::MouseEvent &event)
{
  if (event.eventComponent == &mMainLabel) {
    mMenuSectionVisible = !mMenuSectionVisible;
    mMenuSection.setVisible(mMenuSectionVisible);
    mMainLabel.repaint();
    return;
  }
  for (juce::Label *l : mMenuLabels) {
    if (event.eventComponent == l) {
      mTargetValue.setValue(juce::var(l->getText().getIntValue()));
      return;
    }
  }
}

template <typename T>
void RT_InplaceComboBox<T>::Label::setSelected(bool isSelected)
{
  mSelected = isSelected;
  repaint();
}
template <typename T>
void RT_InplaceComboBox<T>::Label::mouseEnter(const juce::MouseEvent &e)
{
  mHovered = true;
  repaint();
}
template <typename T>
void RT_InplaceComboBox<T>::Label::mouseExit(const juce::MouseEvent &e)
{
  mHovered = false;
  repaint();
}

template <typename T>
void RT_InplaceComboBox<T>::Label::paint(juce::Graphics &g)
{
  auto lafm = mParent->mInterface->getLookAndFeelManager();
  g.setFont(getFont());

  auto border_col_enum = outline;
  auto bg_col_enum     = windowBackground;
  if (!mIsTitle) {
    bg_col_enum
        = mSelected ? outline : (mHovered ? defaultFill : windowBackground);
  }

  auto text_col_enum = mSelected ? highlightedText : defaultText;
  g.fillAll(lafm->getUIColour(border_col_enum));

  g.setColour(lafm->getUIColour(bg_col_enum));
  auto border      = juce::BorderSize<int>(RT_LookAndFeel::widgetBorderSize);
  auto innerBounds = border.subtractedFrom(getLocalBounds());
  g.fillRect(innerBounds);

  g.setColour(lafm->getUIColour(text_col_enum));
  g.drawText(getText(), innerBounds.withLeft(innerBounds.getX() + 10),
             juce::Justification::centredLeft);

  if (mIsTitle) {

    auto triBounds
        = innerBounds.withWidth(innerBounds.getHeight())
              .translated(innerBounds.getWidth() - innerBounds.getHeight(), 0)
              .toFloat();
    triBounds = triBounds.withSizeKeepingCentre(8, 8);
    juce::Path triPath;
    triPath.startNewSubPath(triBounds.getCentreX(), triBounds.getY());
    triPath.lineTo(triBounds.getBottomLeft());
    triPath.lineTo(triBounds.getBottomRight());
    triPath.closeSubPath();
    if (mParent->mMenuSectionVisible) {
      triPath.applyTransform(juce::AffineTransform::rotation(
          juce::MathConstants<float>::pi, triBounds.getCentreX(),
          triBounds.getCentreY()));
    }
    g.setColour(lafm->getUIColour(defaultFill));
    g.fillPath(triPath);
  }
}
#endif
