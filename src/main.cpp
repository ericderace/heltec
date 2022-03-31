#include <Arduino.h>
#include <SPI.h>
#include <U8x8lib.h>
#include "BluetoothSerial.h"

#define PWMPIN 33
#define PWMFREQ 5000
#define PWMCHANNEL 0
#define PWMRES 8

// Structs, constants and variables
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(15, 4, 16);
BluetoothSerial SerialBT;

// Function prototypes

void setup_oled();
void setup_pwm();

// Functions

// sets up PWM
void setup_pwm()
{
  ledcSetup(PWMCHANNEL, PWMFREQ, PWMRES);
  ledcAttachPin(PWMPIN, PWMCHANNEL);
}

// Set up OLED display
void setup_oled()
{
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
}

// Arduino default functions

void setup(void)
{
  Serial.begin(115200);
  setup_oled();
  if (SerialBT.begin("ESP32test")) Serial.println("Bluetooth started."); // Bluetooth device name
}

void loop(void)
{
  static unsigned long timestamp = 0;
  unsigned long since = (millis() - timestamp);

  float fps = 1.0 / since * 1000.0;
  String fps_str = "FPS: " + String(fps);

  if (Serial.available())
  {
    //SerialBT.write(Serial.read());
  }
  if (SerialBT.available())
  {
    Serial.write(SerialBT.read());
    //u8x8.drawString(0, 1, String(SerialBT.read()).c_str());
  }

  //static uint8_t duty = 255;
  //ledcWrite(PWMCHANNEL, duty);

  u8x8.drawString(0, 2, fps_str.c_str());
  u8x8.drawString(0, 3, String(millis()).c_str());
  timestamp = millis();
}
