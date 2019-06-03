
/*----------------------------------------------------------------------------------------------------
  Project Name : Solar Powered WiFi Weather Station V2.1
  Authors: Keith Hungerford and Debasish Dutta / Modifications by 3KU_Delta (MarcS)
  Website : www.opengreenenergy.com
  Main microcontroller (ESP8266) and BME280 both sleep between measurements
  BME280 is used in single shot mode ("forced mode")

  Inspiration and code fragments of Dewpoint and Heatindex calculations are taken from:  
  https://arduinotronics.blogspot.com/2013/12/temp-humidity-w-dew-point-calcualtions.html

  Needed libraries:
  <Adafruit_Sensor.h>  --> Adafruit unified sensor
  <Adafruit_BME280.h>  --> Adafruit BME280 sensor
  <BlynkSimpleEsp8266.h> --> Blynk

  This is a library for the BME280 humidity, temperature & pressure sensor
  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2650
  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface. The device's I2C address is either 0x76 or 0x77.
  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!
  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
  
  Hardware Settings Mac: 
  LOLIN(WEMOS) D1 mini Pro, 80 MHz, Flash, 16M (15M SPIFFS), v2 Lower Memory, Disable, None, Only Sketch, 921600 on /dev/cu.SLAB_USBtoUART

  Last updated on 07/04/2019

////  Features :  //////////////////////////////////////////////////////////////////////////////////////////////////////
                                                                                                                         
// 1. Connect to Wi-Fi, and upload the data to either Blynk App and/or Thingspeak

// 2. Monitoring Weather parameters like Temperature, Pressure abs, Pressure MSL and Humidity.

// 3. Extra Ports to add more Weather Sensors like UV Index, Light and Rain Guage etc.

// 4. Remote Battery Status Monitoring

// 5. Using Sleep mode to reduce the energy consumed                                        

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/***************************************************
 * VERY IMPORTANT:                                 *
 *                                                 *
 * Enter your personal settings in Settings.h !    *
 *                                                 *
 **************************************************/

#include "Settings.h"

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
//#include <ESP8266WiFi.h>            // do we need this? Not when using Blynk
#include <BlynkSimpleEsp8266.h>

Adafruit_BME280 bme; // I2C

float SLpressure_hPa;  //needed for rel pressure calculation
float HeatIndex;       //Heat Index in °C

void setup() {
  
  Serial.begin(115200);

  Serial.println("\nStart of solarWiFiWeatherStation");

  // **************Choose Application**********************************
  
  if (App1 == "BLYNK")  // for posting datas to Blynk App
  { 
    Blynk.begin(auth, ssid, pass);
  } 
  if (App2 == "Thingspeak")  // for posting datas to Thingspeak website
  {
     WiFi.begin(ssid, pass);
     
     while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");  
  } 
  else 
  {
    WiFi.begin(ssid, pass);
    Serial.print(App1);
    Serial.println(" is not a valid application");
  }
  
  //*******************************************************************

  bool bme_status;
  bme_status = bme.begin(0x76);  //address either 0x76 or 0x77
  if (!bme_status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
  }

  Serial.println("forced mode, 1x temperature / 1x humidity / 1x pressure oversampling,");
  Serial.println("filter off");

  bme.setSampling(Adafruit_BME280::MODE_FORCED,
                Adafruit_BME280::SAMPLING_X1, // temperature
                Adafruit_BME280::SAMPLING_X1, // pressure
                Adafruit_BME280::SAMPLING_X1, // humidity
                Adafruit_BME280::FILTER_OFF   );

  //delayTime = 60000; // suggested rate is 1/60Hz (1m) -->in milliseconds
 
  measurementEvent();

  goToSleep();
} // end of void setup()


void loop() {
} // end of void loop()

