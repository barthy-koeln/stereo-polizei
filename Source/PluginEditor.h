#pragma once

#include <JuceHeader.h>
#include <valarray>
#include "PluginProcessor.h"
#include "CorrelationMeter.h"

#define GOOD_INDEX 0
#define BAD_INDEX 1

class StereoPolizeiAudioProcessorEditor :
  public juce::AudioProcessorEditor,
  public juce::Timer
{
 public:
  explicit StereoPolizeiAudioProcessorEditor (StereoPolizeiAudioProcessor &p);
  ~StereoPolizeiAudioProcessorEditor () override;

  void paint (juce::Graphics &) override;
  void resized () override;

  void timerCallback () override;

 private:
  struct image
  {
    const char *name;
    int size;
  };

  const image images[2] = {
    {
      BinaryData::good_png,
      BinaryData::good_pngSize
    },
    {
      BinaryData::bad_png,
      BinaryData::bad_pngSize
    }
  };

  StereoPolizeiAudioProcessor &processor;
  CorrelationMeter correlationMeter;

  int getImageIndex ();

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereoPolizeiAudioProcessorEditor)
};
