/*
  This sketch shows how to work with the ESP32-C6 Waveshare development board.
  Attached is an 1.47 inch TFT display that runs a ST7789 driver.
  The display has a size of 172x320 pixels.
  The library for the display is TFT_eSPI in a modified version, because the original
  version (2.5.43) can't run on ESP32 SDK 3.x so far and this SDK version is needed to
  select ESP32-C6 boards.

  Purpose of the sketch:
  - print data on the TFT display in different colors and sizes
  - get system informations about the ESP32 chip on the board
  - use the onboard RGB led

  This is tested with ESP32 Board version 3.2.0 with SDK 5.4.1 on Arduino IDE 2.3.6
  TFT_eSPI version: 2.5.43
  FastLED version:  3.9.15
*/

/*
Version Management
23.04.2025 V01 Initial programming
*/

/*
TFT 172 x 320 pixels 1.47 inch ST7789 display wiring to an ESP32-C6 Waveshare
This is the factory setup that cannot be changed

TFT   ESP32-C6 Waveshare
GND   GND
VDD   3.3V 
SCL   7
SDA   6 (= "MOSI")
RST  21
DC   15
CS   14
BLK  22 *1)

Note *1) If you don't need a dimming you can connect BLK with 3.3V
Note *2) The display does not have a MISO ("output") terminal, so it is not wired
*/

// -------------------------------------------------------------------------------
// Sketch and Board information
const char *PROGRAM_VERSION = "ESP32-C6 Waveshare ST7789 Starter V01";
const char *PROGRAM_VERSION_SHORT = "   ST7789 Starter V01";
const char *DEVICE_NAME = "  ESP32-C6 Waveshare LCD    1.47 inch 172 x 320 px";
const char *DIVIDER = "-------------------------";

// -------------------------------------------------------------------------------
// TFT Display
#include "SPI.h"
#include "TFT_eSPI.h"

TFT_eSPI tft = TFT_eSPI();

#define TFT_BL_PIN 22              // backlight brightness control, needs to be a PWM pin
#define TFT_BRIGHTNESS_PERCENT 90  // avoids overheating of the device
#define TFT_TEXT_SIZE 2
#define TFT_WIDTH 240
#define TFT_LED_TEXT_START 140
#define TFT_AUTHOR_TEXT_START 140
#define TFT_ORIENTATION_PORTRAIT 1 // The USB Connector is at the right side

// -------------------------------------------------------------------------------
// RGB LED
#include <FastLED.h>  // https://github.com/FastLED/FastLED
#define RGB_LED_PIN 8
#define RGB_NUM_LEDS 1
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define RGB_LED_BRIGHTNESS_PERCENT 30
CRGB leds[RGB_NUM_LEDS];
#define FRAMES_PER_SECOND 120  // rainbow effect
uint8_t gHue = 0;              // rotating "base color" used by many of the patterns

