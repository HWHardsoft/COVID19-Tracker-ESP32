# COVID19-Tracker-ESP32
This little tracker will help you to be up to date about the coronvavirus outbreak and the situation in your country. The display shows alternating the current data of different countries of your choice.

The data is collected by the website www.worldometers.info/coronavirus/

![My image](https://hackster.imgix.net/uploads/attachments/1089824/_tH0IrpXr0o.blob?auto=compress%2Cformat&w=900&h=675&fit=min)

## Hardware 

I've used our AZ-Touch kit for ESP32 as hardware plattform. This kit comes with a 2.4 inch tft touchscreen, which will be used for the data output.

![My image](https://hackster.imgix.net/uploads/attachments/1089826/mkr_extended_kit_8R81xIIxzy.jpg?auto=compress%2Cformat&w=680&h=510&fit=max)

You can find all information about the hardware here:
https://www.hwhardsoft.de/english/projects/arduitouch-esp/


## Libraries

Install the following libraries through Arduino Library Manager

Adafruit GFX Library https://github.com/adafruit/Adafruit-GFX-Library/archive/master.zip 

Adafruit ILI9341 Library https://github.com/adafruit/Adafruit_ILI9341 

You can also download the library also directly as ZIP file and uncompress the folder under yourarduinosketchfolder/libraries/   

After installing the libraries, restart the Arduino IDE. 

## WiFi settings

Enter your WiFi SSID & password in the fields in the WiFi section: 

#define WIFI_SSID "xxxxxx"    // Enter your SSID here 

#define WIFI_PASS "xxxxx"    // Enter your WiFi password here 


## Country settings

You can change/add/delete the countries in the main loop of the program according your interests.

![My image](https://hackster.imgix.net/uploads/attachments/1089828/grafik_eRLuAc5tGD.png?auto=compress%2Cformat&w=680&h=510&fit=max)



# License

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.
