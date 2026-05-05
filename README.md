# Zambretti Forecaster on Solar WiFi Weather Station

Based on the work of Open Green Energy. https://www.instructables.com/id/Solar-Powered-WiFi-Weather-Station-V20/
Authors of the base code: Keith Hungerford and Debasish Dutta - Excellent work, gentlemen!

## NEW in V2.6: SHT45 sensor, configurable sensors, new translation architecture

After ~5 years of outdoor service, the HDC1080 humidity sensor in my reference station drifted to a permanent 100% reading. The polymer membrane on these sensors is consumable in outdoor conditions — chemical contamination, repeated condensation cycles and dust degrade them over time. V2.6 replaces the HDC1080 with the **Sensirion SHT45** (the AD1B variant has a factory PTFE membrane, much better for outdoor use), and applies a 200 mW × 1 s heater pulse before every measurement to drive moisture out of the polymer. This dramatically improves long-term stability, especially in winter when humidity sits near saturation for weeks.

V2.6 also adds **configurable sensor selection** — you can now enable or disable each sensor (BME280, DS18B20, SHT45) directly in `Settings26.h` with simple `#define` switches, and choose which sensor is the canonical (primary) source for temperature and humidity. This makes the project usable with any subset of sensors you have on hand. The BME280 remains required because the project relies on its pressure sensor for the Zambretti forecast.

The **translation system has been completely redesigned**. The old monolithic Translation.h with duplicated summer/winter blocks per language is gone. Each language now lives in its own file (`Translation_DE.h`, `Translation_EN.h`, ...) and uses `{P}` and `{E}` template markers for precipitation words. Summer/winter switching (rain ↔ snow) happens automatically at runtime based on outdoor temperature — no more separate "DW" language blocks. Adding a new language is now just copying a ~80-line file and translating the strings.

Finally, several bugs and robustness issues that accumulated over the years have been fixed — see the changelog below.

### Repo structure

```
Solar_WiFi_Weather_Station/
├── Solar_WiFi_Weather_Station_v2_6.ino    # Main sketch
├── Settings26.h                            # User configuration (sensors, WiFi, MQTT, language)
├── Translation_DE.h                        # German translation
├── Translation_EN.h                        # English translation
└── README.md
```

### Language selection

In `Settings26.h`, uncomment the language you want:

```cpp
#include "Translation_DE.h"
// #include "Translation_EN.h"
```

Summer/winter precipitation words (rain ↔ snow) switch automatically based on measured temperature with hysteresis around 1.5°C / 2.5°C. No manual configuration needed.

### How to add a new language

1. Copy `Translation_DE.h` as `Translation_XX.h`
2. Translate all strings, keeping the `{P}` and `{E}` markers in place
3. Fill in the four precipitation words for your language:
   - `LANG_PRECIP_P_SUMMER` — generic precipitation noun (e.g. "pioggia", "pluie", "rain")
   - `LANG_PRECIP_P_WINTER` — generic winter precipitation (e.g. "neve", "neige", "snow")
   - `LANG_PRECIP_E_SUMMER` — precipitation event (e.g. "acquazzoni", "averses", "showers")
   - `LANG_PRECIP_E_WINTER` — winter precipitation event (e.g. "nevicata", "chutes de neige", "snowfall")
4. In `Settings26.h`, add `#include "Translation_XX.h"` and comment out the old one
5. Pull requests welcome!

### Sensor configuration examples

In `Settings26.h`:

**Full setup** (all three sensors, recommended for outdoor reference station):
```cpp
#define USE_BME280     1
#define USE_DS18B20    1
#define USE_SHT45      1
#define TEMP_SOURCE    SRC_DAL    // Dallas: best thermal buffering in sun
#define HUMI_SOURCE    SRC_SHT    // SHT45: best long-term stability
```

