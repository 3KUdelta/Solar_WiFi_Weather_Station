/*----------------------------------------------------------------------------------------------------
  Project Name : Solar Powered WiFi Weather Station V2.6
  Features: temperature, dewpoint, dewpoint spread, heat index, humidity, absolute pressure, relative pressure, battery status and
  the famous Zambretti Forecaster (multi lingual)
  Authors: Keith Hungerford, Debasish Dutta and Marc Stähli
  Website : www.opengreenenergy.com

  Main microcontroller (ESP8266) and BME280 both sleep between measurements
  BME280 is used in single shot mode ("forced mode")
  CODE: https://github.com/3KUdelta/Solar_WiFi_Weather_Station
  INSTRUCTIONS & HARDWARE: https://www.instructables.com/id/Solar-Powered-WiFi-Weather-Station-V20/
  3D FILES: https://www.thingiverse.com/thing:3551386

  ====================================================================
  Version History (recent):

  v2.6 (April 2026) - SHT45 migration, configurable sensors & robustness pass
  - Replaced HDC1080 (failed after ~5 years outdoor) with Sensirion SHT45
    - Heater now activated BEFORE every measurement (200 mW × 1 s, command 0x39)
    - Better long-term stability, factory PTFE membrane (SHT45-AD1B)
    - Higher accuracy: ±1.0% rH, ±0.1°C
  - Configurable sensor selection via Settings26.h (#define switches)
    - USE_BME280, USE_DS18B20, USE_SHT45 to enable/disable each sensor
    - TEMP_SOURCE / HUMI_SOURCE to choose canonical source when multiple
      sensors are available
    - Compile-time validation: BME280 always required (pressure/Zambretti);
      invalid combinations produce #error messages.
  - Bugfixes:
    - getTemperature(): was reading the same DS18B20 conversion 32 times.
      Now performs a single 12-bit conversion with proper wait.
    - ReadFromMQTT(): added receive-flag and timeout-aware wait so a missing
      MQTT pressure message no longer triggers an unwanted FirstTimeRun()
      that would wipe the 6h pressure curve.
    - exit(0) on SPIFFS failure replaced with goToSleep() to prevent
      battery drain through hung WiFi.
    - reconnect(): added retry loop (3 attempts) instead of single-shot.
    - goToSleep() no longer publishes MQTT status when MQTT==false.
    - Battery voltage: now averaged over 16 ADC reads to reduce noise.
    - NTP wait loop: yield() added to prevent soft WDT reset.
    - Sprach-Hysterese (DE/DW switch) to avoid flapping at ~2°C.
    - Defensive default in Zambretti switch statements.
    - Removed double semicolon typo in case 13.
  - Style:
    - resetFunc replaced with ESP.restart() (cleaner on ESP8266).
    - Cleaned up auskommentierter Fahrenheit-Code.
  - Kept unchanged:
    - DHCP for WiFi (per user preference)
    - Fixed sleep interval (per user preference)
    - All Zambretti / trend / translation / MQTT message logic 1:1

  ////  Features :  /////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // 1. Connect to Wi-Fi, and upload the data to either Blynk App and/or any MQTT broker
  // 2. Monitoring Weather parameters like Temperature, Pressure abs, Pressure MSL and Humidity.
  // 3. Extra Ports to add more Weather Sensors like UV Index, Light and Rain Guage etc.
  // 4. Remote Battery Status Monitoring
  // 5. Using Sleep mode to reduce the energy consumed
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  /***************************************************
   VERY IMPORTANT:
 *                                                 *
   Enter your personal settings in Settings26.h !
 *                                                 *
 **************************************************/

#include "Settings26.h"
// Note: Translation file is now included from Settings26.h (Translations/Translation_XX.h)

// =====================================================================
// Sensor configuration validation (compile-time)
// =====================================================================
#if !USE_BME280
  #error "USE_BME280 must be 1: the project requires the BME280 for pressure (Zambretti forecast)."
#endif

#if (TEMP_SOURCE == SRC_DAL) && !USE_DS18B20
  #warning "TEMP_SOURCE = SRC_DAL but USE_DS18B20 = 0. Falling back to BME280 temperature."
  #undef  TEMP_SOURCE
  #define TEMP_SOURCE SRC_BME
