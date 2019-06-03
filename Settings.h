/*----------------------------------------------------------------------------------------------------
  Project Name : Solar Powered WiFi Weather Station V2.31
  Features: temperature, dewpoint, dewpoint spread, heat index, humidity, absolute pressure, relative pressure, battery status and
  the famous Zambretti Forecaster (multi lingual)
  Authors: Keith Hungerford, Debasish Dutta and Marc Stähli
  Website : www.opengreenenergy.com

******* configuration control constant for use of Blynk and/or Thingspeak ***/

const String App1 = "BLYNK";         //  disable if not applicable
const String App2 = "Thingspeak";    //  disable if not applicable


/****** Blink or ThingSpeak Settings ****************************************/

char auth[] = "your Blynk Auth Token"; // Blynk Auth Token

char ssid[] = "your SSID";                           // WiFi Router ssid
char pass[] = "your Password";             // WiFi Router password

const char* server = "api.thingspeak.com";        // Thingspeak Write API
const char* api_key = "your Thingspeak API key";         // API write key 

/****** Additional Settings **************************************************/

#define LANGUAGE 'DE'               //either 'DE' for German or 'EN' for English

#define TEMP_CORR (-1)              //Manual correction of temp sensor (mine reads 1 degree too high)
#define ELEVATION (505)             //Enter your elevation in m ASL to calculate rel pressure (ASL/QNH) at your place

#define sleepTimeMin (10)           //setting of deepsleep time in minutes (default: 10)

// NTP
#define NTP_SERVER      "ch.pool.ntp.org"
#define TZ              1           // (utc+) TZ in hours
#define DST_MN          60          // use 60mn for summer time in some countries

#define TZ_SEC          ((TZ)*3600)  // don't change this
#define DST_SEC         ((DST_MN)*60)// don't change this

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
virtual pin 9 Tend in Words
virtual pin 10 Dewpoint Spread

ThingSpeak:

Field 1: Relative Pressure (hPa)
Field 2: Temperature (Celcius)
Field 3: Humidity (%)
Field 4: Battery (V)
Field 5: Absolute Pressure (hPa)
Field 6: Dewpoint (Celcius)
Field 7: HeatIndex (Celcius) 

***************************************************************************/
