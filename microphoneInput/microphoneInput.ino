#include <SPIFFS.h>
#include <driver/i2s.h>
#define I2S_WS 15
#define I2S_SD 13
#define I2S_SCK 2
#define I2S_PORT I2S_NUM_0
#define audioBufferDataLength 64
#define RECORD_TIME (20) // Seconds
#define FLASH_RECORD_SIZE (I2S_CHANNEL_NUM * I2S_SAMPLE_RATE * I2S_SAMPLE_BITS / 8 * RECORD_TIME)

int16_t audioDataBuffer[audioBufferDataLength];
File file;
const char audioFilename[] = "/recording.wav";
const int headerSize = 44;

void setup()
{
  Serial.begin(115200);
  delay(100);
  Serial.println("Setting I2S Drivers");
  delay(1000);
  SPIFFSInit();
  delay(500);

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

void SPIFFSInit()
{
  if (!SPIFFS.begin(true))
  {
    Serial.println("!!!! SPIFFS Init Failed !!!!");
    while (1)
      yield();
  }

  SPIFFS.remove(audioFilename); // Removing old file
  file = SPIFFS.open(audioFilename, FILE_WRITE);
  if (!file)
  {
    Serial.println("!!!! File Not Available !!!!");
  }

  byte wavHeader[headerSize];
  wavHeader(wavHeader, FLASH_RECORD_SIZE);

  file.write(wavHeader, headerSize); // Append wav header at the top
  listSPIFFS();
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

void wavHeader(byte *header, int wavSize)
{
  header[0] = 'R';
  header[1] = 'I';
  header[2] = 'F';
  header[3] = 'F';
  unsigned int fileSize = wavSize + headerSize - 8;
  header[4] = (byte)(fileSize & 0xFF);
  header[5] = (byte)((fileSize >> 8) & 0xFF);
  header[6] = (byte)((fileSize >> 16) & 0xFF);
  header[7] = (byte)((fileSize >> 24) & 0xFF);
  header[8] = 'W';
  header[9] = 'A';
  header[10] = 'V';
  header[11] = 'E';
  header[12] = 'f';
  header[13] = 'm';
  header[14] = 't';
  header[15] = ' ';
  header[16] = 0x10;
  header[17] = 0x00;
  header[18] = 0x00;
  header[19] = 0x00;
  header[20] = 0x01;
  header[21] = 0x00;
  header[22] = 0x01;
  header[23] = 0x00;
  header[24] = 0x80;
  header[25] = 0x3E;
  header[26] = 0x00;
  header[27] = 0x00;
  header[28] = 0x00;
  header[29] = 0x7D;
  header[30] = 0x00;
  header[31] = 0x00;
  header[32] = 0x02;
  header[33] = 0x00;
  header[34] = 0x10;
  header[35] = 0x00;
  header[36] = 'd';
  header[37] = 'a';
  header[38] = 't';
  header[39] = 'a';
  header[40] = (byte)(wavSize & 0xFF);
  header[41] = (byte)((wavSize >> 8) & 0xFF);
  header[42] = (byte)((wavSize >> 16) & 0xFF);
  header[43] = (byte)((wavSize >> 24) & 0xFF);
}