#endif

#if (TEMP_SOURCE == SRC_SHT) && !USE_SHT45
  #warning "TEMP_SOURCE = SRC_SHT but USE_SHT45 = 0. Falling back to BME280 temperature."
  #undef  TEMP_SOURCE
  #define TEMP_SOURCE SRC_BME
#endif

#if (HUMI_SOURCE == SRC_SHT) && !USE_SHT45
  #warning "HUMI_SOURCE = SRC_SHT but USE_SHT45 = 0. Falling back to BME280 humidity."
  #undef  HUMI_SOURCE
  #define HUMI_SOURCE SRC_BME
#endif

#if (HUMI_SOURCE == SRC_DAL)
  #error "HUMI_SOURCE = SRC_DAL is invalid: DS18B20 cannot measure humidity. Use SRC_BME or SRC_SHT."
#endif

// =====================================================================
// Conditional includes (only what's actually used)
// =====================================================================
#if USE_DS18B20
  #include <OneWire.h>
  #include <DallasTemperature.h>
#endif

#include <Wire.h>                   // I2C (always needed for BME280)

#if USE_SHT45
  #include "Adafruit_SHT4x.h"        // SHT4x temp & humidity sensor
#endif

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <BlynkSimpleEsp8266.h>     // https://github.com/blynkkk/blynk-library
#include <WiFiUdp.h>
#include "FS.h"
#include <EasyNTPClient.h>          // https://github.com/aharshac/EasyNTPClient
#include <TimeLib.h>                // https://github.com/PaulStoffregen/Time.git
#include <PubSubClient.h>           // For MQTT (in this case publishing only)


#if USE_DS18B20
  #define ONE_WIRE_BUS 13            // Data wire 18d20 Sensor is plugged into port D7 @ ESP8266
  #define DS18B20_RESOLUTION 12      // 12-bit -> 0.0625°C, conversion ~750ms
#endif

Adafruit_BME280 bme;                // I2C

#if USE_SHT45
  Adafruit_SHT4x sht4 = Adafruit_SHT4x();
#endif

#if USE_DS18B20
  OneWire oneWire(ONE_WIRE_BUS);
  DallasTemperature s18d20(&oneWire);
#endif

WiFiUDP udp;
EasyNTPClient ntpClient(udp, NTP_SERVER, 0);  // reading UTC

//varialbes of measured or calculated sensor data
#if USE_DS18B20
  float measured_temp_dal;
#endif
float measured_temp_bme;
#if USE_SHT45
  float measured_temp_sht;
  float measured_humi_sht;
#endif
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
String ZambrettisWords;             // Final statement about weather forecast (after {P}/{E} substitution)
char z_letter;
int trend_idx;                      // Index into LANG_TRENDS[] (0..6)
int pressure_idx;                   // Index into LANG_PRESSURE[] (0..4)
// (Convenience accessors below for readability and serial debug output.)
inline const char* trend_in_words()    { return LANG_TRENDS[trend_idx]; }
inline const char* pressure_in_words() { return LANG_PRESSURE[pressure_idx]; }

// MQTT receive flag (set by callback when retained pressure curve arrives)
volatile bool mqtt_data_received = false;

// Trend index constants (used with LANG_TRENDS[])
#define TREND_RISING_FAST   0
#define TREND_RISING        1
#define TREND_RISING_SLOW   2
#define TREND_STEADY        3
#define TREND_FALLING_SLOW  4
#define TREND_FALLING       5
#define TREND_FALLING_FAST  6

// Pressure-state index constants (used with LANG_PRESSURE[])
#define PRESS_STORM_LOW     0
#define PRESS_STRONG_LOW    1
#define PRESS_LOW           2
#define PRESS_HIGH          3
#define PRESS_STRONG_HIGH   4

WiFiClient espClient;               // MQTT
PubSubClient client(espClient);     // MQTT

