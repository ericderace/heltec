#include <Arduino.h>
#include <SPI.h>
#include <U8x8lib.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <secrets.h>

// Structs, constants and variables
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(15, 4, 16);
AsyncWebServer server(80);
const char *ssid = WIFI_SSID;   // from secrets.h
const char *password = WIFI_PW; // from secrets.h

// Function prototypes

void setup_oled();
String IpAddress2String(IPAddress &);
void setup_wifi();
void setup_webserver();

// Functions

// Converts IPAddress class to String for printing on the screen
String IpAddress2String(const IPAddress &ipAddress)
{
  return String(ipAddress[0]) + String(".") +
         String(ipAddress[1]) + String(".") +
         String(ipAddress[2]) + String(".") +
         String(ipAddress[3]);
}

// Set up OLED display
void setup_oled()
{
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
}

// Set up wifi connection
void setup_wifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("WiFi Auto-reconnect: ");
  Serial.println(WiFi.getAutoReconnect());
  // Wait for connection - bluck further execution until connected to wifi.
  while (WiFi.status() != WL_CONNECTED)
  {
    static unsigned long int timestamp = 0;
    if (millis() - timestamp > 500)
    {
      Serial.print(".");
      timestamp = millis();
    }
    
  }

  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// Set up web server
void setup_webserver()
{
  // Web server request handlers
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", "Hi! I am ESP32."); });

  AsyncElegantOTA.begin(&server); // Start ElegantOTA

  // Start web server
  server.begin();
  Serial.println("HTTP server started");
}

// Arduino default functions

void setup(void)
{
  Serial.begin(115200);
  setup_oled();
  setup_wifi();
}

void loop(void)
{

  static unsigned long timestamp = 0;
  unsigned long since = (millis() - timestamp);

  float fps = 1.0 / since * 1000.0;

  String fps_str = "FPS: " + String(fps);

  u8x8.drawString(0, 0, String("WiFi RSSI: " + String(WiFi.RSSI())).c_str());
  u8x8.drawString(0, 1, IpAddress2String(WiFi.localIP()).c_str());
  u8x8.drawString(0, 2, fps_str.c_str());
  u8x8.drawString(0, 3, String(millis()).c_str());

  timestamp = millis();
}
