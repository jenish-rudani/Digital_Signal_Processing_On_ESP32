#pragma once
#include <cmath>

class MelSpectogram {
 private:
  uint16_t number_of_mel_bins, number_of_audio_data_samples, audio_sample_rate;
  float min_frequency, max_frequency, minimum_volume_threshold;
  float *temp_audio_data;

  // Hamming Window
  float *hamming_window_coefficient = NULL;
  void computeHammingWindowCoefficients();
  void applyHammingWindow();

 public:
  // Constructor
  MelSpectogram(uint16_t number_of_samples, uint16_t n_number_of_mel_bins,
                float min_frequency, float max_frequency,
                uint16_t sampling_rate, float min_volume_threshold);
  // Destructor
  ~MelSpectogram();

  void applyHammingWindow(float *audio_data);
  void computeMelSpectogram(float *input_audio_data, float *mel_audio_data);

  void computeMelSpectogram(float *audio_data, float *mel_spectogram_data);
};