void setup() {
  Serial.begin(115200); while (!Serial); delay(200);
  Serial.println();
  Serial.print("Start of ");
  Serial.print(StationName);
  Serial.print(", Version ");
  Serial.println(Version);

  // Print the active sensor configuration for diagnostic purposes
  Serial.print("Sensors enabled: BME280=");
  Serial.print(USE_BME280 ? "Y" : "N");
  Serial.print("  DS18B20=");
  Serial.print(USE_DS18B20 ? "Y" : "N");
  Serial.print("  SHT45=");
  Serial.println(USE_SHT45 ? "Y" : "N");
  Serial.print("Canonical sources: TEMP=");
  #if   TEMP_SOURCE == SRC_BME
    Serial.print("BME280");
  #elif TEMP_SOURCE == SRC_DAL
    Serial.print("DS18B20");
  #elif TEMP_SOURCE == SRC_SHT
    Serial.print("SHT45");
  #endif
  Serial.print("  HUMI=");
  #if   HUMI_SOURCE == SRC_BME
    Serial.println("BME280");
  #elif HUMI_SOURCE == SRC_SHT
    Serial.println("SHT45");
  #endif

  Serial.print("Language: ");
  Serial.println(LANG_NAME);

  client.setBufferSize(512);       // Increasing PubSubClient

  //******Battery Voltage Monitoring (first thing to do: is battery still ok?)***********
  // FIX v2.6: averaged over 16 ADC reads to reduce ESP8266 ADC noise.

  // Voltage divider R1 = 220k+100k+220k =540k and R2=100k
  float calib_factor = 5.2; // change this value to calibrate the battery voltage

  unsigned long raw_total = 0;
  for (int i = 0; i < 16; i++) {
    raw_total += analogRead(A0);
    delay(2);
  }
  volt = (raw_total / 16.0) * calib_factor / 1024.0;

  Serial.print("Voltage = ");
  Serial.print(volt, 2);
  Serial.println(" V");

  batterypercentage = (volt - 3.4) * 100 / 0.7;   // 3.4 V is the lower limint set to 0%, bandwith 0.7 V
  if (batterypercentage > 100) batterypercentage = 100;
  if (batterypercentage < 0)   batterypercentage = 0;
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
      if (volt > 3.4) {
        goToSleep(10);   // go to sleep and retry after 10 min
      }
      else {
        goToSleep(0);   // hybernate because batt empty
      }
    }
    Serial.print(".");
  }
  Serial.println(" Wifi connected ok");

  if (App1 == "BLYNK") {        // for posting data to Blynk App
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  }

  if (MQTT) connect_to_MQTT();  // connecting to MQTT broker

  else {                        // Checking if SPIFFS available

    Serial.println("SPIFFS Initialization: (First time run can last up to 30 sec - be patient)");

    boolean mounted = SPIFFS.begin();               // load config if it exists. Otherwise use defaults.
    if (!mounted) {
      Serial.println("FS not formatted. Doing that now... (can last up to 30 sec).");
      SPIFFS.format();
      Serial.println("FS formatted...");
      SPIFFS.begin();
    }
  }

  //******** GETTING THE TIME FROM NTP SERVER  ***********************************

  Serial.println("---> Now reading time from NTP Server");
  int ii = 0;
  while (!ntpClient.getUnixTime()) {
    yield();                     // FIX v2.6: feed watchdog to prevent soft WDT reset
    delay(100);
    ii++;
    if (ii > 20) {
      Serial.println("Could not connect to NTP Server!");
      Serial.println("Doing a reset now and retry a connection from scratch.");
      ESP.restart();             // FIX v2.6: cleaner than jump to address 0
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
  Serial.print(month(current_timestamp));
  Serial.print(".");
  Serial.print(year(current_timestamp));
  Serial.println(" Local timezone does not matter - we just need always the same timezone --> using UTC");

  //******** SENSOR INITIALISATION  ********************

  // ----- BME280 (always required for pressure) -----
  bool bme_status = bme.begin(0x76);  //address either 0x76 or 0x77
  if (!bme_status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
  }

  Serial.println("BME280: forced mode, 1x temperature / 1x humidity / 1x pressure oversampling, filter off");

  bme.setSampling(Adafruit_BME280::MODE_FORCED,
                  Adafruit_BME280::SAMPLING_X1, // temperature
                  Adafruit_BME280::SAMPLING_X1, // pressure
                  Adafruit_BME280::SAMPLING_X1, // humidity
                  Adafruit_BME280::FILTER_OFF   );

#if USE_DS18B20
  // ----- DS18B20 -----
  s18d20.begin();
  s18d20.setResolution(DS18B20_RESOLUTION);
  Serial.println("DS18B20 initialized at 12-bit resolution.");
#endif

#if USE_SHT45
  // ----- SHT45 -----
  if (!sht4.begin()) {
    Serial.println("Could not find SHT45 sensor, check wiring!");
  } else {
    Serial.print("SHT4x serial: 0x");
    Serial.println(sht4.readSerial(), HEX);
    sht4.setPrecision(SHT4X_HIGH_PRECISION);
    Serial.println("SHT45 initialized: HIGH precision, heater controlled per measurement.");
  }
#endif

  measurementEvent();             // calling function to get all data from the different sensors

  if (MQTT) ReadFromMQTT();       // reading timestamp, accuracy and pressure curve
  else ReadFromSPIFFS();

  Serial.print("Timestamp difference: ");
  Serial.println(current_timestamp - saved_timestamp);

  if (current_timestamp - saved_timestamp > 21600) {     // last save older than 6 hours -> re-initialize values
    FirstTimeRun();
  }
  else if (current_timestamp - saved_timestamp > 1700) { // it is time for pressure update (1800 sec = 30 min)

    for (int i = 11; i >= 1; i = i - 1) {
      pressure_value[i] = pressure_value[i - 1];        // shifting values one to the right
    }

    pressure_value[0] = rel_pressure_rounded;           // updating with acutal rel pressure (newest value)

    if (accuracy < 12) {
      accuracy = accuracy + 1;
    }

    if (MQTT) WriteToMQTT();
    else WriteToSPIFFS(current_timestamp);
  }

  //**************************Calculate Zambretti Forecast*******************************************

  int accuracy_in_percent = accuracy * 94 / 12;        // 94% is the max predicion accuracy of Zambretti
  if ( volt > 3.4 ) {
    ZambrettisWords = ZambrettiSays(char(ZambrettiLetter()));
  }
  else {
    ZambrettisWords = ZambrettiSays('0');   // send Message that battery is empty
  }

  Serial.println("********************************************************");
  Serial.print("Zambretti says: ");
  Serial.print(ZambrettisWords);
  Serial.print(", ");
  Serial.println(trend_in_words());
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
    Blynk.virtualWrite(V0, adjusted_temp);
    Blynk.virtualWrite(V1, adjusted_humi);
    Blynk.virtualWrite(V2, measured_pres);
    Blynk.virtualWrite(V3, rel_pressure_rounded);
    Blynk.virtualWrite(V4, volt);
    Blynk.virtualWrite(V5, DewpointTemperature);
    Blynk.virtualWrite(V6, HeatIndex);
    Blynk.virtualWrite(V7, ZambrettisWords);
    Blynk.virtualWrite(V8, accuracy_in_percent);
    Blynk.virtualWrite(V9, trend_in_words());
    Blynk.virtualWrite(V10, DewPointSpread);
    Blynk.virtualWrite(V11, pressure_in_words());
    Serial.println("Data written to Blynk ...");
  }

  // *********** code block for publishing all data to MQTT ****************************
  if (MQTT) {
    JsonDocument jsonDoc;

    jsonDoc["temperature"] = adjusted_temp;
    jsonDoc["humidity"] = adjusted_humi;
    jsonDoc["dewpoint"] = DewpointTemperature;
    jsonDoc["relativepressure"] = rel_pressure_rounded;
    jsonDoc["pressurestate"] = pressure_in_words();
    jsonDoc["battery"] = volt;
    jsonDoc["batterypercentage"] = batterypercentage;
    jsonDoc["absolutepressure"] = measured_pres;
    jsonDoc["heatindex"] = HeatIndex;
    jsonDoc["accuracy"] = accuracy_in_percent;
    jsonDoc["dewpointspread"] = DewPointSpread;
    jsonDoc["zambrettisays"] = ZambrettisWords;
    jsonDoc["trendinwords"] = trend_in_words();
    jsonDoc["trend"] = pressure_difference[11];
    jsonDoc["zletter"] = String(z_letter);
    jsonDoc["wifi_strength"] = int(WiFi.RSSI());
    jsonDoc["timestamp"] = current_timestamp;

    char jsonBuffer[512];
    serializeJson(jsonDoc, jsonBuffer);

    Serial.print("Data MQTT message: ");
    Serial.println(jsonBuffer);

    client.publish(mqtt_topic, jsonBuffer, 1);   // , 1 = retained
    delay(50);
  }

  if (volt > 3.4) {
    goToSleep(sleepTimeMin);
  }
  else {
    goToSleep(0);
  }
} // end of void setup()

