#include <cmath>
#include "PluginProcessor.h"
#include "PluginEditor.h"

StereoPolizeiAudioProcessor::StereoPolizeiAudioProcessor ()
  : AudioProcessor(
  BusesProperties()
    .withInput("Input", juce::AudioChannelSet::stereo(), true)
    .withOutput("Output", juce::AudioChannelSet::stereo(), true)
), movingAverage() {
}

StereoPolizeiAudioProcessor::~StereoPolizeiAudioProcessor () = default;

#pragma ide diagnostic ignored "readability-const-return-type"
const juce::String StereoPolizeiAudioProcessor::getName () const {
  return JucePlugin_Name;
}

bool StereoPolizeiAudioProcessor::acceptsMidi () const {
  return false;
}

bool StereoPolizeiAudioProcessor::producesMidi () const {
  return false;
}

bool StereoPolizeiAudioProcessor::isMidiEffect () const {
  return false;
}

double StereoPolizeiAudioProcessor::getTailLengthSeconds () const {
  return 0.0;
}

int StereoPolizeiAudioProcessor::getNumPrograms () {
  return 1;
}

int StereoPolizeiAudioProcessor::getCurrentProgram () {
  return 0;
}

void StereoPolizeiAudioProcessor::setCurrentProgram (int index) {
}

#pragma ide diagnostic ignored "readability-const-return-type"
const juce::String StereoPolizeiAudioProcessor::getProgramName (int index) {
  return {};
}

void StereoPolizeiAudioProcessor::changeProgramName (int index, const juce::String &newName) {
}

//==============================================================================
void StereoPolizeiAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) {
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
  this->leftSamples.resize(samplesPerBlock);
  this->rightSamples.resize(samplesPerBlock);
  this->product.resize(samplesPerBlock);
  this->squareLeft.resize(samplesPerBlock);
  this->squareRight.resize(samplesPerBlock);
}

void StereoPolizeiAudioProcessor::releaseResources () {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
  this->leftSamples.resize(0);
  this->rightSamples.resize(0);
  this->product.resize(0);
  this->squareLeft.resize(0);
  this->squareRight.resize(0);
}

bool StereoPolizeiAudioProcessor::isBusesLayoutSupported (const BusesLayout &layouts) const {
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo()) {
    return false;
  }

  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet()) {
    return false;
  }

  return true;
}

template<typename T>
bool areEqual (const std::valarray<T> &x, const std::valarray<T> &b) {
  for (auto it = std::make_pair(std::begin(x), std::begin(b)); it.first != std::end(x); ++it.first, ++it.second) {
    if ((*it.first) == (*it.second)) { continue; }
    return false;
  }
  return true;
}

void StereoPolizeiAudioProcessor::processBlock (juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) {
  juce::ScopedNoDenormals noDenormals;

  this->calculateCorrelation(buffer);

  if (!this->hasInput()) {
    return;
  }

  this->average = this->movingAverage(this->correlation);
}

bool StereoPolizeiAudioProcessor::hasEditor () const {
  return true;
}

juce::AudioProcessorEditor *StereoPolizeiAudioProcessor::createEditor () {
  return new StereoPolizeiAudioProcessorEditor(*this);
}

//==============================================================================
void StereoPolizeiAudioProcessor::getStateInformation (juce::MemoryBlock &destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.

}

void StereoPolizeiAudioProcessor::setStateInformation (const void *data, int sizeInBytes) {
  // You should use this method to restore your parameters from this memory block,
  // whose contents will have been created by the getStateInformation() call.
}

void StereoPolizeiAudioProcessor::calculateCorrelation (juce::AudioBuffer<float> &buffer) {
  this->leftSamples = *buffer.getReadPointer(0);
  this->rightSamples = *buffer.getReadPointer(1);

  if (this->leftSamples.sum() == 0 && this->rightSamples.sum() == 0) {
    // both silent
    this->correlation = std::nanf("");
    this->input = false;
    return;
  }

  this->input = true;

  if (areEqual(this->leftSamples, this->rightSamples)) {
    // equal, full mono
    this->correlation = 1;
    return;
  }

  this->product = this->leftSamples * this->rightSamples;
  this->squareLeft = std::pow(this->leftSamples, {2});
  this->squareRight = std::pow(this->rightSamples, {2});

  float squareSumProduct = squareLeft.sum() * squareRight.sum();
  if (squareSumProduct == 0) {
    // one is silent, total difference
    this->correlation = 0;
    return;
  }

  this->correlation = product.sum() / std::sqrt(squareSumProduct);
}

bool StereoPolizeiAudioProcessor::hasInput () const {
  return this->input;
}

float StereoPolizeiAudioProcessor::getAverage () const {
  return this->average;
}

float StereoPolizeiAudioProcessor::getCorrelation () const {
  return this->correlation;
}

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter () {
  return new StereoPolizeiAudioProcessor();
}