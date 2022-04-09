/*
  ==============================================================================

    RT_MathUtilities.h
    Created: 19 Mar 2022 3:56:07pm
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <type_traits>

namespace RT_Utilities {

template <typename IntegralType>
IntegralType getNearestPowerOfTwo(IntegralType inValue)
{
  static_assert(std::is_integral<IntegralType>::value,
                "RT_Utilies::getNearestPowerOfTwo must be performed on an "
                "integral type.");
  int finalValue      = inValue;
  int powerOfTwoCheck = (finalValue & (finalValue - 1));
  if (powerOfTwoCheck != 0) {
    int  nextHighestPower = sizeof(finalValue) - 1;
    bool found            = false;
    while (!found && nextHighestPower-- > 1) {
      if ((finalValue & (1 << nextHighestPower)) == 1) {
        nextHighestPower++;
        break;
      }
    }
    finalValue = 1 << nextHighestPower;
  }
  return finalValue;
}

typedef union {
  float    f;
  uint32_t i;
} float_bits;

juce::String serializeFloats(float *data, int count)
{
  return juce::Base64::toBase64((void *)data, count * 4);
}
juce::MemoryBlock deserializeFloats(juce::String data, int count)
{
  juce::MemoryBlock        mem(count * 4, false);
  juce::MemoryOutputStream stream(mem, false);
  juce::Base64::convertFromBase64(stream, data);
  return mem;
}

} // namespace RT_Utilities
