/*----------------------------------------------------------------------------------------------------
  Project Name : Solar Powered WiFi Weather Station
  Features: temperature, dewpoint, dewpoint spread, heat index, humidity, absolute pressure, relative pressure, battery status and
  the famous Zambretti Forecaster (multi lingual)
  Authors: Keith Hungerford, Debasish Dutta and Marc Stähli
  Website : www.opengreenenergy.com */

const String StationName = "SWS_YourPlace";  // SolarWeatherStation (SWS)
const String Version = "2.43";

/******* configuration control constant for use of Blynk and/or Thingspeak **/

const String App1 = "BLYNK";         // empty string if not applicable -> "" else "BLYNK" 
#define BLYNK_TEMPLATE_ID "YOUR_ID"
#define BLYNK_TEMPLATE_NAME "Solar Weather Station"
#define BLYNK_AUTH_TOKEN "YOUR_TOKEN"
//char auth[] = BLYNK_AUTH_TOKEN;

/****** WiFi Settings ******************************************************/

char ssid[] = "YOUR_SSID";                       // WiFi Router ssid
char pass[] = "YOUR_PASSWORD";         // WiFi Router password

/****** MQTT Settings ********************************************************/

bool MQTT = true;                             // If you use MQTT, set to true, else false

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
#define NTP_SERVER      "ch.pool.ntp.org"
