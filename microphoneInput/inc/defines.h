#pragma once

#define I2S_WS 15
#define I2S_SD 13
#define I2S_SCK 2
#define I2S_PORT I2S_NUM_0
#define audioBufferDataLength 64

const uint16_t BUFFER_SIZE = 1024;
const uint8_t N_BUFFER_ROLLING_HISTORY = 2;
const uint16_t SAMPLE_RATE = 44100;
const uint16_t NUMBER_OF_MEL_BIN = 18;
const float MIN_FREQUENCY = 200;
const float MAX_FREQUENCY = 12000;
const float MIN_VOLUME_THRESHOLD = 0.0003;

const uint16_t N_PIXELS = 60;
const int LED_STRIP_DATA_PIN = 21;

float audio_data_buffer[BUFFER_SIZE * N_BUFFER_ROLLING_HISTORY];

float mel_spectogram_gain_alpha_decay = 0.06;
float mel_spectogram_smoothing_alpha_decay = 0.5;

float mel_spectogram_gain_alpha_rise = 0.99;
float mel_spectogram_smoothing_alpha_rise = 0.99;
