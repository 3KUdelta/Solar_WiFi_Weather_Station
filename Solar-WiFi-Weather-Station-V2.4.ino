/*----------------------------------------------------------------------------------------------------
  Project Name : Solar Powered WiFi Weather Station V2.42
  Features: temperature, dewpoint, dewpoint spread, heat index, humidity, absolute pressure, relative pressure, battery status and
  the famous Zambretti Forecaster (multi lingual)
  Authors: Keith Hungerford, Debasish Dutta and Marc Stähli
  Website : www.opengreenenergy.com

  Main microcontroller (ESP8266) and BME280 both sleep between measurements
  BME280 is used in single shot mode ("forced mode")
  CODE: https://github.com/3KUdelta/Solar_WiFi_Weather_Station
  INSTRUCTIONS & HARDWARE: https://www.instructables.com/id/Solar-Powered-WiFi-Weather-Station-V20/
  3D FILES: https://www.thingiverse.com/thing:3551386

  CREDITS:
  Inspiration and code fragments of Dewpoint and Heatindex calculations are taken from:
  https://arduinotronics.blogspot.com/2013/12/temp-humidity-w-dew-point-calcualtions.html
  For Zambretti Ideas:
  http://drkfs.net/zambretti.htm or http://integritext.net/DrKFS/zambretti.htm
  https://raspberrypiandstuff.wordpress.com
  David Bird: https://github.com/G6EJD/ESP32_Weather_Forecaster_TN061

  CREDITS for Adafruit libraries:
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
  LOLIN(WEMOS) D1 mini Pro, 80 MHz, Flash, 16M (14M SPIFFS), v2 Lower Memory, Disable, None, Only Sketch, 921600 on /dev/cu.SLAB_USBtoUART
  major update on 15/05/2019
  -added Zambretti Forecster
  -added translation feature
  -added English language
  -added German language
  updated on 03/06/2019
  -added Dewpoint Spread
  -minor code corrections
  updated 28/06/19
  -added MQTT (publishing all data to MQTT)
  -added Italian and Polish tranlation (Chak10) and (TomaszDom)
  updated 27/11/19 to V2.32
  -added battery protection at 3.3V, sending "batt empty" message and go to hybernate mode
  updated 11/05/20 to v2.33
  -corrected bug in adjustments for summer/winter
  updated 27/05/20 to v2.34
  - added August-Roche-Magnus approximation to automatically adjust humidity with temperature corrections
  updated 18/01/21 to v2.35
  - added HDC1080 temp&humi sensor because the Bosch BME280 is not accurate enough for me on temp and humidity
  - added winter prediction with different translation table
  updated 11/01/22 to v2.36
  - added one-wire 18d20 temperature sensor (better temperature buffering in sunshine - more lazy) to P4 solder pull-up resistor 4.7 kOhm between signal and V+
  - removed HDC1080 again (unreliable humidity readings)
  - added PressureState to indicate current pressure situation (High presure, stormy low, etc)

  major update March 2022 to v2.4
  - Skipped Thingspeak
  - Changed translation.h structure for switching from summer to winter messages (rain vs. snow predictions)
  - Introduced login credentials for MQTT
  - Changed message type for MQTT to Json
  - Changed passowrd login to MQTT

  minor update June 2022 to v2.41
  - Adding time code in EPOCH format into MQTT message (time is UTC)
  - clean-ups

  updated 07/01/23
  - updated Blynk settings (if you were using Blynk legacy, please switch to new Blynk)
  - Important: New ArduinoJson version causes troubles with Blynk - works perfect with ArduinoJson Verson 6.20.1

  updated 03/10/23
  - MQTT, set to non-blocking in case of a broker error
  - switched to open broker HiveMQ

  ////  Features :  /////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // 1. Connect to Wi-Fi, and upload the data to either Blynk App and/or Thingspeak and to any MQTT broker
  // 2. Monitoring Weather parameters like Temperature, Pressure abs, Pressure MSL and Humidity.
  // 3. Extra Ports to add more Weather Sensors like UV Index, Light and Rain Guage etc.
  // 4. Remote Battery Status Monitoring
  // 5. Using Sleep mode to reduce the energy consumed
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  /***************************************************
   VERY IMPORTANT:
 *                                                 *
   Enter your personal settings in Settings.h !
 *                                                 *
 **************************************************/

