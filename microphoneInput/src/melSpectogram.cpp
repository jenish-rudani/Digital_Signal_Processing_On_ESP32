#include "../inc/melSpectogram.h"

MelSpectogram::MelSpectogram(uint16_t number_of_samples,
                             uint16_t n_number_of_mel_bins, float min_frequency,
                             float max_frequency, uint16_t sampling_rate,
                             float min_volume_threshold) {
  number_of_audio_data_samples = number_of_samples;
  number_of_mel_bins = n_number_of_mel_bins;
  min_frequency = min_frequency;
  max_frequency = max_frequency;
  audio_sample_rate = sampling_rate;
  minimum_volume_threshold = min_volume_threshold;
  temp_audio_data =
      (float *)malloc(number_of_audio_data_samples * sizeof(float));

  computeHammingWindowCoefficients();
}

MelSpectogram::~MelSpectogram() {
  // Free resources
  free(temp_audio_data);
  if (hamming_window_coefficient) delete[] hamming_window_coefficient;
}