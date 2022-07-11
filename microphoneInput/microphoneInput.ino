#include <driver/i2s.h>
#define I2S_WS 15
#define I2S_SD 13
#define I2S_SCK 2
#define I2S_PORT I2S_NUM_0
#define audioBufferDataLength 64

int16_t audioDataBuffer[audioBufferDataLength];

void setup()
{
  Serial.begin(115200);
  delay(100);
  Serial.println("Setting I2S Drivers");
  delay(1000);

  setupI2S();

  delay(500);
}

void loop()
{

  size_t numberOfBytesRead = 0;
  esp_err_t result = i2s_read(I2S_PORT, &audioDataBuffer, audioBufferDataLength, &numberOfBytesRead, portMAX_DELAY);
  if (result == ESP_OK)
  {
    int sample = numberOfBytesRead / 8;
    if (sample > 0)
    {
      float meanValue = 0;
      for (int i = 0; i < sample; ++i)
      {
        meanValue += (audioDataBuffer[i]);
      }
      meanValue /= sample;
      Serial.println(meanValue);
    }
  }
}

void setupI2S()
{
  installI2SDrivers();
  setMicrophonePins();
  i2s_stop(I2S_PORT);
  delay(1000);
  i2s_start(I2S_PORT);
}

void installI2SDrivers()
{
  const i2s_config_t i2s_config = {
      .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
      .sample_rate = 44100,
      .bits_per_sample = i2s_bits_per_sample_t(16),
      .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
      .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
      .intr_alloc_flags = 0, // default interrupt priority
      .dma_buf_count = 8,
      .dma_buf_len = audioBufferDataLength,
      .use_apll = false,
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
}

void setMicrophonePins()
{
  const i2s_pin_config_t pin_config = {
      .bck_io_num = I2S_SCK,
      .ws_io_num = I2S_WS,
      .data_out_num = -1,
      .data_in_num = I2S_SD};

  i2s_set_pin(I2S_PORT, &pin_config);
}