#include "Settings24.h"
#include "Translation24.h"
#include <OneWire.h>               // for temperature sensor 18d20
#include <DallasTemperature.h>     // for temperature sensor 18d20
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>           // !!! ONLY WORKS currently with library version 6.20.1 !!!
#include <BlynkSimpleEsp8266.h>    //https://github.com/blynkkk/blynk-library
#include <WiFiUdp.h>
#include "FS.h"
#include <EasyNTPClient.h>         //https://github.com/aharshac/EasyNTPClient
#include <TimeLib.h>               //https://github.com/PaulStoffregen/Time.git
#include <PubSubClient.h>          // For MQTT (in this case publishing only)


#define ONE_WIRE_BUS 13            // Data wire 18d20 Sensor is plugged into port D7 @ ESP8266

Adafruit_BME280 bme;               // I2C
OneWire oneWire(ONE_WIRE_BUS);     // Setup a oneWire instance to communicate with any OneWire devices on D7
DallasTemperature s18d20(&oneWire); // Pass oneWire reference to Dallas Temperature Sensor 18d20
WiFiUDP udp;
EasyNTPClient ntpClient(udp, NTP_SERVER, 0);  // reading UTC

//varialbes of measured or calculated sensor data
float measured_temp_dal;
float measured_temp_bme;
float measured_temp;
float adjusted_temp;
float measured_humi;
float measured_humi_bme;
float adjusted_humi;
float measured_pres;
float SLpressure_hPa;               // needed for rel pressure calculation
float HeatIndex;                    // Heat Index in °C
float volt;
int batterypercentage;
int rel_pressure_rounded;
double DewpointTemperature;
float DewPointSpread;               // Difference between actual temperature and dewpoint

//variables for trend calculation
unsigned long current_timestamp;    // Actual timestamp read from NTPtime_t now;
unsigned long saved_timestamp;      // Timestamp stored in SPIFFS
float pressure_value[12];           // Array for the historical pressure values (6 hours, all 30 mins), where as pressure_value[0] is always the most recent value
float pressure_difference[12];      // Array to calculate trend with pressure differences

// variables for forcasting result
int accuracy;                       // Counter, if enough values for accurate forecasting
String ZambrettisWords;             // Final statement about weather forecast
char z_letter;
String trend_in_words;              // Trend in words
String forecast_in_words;           // Weather forecast in words
String pressure_in_words;           // Air pressure in words
String accuracy_in_words;           // Zambretti's prediction accuracy in words
String PressureState;               // current state of pressure (high, low, stormy low, etc)

void(* resetFunc) (void) = 0;       // declare reset function @ address 0

WiFiClient espClient;               // MQTT
PubSubClient client(espClient);     // MQTT

