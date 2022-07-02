#include "fftAlgorithm.h"

/*
 * Initialize FFT with Inputs, Outputs, fftType of FFT, and the Direction Of FFT
 */

void calculateTwiddleFactor(float *twiddleFactorBuffer, int fftSize)
{
  float twoPiDividedByn = TWO_PI / fftSize;
  int i, m;
  for (int k = 0; k < fftSize; k++)
  {
    twiddleFactorBuffer[m] = cosf(twoPiDividedByn * k);     // Real part of Twiddle Factor
    twiddleFactorBuffer[m + 1] = sinf(twoPiDividedByn * k); // Imaginary Part of Twiddle Factor
    m += 2;                                                 // skip in step of two
  }
}

fft_config_t *initializeFFT(int fftSize, fft_type_t fftType, fft_direction_t direction, float *input, float *output)
{

  fft_config_t *myFFTConfig = (fft_config_t *)malloc(sizeof(fft_config_t));

  myFFTConfig->fftFlags = 0;
  myFFTConfig->fftType = fftType;
  myFFTConfig->fftDirection = direction;
  myFFTConfig->fftSize = fftSize;

  if (myFFTConfig->fftType == FFT_REAL)
    myFFTConfig->inputBuffer = (float *)malloc(myFFTConfig->fftSize * sizeof(float));
  else if (myFFTConfig->fftType == FFT_COMPLEX)
    myFFTConfig->inputBuffer = (float *)malloc(2 * myFFTConfig->fftSize * sizeof(float));

  if (myFFTConfig->fftType == FFT_REAL)
    myFFTConfig->outputBuffer = (float *)malloc(myFFTConfig->fftSize * sizeof(float));
  else if (myFFTConfig->fftType == FFT_COMPLEX)
    myFFTConfig->outputBuffer = (float *)malloc(2 * myFFTConfig->fftSize * sizeof(float));

  myFFTConfig->twiddleFactorBuffer = (float *)malloc(2 * myFFTConfig->fftSize * sizeof(float));
  calculateTwiddleFactor(myFFTConfig->twiddleFactorBuffer, fftSize);

  return myFFTConfig;
}

void fourPointFFT(float *input, int InputStepsToJump, float *output, int outputStepsToJump)
{
  // input is designed this way: [Re[0], Im[0], Re[1], Im[1], ... , Re[N-1], Im[N-1],]
  // Since Bit reveresal is expensive, I am hardcdoing the indexes this way
  // Algorithm:
  // From Butterfly, stepsToJump = 2
  // Real{0} X[0] = input[0] + input[2] + input[2*stepsToJump] + input[3 * stepsToJump]
  // Imaginary{0} X[1] = input[0] + input[2 + 1] + input[2*stepsToJump + 1] + input[3 * stepsToJump + 1]

  float t1, t2;
  t1 = input[0] + input[2 * InputStepsToJump];
  t2 = input[InputStepsToJump] + input[3 * InputStepsToJump];
  output[0] = t1 + t2;
  output[2 * outputStepsToJump] = t1 - t2;

  t1 = input[1] + input[2 * InputStepsToJump + 1];
  t2 = input[InputStepsToJump + 1] + input[3 * InputStepsToJump + 1];
  output[1] = t1 + t2;
  output[2 * outputStepsToJump + 1] = t1 - t2;

  t1 = input[0] - input[2 * InputStepsToJump];
  t2 = input[InputStepsToJump + 1] - input[3 * InputStepsToJump + 1];
  output[outputStepsToJump] = t1 + t2;
  output[3 * outputStepsToJump] = t1 - t2;

  t1 = input[1] - input[2 * InputStepsToJump + 1];
  t2 = input[3 * InputStepsToJump] - input[InputStepsToJump];
  output[outputStepsToJump + 1] = t1 + t2;
  output[3 * outputStepsToJump + 1] = t1 - t2;
}