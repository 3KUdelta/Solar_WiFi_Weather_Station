/*----------------------------------------------------------------------------------------------------
  Project Name : Solar Powered WiFi Weather Station V2.35
  Features: temperature, dewpoint, dewpoint spread, heat index, humidity, absolute pressure, relative pressure, battery status and
  the famous Zambretti Forecaster (multi lingual)
  Authors: Keith Hungerford, Debasish Dutta and Marc Stähli
  Website : www.opengreenenergy.com

******* configuration control constant for use of Blynk and/or Thingspeak ***/

const String App1 = "BLYNK";         // empty string if not applicable -> "" else "BLYNK"
const String App2 = "THINGSPEAK";    // empty string if not applicable -> "" else "THINGSPEAK"

/****** Blink or ThingSpeak Settings ****************************************/

#define BLYNK_TEMPLATE_ID "YOUR_BLYNK_TEMPL_ID"
#define BLYNK_TEMPLATE_NAME "Solar Weather Station"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_TEMPL_ID"
char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "";                       // WiFi Router ssid
char pass[] = "";                       // WiFi Router password

unsigned long ts_ch_id = 000000;                  // Thingspeak Channel ID
const char* ts_api_key = "";      // API write key 

/****** MQTT Settings ********************************************************/

const char* mqtt_server = "192.xxx.xxx.xx";       // MQTT Server (broker) address

/****** Additional Settings **************************************************/

String LANGUAGE = "DE";              // either 'DE' for German or 'EN' for English

#define TEMP_CORR (-1)               // Manual correction of temp sensor
#define ELEVATION (130)              // Enter your elevation in m ASL to calculate rel pressure (ASL/QNH) at your place

#define sleepTimeMin (10)            // setting of deepsleep time in minutes (default: 10)

// NTP
#define NTP_SERVER      "ch.pool.ntp.org"  // Swiss NTP pool - use any in your country
#define TZ              1                  // (utc+) TZ in hours
#define DST_MN          60                 // use 60mn for summer time in some countries

#define TZ_SEC          ((TZ)*3600)
#define DST_SEC         ((DST_MN)*60)

/**********Blynk & ThingSpeak assginments ---------------------------------

Blynk:

virtual pin 0 Temperature (Celcius)
virtual pin 1 Humidity (%)
virtual pin 2 Absolute Pressure (hPa)
virtual pin 3 Relative Pressure (hPa)
virtual pin 4 Battery Volts (V)
virtual pin 5 Dewpoint (Celcius)
virtual pin 6 HeatIndex (Celcius)
virtual pin 7 Zambrettis Words
virtual pin 8 Accuracy in percent (%)
virtual pin 9 Trend in Words
virtual pin 10 Dewpoint Spread

ThingSpeak:

Field 1: Relative Pressure (hPa)
Field 2: Temperature (Celcius)
Field 3: Humidity (%)
Field 4: Battery (V)
Field 5: Absolute Pressure (hPa)
Field 6: Dewpoint (Celcius)
Field 7: HeatIndex (Celcius) 

MQTT

home/weather/solarweatherstation/tempc
home/weather/solarweatherstation/heatindexc
home/weather/solarweatherstation/dewpointc
home/weather/solarweatherstation/spreadc
home/weather/solarweatherstation/abshpa
home/weather/solarweatherstation/relhpa
home/weather/solarweatherstation/humi
home/weather/solarweatherstation/battv
home/weather/solarweatherstation/zambrettisays
home/weather/solarweatherstation/trend
home/weather/solarweatherstation/accuracy

***************************************************************************/