void setup() {
  Serial.begin(115200); while (!Serial); delay(200);
  Serial.println();
  Serial.print("Start of ");
  Serial.print(StationName);
  Serial.print(", Version ");
  Serial.println(Version);

  translate();                     // assign translation variables
  
  client.setBufferSize(512);       // Increasing PubSubClient

  //******Battery Voltage Monitoring (first thing to do: is battery still ok?)***********

  // Voltage divider R1 = 220k+100k+220k =540k and R2=100k
  float calib_factor = 5.1; // change this value to calibrate the battery voltage
  unsigned long raw = analogRead(A0);
  volt = raw * calib_factor / 1024;

  Serial.print("Voltage = ");
  Serial.print(volt, 2); // print with 2 decimal places
  Serial.println (" V");

  batterypercentage = (volt - 3.6) * 100 / 0.6;   // 3.6 V is the lower limint set to 0%, bandwith 0.6 V
  if (batterypercentage > 100) batterypercentage = 100;
  Serial.print("Battery charge: ");
  Serial.print(batterypercentage);
  Serial.println("%");

  // **************Application going online**********************************************

  WiFi.mode(WIFI_STA);
  WiFi.hostname(StationName); //This changes the hostname of the ESP8266 to display neatly on the network esp on router.
  WiFi.begin(ssid, pass);
  Serial.print("---> Connecting to WiFi ");
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    i++;
    if (i > 20) {
      Serial.println("Could not connect to WiFi!");
      Serial.println("Going to sleep for 10 minutes and try again.");
      if (volt > 3.6) {
        goToSleep(10);   // go to sleep and retry after 10 min
      }
      else {
        goToSleep(0);   // hybernate because batt empty - this is just to avoid that an endless
      }                 // try to get a WiFi signal will drain the battery empty
    }
    Serial.print(".");
  }
  Serial.println(" Wifi connected ok");

  if (App1 == "BLYNK") {        // for posting data to Blynk App
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  }
  if (MQTT) connect_to_MQTT();  // connecting to MQTT broker

  //*****************Checking if SPIFFS available********************************

  Serial.println("SPIFFS Initialization: (First time run can last up to 30 sec - be patient)");

  boolean mounted = SPIFFS.begin();               // load config if it exists. Otherwise use defaults.
  if (!mounted) {
    Serial.println("FS not formatted. Doing that now... (can last up to 30 sec).");
    SPIFFS.format();
    Serial.println("FS formatted...");
    SPIFFS.begin();
  }

  //******** GETTING THE TIME FROM NTP SERVER  ***********************************

  Serial.println("---> Now reading time from NTP Server");
  int ii = 0;
  while (!ntpClient.getUnixTime()) {
    delay(100);
    ii++;
    if (ii > 20) {
      Serial.println("Could not connect to NTP Server!");
      Serial.println("Doing a reset now and retry a connection from scratch.");
      resetFunc();
    }
    Serial.print(".");
  }
  current_timestamp = ntpClient.getUnixTime();      // get UNIX timestamp (seconds from 1.1.1970 on)

  Serial.print("Current UNIX Timestamp: ");
  Serial.println(current_timestamp);

  Serial.print("Time & Date: ");
  Serial.print(hour(current_timestamp));
  Serial.print(":");
  Serial.print(minute(current_timestamp));
  Serial.print(":");
  Serial.print(second(current_timestamp));
  Serial.print("; ");
  Serial.print(day(current_timestamp));
  Serial.print(".");
  Serial.print(month(current_timestamp));         // needed later: month as integer for Zambretti calcualtion
  Serial.print(".");
  Serial.print(year(current_timestamp));
  Serial.println("Local timezone does not matter - we just need always the same timezone --> using UTC");

  //******** GETTING RELATIVE PRESSURE DATA FROM SENSOR (BME680)  ********************

  bool bme_status = bme.begin(0x76);  //address either 0x76 or 0x77
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

  s18d20.begin();                 // starting 18d20
  s18d20.requestTemperatures();   // Send the command to get temperatures

  measurementEvent();            //calling function to get all data from the different sensors

  //*******************SPIFFS operations***************************************************************

  ReadFromSPIFFS();              //read stored values and update data if more recent data is available

  Serial.print("Timestamp difference: ");
  Serial.println(current_timestamp - saved_timestamp);

  if (current_timestamp - saved_timestamp > 21600) {   // last save older than 6 hours -> re-initialize values
    FirstTimeRun();
  }
  else if (current_timestamp - saved_timestamp > 1800) { // it is time for pressure update (1800 sec = 30 min)

    for (int i = 11; i >= 1; i = i - 1) {
      pressure_value[i] = pressure_value[i - 1];        // shifting values one to the right
    }

    pressure_value[0] = rel_pressure_rounded;             // updating with acutal rel pressure (newest value)

    if (accuracy < 12) {
      accuracy = accuracy + 1;                            // one value more -> accuracy rises (up to 12 = 100%)
    }
    WriteToSPIFFS(current_timestamp);                   // update timestamp on storage
  }
  else {
    WriteToSPIFFS(saved_timestamp);                     // do not update timestamp on storage
  }

  //**************************Calculate Zambretti Forecast*******************************************

  int accuracy_in_percent = accuracy * 94 / 12;        // 94% is the max predicion accuracy of Zambretti
  if ( volt > 3.7 ) {                       // check if batt is still ok
    ZambrettisWords = ZambrettiSays(char(ZambrettiLetter()));
    forecast_in_words = TEXT_ZAMBRETTI_FORECAST;
    pressure_in_words = TEXT_AIR_PRESSURE;
    accuracy_in_words = TEXT_ZAMBRETTI_ACCURACY;
  }
  else {
    ZambrettisWords = ZambrettiSays('0');   // send Message that battery is empty
  }

  Serial.println("********************************************************");
  Serial.print("Zambretti says: ");
  Serial.print(ZambrettisWords);
  Serial.print(", ");
  Serial.println(trend_in_words);
  Serial.print("Prediction accuracy: ");
  Serial.print(accuracy_in_percent);
  Serial.println("%");
  if (accuracy < 12) {
    Serial.println("Reason: Not enough weather data yet.");
    Serial.print("We need ");
    Serial.print((12 - accuracy) / 2);
    Serial.println(" hours more to get sufficient data.");
  }
  Serial.println("********************************************************");

  // ************ code block for uploading data to BLYNK App ***************

  if (App1 == "BLYNK") {
    Blynk.virtualWrite(V0, adjusted_temp);            // virtual pin 0
    Blynk.virtualWrite(V1, adjusted_humi);            // virtual pin 1
    Blynk.virtualWrite(V2, measured_pres);            // virtual pin 2
    Blynk.virtualWrite(V3, rel_pressure_rounded);     // virtual pin 3
    Blynk.virtualWrite(V4, volt);                     // virtual pin 4
    Blynk.virtualWrite(V5, DewpointTemperature);      // virtual pin 5
    Blynk.virtualWrite(V6, HeatIndex);                // virtual pin 6
    Blynk.virtualWrite(V7, ZambrettisWords);          // virtual pin 7
    Blynk.virtualWrite(V8, accuracy_in_percent);      // virtual pin 8
    Blynk.virtualWrite(V9, trend_in_words);           // virtual pin 9
    Blynk.virtualWrite(V10, DewPointSpread);          // virtual pin 10
    Blynk.virtualWrite(V11, PressureState);           // virtual pin 11
    Serial.println("Data written to Blynk ...");
  }

  // *********** code block for publishing all data to MQTT ****************************
  if (MQTT) {
    StaticJsonDocument<512> jsonDoc;
    JsonObject rootObj = jsonDoc.createNestedObject("root");

    // Write the the values. Here you can use any C++ type (and you can refer to variables)

    rootObj["temperature"] = adjusted_temp;
    rootObj["humidity"] = adjusted_humi;
    rootObj["dewpoint"] = DewpointTemperature;
    rootObj["relativepressure"] = rel_pressure_rounded;
    rootObj["pressurestate"] = PressureState;
    rootObj["battery"] = volt;
    rootObj["batterypercentage"] = batterypercentage;
    rootObj["absolutepressure"] = measured_pres;
    rootObj["heatindex"] = HeatIndex;
    rootObj["accuracy"] = accuracy_in_percent;
    rootObj["dewpointspread"] = DewPointSpread;
    rootObj["zambrettisays"] = ZambrettisWords;
    rootObj["trendinwords"] = trend_in_words;
    rootObj["trend"] = pressure_difference[11];
    rootObj["zletter"] = String(z_letter);
    rootObj["wifi_strength"] = int(WiFi.RSSI());
    rootObj["timestamp"] = current_timestamp;

    char jsonBuffer[512];
    serializeJson(jsonDoc, jsonBuffer);

    Serial.print("mqtt message: ");
    Serial.println(jsonBuffer);

    client.publish(mqtt_topic, jsonBuffer, 1);   // , 1 = retained
    delay(500);
  }

  if (volt > 3.6) {          //check if batt still ok, if yes
    goToSleep(sleepTimeMin); //go for a nap
  }
  else {                     //if not,
    goToSleep(0);            //hybernate because batt is empty
  }
} // end of void setup()

