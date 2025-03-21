/*----------------------------------------------------------------------------------------------------
  Project Name : Solar Powered WiFi Weather Station
  Features: temperature, dewpoint, dewpoint spread, heat index, humidity, absolute pressure, relative pressure, battery status and
  the famous Zambretti Forecaster (multi lingual)
  Authors: Keith Hungerford, Debasish Dutta and Marc Stähli
  Website : www.opengreenenergy.com */

const String StationName = "SWS_YourPlace";  // SolarWeatherStation (SWS)
const String Version = "2.44";

/******* Hardware settings ****************************************************/
#define EIGHTEENDTWENTY  // if you use a DS18B20 or DS18S20 temperature sensor, uncomment this line
float bat_calib_factor = 5.1; // change this value to calibrate the battery voltage
float bat_volt_minimum = 3.6; // minimum battery voltage, when the device goes to deep sleep

/******* Blynk ****************************************************************/
#define BLYNK // uncomment to enable Blynk
const char* BLYNK_AUTH_TOKEN = "YOUR_TOKEN";

/******* Thingspeak ***********************************************************/
// #define THINGSPEAK // uncomment to enable Thingspeak
const unsigned long THINGSPEAK_CH_ID = 0000000; // Thingspeak Channel ID
const char* THINGSPEAK_API_KEY = ""; // API write key 

/****** WiFi Settings ******************************************************/

char ssid[] = "YOUR_SSID";                       // WiFi Router ssid
char pass[] = "YOUR_PASSWORD";         // WiFi Router password

/****** MQTT Settings ********************************************************/

#define MQTT // uncomment to enable MQTT
const char* mqtt_server = "broker.hivemq.com";       // MQTT Server (broker) address
const char* mqtt_user = "";                          // MQTT Server (broker) userid
const char* mqtt_pass = "";                          // MQTT Server (broker) password
const char* mqtt_topic = "YOUR_TOPIC";  // e.g. myname/weather/my_location
const char* mqtt_status = "YOUR_STATUS"; // e.g. myname/status

/****** Additional Settings **************************************************/

String LANGUAGE = "DE";              // Using 'DE' for German for summer and winter messages ('EN' for English as example still in the code)

#define TEMP_CORR (0)                // Manual correction of temp sensor, humidity will automatically corrected with August-Roche-Magnus approximation

#define ELEVATION (420)              // Enter your elevation in m ASL to calculate rel pressure (ASL/QNH) at your place

#define sleepTimeMin (10)            // setting of deepsleep time in minutes (default: 10)

// NTP (reading UTC; local timezone does not matter, time is only needed for raising/falling pressure calcualtions)
#define NTP_SERVER "ch.pool.ntp.org"
