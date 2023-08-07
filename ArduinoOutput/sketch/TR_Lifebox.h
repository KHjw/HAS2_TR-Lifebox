#line 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\롤백코드\\생장 롤백\\HAS2_TR-Lifebox\\TR_Lifebox.h"
#ifndef _DONE_EMLOCK_CODE_
#define _DONE_EMLOCK_CODE_

#include "Library_and_pin.h"

//****************************************MQTT SETUP****************************************
HAS2_MQTT has2_mqtt;

//****************************************Game SETUP****************************************
// game_ptr
void Game_Void();
void Game_Manual();
void Game_Setting();
void Game_Ready();
void Game_Selected();
void Game_Login();
void Game_Used();
void Game_ptrPrint(String print);
// rfid_ptr
void Mode_Mannual();

void (*game_ptr)() = Game_Manual;
void (*rfid_ptr)() = Mode_Mannual;

String Language = "Kor";
String game_ptr_state = "";
bool machine_used = false;

//****************************************SimpleTimer SETUP****************************************
SimpleTimer BlinkTimer;

void TimerInit();
void BlinkTimerStart(int Neo, int NeoColor);
void BlinkTimerFunc();
int blinkNeo = 0;
int blinkColor = 0;
bool blinkOn = false;

int blinkTimerId;

unsigned long blinkTime = 1300;   // 1sec

//****************************************Neopixel SETUP****************************************
void NeopixelInit();
void AllNeoColor(int color_code);
void NeoBlink(int neo_code, int color_code, int blink_num, int blink_time);

const int NeoNum = 3;
const int NumPixels[3] = {28,24,24};

enum {TOP = 0, MID, BOT, ALLNEO};
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
Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

bool rfid_init_complete;
void RfidInit();
void RfidLoop();
void CheckingPlayers(uint8_t rfidData[32]);

//****************************************Nextion SETUP****************************************
HardwareSerial nexHwSerial(2);

void NextionInit();

#endif