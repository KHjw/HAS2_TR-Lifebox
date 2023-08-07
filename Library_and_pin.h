#ifndef _LIBRARY_AND_PIN_
#define _LIBRARY_AND_PIN_

#include <Arduino.h>

#include <Adafruit_PN532.h>
#include <Adafruit_NeoPixel.h>

#include <Nextion.h>
#include <HAS2_MQTT.h>
#include <SimpleTimer.h>

#define SERIAL2_RX_PIN    39
#define SERIAL2_TX_PIN    33

#define PN532_SCK         18
#define PN532_MISO        19
#define PN532_MOSI        23
#define PN532_SS          5 

#define NEOPIXEL_TOP_PIN  25
#define NEOPIXEL_MID_PIN  26
#define NEOPIXEL_BOT_PIN  27

#endif