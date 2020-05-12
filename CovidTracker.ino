/*
 *  Application note: Covid 19 tracker for AZ-Touch and ESP32 DEV KIT 3
 *  Version 1.1
 *  Copyright (C) 2020  Hartmut Wendt  www.zihatec.de
 *  
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/ 

/*______Import Libraries_______*/
#include <Arduino.h> 
#include <SPI.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>  
#include <ArduinoHttpClient.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

/*______End of Libraries_______*/

/*__Pin definitions for the Arduino MKR__*/
#define TFT_CS   5
#define TFT_DC   4
#define TFT_MOSI 23
#define TFT_CLK  18
#define TFT_RST  22
#define TFT_MISO 19
#define TFT_LED  15  


#define HAVE_TOUCHPAD
#define TOUCH_CS 14
#define TOUCH_IRQ 2 
/*_______End of definitions______*/

/*____Calibrate Touchscreen_____*/
#define MINPRESSURE 10      // minimum required force for touch event
#define TS_MINX 370
#define TS_MINY 470
#define TS_MAXX 3700
#define TS_MAXY 3600
/*______End of Calibration______*/


/*____Wifi _____________________*/
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
#define WIFI_SSID "xxxxxx"       // Enter your SSID here
#define WIFI_PASS "xxxxx"    // Enter your WiFi password here

// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30*1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 2000; 
/*______End of Wifi______*/


int status = WL_IDLE_STATUS;
int infected=0;
int recovered=0;
int deaths=0;


WiFiClientSecure client;
HttpClient http(client,"www.worldometers.info", 443); 

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);


void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // init GPIOs
  pinMode(TFT_LED, OUTPUT); // define as output for backlight control

  // initialize the TFT
  Serial.println("Init TFT ...");
  tft.begin();          
  tft.setRotation(3);   // landscape mode  
  tft.fillScreen(ILI9341_BLACK);// clear screen 

  tft.setCursor(70,110);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print("Connecting...");
  digitalWrite(TFT_LED, LOW);    // LOW to turn backlight on; 


 // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip); 
 

}

void loop() {
 check_country("US");
 delay(2000);
 check_country("UK");
 delay(2000);
 check_country("Italy");
 delay(2000); 
 check_country("Germany");
 delay(2000); 
 check_country("Spain");
 delay(2000); 
 check_country("Russia");
 delay(2000); 
 check_country("Brazil");
 delay(2000); 
}


void draw_country_screen(String sCountry){
  tft.fillScreen(ILI9341_BLACK);// clear screen

  // headline
  tft.setCursor(10,10);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(3);
  tft.print(sCountry + ":");

  // infected
  tft.setCursor(5,70);
  tft.setTextColor(ILI9341_RED);
  tft.print("Infected:");
  tft.setCursor(190,70);
  tft.print(infected);

  // recovered
  tft.setCursor(5,130);
  tft.setTextColor(ILI9341_GREEN);
  tft.print("Recovered:");
  tft.setCursor(190,130);
  tft.print(recovered);

  // deaths
  tft.setCursor(5,190);
  tft.setTextColor(ILI9341_LIGHTGREY);
  tft.print("Deaths:");
  tft.setCursor(190,190);
  tft.print(deaths); 
      
}

void check_country(String sCountry) {
  int err =0;
  int readcounter = 0;
  int read_value_step = 0;
  String s1 = "";
  String s2 = "";
  
  err = http.get("/coronavirus/country/" + sCountry +"/");
  if (err == 0)
  {
    Serial.println("startedRequest ok");

    err = http.responseStatusCode();
    if (err >= 0)
    {
      Serial.print("Got status code: ");
      Serial.println(err);

      // Usually you'd check that the response code is 200 or a
      // similar "success" code (200-299) before carrying on,
      // but we'll print out whatever response we get

      // If you are interesting in the response headers, you
      // can read them here:
      //while(http.headerAvailable())
      //{
      //  String headerName = http.readHeaderName();
      //  String headerValue = http.readHeaderValue();
      //}

      Serial.print("Request data for ");
      Serial.println(sCountry);
    
      // Now we've got to the body, so we can print it out
      unsigned long timeoutStart = millis();
      char c;
      // Whilst we haven't timed out & haven't reached the end of the body
      while ( (http.connected() || http.available()) &&
             (!http.endOfBodyReached()) &&
             ((millis() - timeoutStart) < kNetworkTimeout) )
      {
          if (http.available())
          {
              c = http.read();
              s2 = s2 + c;
              if (readcounter < 300) {
                readcounter++;
              } else {
                readcounter = 0;
                String tempString = "";
                tempString.concat(s1);
                tempString.concat(s2);
                // check infected first 
                if (read_value_step == 0) {                               
                  int place = tempString.indexOf("Coronavirus Cases:");
                  if ((place != -1) && (place < 350)) { 
                    read_value_step = 1;
                    s2 = tempString.substring(place + 15);
                    tempString = s2.substring(s2.indexOf("#aaa") + 6);
                    s1 = tempString.substring(0, (tempString.indexOf("</")));
                    s1.remove(s1.indexOf(","),1);  
                    s1.remove(s1.indexOf(","),1); // for large 7 digit numbers
                    Serial.print("Coronavirus Cases: ");
                    Serial.println(s1);
                    infected = s1.toInt();
                  }
                  
                }
                // check deaths               
                if (read_value_step == 1) {
                  int place = tempString.indexOf("Deaths:");
                  if ((place != -1) && (place < 350)) { 
                    read_value_step = 2;
                    s2 = tempString.substring(place + 15);
                    tempString = s2.substring(s2.indexOf("<span>") + 6);
                    s1 = tempString.substring(0, (tempString.indexOf("</")));
                    s1.remove(s1.indexOf(","),1);  
                    s1.remove(s1.indexOf(","),1); // for large 7 digit numbers
                    Serial.print("Deaths: ");
                    Serial.println(s1);
                    deaths = s1.toInt();
                  }
                }                
                // check recovered               
                if (read_value_step == 2) {
                  int place = tempString.indexOf("Recovered:");
                  if ((place != -1) && (place < 350)) {                   
                    s2 = tempString.substring(place + 15);
                    tempString = s2.substring(s2.indexOf("<span>") + 6);
                    s1 = tempString.substring(0, (tempString.indexOf("</")));
                    s1.remove(s1.indexOf(","),1);  
                    s1.remove(s1.indexOf(","),1); // for large 7 digit numbers
                    Serial.print("Recovered: ");
                    Serial.println(s1);
                    recovered = s1.toInt();
                    draw_country_screen(sCountry);
                    http.stop();
                    return;
                  }
                }                
      
                s1 = s2;
                s2 = ""; 
              }              
              
              // We read something, reset the timeout counter
              timeoutStart = millis();
          }
          else
          {
              // We haven't got any data, so let's pause to allow some to
              // arrive
              delay(kNetworkDelay);
          }
      }
    }
    else
    {    
      Serial.print("Getting response failed: ");
      Serial.println(err);
    }
  }
  else
  {
    Serial.print("Connect failed: ");
    Serial.println(err);
  }
  http.stop();
  
}


void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
