/*
  ==============================================================================

    RT_GearButton.cpp
    Created: 29 Apr 2022 7:18:20pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_GearButton.h"
#include "../../Managers/LookAndFeelManagement/RT_LookAndFeelManagement.h"

RT_GearButton::RT_GearButton(RT_ProcessorInterface *inInterface)
    : RT_Component(inInterface)
{
}
void RT_GearButton::paint(juce::Graphics &g)
{
  float                  gearToothThickness = 0.23f;
  float                  gearToothWidth     = getWidth() * gearToothThickness;
  int                    numTeeth           = 8;
  float                  gtw_2              = gearToothWidth / 2.f;
  auto                   b                  = getLocalBounds();
  juce::Rectangle<float> bounds
      = juce::Rectangle<float>(b.getX(), b.getY(), b.getWidth(), b.getHeight());
  juce::Path gearPath;
  auto       baseRect = juce::Rectangle<float>(bounds.getCentreX() - gtw_2, 0,
                                         gearToothWidth, getHeight());
  juce::Point<float> centre = bounds.getCentre();
  float              angle  = juce::MathConstants<float>::twoPi / numTeeth;
  for (int i = 0; i < numTeeth / 2; i++) {
    juce::Path temp;
    temp.addRectangle(baseRect);
    gearPath.addRectangle(baseRect);
    gearPath.applyTransform(
        juce::AffineTransform::rotation(angle, centre.x, centre.y));
  }

  juce::Path internalHolePath;
  auto       holeBounds
      = bounds.withSizeKeepingCentre(gearToothWidth, gearToothWidth);
  internalHolePath.addEllipse(holeBounds);

  if (mActive) {
    auto rot = juce::AffineTransform::rotation(angle * 0.5, centre.x, centre.y);
    gearPath.applyTransform(rot);
    internalHolePath.applyTransform(rot);
  }
  auto *lafm = mInterface->getLookAndFeelManager();
  g.setColour(lafm->getUIColour(outline));
  g.fillPath(gearPath);
  g.setColour(lafm->getUIColour(windowBackground));
  g.fillPath(internalHolePath);
}
void RT_GearButton::resized()
{
}
void RT_GearButton::mouseDown(const juce::MouseEvent &e)
{
  mActive       = !mActive;
  using checker = juce::Component::BailOutChecker;
  mListenerList.call([this](Listener &l) { l.gearButtonChanged(mActive); });
  repaint();
}

void RT_GearButton::addListener(Listener *inListener)
{
  mListenerList.add(inListener);
}