void loop() {               //loop is not used
} // end of void loop()

void measurementEvent() {

  //Measures absolute Pressure, Temperature, Humidity, Voltage, calculate relative pressure,
  //Dewpoint, Dewpoint Spread, Heat Index, current pressure state

  // ----- BME280 (always read - needed for pressure, used as cross-check) -----
  bme.takeForcedMeasurement();
  measured_temp_bme = bme.readTemperature();
  measured_humi_bme = bme.readHumidity();
  measured_pres    = bme.readPressure() / 100.0F;

  Serial.print("Temp BME: ");
  Serial.print(measured_temp_bme);
  Serial.print("°C; Humidity BME: ");
  Serial.print(measured_humi_bme);
  Serial.println("%; ");

#if USE_DS18B20
  // ----- DS18B20 -----
  measured_temp_dal = getTemperature();
  Serial.print("Temp Dallas: ");
  Serial.print(measured_temp_dal);
  Serial.println("°C; ");
#endif

#if USE_SHT45
  // ----- SHT45: heater pulse BEFORE measurement -----
  // Command 0x39: 200 mW × 1 s, then automatic measurement.
  // The heater pulse drives moisture out of the polymer membrane;
  // the follow-up read at ambient temperature is the "clean" value.
  Serial.println("SHT45: applying 200mW heater pulse (1s)...");
  sht4.setHeater(SHT4X_HIGH_HEATER_1S);
  sensors_event_t humidity_h, temp_h;
  sht4.getEvent(&humidity_h, &temp_h);   // triggers heated measurement (~1.1s)
  delay(50);

  // Now take the actual ambient measurement without heater
  sht4.setHeater(SHT4X_NO_HEATER);
  sensors_event_t humidity, temp;
  sht4.getEvent(&humidity, &temp);

  measured_temp_sht = temp.temperature;
  measured_humi_sht = humidity.relative_humidity;

  Serial.print("Temp SHT45: ");
  Serial.print(measured_temp_sht);
  Serial.print("°C; Humidity SHT45: ");
  Serial.print(measured_humi_sht);
  Serial.println("%; ");
#endif

  // ----- Selection of canonical values per Settings26.h -----
#if   TEMP_SOURCE == SRC_BME
  measured_temp = measured_temp_bme;
#elif TEMP_SOURCE == SRC_DAL
  measured_temp = measured_temp_dal;
#elif TEMP_SOURCE == SRC_SHT
  measured_temp = measured_temp_sht;
#endif

#if   HUMI_SOURCE == SRC_BME
  measured_humi = measured_humi_bme;
#elif HUMI_SOURCE == SRC_SHT
  measured_humi = measured_humi_sht;
#endif

  // ----- Pressure (always BME280) -----
  Serial.print("Pressure: ");
  Serial.print(measured_pres);
  Serial.print("hPa; ");

  SLpressure_hPa = (((measured_pres * 100.0) / pow((1 - ((float)(ELEVATION)) / 44330), 5.255)) / 100.0);
  rel_pressure_rounded = (int)(SLpressure_hPa + .5);
  Serial.print("Pressure rel: ");
  Serial.print(rel_pressure_rounded);
  Serial.print("hPa; ");

  // ----- Dewpoint -----
  double a = 17.271;
  double b = 237.7;
  double tempcalc = (a * measured_temp) / (b + measured_temp) + log(measured_humi * 0.01);
  DewpointTemperature = (b * tempcalc) / (a - tempcalc);
  Serial.print("Dewpoint: ");
  Serial.print(DewpointTemperature);
  Serial.println("°C; ");

  if (TEMP_CORR != 0) {
    adjusted_temp = measured_temp + TEMP_CORR;
    if (adjusted_temp < DewpointTemperature) adjusted_temp = DewpointTemperature; //compensation, if offset too high
    //August-Roche-Magnus approximation (http://bmcnoldy.rsmas.miami.edu/Humidity.html)
    adjusted_humi = 100 * (exp((a * DewpointTemperature) / (b + DewpointTemperature)) / exp((a * adjusted_temp) / (b + adjusted_temp)));
    if (adjusted_humi > 100) adjusted_humi = 100;
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

  // Dewpoint spread
  DewPointSpread = adjusted_temp - DewpointTemperature;
  Serial.print("Dewpoint Spread: ");
  Serial.print(DewPointSpread);
  Serial.println("°C; ");

  // Heat Index (>26.7°C only)
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

  // Pressure state (index into LANG_PRESSURE[])
  if      (rel_pressure_rounded < 990)                                     pressure_idx = PRESS_STORM_LOW;
  else if (rel_pressure_rounded >= 990  && rel_pressure_rounded < 1000)    pressure_idx = PRESS_STRONG_LOW;
  else if (rel_pressure_rounded >= 1000 && rel_pressure_rounded < 1013)    pressure_idx = PRESS_LOW;
  else if (rel_pressure_rounded >= 1013 && rel_pressure_rounded < 1025)    pressure_idx = PRESS_HIGH;
  else                                                                     pressure_idx = PRESS_STRONG_HIGH;

  Serial.print("Pressure State: ");
  Serial.println(pressure_in_words());

} // end of void measurementEvent()

int CalculateTrend() {
  int trend;                                    // -1 falling; 0 steady; 1 raising
  Serial.println("---> Calculating trend");

  //--> giving the most recent pressure reads more weight
  pressure_difference[0]  = (pressure_value[0] - pressure_value[1])   * 1.5;
  pressure_difference[1]  = (pressure_value[0] - pressure_value[2]);
  pressure_difference[2]  = (pressure_value[0] - pressure_value[3])   / 1.5;
  pressure_difference[3]  = (pressure_value[0] - pressure_value[4])   / 2;
  pressure_difference[4]  = (pressure_value[0] - pressure_value[5])   / 2.5;
  pressure_difference[5]  = (pressure_value[0] - pressure_value[6])   / 3;
  pressure_difference[6]  = (pressure_value[0] - pressure_value[7])   / 3.5;
  pressure_difference[7]  = (pressure_value[0] - pressure_value[8])   / 4;
  pressure_difference[8]  = (pressure_value[0] - pressure_value[9])   / 4.5;
  pressure_difference[9]  = (pressure_value[0] - pressure_value[10])  / 5;
  pressure_difference[10] = (pressure_value[0] - pressure_value[11])  / 5.5;

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

  if      (pressure_difference[11] >  3.5) {
    trend_idx = TREND_RISING_FAST;
    trend = 1;
  }
  else if (pressure_difference[11] >  1.5  && pressure_difference[11] <=  3.5)  {
    trend_idx = TREND_RISING;
    trend = 1;
  }
  else if (pressure_difference[11] >  0.25 && pressure_difference[11] <=  1.5)  {
    trend_idx = TREND_RISING_SLOW;
    trend = 1;
  }
  else if (pressure_difference[11] > -0.25 && pressure_difference[11] <   0.25) {
    trend_idx = TREND_STEADY;
    trend = 0;
  }
  else if (pressure_difference[11] >= -1.5 && pressure_difference[11] <  -0.25) {
    trend_idx = TREND_FALLING_SLOW;
    trend = -1;
  }
  else if (pressure_difference[11] >= -3.5 && pressure_difference[11] <  -1.5)  {
    trend_idx = TREND_FALLING;
    trend = -1;
  }
  else /* pressure_difference[11] <= -3.5 */ {
    trend_idx = TREND_FALLING_FAST;
    trend = -1;
  }

  Serial.println(trend_in_words());
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
      default: z_letter = 'A'; break;       //defensive default (FIX v2.6)
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
      default: z_letter = 'A'; break;       //defensive default (FIX v2.6)
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
      case 13: z_letter = 'Z'; break;       //Stormy, much rain  (FIX v2.6: removed double semicolon)
      default: z_letter = 'A'; break;       //defensive default (FIX v2.6)
    }
  }
  Serial.print("This is Zambretti's famous letter: ");
  Serial.println(z_letter);
  return z_letter;
}

