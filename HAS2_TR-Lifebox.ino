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

void setup() {
  Serial.begin(115200);
  // has2wifi.Setup("city");
  // has2wifi.Setup("badland");
  // has2wifi.Setup();
  // has2wifi.Setup("KT_GiGA_6C64","ed46zx1198");
  TimerInit();
  NeopixelInit();
  NextionInit();
}

void loop() {
  game_ptr();
  BlinkTimer.run();
}