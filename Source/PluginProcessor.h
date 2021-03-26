#pragma once

#include <JuceHeader.h>
#include "MovingAverage.h"
#include <valarray>

class StereoPolizeiAudioProcessor : public juce::AudioProcessor
{
 public:
  StereoPolizeiAudioProcessor ();
  ~StereoPolizeiAudioProcessor () override;

  void prepareToPlay (double sampleRate, int samplesPerBlock) override;
  void releaseResources () override;

  bool isBusesLayoutSupported (const BusesLayout &layouts) const override;

  void processBlock (juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

  juce::AudioProcessorEditor *createEditor () override;
  bool hasEditor () const override;

  const juce::String getName () const override;

  bool acceptsMidi () const override;
  bool producesMidi () const override;
  bool isMidiEffect () const override;
  double getTailLengthSeconds () const override;

  int getNumPrograms () override;
  int getCurrentProgram () override;
  void setCurrentProgram (int index) override;
  const juce::String getProgramName (int index) override;
  void changeProgramName (int index, const juce::String &newName) override;

  void getStateInformation (juce::MemoryBlock &destData) override;
  void setStateInformation (const void *data, int sizeInBytes) override;

  float getAverage () const;
  float getCorrelation () const;
  bool hasInput () const;

 private:
  float correlation = std::nanf("");
  bool input = false;
  float average = 0;

  MovingAverage<8> movingAverage{};

  std::valarray<float> leftSamples;
  std::valarray<float> rightSamples;
  std::valarray<float> product;
  std::valarray<float> squareLeft;
  std::valarray<float> squareRight;

  void calculateCorrelation (juce::AudioBuffer<float> &buffer);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereoPolizeiAudioProcessor)
};
