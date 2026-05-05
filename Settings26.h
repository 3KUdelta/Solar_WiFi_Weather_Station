/*----------------------------------------------------------------------------------------------------
  Project Name : Solar Powered WiFi Weather Station V2.6
  Features: temperature, dewpoint, dewpoint spread, heat index, humidity, absolute pressure, relative pressure, battery status and
  the famous Zambretti Forecaster (multi lingual)
  Authors: Keith Hungerford, Debasish Dutta and Marc Stähli
  Website : www.opengreenenergy.com */

const String StationName = "SWS_YourPlace";  // SolarWeatherStation (SWS)
const String Version = "2.6";

/******* Language Selection **************************************************
 * Choose the language by including the corresponding translation file.
 * Available languages (one .h file per language in the Translations/ folder):
 *   - Translation_DE.h  : German
 *   - Translation_EN.h  : English
 *
 * Summer/winter switch (rain ↔ snow) is automatic in every language and
 * happens at runtime based on outdoor temperature. No separate "winter
 * version" of a language is needed any more.
 *
 * To add a new language: copy Translation_DE.h, translate the strings,
 * and change the include below.
 ****************************************************************************/

#include "Translation_DE.h"
// #include "Translation_EN.h"

/******* Sensor Configuration ************************************************
 * Choose which sensors are physically connected. The BME280 is required as
 * the project relies on its pressure sensor for the Zambretti forecast.
 * Additional sensors are optional and can be enabled or disabled here.
 *
 * Multiple temperature/humidity sensors can be enabled simultaneously for
 * cross-checking and logging. Use TEMP_SOURCE / HUMI_SOURCE below to choose
 * which one is the canonical (primary) value used for forecasts and MQTT.
 ****************************************************************************/

// Sensor presence flags (1 = enabled, 0 = disabled)
#define USE_BME280     1     // Bosch BME280: pressure (REQUIRED), humidity, temperature
#define USE_DS18B20    1     // Dallas 18B20:  temperature only (one-wire on D7)
#define USE_SHT45      1     // Sensirion SHT45: temperature + humidity (I²C @ 0x44)

// Canonical source selection
//   For TEMP_SOURCE choose one of: SRC_BME, SRC_DAL, SRC_SHT
//   For HUMI_SOURCE choose one of: SRC_BME, SRC_SHT
// (Make sure the chosen source is also enabled above; otherwise the code
// falls back to BME280 with a warning at startup.)
#define SRC_BME 1
#define SRC_DAL 2
#define SRC_SHT 3

#define TEMP_SOURCE    SRC_DAL    // recommended outdoor: Dallas (better thermal buffering)
#define HUMI_SOURCE    SRC_SHT    // recommended outdoor: SHT45 (PTFE membrane, integrated heater)

/******* configuration control constant for use of Blynk and/or Thingspeak **/

const String App1 = "BLYNK";         // empty string if not applicable -> "" else "BLYNK"
#define BLYNK_TEMPLATE_ID "YOUR_ID"
#define BLYNK_TEMPLATE_NAME "Solar Weather Station"
#define BLYNK_AUTH_TOKEN "YOUR_TOKEN"
//char auth[] = BLYNK_AUTH_TOKEN;

/****** WiFi Settings ******************************************************/

char ssid[] = "YOUR_SSID";                           // WiFi Router ssid
char pass[] = "YOUR_PASSWORD";                       // WiFi Router password

/****** MQTT Settings ********************************************************/

bool MQTT = true;                                    // If you use MQTT, set to true, else false (if true, SPIFFS is not used)

const char* mqtt_server = "broker.hivemq.com";       // MQTT Server (broker) address
const int mqtt_port = 1883;                          // MQTT Server Port
const char* mqtt_user = "";                          // MQTT Server (broker) userid
const char* mqtt_pass = "";                          // MQTT Server (broker) password
const char* mqtt_topic = "YOUR_TOPIC";               // e.g. myname/weather/my_location
const char* mqtt_press_topic = "YOUR_TOPIC";         // e.g. myname/weather/my_location/pressure
const char* mqtt_status = "YOUR_STATUS";             // e.g. myname/status

/****** Additional Settings **************************************************/

#define TEMP_CORR (0)                // Manual correction of temp sensor, humidity will automatically corrected with August-Roche-Magnus approximation
#define ELEVATION (420)              // Enter your elevation in m ASL to calculate rel pressure (ASL/QNH) at your place
#define sleepTimeMin (10)            // setting of deepsleep time in minutes (default: 10)
#define NTP_SERVER "ch.pool.ntp.org" // NTP (reading UTC; local timezone does not matter, time is only needed for raising/falling pressure calcualtions)

// Temperature threshold (°C) for switching between summer (rain) and winter
// (snow) precipitation words. Hysteresis prevents flapping near 2°C.
#define WINTER_THRESHOLD_LOW   (1.5)   // below this, switch to winter precipitation
#define WINTER_THRESHOLD_HIGH  (2.5)   // above this, switch back to summer