// ----- Seasonal precipitation word selection with hysteresis -----
// Returns true while we're in winter mode (snow), false otherwise (rain).
// Uses static state so the threshold can be crossed without flapping
// between summer and winter when the temperature hovers near 2°C.
bool isWinterMode() {
  static bool winter = false;
  if (winter && measured_temp > WINTER_THRESHOLD_HIGH) winter = false;
  if (!winter && measured_temp <= WINTER_THRESHOLD_LOW) winter = true;
  return winter;
}

// ----- Replace one occurrence of `marker` in `dest` with `replacement` -----
// Simple in-place substitution. Returns the modified String.
// We use String here because the result length varies and the call site
// only happens once per measurement cycle (no hot path).
static String replaceMarker(const String& src, const char* marker, const char* replacement) {
  String result = src;
  int pos = result.indexOf(marker);
  while (pos >= 0) {
    result = result.substring(0, pos) + replacement + result.substring(pos + strlen(marker));
    pos = result.indexOf(marker, pos + strlen(replacement));
  }
  return result;
}

String ZambrettiSays(char code) {
  // Map the Zambretti char to an array index:
  //   'A'..'Z' -> 0..25
  //   '0'      -> 26  (low battery message)
  //   anything else -> default fallback string
  int idx;
  if (code >= 'A' && code <= 'Z')      idx = code - 'A';
  else if (code == '0')                 idx = 26;
  else                                  return String(LANG_FORECAST_DEFAULT);

  // Determine the precipitation words for the current season
  bool winter = isWinterMode();
  const char* precip_p = winter ? LANG_PRECIP_P_WINTER : LANG_PRECIP_P_SUMMER;
  const char* precip_e = winter ? LANG_PRECIP_E_WINTER : LANG_PRECIP_E_SUMMER;

  // Look up the template and substitute the markers
  String result(LANG_ZAMBRETTI[idx]);
  if (result.indexOf("{P}") >= 0) result = replaceMarker(result, "{P}", precip_p);
  if (result.indexOf("{E}") >= 0) result = replaceMarker(result, "{E}", precip_e);
  return result;
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
  Serial.println("Doing a first time run");
  accuracy = 1;
  for (int b = 0; b < 12; b++) {
    pressure_value[b] = rel_pressure_rounded;               // Filling pressure array with current pressure
  }
  if (MQTT) WriteToMQTT();
  else {
    Serial.println("---> Starting SPIFFS initializing process.");

    char filename [] = "/data.txt";
    File myDataFile = SPIFFS.open(filename, "w");            // Open a file for writing
    if (!myDataFile) {
      Serial.println("Failed to open file");
      Serial.println("Stopping process - maybe flash size not set (to SPIFFS).");
      goToSleep(60);    // FIX v2.6: was exit(0); that left WiFi active and drained the battery.
      return;
    }
    myDataFile.println(current_timestamp);                   // Saving timestamp to /data.txt
    myDataFile.println(accuracy);                            // Saving accuracy value to /data.txt
    for ( int i = 0; i < 12; i++) {
      myDataFile.println(rel_pressure_rounded);              // Storing pressure array with current pressure
    }
    Serial.println("** Saved initial pressure data. **");
    myDataFile.close();
    Serial.println("---> Doing a reset now.");
    ESP.restart();                                            // FIX v2.6: cleaner than jump to addr 0
  }
}