void loop() {               //loop is not used
} // end of void loop()

void measurementEvent() {

  //Measures absolute Pressure, Temperature, Humidity, Voltage, calculate relative pressure,
  //Dewpoint, Dewpoint Spread, Heat Index, current pressure state

  bme.takeForcedMeasurement();

  // Get temperature
  measured_temp_bme = bme.readTemperature();
  measured_temp_dal = getTemperature();
  measured_temp = measured_temp_dal;
  // print on serial monitor
  Serial.print("Temp BME: ");
  Serial.print(measured_temp_bme);
  Serial.println("°C; ");

  Serial.print("Temp Dallas: ");
  Serial.print(measured_temp_dal);
  Serial.println("°C; ");

  // Get humidity
  measured_humi_bme = bme.readHumidity();
  measured_humi = measured_humi_bme;

  Serial.print("Humidity BME: ");
  Serial.print(measured_humi_bme);
  Serial.println("%; ");

  // Get pressure
  measured_pres = bme.readPressure() / 100.0F;
  // print on serial monitor
  Serial.print("Pressure: ");
  Serial.print(measured_pres);
  Serial.print("hPa; ");

  // Calculate and print relative pressure
  SLpressure_hPa = (((measured_pres * 100.0) / pow((1 - ((float)(ELEVATION)) / 44330), 5.255)) / 100.0);
  rel_pressure_rounded = (int)(SLpressure_hPa + .5);
  // print on serial monitor
  Serial.print("Pressure rel: ");
  Serial.print(rel_pressure_rounded);
  Serial.print("hPa; ");

  // Calculate dewpoint
  double a = 17.271;
  double b = 237.7;
  double tempcalc = (a * measured_temp) / (b + measured_temp) + log(measured_humi * 0.01);
  DewpointTemperature = (b * tempcalc) / (a - tempcalc);
  Serial.print("Dewpoint: ");
  Serial.print(DewpointTemperature);
  Serial.println("°C; ");

  if (TEMP_CORR != 0) {
    // With the dewpoint calculated we can correct temp and automatically calculate humidity
    adjusted_temp = measured_temp + TEMP_CORR;
    if (adjusted_temp < DewpointTemperature) adjusted_temp = DewpointTemperature; //compensation, if offset too high
    //August-Roche-Magnus approximation (http://bmcnoldy.rsmas.miami.edu/Humidity.html)
    adjusted_humi = 100 * (exp((a * DewpointTemperature) / (b + DewpointTemperature)) / exp((a * adjusted_temp) / (b + adjusted_temp)));
    if (adjusted_humi > 100) adjusted_humi = 100;    // just in case
    // print on serial monitor
    Serial.print("Temp adjusted: ");
    Serial.print(adjusted_temp);
    Serial.print("°C; ");
    Serial.print("Humidity adjusted: ");
    Serial.print(adjusted_humi);
    Serial.print("%; ");
  }
  else
  {
    adjusted_temp = measured_temp;
    adjusted_humi = measured_humi;
  }

  // Calculate dewpoint spread (difference between actual temp and dewpoint -> the smaller the number: rain or fog

  DewPointSpread = adjusted_temp - DewpointTemperature;
  Serial.print("Dewpoint Spread: ");
  Serial.print(DewPointSpread);
  Serial.println("°C; ");

  // Calculate HI (heatindex in °C) --> HI starts working above 26,7 °C
  if (adjusted_temp > 26.7) {
    double c1 = -8.784, c2 = 1.611, c3 = 2.338, c4 = -0.146, c5 = -1.230e-2, c6 = -1.642e-2, c7 = 2.211e-3, c8 = 7.254e-4, c9 = -2.582e-6  ;
    double T = adjusted_temp;
    double R = adjusted_humi;

    double A = (( c5 * T) + c2) * T + c1;
    double B = ((c7 * T) + c4) * T + c3;
    double C = ((c9 * T) + c8) * T + c6;
    HeatIndex = (C * R + B) * R + A;
  }
  else {
    HeatIndex = adjusted_temp;
    Serial.println("Not warm enough (less than 26.7 °C) for Heatindex");
  }
  Serial.print("HeatIndex: ");
  Serial.print(HeatIndex);
  Serial.println("°C; ");

  // calculation of current pressure state (global variable String PressureState)
  if (rel_pressure_rounded < 990) PressureState = TEXT_STORM_LOW;
  else if (rel_pressure_rounded >= 990 && rel_pressure_rounded < 1000) PressureState = TEXT_STRONG_LOW;
  else if (rel_pressure_rounded >= 1000 && rel_pressure_rounded < 1013) PressureState = TEXT_LOWPRESSURE;
  else if (rel_pressure_rounded >= 1013 && rel_pressure_rounded < 1025) PressureState = TEXT_HIGHPRESSURE;
  else if (rel_pressure_rounded >= 1025) PressureState = TEXT_STRONG_HIGH;

  Serial.print("Pressure State: ");
  Serial.println(PressureState);

} // end of void measurementEvent()

