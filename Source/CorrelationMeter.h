#pragma once

class CorrelationMeter : public juce::Component
{
 public:

  CorrelationMeter () : red(0xfffa4659), green(0xffa3de83) {};

  void paint (juce::Graphics &g) override {
    auto rect = this->getLocalBounds();
    auto barRect = rect.withHeight(16);
    int halfWidth = rect.getWidth() / 2;
    int halfInnerWidth = halfWidth - 2;
    int needle = halfInnerWidth + (int) (this->correlation * (float) halfInnerWidth);

    g.setColour(this->red);
    g.fillRect(barRect.withWidth(halfWidth));
    g.setColour(this->green);
    g.fillRect(barRect.withWidth(halfWidth).withX(barRect.getX() + halfWidth));
    g.setColour(juce::Colours::black);
    g.drawRect(barRect, 1);

    g.setColour(juce::Colours::white);
    g.drawRect(barRect.withX(needle).withWidth(2));

    g.drawText("-1", 0, 16, 16, 16, juce::Justification::left);
    g.drawText("0", halfWidth - 8, 16, 16, 16, juce::Justification::horizontallyCentred);
    g.drawText("1", rect.getWidth() - 16, 16, 16, 16, juce::Justification::right);
  }

  void setCorrelation (float correlationIn) {
    this->correlation = correlationIn;
  }

 private:
  float correlation = 0;

  juce::Colour red;
  juce::Colour green;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CorrelationMeter)
};