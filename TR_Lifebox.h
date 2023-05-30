#ifndef _DONE_EMLOCK_CODE_
#define _DONE_EMLOCK_CODE_

#include "Library_and_pin.h"

//****************************************Game SETUP****************************************

//****************************************Neopixel SETUP****************************************
void NeopixelInit();
void AllNeoColor(int color_code);
void NeoBlink(int neo_code, int color_code, int blink_num, int blink_time);

const int NeoNum = 3;
const int NumPixels[3] = {28,24,24};

enum {TOP = 0, MID, BOT};
Adafruit_NeoPixel pixels[3] = {Adafruit_NeoPixel(NumPixels[TOP], NEOPIXEL_TOP_PIN, NEO_GRB + NEO_KHZ800),
                               Adafruit_NeoPixel(NumPixels[MID], NEOPIXEL_MID_PIN, NEO_GRB + NEO_KHZ800),
                               Adafruit_NeoPixel(NumPixels[BOT], NEOPIXEL_BOT_PIN, NEO_GRB + NEO_KHZ800)};

enum {WHITE = 0, RED, YELLOW, GREEN, BLUE, PURPLE, BLACK, BLUE0, BLUE1, BLUE2, BLUE3};     // Neopixel 색상정보
int color[11][3] = {  {20, 20, 20},   //WHITE
                      {40, 0, 0},     //RED
                      {40, 40, 0},    //YELLOW
                      {0, 40, 0},     //GREEN
                      {0, 0, 40},     //BLUE
                      {40, 0, 40},    //PURPLE
                      {0, 0, 0},      //BLACK
                      {0, 0, 20},     //ENCODERBLUE0
                      {0, 0, 40},     //ENCODERBLUE1
                      {0, 0, 60},     //ENCODERBLUE2
                      {0, 0, 80}};    //ENCODERBLUE3

//****************************************RFID SETUP****************************************
int rfid_num = 1;
Adafruit_PN532 nfc[1] = {Adafruit_PN532(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS)};

bool rfid_init_complete[2];
void RfidInit();
void RfidLoop(int pn532_code);
void RfidLoop_All();
void CheckingPlayers(uint8_t rfidData[32]);

void Quiz_Start();
void Quiz_Solved();
void Item_Took();

#endif