int CalculateTrend() {
  int trend;                                    // -1 falling; 0 steady; 1 raising
  Serial.println("---> Calculating trend");

  //--> giving the most recent pressure reads more weight
  pressure_difference[0] = (pressure_value[0] - pressure_value[1])   * 1.5;
  pressure_difference[1] = (pressure_value[0] - pressure_value[2]);
  pressure_difference[2] = (pressure_value[0] - pressure_value[3])   / 1.5;
  pressure_difference[3] = (pressure_value[0] - pressure_value[4])   / 2;
  pressure_difference[4] = (pressure_value[0] - pressure_value[5])   / 2.5;
  pressure_difference[5] = (pressure_value[0] - pressure_value[6])   / 3;
  pressure_difference[6] = (pressure_value[0] - pressure_value[7])   / 3.5;
  pressure_difference[7] = (pressure_value[0] - pressure_value[8])   / 4;
  pressure_difference[8] = (pressure_value[0] - pressure_value[9])   / 4.5;
  pressure_difference[9] = (pressure_value[0] - pressure_value[10])  / 5;
  pressure_difference[10] = (pressure_value[0] - pressure_value[11]) / 5.5;

  //--> calculating the average and storing it into [11]
  pressure_difference[11] = (  pressure_difference[0]
                               + pressure_difference[1]
                               + pressure_difference[2]
                               + pressure_difference[3]
                               + pressure_difference[4]
                               + pressure_difference[5]
                               + pressure_difference[6]
                               + pressure_difference[7]
                               + pressure_difference[8]
                               + pressure_difference[9]
                               + pressure_difference[10]) / 11;

  Serial.print("Current trend: ");
  Serial.println(pressure_difference[11]);

  if      (pressure_difference[11] > 3.5) {
    trend_in_words = TEXT_RISING_FAST;
    trend = 1;
  }
  else if (pressure_difference[11] > 1.5   && pressure_difference[11] <= 3.5)  {
    trend_in_words = TEXT_RISING;
    trend = 1;
  }
  else if (pressure_difference[11] > 0.25  && pressure_difference[11] <= 1.5)  {
    trend_in_words = TEXT_RISING_SLOW;
    trend = 1;
  }
  else if (pressure_difference[11] > -0.25 && pressure_difference[11] < 0.25)  {
    trend_in_words = TEXT_STEADY;
    trend = 0;
  }
  else if (pressure_difference[11] >= -1.5 && pressure_difference[11] < -0.25) {
    trend_in_words = TEXT_FALLING_SLOW;
    trend = -1;
  }
  else if (pressure_difference[11] >= -3.5 && pressure_difference[11] < -1.5)  {
    trend_in_words = TEXT_FALLING;
    trend = -1;
  }
  else if (pressure_difference[11] <= -3.5) {
    trend_in_words = TEXT_FALLING_FAST;
    trend = -1;
  }

  Serial.println(trend_in_words);
  return trend;
}

