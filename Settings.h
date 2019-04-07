/*----------------------------------------------------------------------------------------------------
  Project Name : Solar Powered WiFi Weather Station V2.1
  Authors: Keith Hungerford and Debasish Dutta / Modifications by 3KU_Delta (MarcS)
  Website : www.opengreenenergy.com

******* configuration control constant for use of Blynk and/or Thingspeak ***/

const String App1 = "BLYNK";         //  disable if not applicable
const String App2 = "Thingspeak";    //  disable if not applicable


/****** Blink or ThingSpeak Settings ****************************************/

char auth[] = "your_blynk_auth_code"; // Blynk Auth Token

char ssid[] = "your_ssid";                           // WiFi Router ssid
char pass[] = "your_router_password";             // WiFi Router password

const char* server = "api.thingspeak.com";        // Thingspeak Write API
const char* api_key = "your_thingspeak_write_api";         // API write key 

/****** Additional Settings **************************************************/

#define TEMP_CORR (-1)              //Manual correction of temp sensor (mine reads 1 degree too high)
#define ELEVATION (505)             //Enter your elevation in m above ASL to calculate rel pressure (MSL/QNH) at your place

#define sleepTimeMin (1)            //setting of deepsleep time in minutes (pause between reads)

/**********Blynk & ThingSpeak assginments ---------------------------------

Blynk:

virtual pin 0 Temperature (Celsius)
virtual pin 1 Humidity (%)
virtual pin 2 Absolute Pressure (hPa)
virtual pin 3 Relative Pressure (hPa)
virtual pin 4 Battery Volts (V)
virtual pin 5 Dewpoint (Celcius)
virtual pin 6 HeatIndex (Celsius)

ThingSpeak:

Field 1: Relative Pressure (hPa)
Field 2: Temperature (Celcius)
Field 3: Humidity (%)
Field 4: Battery (V)
Field 5: Absolute Pressure (hPa)
Field 6: Dewpoint (Celcius)
Field 7: HeatIndex (Celcius) 

***************************************************************************/
