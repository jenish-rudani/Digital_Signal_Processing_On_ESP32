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
      (float*)malloc(number_of_audio_data_samples * sizeof(float));

  // Calculate Hamming Coefficients, place them in a Lookuptable for dynamic use
  computeHammingWindowCoefficients();

  // Calculate Power Of Two based on number of samples
  calculatePowerOfTwoBasedOnNumberOfSamples();
}

MelSpectogram::~MelSpectogram() {
  // Free resources
  free(temp_audio_data);
  if (hamming_window_coefficient) delete[] hamming_window_coefficient;
}

void MelSpectogram::calculatePowerOfTwoBasedOnNumberOfSamples() {
  // For 1024, power_of_two would be 10
  // We increase power by One until the expression "numberOfSample shift right
  // by power_of_two Value" reaches One. For Ex,
  // for numberOfSamples = 8, and power_of_two = 0
  // (8 >> 0) & 1 = False, -> power_of_two: 1,
  // (8 >> 1) & 1 = False, -> power_of_two: 2,
  // (8 >> 2) & 1 = False, -> power_of_two: 3,
  // (8 >> 3) & 1 = True, -> breaks the while loop,

  while (((number_of_audio_data_samples >> power_of_two) & 1) != 1)
    power_of_two++;
}

bool MelSpectogram::findMinMaxFromAudioBuffer(float* audio_buffer,
                                              float* mel_spectogram_data,
                                              float* min_data,
                                              float* max_data) {
  uint16_t i = 0;
  *min_data = audio_buffer[i];
  *max_data = audio_buffer[i];

  for (i = 1; i < number_of_audio_data_samples; i++) {
    if (audio_buffer[i] < *min_data) *min_data = audio_buffer[i];

    if (audio_buffer[i] > *max_data) *max_data = audio_buffer[i];

    if (*max_data - *min_data > minimum_volume_threshold) break;
  }

  if (i == number_of_audio_data_samples) {
    // Reset Mel Spectogram Data (no min or max found)
    for (i = 0; i < number_of_audio_data_samples; i++)
      mel_spectogram_data[i] = 0.0;

    return false;
  } else {
    return true;
  }
}

void MelSpectogram::computeMelSpectogram(float* input_audio_data,
                                         float* mel_audio_data) {
  float min_audio_data, max_audio_data;
  findMinMaxFromAudioBuffer(input_audio_data, mel_audio_data, &min_audio_data,
                            &max_audio_data);
  memcpy(temp_audio_data, input_audio_data,
         sizeof(float) * number_of_audio_data_samples);

  // Apply Smoothing to remove noise
  applyHammingWindow(temp_audio_data);

  // FFT Calculation
  computeFFT(temp_audio_data);
}
void MelSpectogram::arrangeAudioBufferDataBasedOnBitReversedIndex(
    float* audio_data) {
  uint16_t j = 0;
  float tmp;
  for (uint16_t i = 0; i < (number_of_audio_data_samples - 1); i++) {
    if (i < j) {
      tmp = audio_data[i];
      audio_data[i] = audio_data[j];
      audio_data[j] = tmp;
    }
    uint16_t k = (number_of_audio_data_samples >> 1);
    while (k <= j) {
      j -= k;
      k >>= 1;
    }
    j += k;
  }
}

void MelSpectogram::computeAmplitude(float* audio_data_real,
                                     float* audio_data_imaginary) {
  for (int i = 0; i < number_of_audio_data_samples; i++) {
    audio_data_real[i] =
        sqrt((audio_data_real[i] * audio_data_real[i]) +
             (audio_data_imaginary[i] * audio_data_imaginary[i]));
  }
}

void MelSpectogram::computeFFT(float* audio_data_real) {
  float* audio_data_imaginary;
  audio_data_imaginary = new float[number_of_audio_data_samples]();

  arrangeAudioBufferDataBasedOnBitReversedIndex(audio_data_real);
  // Compute the FFT
  float c1 = -1.0;
  float c2 = 0.0;
  uint16_t l2 = 1;
  uint16_t j = 0;
  for (uint8_t l = 0; (l < power_of_two); l++) {
    uint16_t l1 = l2;
    l2 <<= 1;
    float u1 = 1.0;
    float u2 = 0.0;
    for (j = 0; j < l1; j++) {
      for (uint16_t i = j; i < number_of_audio_data_samples; i += l2) {
        uint16_t i1 = i + l1;
        float t1 = u1 * audio_data_real[i1] - u2 * audio_data_imaginary[i1];
        float t2 = u1 * audio_data_imaginary[i1] + u2 * audio_data_real[i1];
        audio_data_real[i1] = audio_data_real[i] - t1;
        audio_data_imaginary[i1] = audio_data_imaginary[i] - t2;
        audio_data_real[i] += t1;
        audio_data_imaginary[i] += t2;
      }
      float z = ((u1 * c1) - (u2 * c2));
      u2 = ((u1 * c2) + (u2 * c1));
      u1 = z;
    }
    c2 = sqrt((1.0 - c1) / 2.0);
    c2 = -c2;
    c1 = sqrt((1.0 + c1) / 2.0);
  }
  computeAmplitude(audio_data_real, audio_data_imaginary);
  delete[] audio_data_imaginary;
}