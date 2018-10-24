#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
//#include "LionKing.h"
#include "YinYang.h"
//#include "Toad.h"
#include "Motor.h"

#define numLeds 256
#define stripPin 6
#define brightness 10

// Dominoblocks are 24 by 48 by 8 mm
#define blockWidth 24
#define blockHeight 48
#define rowSpacing 4
bool toggleRows = false; 

#define dir1PinA 3
#define dir2PinA 4
#define speedPinA 5 // Needs to be a PWM pin to be able to control motor speed
#define dir1PinB 7
#define dir2PinB 8
#define speedPinB 6 // Needs to be a PWM pin to be able to control motor speed
#define servoBlackPin 9
#define servoWhitePin 10
Motor Motor1{dir1PinA,dir2PinA,speedPinA};
Motor Motor2{dir1PinB,dir2PinB,speedPinB};

// macros te generate loopup table (at compile-time)
#define B2(n) n, n + 1, n + 1, n + 2
#define B4(n) B2(n), B2(n + 1), B2(n + 1), B2(n + 2)
#define B6(n) B4(n), B4(n + 1), B4(n + 1), B4(n + 2)
#define Count_Bits_Macro B6(0), B6(1), B6(1), B6(2)

Adafruit_NeoPixel strip =
    Adafruit_NeoPixel(numLeds, stripPin, NEO_GRB + NEO_KHZ800);

byte countBts[256] = {Count_Bits_Macro}; // calculate lookuptable to count bits in byte
byte countBits(byte Im[], int rows, int cols) {
  byte sum = 0;
  byte nBytesPerRow = cols % 8 == 0 ? cols / 8 : cols / 8 + 1; // count how many bytes make one row
  for (byte i = 0; i < rows * nBytesPerRow; i++) {
    byte b = Im[i];
    if (i % nBytesPerRow == nBytesPerRow -1 && cols % 8) {
      byte droppedBits = 8 - (cols % 8);
      b = b >> droppedBits;
    }
    sum += countBts[b];
  }
  return sum;
}



void processImage(byte Im[], int rows, int cols) {
  byte nBytesPerRow = cols % 8 == 0
                          ? cols / 8
                          : cols / 8 + 1; // count how many bytes make one row
  for (byte row = 0; row < rows; row++) {
    bool toggle = toggleRows ? row % 2 : false; // determine toggle only when toggleRows is on.
    if (!toggle) {
      for (int b = 0; b < nBytesPerRow; b++) {
        int n = row * nBytesPerRow + b;
        byte imageByte = Im[n];
        for (int i = 0; i < min(8, cols - 8 * b); i++) {
          if (imageByte & (1 << (7 - i))) {
            Serial.print("8");
          } else {
            Serial.print(".");
          }
        }
      }
    }
    else {
      for (int b = nBytesPerRow - 1; b >= 0; b--) {
        int n = row * nBytesPerRow + b;
        byte imageByte = Im[n];
        byte droppedBits = 0;
        if (b == nBytesPerRow - 1 && cols % 8) { // if b is de laatste byte in de rij en if cols % 8 een rest geeft (dan moeten er kolommen wegvallen)
          droppedBits = 8 - (cols % 8);
        } // if b is the last bit
        for (byte i = 0; i < min(8, cols - 8 * b); i++) {
          if (imageByte & (1 << (i + droppedBits))) {
            Serial.print("8");
          } else {
            Serial.print(".");
          }
        }
      }
    }
    Serial.println("");
  }
}

void printImageInfo(byte Im[], int rows, int cols){
  Serial.print(F("Your image has ")); Serial.print(rows); Serial.print(F(" rows and ")); Serial.print(cols); Serial.println(F(" columns."));
  Serial.print(F("External dimensions are approximately "));
  int width = (.5 * cols + .5) * blockHeight / 10.0 + 0.5; // +0.5 zorgt voor correcte afronding
  int height = (rows * blockWidth + rowSpacing * (rows-1)) / 10.0 + 0.5;
  Serial.print(width); Serial.print(F(" by ")); Serial.print(height); Serial.println(F(" cm."));
  int ones = countBits(Im, rows, cols);
  Serial.print(F("The image contains ")); Serial.print(ones);  Serial.print(F(" black and ")); Serial.print(rows * cols - ones); Serial.println(F(" white blocks."));
}

void dropBlack(){
  Serial.println("Drop zwart blokje.");
  // TODO: Twee servomotoren aansturen met een bitShiftRegister
  
}

void setup() {
  strip.setBrightness(brightness);
  strip.begin();
  strip.clear();
  strip.show();
  Serial.begin(9600);
  ////processImage(LionKing, 64, 128);
  //printImageInfo(yinYang, 16, 16);
  //processImage(yinYang, 16, 16);
  Motor1.rename("Voorwaartse Motor 1");
  Motor2.rename("Zijwaartse Motor 2");
<<<<<<< HEAD
  Motor1.toggleDebugMode();
  Motor2.toggleDebugMode();
=======
  Motor1.disableDebuggingOutput();
  Motor2.disableDebuggingOutput();
>>>>>>> 89786875b182db9d4669ebb206d2aebcc4dca0ac
}

void loop() {
  Motor1.move(Motor::forward,128,1000);
  dropBlack();
  Motor2.move(Motor::backward,128,1000);
  dropBlack();
}