void connect_to_MQTT() {
  Serial.print("---> Connecting to MQTT");
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  if (!client.connected()) {
    reconnect();
  }
} //end connect_to_MQTT

void reconnect() {
  // FIX v2.6: retry up to 3 times before giving up.
  int attempts = 0;
  while (!client.connected() && attempts < 3) {
    Serial.print("..attempting MQTT connection (try ");
    Serial.print(attempts + 1);
    Serial.print("/3) with ");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    Serial.println(clientId.c_str());

    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println("MQTT is connected");
      char tmp[128];
      String statusmessage =  StationName + ", " + Version + ": client started";
      statusmessage.toCharArray(tmp, 128);
      client.publish(mqtt_status, tmp);

      // Subscribe to pressure topic
      client.subscribe(mqtt_press_topic);
      delay(50);
      return;
    } else {
      Serial.print(" ...failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying...");
      delay(1000);
      attempts++;
    }
  }
  Serial.println("MQTT failed after 3 attempts - data this cycle may be lost.");
} //end void reconnect

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(": containing: ");

  // print received message raw to serial monitor
  String messageTemp;
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    messageTemp += (char)payload[i];
  }
  Serial.println();

  if (String(topic) == mqtt_press_topic) {

    JsonDocument inDoc;

    DeserializationError err = deserializeJson(inDoc, payload, length);
    if (err) {
      Serial.print("deserializeJson() failed with code ");
      Serial.println(err.f_str());
      return;
    }

    for (int a = 0; a < 12; a++) {
      pressure_value[a] = inDoc["pressurecurve"][String(a)];
    }
    accuracy = inDoc["pressurecurve"]["accuracy"];
    saved_timestamp = inDoc["pressurecurve"]["datatime"];

    mqtt_data_received = true;          // FIX v2.6: signal successful receipt
  }

} // end void callback()

