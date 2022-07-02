#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <Arduino.h>
#include "typedef.h"

#define FFT_OWN_INPUT_MEM 1
#define FFT_OWN_OUTPUT_MEM 2

fft_config_t *initializeFFT(int size, fft_type_t fftType, fft_direction_t direction, float *input, float *output);
void fourPointFFT(float *input, int stride_in, float *output, int outputStepsToJump);