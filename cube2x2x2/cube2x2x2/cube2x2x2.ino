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
int arrIdentity[numSquares][numSquares] = { // arrTrans1[i][j] is a translation matrix. Inv is arrTrans1[j][i].
  {
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  //
  ,{
    0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  //
  ,{
    0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0       }
  //
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0       }
  //
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0       }
  //
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1       }
};

/*
// Rotate top face (squares 1,2,3,4 and 9-12,12-13,13-16,16-18,18-19,19-5,5-8,8-9)
// Oops, this actually only rotates one spot at a time. Use arrTrans3 instead.
int arrTransFace1[numSquares][numSquares] = { // arrTrans1[i][j] is a translation matrix. Inv is arrTrans1[j][i].
  // 5
  {
    0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  // 5
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0       }
  ,{
    0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  // 9
  ,{
    0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  // 13
  ,{
    0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0       }
  // 17
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0       }
  // 21
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1       }
};
*/

// Rotate top face (squares 1,2,3,4 and 9-13,12-16,13-17,16-18,17-5,18-8,5-9,8-12)
int arrTransFace1[numSquares][numSquares] = { // arrTrans1[i][j] is a translation matrix. Inv is arrTrans1[j][i].
  // 5
  {
    0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  // 5
  ,{
    0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0       }
  // 9 4 8 2 6 0 4
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0       }
  // 13
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0       }
  // 17
  ,{
    0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0       }
  // 21
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1       }
};


// 5-6,6-7,7-8,8-5, 1-18,4-19,18-24,19-23,24-10,23-9,10-1,9-4
int arrTransFace2[numSquares][numSquares] = { // arrTrans1[i][j] is a translation matrix. Inv is arrTrans1[j][i].
  {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0       }
  ,{
    0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0       }
  // 5
  ,{
    0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  // 9
  ,{
    0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0       }
  // 13
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0       }
  // 17
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0       }
  // 21
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0       }
  ,{
    0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
};


int arrTransFace3[numSquares][numSquares] = { // arrTrans1[i][j] is a translation matrix. Inv is arrTrans1[j][i].
  {
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  //
  ,{
    0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  //
  ,{
    0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0       }
  //
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0       }
  //
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0       }
  //
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1       }
};

int arrTransFace4[numSquares][numSquares] = { // arrTrans1[i][j] is a translation matrix. Inv is arrTrans1[j][i].
  {
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  //
  ,{
    0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  //
  ,{
    0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0       }
  //
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0       }
  //
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0       }
  //
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1       }
};

int arrTransFace5[numSquares][numSquares] = { // arrTrans1[i][j] is a translation matrix. Inv is arrTrans1[j][i].
  {
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  //
  ,{
    0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  //
  ,{
    0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0       }
  //
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0       }
  //
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0       }
  //
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1       }
};

int arrTransFace6[numSquares][numSquares] = { // arrTrans1[i][j] is a translation matrix. Inv is arrTrans1[j][i].
  {
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  //
  ,{
    0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  //
  ,{
    0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0       }
  //
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0       }
  //
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0       }
  //
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0       }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1       }
};

// Declare pins for pushbutton controls.
int rev = 23;
int face1 = 25;
int face2 = 27;
int face3 = 29;
int face4 = 31;
int face5 = 33;
int face6 = 35;

// Initialize button states.
int revState = 0;
int face1State = 0;
int face2State = 0;
int face3State = 0;
int face4State = 0;
int face5State = 0;
int face6State = 0;

int rotationExecuted = 0; // Variable for tracking if a rotation has been triggered during a given loop cycle.



void setup() {
  Serial.begin(9600);
  strip.begin();
  delay(5000);
  Serial.println("Working");
  // Set pushbutton controls as INPUTs.
  pinMode(rev,INPUT); // Button to reverse "rotation direction".
  pinMode(face1,INPUT); // Rotate face 1.
  pinMode(face2,INPUT); // Rotate face 2.
  pinMode(face3,INPUT); // Rotate face 3.
  pinMode(face4,INPUT); // Rotate face 4.
  pinMode(face5,INPUT); // Rotate face 5.
  pinMode(face6,INPUT); // Rotate face 6.
}

