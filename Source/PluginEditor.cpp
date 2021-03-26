#include "PluginProcessor.h"
#include "PluginEditor.h"

StereoPolizeiAudioProcessorEditor::StereoPolizeiAudioProcessorEditor (StereoPolizeiAudioProcessor &p)
  : AudioProcessorEditor(&p), processor(p) {
  this->setSize(512, 512);
  this->startTimerHz(30);

  this->addAndMakeVisible(this->correlationMeter);
}

StereoPolizeiAudioProcessorEditor::~StereoPolizeiAudioProcessorEditor () = default;

void StereoPolizeiAudioProcessorEditor::timerCallback () {
  this->correlationMeter.setCorrelation(this->processor.getCorrelation());
  this->repaint();
}

void StereoPolizeiAudioProcessorEditor::paint (juce::Graphics &g) {
  int imageIndex = this->getImageIndex();
  auto rect = this->getLocalBounds().reduced(16);

  int height = 32;
  this->correlationMeter.setBounds(rect.removeFromTop(height).withHeight(height));

  g.drawImageAt(
    juce::ImageCache::getFromMemory(
      this->images[imageIndex].name,
      this->images[imageIndex].size
    ),
    0,
    0
  );
}

void StereoPolizeiAudioProcessorEditor::resized () {

}

int StereoPolizeiAudioProcessorEditor::getImageIndex () {
  if (!this->processor.hasInput()) {
    return 0;
  }

  return this->processor.getAverage() < 0 ? BAD_INDEX : GOOD_INDEX;
}