char ZambrettiLetter() {
  Serial.println("---> Calculating Zambretti letter");
  int(z_trend) = CalculateTrend();
  // Case trend is falling
  if (z_trend == -1) {
    float zambretti = 0.0009746 * rel_pressure_rounded * rel_pressure_rounded - 2.1068 * rel_pressure_rounded + 1138.7019;
    //A Winter falling generally results in a Z value lower by 1 unit
    if (month(current_timestamp) < 4 || month(current_timestamp) > 9) zambretti = zambretti + 1;
    if (zambretti > 9) zambretti = 9;
    Serial.print("Calculated and rounded Zambretti in numbers: ");
    Serial.println(round(zambretti));
    switch (int(round(zambretti))) {
      case 0:  z_letter = 'A'; break;       //Settled Fine
      case 1:  z_letter = 'A'; break;       //Settled Fine
      case 2:  z_letter = 'B'; break;       //Fine Weather
      case 3:  z_letter = 'D'; break;       //Fine Becoming Less Settled
      case 4:  z_letter = 'H'; break;       //Fairly Fine Showers Later
      case 5:  z_letter = 'O'; break;       //Showery Becoming unsettled
      case 6:  z_letter = 'R'; break;       //Unsettled, Rain later
      case 7:  z_letter = 'U'; break;       //Rain at times, worse later
      case 8:  z_letter = 'V'; break;       //Rain at times, becoming very unsettled
      case 9:  z_letter = 'X'; break;       //Very Unsettled, Rain
    }
  }
  // Case trend is steady
  if (z_trend == 0) {
    float zambretti = 138.24 - 0.133 * rel_pressure_rounded;
    Serial.print("Calculated and rounded Zambretti in numbers: ");
    Serial.println(round(zambretti));
    switch (int(round(zambretti))) {
      case 0:  z_letter = 'A'; break;       //Settled Fine
      case 1:  z_letter = 'A'; break;       //Settled Fine
      case 2:  z_letter = 'B'; break;       //Fine Weather
      case 3:  z_letter = 'E'; break;       //Fine, Possibly showers
      case 4:  z_letter = 'K'; break;       //Fairly Fine, Showers likely
      case 5:  z_letter = 'N'; break;       //Showery Bright Intervals
      case 6:  z_letter = 'P'; break;       //Changeable some rain
      case 7:  z_letter = 'S'; break;       //Unsettled, rain at times
      case 8:  z_letter = 'W'; break;       //Rain at Frequent Intervals
      case 9:  z_letter = 'X'; break;       //Very Unsettled, Rain
      case 10: z_letter = 'Z'; break;       //Stormy, much rain
    }
  }
  // Case trend is rising
  if (z_trend == 1) {
    float zambretti = 142.57 - 0.1376 * rel_pressure_rounded;
    //A Summer rising, improves the prospects by 1 unit over a Winter rising
    if (month(current_timestamp) >= 4 && month(current_timestamp) <= 9) zambretti = zambretti - 1;
    if (zambretti < 0) zambretti = 0;
    Serial.print("Calculated and rounded Zambretti in numbers: ");
    Serial.println(round(zambretti));
    switch (int(round(zambretti))) {
      case 0:  z_letter = 'A'; break;       //Settled Fine
      case 1:  z_letter = 'A'; break;       //Settled Fine
      case 2:  z_letter = 'B'; break;       //Fine Weather
      case 3:  z_letter = 'C'; break;       //Becoming Fine
      case 4:  z_letter = 'F'; break;       //Fairly Fine, Improving
      case 5:  z_letter = 'G'; break;       //Fairly Fine, Possibly showers, early
      case 6:  z_letter = 'I'; break;       //Showery Early, Improving
      case 7:  z_letter = 'J'; break;       //Changeable, Improving
      case 8:  z_letter = 'L'; break;       //Rather Unsettled Clearing Later
      case 9:  z_letter = 'M'; break;       //Unsettled, Probably Improving
      case 10: z_letter = 'Q'; break;       //Unsettled, short fine Intervals
      case 11: z_letter = 'T'; break;       //Very Unsettled, Finer at times
      case 12: z_letter = 'Y'; break;       //Stormy, possibly improving
      case 13: z_letter = 'Z'; break;;      //Stormy, much rain
    }
  }
  Serial.print("This is Zambretti's famous letter: ");
  Serial.println(z_letter);
  return z_letter;
}

