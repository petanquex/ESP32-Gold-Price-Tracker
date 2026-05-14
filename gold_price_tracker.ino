#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "secret.h" 

// API & Timer
const char* api_url = "https://api.gold-api.com/price/XAU/USD";
unsigned long lastTime = 0;
unsigned long timerDelay = 60000; // 60 seconds

// --- OLED Setup ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SDA_PIN 8
#define SCL_PIN 9

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- Price Variables ---
double previousPrice = 0.0;
bool isFirstRun = true;

void setup() {
  Serial.begin(115200);

  // Initialize I2C pins
  Wire.begin(SDA_PIN, SCL_PIN);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Connecting WiFi...");
  display.display();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  WiFi.setTxPower(WIFI_POWER_8_5dBm);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println("WiFi Connected!");
  display.display();
  delay(2000);
}

void loop() {
  if ((millis() - lastTime) > timerDelay || lastTime == 0) {
    if (WiFi.status() == WL_CONNECTED) {
      
      WiFiClientSecure client;
      client.setInsecure(); 
      HTTPClient http;

      http.begin(client, api_url);
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0) {
        String payload = http.getString();
        
        JsonDocument doc; 
        DeserializationError error = deserializeJson(doc, payload);

        if (!error) {
          double currentPrice = doc["price"].as<double>();
          String goldPriceStr = String(currentPrice, 2); 
          
          // Calculate Percentage Change
          String percentChangeStr = "0.000%";
          
          if (!isFirstRun && previousPrice > 0.0) {
            double percentChange = ((currentPrice - previousPrice) / previousPrice) * 100.0;
            
            // Format the string and add a '+' sign if it's positive (negative is automatic)
            if (percentChange > 0) {
              percentChangeStr = "+" + String(percentChange, 3) + "%";
            } else {
              percentChangeStr = String(percentChange, 3) + "%"; 
            }
          }

          // Draw Minimal UI
          display.clearDisplay();
          
          // 1. Title (Top Center-ish)
          display.setTextSize(1);
          display.setCursor(22, 0);
          display.print("Gold (XAU/USD)");

          // 2. Big Current Price (Middle)
          display.setTextSize(2);
          display.setCursor(10, 22);
          display.print("$");
          display.print(goldPriceStr);

          // 3. Percentage Change (Bottom Center-ish)
          display.setTextSize(1);
          display.setCursor(40, 48);
          display.print(percentChangeStr);
          
          display.display();
          
          Serial.println("Updated Price: $" + goldPriceStr + " | Change: " + percentChangeStr);

          // Update tracking variables for the next loop
          previousPrice = currentPrice;
          isFirstRun = false;
          
        } else {
          Serial.println("JSON Parse Error");
        }
      } else {
        Serial.println("HTTP Error");
      }
      http.end(); 
    }
    lastTime = millis();
  }
}