void loop() {

  revState = digitalRead(rev);
  face1State = digitalRead(face1);
  face2State = digitalRead(face2);
  face3State = digitalRead(face3);
  face4State = digitalRead(face4);
  face5State = digitalRead(face5);
  face6State = digitalRead(face6);

  if ( face1State == HIGH) {
    rotationExecuted = 1;
    if (revState == LOW) {
      for (int i = 0; i < numSquares; i++) {
        for (int j = 0; j < numSquares; j++) {
          arrayTemp[i] += arrTransFace1[j][i] * arrayShow[j];
        }     
      }
    }
    else {
      for (int i = 0; i < numSquares; i++) {
        for (int j = 0; j < numSquares; j++) {
          arrayTemp[i] += arrTransFace1[i][j] * arrayShow[j];
        }      
      } 
    }
  }
  if ( face2State == HIGH) {
    rotationExecuted = 1;
    if (revState == LOW) {
      for (int i = 0; i < numSquares; i++) {
        for (int j = 0; j < numSquares; j++) {
          arrayTemp[i] += arrTransFace2[j][i] * arrayShow[j];
        }     
      }
    }
    else {
      for (int i = 0; i < numSquares; i++) {
        for (int j = 0; j < numSquares; j++) {
          arrayTemp[i] += arrTransFace2[i][j] * arrayShow[j];
        }      
      } 
    }
  }
  if ( face3State == HIGH) {
    rotationExecuted = 1;
    if (revState == LOW) {
      for (int i = 0; i < numSquares; i++) {
        for (int j = 0; j < numSquares; j++) {
          arrayTemp[i] += arrTransFace3[j][i] * arrayShow[j];
        }     
      }
    }
    else {
      for (int i = 0; i < numSquares; i++) {
        for (int j = 0; j < numSquares; j++) {
          arrayTemp[i] += arrTransFace3[i][j] * arrayShow[j];
        }      
      } 
    }
  }
  if ( face4State == HIGH) {
    rotationExecuted = 1;
    if (revState == LOW) {
      for (int i = 0; i < numSquares; i++) {
        for (int j = 0; j < numSquares; j++) {
          arrayTemp[i] += arrTransFace4[j][i] * arrayShow[j];
        }     
      }
    }
    else {
      for (int i = 0; i < numSquares; i++) {
        for (int j = 0; j < numSquares; j++) {
          arrayTemp[i] += arrTransFace4[i][j] * arrayShow[j];
        }      
      } 
    }
  }
  if ( face5State == HIGH) {
    rotationExecuted = 1;
    if (revState == LOW) {
      for (int i = 0; i < numSquares; i++) {
        for (int j = 0; j < numSquares; j++) {
          arrayTemp[i] += arrTransFace5[j][i] * arrayShow[j];
        }     
      }
    }
    else {
      for (int i = 0; i < numSquares; i++) {
        for (int j = 0; j < numSquares; j++) {
          arrayTemp[i] += arrTransFace5[i][j] * arrayShow[j];
        }      
      } 
    }
  }
  if ( face6State == HIGH) {
    rotationExecuted = 1;
    if (revState == LOW) {
      for (int i = 0; i < numSquares; i++) {
        for (int j = 0; j < numSquares; j++) {
          arrayTemp[i] += arrTransFace6[j][i] * arrayShow[j];
        }     
      }
    }
    else {
      for (int i = 0; i < numSquares; i++) {
        for (int j = 0; j < numSquares; j++) {
          arrayTemp[i] += arrTransFace6[i][j] * arrayShow[j];
        }      
      } 
    }
  } 

  // Placeholder example rotation. Every 5s, rotates one of the cube's planes.
  // Retained for testing code without controllers.

  /*  if( (millis() % 5000) == 0) {
   for (int i = 0; i < numSquares; i++) {
   for (int j = 0; j < numSquares; j++) {
   arrayTemp[i] += arrTrans3[j][i] * arrayShow[j];
   }
   }
   */

  if (rotationExecuted == 1) {
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
    delay(500);
    rotationExecuted = 0;
  }
  // Serial.println(millis());

}