**BME280 only** (entry-level, single sensor):
```cpp
#define USE_BME280     1
#define USE_DS18B20    0
#define USE_SHT45      0
#define TEMP_SOURCE    SRC_BME
#define HUMI_SOURCE    SRC_BME
```

**Indoor setup** (BME + SHT45, no Dallas needed):
```cpp
#define USE_BME280     1
#define USE_DS18B20    0
#define USE_SHT45      1
#define TEMP_SOURCE    SRC_SHT
#define HUMI_SOURCE    SRC_SHT
```


## FLASH Memory at the end of its lifespan!

Dear Weather Station fans. For the ones who are using their Weather Station already from the start (we started in 2019), the flash memory is probably getting at its end. Let's do a quick calculation:
e.g. 5 years = 1'825 days. As we are doing 144 read/write cycles per day (all 10 Minutes) this results in 262'800 read/write clycles by now. Flash memory has a finite lifetime of about 100,000 write cycles (source: https://learn.adafruit.com/memories-of-an-arduino/arduino-memories#). Here we go. This is exactly what happened to my station. A flash write error causes the ESP8266 to loop for ever and sucking the battery empty. I discovered this just recently.

Easy fix:

Code has been changed in order to reduce writing to flash memory by a factor of 3. Updating is highly recommended.
Get a new ESP8266 D1 mini Pro CH9102 16M (e.g. https://www.aliexpress.com/item/1005006018009983.html) for roughly a dollar incl. shipment.

**Even better in V2.4+:** Set `MQTT = true` in Settings — pressure curve is then stored on a retained MQTT topic instead of local flash. Effectively unlimited lifespan.

## BLYNK UPDATE! Move now to new Version!

Running Blynk legacy will drain your battery and your device will stop working. Please update to new Blynk (free version works very well).

1. Create new Blynk account (https://blynk.io) Top right.
2. Add new template (see example below)
3. Add new device using your new template
4. Load Blynk App for your mobile device
5. Add widgets as before
6. done!


## Zambretti Weather Station

Major changes:

- simplified, restructured code (used Adafruit libraries for BME280 instead, sorry for this Keith)
- added relative pressure, dewpoint, dewpoint spread and heatindex calculations
- allow Blynk (** UPDATED, PLEASE SEE CHANGES **), ThingSpeak and MQTT data transmission
- redesigned box (simplified printing, less plastic usage, full snap-in)
- available languages in Version V2.3 (a big thank you to the contributors!)
  * English
  * German
  * Italian (Chak10)
  * Polish (TomaszDom)
  * Romanian (zangaby)
  * French (Ludestru)
  * Spanish (Fedecatt)
  * Turkish (Mert Sarac)
  * Dutch (Rickthefrog)
  * Norwegian (solbero)

Note: V2.6 ships with German and English. The other languages from V2.3 have not yet been migrated to the new template format. If you'd like to help, see "How to add a new language" above — it's a quick job of ~30 minutes per language.


Changes in V2.3

- included famous Zambretti forecaster (see Blynk example)
- added translation table for Zambretti forecast
- added multi language feature


Changes in V2.31

- added Dewpoint Spread
- fixed some minor things
- added Zambretti forecast in Thingspeak (thank you ThomaszDom)


Changes in V2.31 (MQTT version)

- allows to publish data to MQTT broker (alternative .ino file)


Changes in V2.32

- Battery monitoring and going to hibernate if battery low (battery protection)
- Warning text will be shown instead of Zambretti prediction if batt low


Changes in V2.33

- Corrected bug in the winter/summer adjustment for the Zambretti forecast


Changes in V2.34

- added August-Roche-Magnus approximation to automatically adjust humidity with temperature corrections
- Code cleanup


Changes in V2.35

- corrected TingSpeak communication changes (needs now Channel ID and KEY)
- moved V2.35 into history folder - will not maintained by me anymore
(pull requests for this version are still welcome)


Changes in V2.4

- updated Blynk (simple code changes - needs more to do on the Blynk server side)
- reformatted data into json
- decide in settings24.h if you want MQTT or not
- non-blocking MQTT connector if broker is not available
- added one-wire 18d20 temperature sensor (better temperature buffering in sunshine - more lazy) to P4 solder pull-up resistor 4.7 kOhm between signal and V+
- minor bug fixes
- changed translation to summer and winter messages (unfortunately, only german and english translation for this version available) If you still need different languages please translate into your language in translation24.h


Changes in V2.6

- **Sensor migration**: replaced HDC1080 with Sensirion SHT45 (AD1B variant with PTFE membrane recommended for outdoor use). Heater pulse (200 mW × 1 s) applied before every measurement for better long-term stability.
- **Configurable sensors via Settings26.h**: enable/disable BME280, DS18B20 and SHT45 individually with `USE_*` defines. Choose canonical temperature and humidity source with `TEMP_SOURCE` / `HUMI_SOURCE`. Compile-time validation catches invalid combinations. BME280 stays mandatory (pressure / Zambretti).
- **New translation architecture**: one file per language with `{P}` / `{E}` template markers for precipitation words. Summer/winter switching is automatic. The old monolithic Translation.h with duplicated DE/DW blocks is gone. See "How to add a new language" above.
- **Required libraries**: install `Adafruit SHT4x Library` (and its dependency `Adafruit Unified Sensor`). The old `ClosedCube_HDC1080` library is no longer needed.
- **Bugfixes** accumulated from issues over the years:
  * `getTemperature()` was reading the same DS18B20 conversion 32 times in a row (no-op smoothing) — now performs a single proper 12-bit conversion.
  * `ReadFromMQTT()` race condition could trigger an unwanted FirstTimeRun() and wipe the 6 h pressure curve when a retained message arrived late — now uses an event flag with 5 s timeout.
  * `exit(0)` on SPIFFS failure left WiFi active and drained the battery — replaced with `goToSleep(60)`.
  * MQTT `reconnect()` now retries 3 times instead of giving up after one attempt.
  * `goToSleep()` no longer publishes status when MQTT is disabled or not connected.
  * Battery voltage now averaged over 16 ADC reads (single-shot reads on ESP8266 are unreliable due to WiFi RF interference).
  * NTP wait loop now calls `yield()` to prevent soft WDT reset on slow NTP servers.
  * `resetFunc()` (jump to address 0) replaced with `ESP.restart()` everywhere — cleaner reset, no more Exception 4 on reboot.
  * Defensive defaults in Zambretti switch statements; double semicolon typo fixed.
  * Battery percentage is now clamped to 0-100% (no more negative values when the divider is disconnected).


Print the box yourself: https://www.thingiverse.com/thing:3551386

![Solar Wifi Weather Station](https://github.com/3KUdelta/Solar_WiFi_Weather_Station/raw/master/IMG_2951.jpg)

New Blynk App Example (free widgets)
![Solar Wifi Weather Station](https://github.com/3KUdelta/Solar_WiFi_Weather_Station/raw/master/New_Blynk_App.jpeg)

Blynk template definition
![Solar Wifi Weather Station](https://github.com/3KUdelta/Solar_WiFi_Weather_Station/raw/master/Blynk_Template_Definition.png)

Showing the data on a LED display: https://github.com/3KUdelta/MDparola_MQTT_monitor
![LED matrix MQTT monitor](https://github.com/3KUdelta/MDparola_MQTT_monitor/raw/master/pictures/IMG_3180.JPG)

Node-Red example showing MQTT messages on dashboard.
![Solar Wifi Weather Station](https://github.com/3KUdelta/Solar_WiFi_Weather_Station/raw/master/Node-Red-Dashboard.png)

ThinkSpeak Example (ThingView iOS):
![Solar Wifi Weather Station](https://github.com/3KUdelta/Solar_WiFi_Weather_Station/raw/master/IMG_2617B43DD8C8-1.jpeg)
