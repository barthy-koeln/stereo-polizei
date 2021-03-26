#pragma once

#include <JuceHeader.h>

template <uint8_t N, class input_t = float_t, class sum_t = float_t>
class MovingAverage {

 public:
  float operator()(input_t input) {
    this->sum -= this->buffer[this->index];
    this->sum += input;
    this->buffer[index] = input;

    if(++this->index == N){
      this->index = 0;
    }

    return this->sum / N;
  }

 private:
  uint8_t index;
  sum_t sum;
  input_t buffer[N];
};