void ReadFromMQTT() {
  // FIX v2.6: wait for actual message arrival (with 5s timeout) instead of
  // a blind 1s loop. Prevents ghost-FirstTimeRun() that wipes the 6h curve.
  Serial.println("Waiting for retained MQTT pressure curve...");
  unsigned long start = millis();
  while (!mqtt_data_received && (millis() - start < 5000)) {
    client.loop();
    delay(10);
  }
  if (!mqtt_data_received) {
    Serial.println("WARNING: No MQTT pressure curve received within 5s.");
    Serial.println("Skipping pressure-curve update for this cycle to preserve last known state.");
    saved_timestamp = current_timestamp;
  } else {
    Serial.println("MQTT pressure curve received.");
  }
} // end of ReadFromMQTT()

#if USE_DS18B20
float getTemperature() {
  // FIX v2.6: previously took 32 reads of the SAME conversion (no-op smoothing).
  // Now performs a single proper 12-bit conversion (0.0625°C resolution).
  s18d20.requestTemperatures();
  delay(750);   // 12-bit conversion time per Maxim datasheet
  float t = s18d20.getTempCByIndex(0);

  if (t > -127 && t < 85) {
    return t;
  }
  else {
    return -88;
  }
}
#endif

void WriteToMQTT() {     // Write the pressure data to MQTT instead to SPIFFS (because of finite lifespan of Flash memory)

  JsonDocument pressureDoc;

  pressureDoc["pressurecurve"]["datatime"] = current_timestamp;
  pressureDoc["pressurecurve"]["accuracy"] = accuracy;

  for (int i = 0; i < 12; i++) {
    pressureDoc["pressurecurve"][String(i)] = pressure_value[i];
  }

  char p_buffer[512];
  serializeJson(pressureDoc, p_buffer);

  Serial.print("Outgoing pressure MQTT message: ");
  Serial.println(p_buffer);

  client.publish(mqtt_press_topic, p_buffer, 1);   // , 1 = retained
  delay(50);
}

void goToSleep(unsigned int sleepmin) {
  // FIX v2.6: only publish status if MQTT is actually connected.
  if (MQTT && client.connected()) {
    char tmp[128];
    String sleepmessage =  StationName + ", " + Version + ": Taking a nap for " + String (sleepmin) + " Minutes";
    sleepmessage.toCharArray(tmp, 128);
    client.publish(mqtt_status, tmp);
    delay(50);

    Serial.println("INFO: Closing the MQTT connection");
    client.disconnect();
  }

  Serial.println("INFO: Closing the Wifi connection");
  WiFi.disconnect();

  unsigned long shutdown_start = millis();
  while ((client.connected() || (WiFi.status() == WL_CONNECTED)) && (millis() - shutdown_start < 2000)) {
    Serial.println("Waiting for shutdown before sleeping");
    delay(10);
  }
  delay(50);

  Serial.print ("Going to sleep now for ");
  Serial.print (sleepmin);
  Serial.print (" Minute(s).");
  ESP.deepSleep(sleepmin * 60 * 1000000); // convert to microseconds
} // end of goToSleep()