String ZambrettiSays(char code) {
  String zambrettis_words = "";

  // switch from summer (rain) to winter (snow)
  if (measured_temp <= 2 && LANGUAGE == "DE") LANGUAGE = "DW";
  if (measured_temp > 2 && LANGUAGE == "DW") LANGUAGE = "DE";
  translate();

  switch (code) {
    case 'A': zambrettis_words = TEXT_ZAMBRETTI_A; break;  //see Tranlation.h
    case 'B': zambrettis_words = TEXT_ZAMBRETTI_B; break;
    case 'C': zambrettis_words = TEXT_ZAMBRETTI_C; break;
    case 'D': zambrettis_words = TEXT_ZAMBRETTI_D; break;
    case 'E': zambrettis_words = TEXT_ZAMBRETTI_E; break;
    case 'F': zambrettis_words = TEXT_ZAMBRETTI_F; break;
    case 'G': zambrettis_words = TEXT_ZAMBRETTI_G; break;
    case 'H': zambrettis_words = TEXT_ZAMBRETTI_H; break;
    case 'I': zambrettis_words = TEXT_ZAMBRETTI_I; break;
    case 'J': zambrettis_words = TEXT_ZAMBRETTI_J; break;
    case 'K': zambrettis_words = TEXT_ZAMBRETTI_K; break;
    case 'L': zambrettis_words = TEXT_ZAMBRETTI_L; break;
    case 'M': zambrettis_words = TEXT_ZAMBRETTI_M; break;
    case 'N': zambrettis_words = TEXT_ZAMBRETTI_N; break;
    case 'O': zambrettis_words = TEXT_ZAMBRETTI_O; break;
    case 'P': zambrettis_words = TEXT_ZAMBRETTI_P; break;
    case 'Q': zambrettis_words = TEXT_ZAMBRETTI_Q; break;
    case 'R': zambrettis_words = TEXT_ZAMBRETTI_R; break;
    case 'S': zambrettis_words = TEXT_ZAMBRETTI_S; break;
    case 'T': zambrettis_words = TEXT_ZAMBRETTI_T; break;
    case 'U': zambrettis_words = TEXT_ZAMBRETTI_U; break;
    case 'V': zambrettis_words = TEXT_ZAMBRETTI_V; break;
    case 'W': zambrettis_words = TEXT_ZAMBRETTI_W; break;
    case 'X': zambrettis_words = TEXT_ZAMBRETTI_X; break;
    case 'Y': zambrettis_words = TEXT_ZAMBRETTI_Y; break;
    case 'Z': zambrettis_words = TEXT_ZAMBRETTI_Z; break;
    case '0': zambrettis_words = TEXT_ZAMBRETTI_0; break;
    default:  zambrettis_words = TEXT_ZAMBRETTI_DEFAULT; break;
  }
  return zambrettis_words;
}

void ReadFromSPIFFS() {
  char filename [] = "/data.txt";
  File myDataFile = SPIFFS.open(filename, "r");       // Open file for reading
  if (!myDataFile) {
    Serial.println("Failed to open file");
    FirstTimeRun();                                   // no file there -> initializing
  }

  Serial.println("---> Now reading from SPIFFS");

  String temp_data;

  temp_data = myDataFile.readStringUntil('\n');
  saved_timestamp = temp_data.toInt();
  Serial.print("Timestamp from SPIFFS: ");  Serial.println(saved_timestamp);

  temp_data = myDataFile.readStringUntil('\n');
  accuracy = temp_data.toInt();
  Serial.print("Accuracy value read from SPIFFS: ");  Serial.println(accuracy);

  Serial.print("Last 12 saved pressure values: ");
  for (int i = 0; i <= 11; i++) {
    temp_data = myDataFile.readStringUntil('\n');
    pressure_value[i] = temp_data.toInt();
    Serial.print(pressure_value[i]);
    Serial.print("; ");
  }
  myDataFile.close();
  Serial.println();
}

