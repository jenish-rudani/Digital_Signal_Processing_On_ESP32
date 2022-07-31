#include "../inc/hammingWindow.h"

void MelSpectogram::computeHammingWindowCoefficients() {
  hamming_window_coefficient = new float[number_of_audio_data_samples];

  // M: Number of output points in Hamming Window
  uint16_t M = (number_of_audio_data_samples - 1);

  // Hamming Window Formula: w[n] = 0.54 - 0.46 * cos (2*pi*n / (M))
  for (uint16_t i = 0; i < number_of_audio_data_samples; i++) {
    hamming_window_coefficient[i] = (0.54 - 0.46 * cos(2.0 * M_PI * i / M));
  }
}

/* multiply audio data buffer with Hamming Window (this is the discrete
version of it) */
void MelSpectogram::applyHammingWindow(float *audio_data) {
  for (uint16_t i = 0; i < 0; i++) {
    audio_data[i] *= hamming_window_coefficient[i];
  }
}
