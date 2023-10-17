#ifndef _DONE_EMLOCK_CODE_
#define _DONE_EMLOCK_CODE_

#include "Library_and_pin.h"

//****************************************Device SETUP****************************************
String Language = "Kor";
String myDN = "";
String device_ptr_state = "";
String current_scenario = "";
bool IsMachineUsed = false;
bool IsTakingChip = false;
bool IsScenarioMode = false;
bool IsDsSkip = false;

//****************************************MQTT SETUP****************************************
HAS2_MQTT has2_mqtt;
void callback(char* topic, byte* payload, unsigned int length);
void Mqtt_myDN(String input_data);
void Mqtt_updateDS(String myDS);
void Mqtt_updateSCN(String mySCN);
void SituationSend();

// device_ptr
void Device_Manual();
void Device_MiniGame();
void Device_Setting();
void Device_Ready();
void Device_Selected();
void Device_Stelth();
void Device_GhostLogin();
void Device_PlayerLogin();
void Device_Used();
void Device_ptrPrint(String print);

void VoidFunc();    // 포인터 초기화용 빈 함수
void (*device_ptr)() = Device_MiniGame;

//****************************************RFID SETUP****************************************
Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

void RfidInit();
void RfidLoop(String mode);

String tagUser = "";

// rfid_func
void Rfid_Manual();
void Rfid_Ghost();
void Rfid_Player();
void Rfid_Used();

//****************************************SimpleTimer SETUP****************************************
SimpleTimer BlinkTimer;
SimpleTimer TakechipTimer;
int BlinkTimerId;
int TakechipTimerId;

void TimerInit();

// blink timer
unsigned long blinkTime = 1000;   // 1sec
void BlinkTimerStart(int Neo, int NeoColor);
void BlinkTimerFunc();
int blinkNeo = 0;
int blinkColor = 0;
bool blinkOn = false;
// takechip timer
unsigned long TakechipTime = 1000;   // 1sec
void TakechipTimerFunc();
int takechipCNT = 0;

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

//****************************************Nextion SETUP****************************************
HardwareSerial nexHwSerial(2);

void NextionInit();
void SendCmd(String command);

#endif