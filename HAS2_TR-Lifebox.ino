 /*
 *
 * @file TR_Lifebox.ino
 * @author 홍진우 (kevinlike@naver.com)
 * @brief
 * @version 1.0
 * @date 2023-05-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "TR_Lifebox.h"

void setup(){
  Serial.begin(115200);
  has2_mqtt.Setup(callback);         // tp-link 접속; ALL, myMAC 구독
  // has2_mqtt.AddSubscirbe("EI2");
  TimerInit();
  RfidInit();
  NeopixelInit();
  NextionInit();
}

void loop(){
  game_ptr();
  BlinkTimer.run();
}