// -------------------------------------------------------------------------------
// Onboard LED
//#define ONBOARD_LED_PIN 15  // HIGH = LED on, LOW = LED off

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println(PROGRAM_VERSION);

  // init the display
  tft.begin();

  // set the brightness to 90% to avoid heating of the device
  pinMode(TFT_BL_PIN, OUTPUT);
  analogWrite(TFT_BL_PIN, 255 * TFT_BRIGHTNESS_PERCENT / 100);
  delay(10);

  // setup the RGB LED
  CFastLED::addLeds<LED_TYPE, RGB_LED_PIN, COLOR_ORDER>(leds, RGB_NUM_LEDS);
  FastLED.setBrightness(RGB_LED_BRIGHTNESS_PERCENT);

  // setup the onboard LED
  //pinMode(ONBOARD_LED_PIN, OUTPUT);
  //digitalWrite(ONBOARD_LED_PIN, HIGH);  // LED on

  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(TFT_TEXT_SIZE);
  tft.setRotation(TFT_ORIENTATION_PORTRAIT);  // portrait

  tft.setTextSize(TFT_TEXT_SIZE);
  tft.setTextColor(TFT_GREEN);
  tft.setCursor(0, 0);
  tft.println(DEVICE_NAME);
  tft.setTextColor(TFT_RED);
  tft.println(PROGRAM_VERSION_SHORT);

  tft.setTextColor(TFT_WHITE);
  tft.println(DIVIDER);

  tft.setTextColor(TFT_BLUE);
  tft.print(F("Processor:"));
  tft.setTextColor(TFT_YELLOW);
  tft.print(ESP.getChipModel());

  tft.setTextColor(TFT_BLUE);
  tft.print(F(" Rev:"));
  tft.setTextColor(TFT_YELLOW);
  tft.println(ESP.getChipRevision());

  tft.setTextColor(TFT_BLUE);
  tft.print(F("Cores:"));
  tft.setTextColor(TFT_YELLOW);
  tft.print (ESP.getChipCores());

  tft.setTextColor(TFT_BLUE);
  tft.print(F(" CPU Freq: "));
  tft.setTextColor(TFT_YELLOW);
  tft.print(ESP.getCpuFreqMHz());
  tft.println(F(" MHz"));

  tft.setTextColor(TFT_BLUE);
  tft.print(F("Flash:"));
  tft.setTextColor(TFT_YELLOW);
  tft.print(ESP.getFlashChipSize() / (1024.0 * 1024));
  tft.print(F("mb"));

  tft.setTextColor(TFT_BLUE);
  tft.print(F(" PSRAM:"));
  tft.setTextColor(TFT_YELLOW);
  tft.print(ESP.getPsramSize() / (1024.0 * 1024));
  tft.println(F("mb"));

  tft.setTextColor(TFT_BLUE);
  tft.print(F("HEAP:"));
  tft.setTextColor(TFT_YELLOW);
  tft.print(ESP.getHeapSize() / 1024.0);
  tft.print(F(" kb"));

  tft.setTextColor(TFT_BLUE);
  tft.print(F(" SDK:"));
  tft.setTextColor(TFT_YELLOW);
  String shortSdkVersion = ESP.getSdkVersion();
  tft.println(shortSdkVersion.substring(0, 6));

  tft.setTextColor(TFT_WHITE);
  tft.println(DIVIDER);

  tft.setTextSize(2);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.drawString(F(" RED LED    "), 0, TFT_LED_TEXT_START, 1);
  leds[0] = CRGB::Red;
  FastLED.delay(500);
  delay(1000);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString(F(" GREEN LED  "), 0, TFT_LED_TEXT_START, 1);
  leds[0] = CRGB::Green;
  FastLED.delay(500);
  delay(1000);
  tft.setTextColor(TFT_BLUE, TFT_BLACK);
  tft.drawString(F(" BLUE  LED "), 0, TFT_LED_TEXT_START, 1);
  leds[0] = CRGB::Blue;
  FastLED.delay(500);
  delay(1000);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(F(" WHITE LED "), 0, TFT_LED_TEXT_START, 1);
  leds[0] = CRGB::White;
  FastLED.delay(500);
  delay(1000);
  tft.setTextColor(TFT_MAGENTA, TFT_BLACK);
  tft.drawString(F(" RAINBOW LED"), 0, TFT_LED_TEXT_START, 1);
  leds[0] = CRGB::White;
  fill_rainbow(&(leds[0]), RGB_NUM_LEDS, gHue);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(TFT_TEXT_SIZE);
  tft.drawString(F("AndroidCrypto"), 150, TFT_AUTHOR_TEXT_START, 1);
}

void loop() {
  // Rainbow effect
  // send the 'leds' array out to the actual LED strip
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);

  // do some periodic updates
  EVERY_N_MILLISECONDS(20) {
    gHue++;
  }  // slowly cycle the "base color" through the rainbow
  fill_rainbow(&(leds[0]), RGB_NUM_LEDS, gHue);
}
