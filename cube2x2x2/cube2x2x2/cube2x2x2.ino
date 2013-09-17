// Arduino code.

/*
Create an array containing values between 0 and 5.
 Create a transition matrix that can operate on the original array.
 Set up a condition that causes the translation matrix to operate on the original.
 Print the result.
 */

// Set up for LPD8806 strip
#include "LPD8806.h"
#include "SPI.h"

int dataPin = 2;
int clockPin = 3;
const int numPixels = 24;
LPD8806 strip = LPD8806(numPixels,dataPin,clockPin);

// Create globals

const int numSquares = 24; // Number of square tiles on our cube (2x2x2 cube = 24 squares, 3x3x3 cube = 54 squares)

int arrayShow[numSquares] = {
  0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5 };
// Our initial matrix representing the 54 squares.
int arrayTemp[numSquares] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
// This is a temp matrix to hold results of any calculations.

// Identity transformation.
int arrTrans1[numSquares][numSquares] = { // arrTrans1[i][j] is a translation matrix. Inv is arrTrans1[j][i].
  {
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  //
  ,{
    0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  //
  ,{
    0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0   }
  //
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0   }
  //
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0   }
  //
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1   }
};

// Rotate top face (squares 1,2,3,4 and 9-12,12-13,13-16,16-18,18-19,19-5,5-8,8-9)
// Oops, this actually only rotates one spot at a time. Use arrTrans3 instead.
int arrTrans2[numSquares][numSquares] = { // arrTrans1[i][j] is a translation matrix. Inv is arrTrans1[j][i].
  // 5
  {
    0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  // 5
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0   }
  ,{
    0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  // 9
  ,{
    0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  // 13
  ,{
    0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0   }
  // 17
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0   }
  // 21
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1   }
};

// Rotate top face (squares 1,2,3,4 and 9-13,12-16,13-18,16-19,18-5,19-8,5-9,8-12)
int arrTrans3[numSquares][numSquares] = { // arrTrans1[i][j] is a translation matrix. Inv is arrTrans1[j][i].
  // 5
  {
    0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  // 5
  ,{
    0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0   }
  // 9 4 8 2 6 0 4
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0   }
  // 13
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0   }
  // 17
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0   }
  // 21
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0   }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1   }
};

void setup() {
  Serial.begin(9600);
  strip.begin();
}

void loop() {
  // delay(10);
  if( (millis() % 5000) == 0) {
    for (int i = 0; i < numSquares; i++) {
      for (int j = 0; j < numSquares; j++) {
        arrayTemp[i] += arrTrans3[j][i] * arrayShow[j];
      }
    }
    for (int i = 0; i < numSquares; i++) {
      arrayShow[i] = arrayTemp[i];
      arrayTemp[i] = 0;
      if (arrayShow[i] == 0) {
        strip.setPixelColor(i,127,127,127);
      }
      else if (arrayShow[i] == 1) {
        strip.setPixelColor(i,127,0,0);
      }
      else if (arrayShow[i] == 2) {
        strip.setPixelColor(i,0,127,0);
      }
      else if (arrayShow[i] == 3) {
        strip.setPixelColor(i,0,0,127);
      }
      else if (arrayShow[i] == 4) {
        strip.setPixelColor(i,127,127,0);
      }
      else if (arrayShow[i] == 5) {
        strip.setPixelColor(i,0,127,127);
      }
      Serial.print(arrayShow[i]);
      Serial.print(' ');
    }
    Serial.println(' ');
    strip.show();
    // Serial.println(millis());
  }
}

