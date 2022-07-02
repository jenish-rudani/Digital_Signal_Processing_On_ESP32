#include "./fftAlgorithm.h"
#include <stdlib.h>
#include <stdio.h>
char printBuffer[300];
void setup()
{
  Serial.begin(115200); // Using Serial Port
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.print("Starting 4-Point FFT\n");
  int N = 4;
  float inputData[] = {3, 0, 4, 0, 5, 0, 6, 0};
  for (int i = 0; i < 4; i++)
  {
    Serial.print(inputData[(i * 2)]);
    Serial.print(" + ");
    Serial.print(inputData[(i * 2) + 1]);
    Serial.println(" i");
  }
  float outputData[8];
  fourPointFFT(inputData, 2, outputData, 2);
  for (int k = 0; k < 8; k += 2)
  {
    sprintf(printBuffer, "%.2f% +.2fi , ", outputData[k], outputData[k + 1]);
    Serial.print(printBuffer);
  }
  Serial.println("\n\nFinished Calculating FFT");
}
void loop()
{
}