void WriteToSPIFFS(int write_timestamp) {
  char filename [] = "/data.txt";
  File myDataFile = SPIFFS.open(filename, "w");        // Open file for writing (appending)
  if (!myDataFile) {
    Serial.println("Failed to open file");
  }

  Serial.println("---> Now writing to SPIFFS");

  myDataFile.println(write_timestamp);                 // Saving timestamp to /data.txt
  myDataFile.println(accuracy);                        // Saving accuracy value to /data.txt

  for ( int i = 0; i <= 11; i++) {
    myDataFile.println(pressure_value[i]);             // Filling pressure array with updated values
  }
  myDataFile.close();

  Serial.println("File written. Now reading file again.");
  myDataFile = SPIFFS.open(filename, "r");             // Open file for reading
  Serial.print("Found in /data.txt = ");
  while (myDataFile.available()) {
    Serial.print(myDataFile.readStringUntil('\n'));
    Serial.print("; ");
  }
  Serial.println();
  myDataFile.close();
}

void FirstTimeRun() {
  Serial.println("---> Starting initializing process.");
  accuracy = 1;
  char filename [] = "/data.txt";
  File myDataFile = SPIFFS.open(filename, "w");            // Open a file for writing
  if (!myDataFile) {
    Serial.println("Failed to open file");
    Serial.println("Stopping process - maybe flash size not set (SPIFFS).");
    exit(0);
  }
  myDataFile.println(current_timestamp);                   // Saving timestamp to /data.txt
  myDataFile.println(accuracy);                            // Saving accuracy value to /data.txt
  for ( int i = 0; i < 12; i++) {
    myDataFile.println(rel_pressure_rounded);              // Filling pressure array with current pressure
  }
  Serial.println("** Saved initial pressure data. **");
  myDataFile.close();
  Serial.println("---> Doing a reset now.");
  resetFunc();                                              //call reset
}

void connect_to_MQTT() {
  Serial.print("---> Connecting to MQTT, ");
  client.setServer(mqtt_server, 1883);

  if (!client.connected()) {
    Serial.println("reconnecting MQTT...");
    reconnect();
  }
  Serial.println("MQTT connected ok.");
} //end connect_to_MQTT

void reconnect() {
  Serial.print("Attempting MQTT connection with ");
  // Create a random client ID
  String clientId = "ESP8266Client-";
  clientId += String(random(0xffff), HEX);
  Serial.print(clientId.c_str());
  // Attempt to connect
  if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
    Serial.println("MQTT is connected");
    // Once connected, publish an announcement...
    char tmp[128];
    String statusmessage =  StationName + ", " + Version + ": client started";
    statusmessage.toCharArray(tmp, 128);
    client.publish(mqtt_status, tmp);
    delay(50);
  } else {
    Serial.print(" ...failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again later");
  }

} //end void reconnect*/

float getTemperature() {
  const int numReadings = 32;                  // the higher the value, the smoother the average (default: 32)
  float readings[numReadings];                 // the readings from the analog input
  float total = 0;                             // the running total
  float tempTemp;

  for (int thisReading = 0; thisReading < numReadings; thisReading++) // take # of readings for smoothening
  {
    readings[thisReading] = s18d20.getTempCByIndex(0);
    total = total + readings[thisReading];
  }
  tempTemp = total / numReadings;

  // if (!is_metric) tempTemp = tempTemp * 1.8 + 32;  // conversion into Fahrenheit

  if (tempTemp > -127 && tempTemp < 85) {                            // error handling for bogus readings
    return tempTemp;
  }
  else {
    return -88;
  }
}

void goToSleep(unsigned int sleepmin) {
  char tmp[128];
  String sleepmessage =  StationName + ", " + Version + ": Taking a nap for " + String (sleepmin) + " Minutes";
  sleepmessage.toCharArray(tmp, 128);
  client.publish(mqtt_status, tmp);
  delay(50);

  Serial.println("INFO: Closing the MQTT connection");
  client.disconnect();

  Serial.println("INFO: Closing the Wifi connection");
  WiFi.disconnect();

  while (client.connected() || (WiFi.status() == WL_CONNECTED)) {
    Serial.println("Waiting for shutdown before sleeping");
    delay(10);
  }
  delay(50);

  Serial.print ("Going to sleep now for ");
  Serial.print (sleepmin);
  Serial.print (" Minute(s).");
  ESP.deepSleep(sleepmin * 60 * 1000000); // convert to microseconds
} // end of goToSleep()
