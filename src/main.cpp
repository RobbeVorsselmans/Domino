#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#define numLeds 256
#define stripPin 6
#define brightness 10
#define nRows 16
#define nCols 16

bool toggleRows = true;

Adafruit_NeoPixel strip =
    Adafruit_NeoPixel(numLeds, stripPin, NEO_GRB + NEO_KHZ800);

byte Toad[] = {0b00000111, 0b11100000, 0b00001100, 0b01110000, 0b00110000,
               0b01111100, 0b01111000, 0b00011110, 0b01111000, 0b00000010,
               0b11000001, 0b11000011, 0b10000001, 0b11000001, 0b11000001,
               0b11000001, 0b11000000, 0b00000001, 0b11000000, 0b00001101,
               0b10011111, 0b11111101, 0b11110110, 0b01101111, 0b01100110,
               0b01100110, 0b00100000, 0b00000100, 0b00111000, 0b00001100,
               0b00001111, 0b11111000};

byte Image[] = {0b00001111, 0b00001111, 0b00001111, 0b00001111, 0b00001111,
                0b00001111, 0b00001111, 0b00001111, 0b00001111, 0b00001111,
                0b00001111, 0b00001111, 0b00001111, 0b00001111, 0b00001111,
                0b00001111, 0b00001111, 0b00001111, 0b00001111, 0b00001111,
                0b00001111, 0b00001111, 0b00001111, 0b00001111, 0b00001111,
                0b00001111, 0b00001111, 0b00001111, 0b00001111, 0b00001111,
                0b00001111, 0b00001111};

unsigned int ImageOld[] = {
    0b0000011111100000, 0b0000110001110000, 0b0011000001111100,
    0b0111100000011110, 0b0111100000000010, 0b1100000111000011,
    0b1000000111000001, 0b1100000111000001, 0b1100000000000001,
    0b1100000000001101, 0b1001111111111101, 0b1111011001101111,
    0b0110011001100110, 0b0010000000000100, 0b0011100000001100,
    0b0000111111111000};

byte ImageBytes[] = {0b00011100, 0b01010101, 0b00110011,
                     0b11111111, 0b11010101, 0b11110010};

void processImage16(unsigned int Im[]) {
  for (int row = 0; row < nRows; row++) {
    unsigned int bin16 = Im[row];
    bool toggle = toggleRows ? row % 2 : false;
    for (int col = 0; col < nCols; col++) {
      if (bin16 & (1 << col)) {
        Serial.print("8");
        if (toggle) {
          strip.setPixelColor(row * nCols + (nCols - 1 - col), 0, 255, 0);
        } else {
          strip.setPixelColor(row * nCols + col, 0, 255, 0);
        }

      } else {
        Serial.print(".");
        if (toggle) {
          strip.setPixelColor(row * nCols + (nCols - 1 - col), 0, 0, 0);
        } else {
          strip.setPixelColor(row * nCols + col, 0, 0, 0);
        }
      }
    }
    Serial.println("");
  }
  strip.show();
}

void processImage(byte Im[], int rows, int cols) {
  byte nBytesPerRow = cols % 8 == 0
                          ? cols / 8
                          : cols / 8 + 1; // count how many bytes make one row
  for (byte row = 0; row < rows; row++) {
    // bool toggle = toggleRows ? row % 2 : false; // determine toggle only when
    // toggleRows is on.
    for (byte b = 0; b < nBytesPerRow; b++) {
      byte n = row * nBytesPerRow + b;
      byte imageByte = Im[n];
      for (int i = 0; i < min(8, cols - 8 * b); i++) {
        if (imageByte & (1 << (7 - i))) {
          Serial.print("8");
        } else {
          Serial.print(".");
        }
      }
    }
    Serial.println("");
  }
}

void setup() {

  strip.setBrightness(brightness); // maximale lichtintensiteit
  strip.begin();
  strip.clear();
  strip.show();
  Serial.begin(9600);
  // processImage16(Image);
  processImage(Image, 16, 16);
}

void loop() {}