#include <FastLED.h>
#include <SPIFFS.h>
#include <driver/i2s.h>

#include "inc/defines.h"
#include "inc/melSpectogram.h"

CRGB physic_leds[N_PIXELS];

int16_t audioDataBuffer[audioBufferDataLength];
uint16_t number_of_total_samples = BUFFER_SIZE * N_BUFFER_ROLLING_HISTORY;
class MelSpectogram my_mel_spectogram(number_of_total_samples,
                                      NUMBER_OF_MEL_BIN, MIN_FREQUENCY,
                                      MAX_FREQUENCY, SAMPLE_RATE,
                                      MIN_VOLUME_THRESHOLD);

void setup() {
  Serial.begin(115200);
  delay(100);

  FastLED.addLeds<NEOPIXEL, LED_STRIP_DATA_PIN>(physic_leds, N_PIXELS);
  delay(100);

  Serial.println("Setting I2S Drivers");
  setupI2S();

  delay(500);
}

void loop() {
  // Copy old buffer based on Rollig History
  for (int i = 0; i < N_BUFFER_ROLLING_HISTORY - 1; i++)
    memcpy(audio_data_buffer + i * BUFFER_SIZE,
           audio_data_buffer + (i + 1) * BUFFER_SIZE,
           sizeof(float) * BUFFER_SIZE);

  int16_t temp_audio_data_buffer[BUFFER_SIZE];

  unsigned int number_of_samples_read;
  i2s_read(I2S_NUM_0, temp_audio_data_buffer, BUFFER_SIZE * 2,
           &number_of_samples_read, portMAX_DELAY);

  for (int i = 0; i < BUFFER_SIZE; i++) {
    audio_data_buffer[BUFFER_SIZE * (N_BUFFER_ROLLING_HISTORY - 1) + i] =
        temp_audio_data_buffer[i] / 32768.0;
  }

  static float mel_spectogram_data_buffer[NUMBER_OF_MEL_BIN];
  my_mel_spectogram.computeMelSpectogram(audio_data_buffer,
                                         mel_spectogram_data_buffer);
}

void setupI2S() {
  installI2SDrivers();
  setMicrophonePins();

  // Reset I2S Drivers in case of Power on Reset
  i2s_stop(I2S_PORT);
  delay(1000);
  i2s_start(I2S_PORT);
}

void installI2SDrivers() {
  const i2s_config_t i2s_config = {
      .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
      .sample_rate = SAMPLE_RATE,
      .bits_per_sample = i2s_bits_per_sample_t(16),
      .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
      .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
      .intr_alloc_flags = 0,  // default interrupt priority
      .dma_buf_count = 8,
      .dma_buf_len = audioBufferDataLength,
      .use_apll = false,
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
}

void setMicrophonePins() {
  const i2s_pin_config_t pin_config = {.bck_io_num = I2S_SCK,
                                       .ws_io_num = I2S_WS,
                                       .data_out_num = -1,
                                       .data_in_num = I2S_SD};

  i2s_set_pin(I2S_PORT, &pin_config);
}