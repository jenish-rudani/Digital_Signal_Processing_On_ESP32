#pragma once

typedef enum
{
  FFT_REAL,
  FFT_COMPLEX
} fft_type_t;

typedef enum
{
  FFT_ANALYSIS,
  FFT_SYNTHESIS
} fft_direction_t;

typedef struct
{
  fft_type_t fftType;           // real or complex
  fft_direction_t fftDirection; // forward(analysis) or backward (synthesis)
  float *inputBuffer;
  float *outputBuffer;
  float *twiddleFactorBuffer;
  unsigned int fftFlags;
  int fftSize; // FFT size (N point FFT then size is N)
} fft_config_t;