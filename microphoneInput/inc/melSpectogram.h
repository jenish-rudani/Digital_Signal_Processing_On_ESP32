#pragma once
#include <cmath>

class MelSpectogram {
 private:
  uint16_t number_of_mel_bins, number_of_audio_data_samples, audio_sample_rate;
  float min_frequency, max_frequency, minimum_volume_threshold;
  float *temp_audio_data;
  uint8_t power_of_two = 0;
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

  void calculatePowerOfTwoBasedOnNumberOfSamples();
  void applyHammingWindow(float *audio_data);
  void computeMelSpectogram(float *input_audio_data, float *mel_audio_data);

  void arrangeAudioBufferDataBasedOnBitReversedIndex(float *audio_data);
  void computeFFT(float *audio_data_real);
  void computeAmplitude(float *audio_data_real, float *audio_data_imaginary);
  bool findMinMaxFromAudioBuffer(float *audio_buffer,
                                 float *mel_spectogram_data, float *min_data,
                                 float *max_data);
};