void measurementEvent() { 
    
  //*********** Measures absolute Pressure, Temperature, Humidity, Voltage, calculate relative pressure, Dewpoint, Heat Index
  // then reports all of these to Blynk and/or Thingspeak
  
  bme.takeForcedMeasurement();

  // Get temperature
  float temperature = bme.readTemperature();
  temperature = temperature + TEMP_CORR;
  // print on serial monitor
  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print("°C; ");
 
  // Get humidity
  float humi = bme.readHumidity();
  // print on serial monitor
  Serial.print("Humidity: ");
  Serial.print(humi);
  Serial.print("%; ");

  // Get pressure
  float pres = bme.readPressure() / 100.0F;
  // print on serial monitor
  Serial.print("Pressure: ");
  Serial.print(pres);
  Serial.print("hPa; ");

  // Calculate and print relative pressure (QNH)
  SLpressure_hPa = (((pres * 100.0)/pow((1-((float)(ELEVATION))/44330), 5.255))/100.0);
  int hPa_rounded=(int)(SLpressure_hPa+.5);
  // print on serial monitor
  Serial.print("Pressure rel: ");
  Serial.print(hPa_rounded);
  Serial.print("hPa; ");

  // Calculate dewpoint
  double a = 17.271;
  double b = 237.7;
  double tempcalc = (a * temperature) / (b + temperature) + log(humi*0.01);
  double DewpointTemperature = (b * tempcalc) / (a - tempcalc);
  Serial.print("Dewpoint: ");
  Serial.print(DewpointTemperature);
  Serial.println("°C; ");

  // Calculate HI (heatindex in °C) --> HI starts working above 26,7 °C
  if (temperature > 26.7) {
  double c1 = -8.784, c2 = 1.611, c3 = 2.338, c4 = -0.146, c5= -1.230e-2, c6=-1.642e-2, c7=2.211e-3, c8=7.254e-4, c9=-2.582e-6  ;
  double T = temperature;
  double R = humi;
  
  double A = (( c5 * T) + c2) * T + c1;
  double B = ((c7 * T) + c4) * T + c3;
  double C = ((c9 * T) + c8) * T + c6;
  HeatIndex = (C * R + B) * R + A; 
  } 
  else {
    HeatIndex = temperature;
    Serial.println("Not warm enough (less than 26.7 °C) for Heatindex");
  }
  Serial.print("HeatIndex: ");
  Serial.print(HeatIndex);
  Serial.print("°C; ");
  
  //******Battery Voltage Monitoring*********************************************
  
  // Voltage divider R1 = 220k+100k+220k =540k and R2=100k
  float calib_factor = 5.28; // change this value to calibrate the battery voltage
  unsigned long raw = analogRead(A0);
  float volt= raw * calib_factor/1024; 
  
  Serial.print( "Voltage = ");
  Serial.print(volt, 2); // print with 2 decimal places
  Serial.println (" V");
  
 //*******************************************************************************
  // code block for uploading data to BLYNK App
  
  if (App1 == "BLYNK") {                    // choose application
    Blynk.virtualWrite(0, temperature);         // virtual pin 0
    Blynk.virtualWrite(1, humi);                // virtual pin 1
    Blynk.virtualWrite(2, pres);                // virtual pin 2
    Blynk.virtualWrite(3, hPa_rounded);         // virtual pin 3
    Blynk.virtualWrite(4, volt);                // virtual pin 4
    Blynk.virtualWrite(5, DewpointTemperature); // virtual pin 5
    Blynk.virtualWrite(6, HeatIndex);          // virtual pin 6
    Serial.println("Data written to Blink ...");
  } 
 //*******************************************************************************
 // code block for uploading data to Thingspeak website
 
  if (App2 == "Thingspeak") {
  // Send data to ThingSpeak 
  WiFiClient client;  
  if (client.connect(server,80)) {
  Serial.println("Connect to ThingSpeak - OK"); 

  String postStr = "";
  postStr+="GET /update?api_key=";
  postStr+=api_key;   
  postStr+="&field1=";
  postStr+=String(hPa_rounded);
  postStr+="&field2=";
  postStr+=String(temperature);
  postStr+="&field3=";
  postStr+=String(humi);
  postStr+="&field4=";
  postStr+=String(volt);
  postStr+="&field5=";
  postStr+=String(pres);  
  postStr+="&field6=";
  postStr+=String(DewpointTemperature);  
  postStr+="&field7=";
  postStr+=String(HeatIndex);
  postStr+=" HTTP/1.1\r\nHost: a.c.d\r\nConnection: close\r\n\r\n";
  postStr+="";
  client.print(postStr);
  Serial.println("Data written to Thingspeak ...");
 //*******************************************************************************

  }
   while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
  }
} // end of void measurementEvent()


void goToSleep() {
  Serial.print ("Going to sleep now for ");
  Serial.print (sleepTimeMin);
  Serial.print (" Minute(s).");
  
  ESP.deepSleep(sleepTimeMin * 60 * 1000000); // convert to microseconds
} // end of void goToSleep()
