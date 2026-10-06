# Zambretti Forecaster on Solar WiFi Weather Station

Based on the work of Open Green Energy. https://www.instructables.com/id/Solar-Powered-WiFi-Weather-Station-V20/
Authors of the base code: Keith Hungerford and Debasish Dutta - Excellent work, gentlemen!

## NEW in V2.7: Firmware updates over WiFi (OTA)

Until now every software or configuration change meant taking the station down from its place outside, opening the box and connecting a cable. V2.7 ends this: **the station updates itself**. On every wake-up it looks for a new firmware file on a web server in your home network, and if there is one, it downloads it, flashes itself and reports the result via MQTT.

### How it works

Right after connecting to WiFi and MQTT, the station fetches a small file `firmware.md5` from your web server and compares it with the MD5 checksum of the firmware it is running.

- No file on the server, or server not reachable: nothing happens, normal measurement cycle.
- Same checksum: firmware is up to date, normal measurement cycle.
- Different checksum: `firmware.bin` is downloaded, verified against the checksum, flashed, and the station restarts with the new firmware.

All settings are compile-time constants, so a configuration change (sleep time, temperature correction, language, ...) is simply a new firmware file.

### Setting it up

1. You need a web server in your LAN that serves static files over plain **http** (a NAS, a Raspberry Pi, ...). Create a folder for the station, e.g. `http://192.168.1.10/sws/`.
2. In `Settings27.h` set `OTA_ENABLED` to `1` and `OTA_BASE_URL` to that folder (IP address, no `https`, no `.local` names, trailing `/`). OTA is switched off by default.
3. In the Arduino IDE choose a flash layout with OTA space, e.g. Tools → Flash Size → **4MB (FS:2MB OTA:~1019KB)**, and keep this setting for all later builds.
4. Flash V2.7 **once by cable** and press the reset button once afterwards (on an ESP8266 the first software restart after a serial upload can hang).

### Publishing a new firmware

1. Arduino IDE: Sketch → Export Compiled Binary.
2. Copy the exported `.bin` to the web server folder as `firmware.bin`.
3. Write its MD5 checksum (32 hex characters) into `firmware.md5` in the same folder. Write this file **last**, it is the trigger.

The script `deploy.command` in this repo does steps 2 and 3 for you on macOS (double-click) and Linux (`bash deploy.command`): put it into the web server folder and set the path to your sketch folder at the top. On Windows, `certutil -hashfile firmware.bin MD5` gives you the checksum.

The station updates itself on its next wake-up, i.e. within one sleep interval.

### What you see on MQTT

Messages are published retained on `mqtt_ota_topic` (so they are still there while the station sleeps) and additionally on `mqtt_status`:

```
SWS_YourPlace, 2.7: OTA start: new firmware 3f9a12c7 found, attempt 1/3
SWS_YourPlace, 2.7: OTA: firmware flashed, restarting
SWS_YourPlace, 2.7.1: OTA ok: 2.7 -> 2.7.1
SWS_YourPlace, 2.7: OTA error (attempt 1/3): HTTP error: connection failed - continuing with 2.7
SWS_YourPlace, 2.7: OTA postponed: battery 3.62 V is below 3.70 V
```

Success is reported by the **new** firmware after the restart. Change `Version` in `Settings27.h` with every build (max. 8 characters), then the message shows what happened. Without MQTT (`MQTT = false`) the update works as well, the result is only visible on the serial monitor.

### Things you should know

- **The firmware file contains your WiFi and MQTT credentials in clear text.** Keep it inside your LAN. Do not put it on a public web server and do not attach it to a GitHub issue.
- **There is no way back.** An ESP8266 cannot roll back to the previous firmware. A failed download is harmless (the old firmware keeps running), but a firmware with a wrong WiFi password or one that crashes before the update check can only be replaced by cable. This is why the update check runs first, before Blynk, NTP and the sensors: as long as WiFi and the update check work, almost any later bug can be fixed remotely.
- **Built-in brakes:** no flashing below `OTA_MIN_VOLT` (default 3.7 V), and the same firmware file is tried at most `OTA_MAX_ATTEMPTS` times (default 3), so a broken file cannot cause an endless update loop.
- **Do not change the Flash Size setting** between builds. That is the one change you should make by cable.

### Other changes in V2.7

- **Empty battery no longer means "dead forever".** Below 3.4 V the station used to go into `deepSleep(0)`, which never wakes up, even after the solar panel had recharged the battery. It now sleeps `lowBattSleepMin` (default 60) minutes and resumes by itself.
- **No more restart loop without internet.** If the NTP server could not be reached, the station restarted immediately, again and again, and drained the battery. It now goes to sleep and retries on the next wake-up.
- **Pressure curve on a new broker.** With `MQTT = true` and a broker (or topic) that has no retained pressure curve yet, the curve was never created and the forecast never started. It is now created after 3 runs without curve.
- **DS18B20 diagnosis.** One retry on a bogus reading, and a diagnosis line published retained on `<mqtt_topic>/diag` (`DS18B20: ok`, or the raw value and the number of devices found on the bus).
- **Rain/snow hysteresis really works now** (its state survives deep sleep), and implausible pressure readings no longer overwrite the pressure curve.
- **Fixed MQTT client id** (`mqtt_client_id` in `Settings27.h`) instead of a random one.

### Upgrading from V2.6

Copy your values from `Settings26.h` into the new `Settings27.h`. New entries: `mqtt_client_id`, `lowBattSleepMin` and the OTA block. If you keep your personal settings in a file named `Settings27_mst.h` next to the sketch, the sketch uses that one automatically (it is excluded from Git by `.gitignore`).

The [Solar Station Watchdog](https://github.com/3KUdelta/Solar_Station_Watchdog) works with V2.7 without changes.

## V2.6: SHT45 sensor, configurable sensors, new translation architecture

After ~5 years of outdoor service, the HDC1080 humidity sensor in my reference station drifted to a permanent 100% reading. The polymer membrane on these sensors is consumable in outdoor conditions — chemical contamination, repeated condensation cycles and dust degrade them over time. V2.6 replaces the HDC1080 with the **Sensirion SHT45** (the AD1B variant has a factory PTFE membrane, much better for outdoor use), and applies a 200 mW × 1 s heater pulse before every measurement to drive moisture out of the polymer. This dramatically improves long-term stability, especially in winter when humidity sits near saturation for weeks.

V2.6 also adds **configurable sensor selection** — you can now enable or disable each sensor (BME280, DS18B20, SHT45) directly in the settings file with simple `#define` switches, and choose which sensor is the canonical (primary) source for temperature and humidity. This makes the project usable with any subset of sensors you have on hand. The BME280 remains required because the project relies on its pressure sensor for the Zambretti forecast.

The **Blynk connection is now non-blocking**. Previous versions used `Blynk.begin()` which would hang the ESP and trigger a Soft WDT reset if the Blynk server was unreachable or the credentials were wrong. V2.6 uses `Blynk.config()` + `Blynk.connect(5000)` with a 5-second timeout instead — if Blynk fails, the station continues normally with MQTT. No more crashes due to Blynk server issues.

The **translation system has been completely redesigned**. The old monolithic Translation.h with duplicated summer/winter blocks per language is gone. Each language now lives in its own file (`Translation_DE.h`, `Translation_EN.h`, ...) and uses `{P}` and `{E}` template markers for precipitation words. Summer/winter switching (rain ↔ snow) happens automatically at runtime based on outdoor temperature — no more separate "DW" language blocks. Adding a new language is now just copying a ~80-line file and translating the strings.

Finally, several bugs and robustness issues that accumulated over the years have been fixed — see the full changelog below.

### Repo structure V2.7

```
Solar_WiFi_Weather_Station/
├── Solar_WiFi_Weather_Station_v2_7.ino    # Main sketch
├── Settings27.h                            # User configuration (sensors, WiFi, MQTT, OTA, language)
├── Translation_DE.h                        # German translation
├── Translation_EN.h                        # English translation (and 8 more languages)
├── deploy.command                          # Helper to publish a firmware for OTA
├── history/                                # Older versions
└── README.md
```

### Language selection

In `Settings27.h`, uncomment the language you want:

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
4. In `Settings27.h`, add `#include "Translation_XX.h"` and comment out the old one
5. Pull requests welcome!

### Sensor configuration examples

In `Settings27.h`:

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


## NEW: Solar Station Watchdog

A companion project that monitors your weather station via MQTT and alerts you when something goes wrong. Runs on a second ESP8266 with a small OLED display, shows live weather data and blinks the onboard LED on alarm conditions (station offline, battery critical, humidity sensor stuck, DS18B20 bus error).

**Repository: https://github.com/3KUdelta/Solar_Station_Watchdog**

Hardware needed: Wemos D1 Mini + SSD1306 OLED 128×64 (~5 €). Powered by USB, runs continuously.


## FLASH Memory at the end of its lifespan!

Dear Weather Station fans. For the ones who are using their Weather Station already from the start (we started 2019), the flash memory is probably getting at its end. Let's do a quick calculation
for example: 5 years = 1'825 days. As we are doing 144 read/write cycles per day (all 10 Minutes) this results in 262'800 read/write clycles by now. Flash memory has a finite lifetime of about 100,000 write cycles (source: https://learn.adafruit.com/memories-of-an-arduino/arduino-memories#). Here we go. This is exactly what happened to my station. A flash write error causes the ESP8266 to loop for ever and sucking the battery empty. I discovered this just recently.

Easy fix:

Code has been changed in order to reduce writing to flash memory by a factor of 3. Updating is highly recommended.
Get a new ESP8266 D1 mini Pro CH9102 16M (e.g. https://www.aliexpress.com/item/1005006018009983.html) for roughly a dollar incl. shipment.

**Even better in V2.4+:** Set `MQTT = true` in Settings — pressure curve is then stored on a retained MQTT topic instead of local flash. Effectively unlimited lifespan.

## BLYNK UPDATE! Move now to new Version!

Running Blynk legacy will drain your battery and your device will stop working. Please update to new Blynk (free version works very well).

**Since V2.6:** The Blynk connection is non-blocking. If the Blynk server is unreachable or your credentials are wrong, the station will continue normally instead of crashing with a Soft WDT reset. This was a common issue reported by users on V2.4.

1. Create new Blynk account (https://blynk.io) Top right.
2. Add new template (see example https://github.com/3KUdelta/Solar_WiFi_Weather_Station/blob/master/Blynk_Template_Definition.png)
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
- **Blynk non-blocking**: `Blynk.begin()` replaced with `Blynk.config()` + `Blynk.connect(5000)`. Station continues without Blynk if the server is unreachable. `Blynk.virtualWrite()` calls are now guarded by `Blynk.connected()`.
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


Changes in V2.7

- **Firmware update over WiFi (OTA)**: on every wake-up the station compares `firmware.md5` on a web server in the LAN with its own firmware and, if they differ, downloads, verifies and flashes `firmware.bin`. Start, success and failure are published via MQTT (`mqtt_ota_topic`, `mqtt_status`). Off by default; see "NEW in V2.7" above for setup and safety notes.
- **Bugfixes**:
  * Empty battery (≤ 3.4 V) used `deepSleep(0)` and never woke up again — now sleeps `lowBattSleepMin` (60) minutes and resumes after recharging.
  * NTP server not reachable caused an endless restart loop without sleep — now sleeps and retries on the next wake-up.
  * Pressure curve was never created on a broker without retained curve — now created after 3 consecutive runs without curve.
  * Implausible pressure readings (BME280 missing or defective) no longer overwrite the stored pressure curve.
  * Rain/snow hysteresis had no effect because its state was lost in deep sleep — now kept in RTC memory.
  * With empty battery, trend and Zambretti letter were not calculated (published "rising fast" and an empty letter).
  * Sleep time calculation overflowed above 71 minutes.
- **DS18B20**: one retry on a bogus reading; diagnosis line on `<mqtt_topic>/diag`.
- **MQTT client id** is a fixed, configurable name (`mqtt_client_id`).
- **Settings**: the sketch uses a personal `Settings27_mst.h` if present, otherwise `Settings27.h`.


Print the box yourself: https://www.thingiverse.com/thing:3551386

![Solar Wifi Weather Station](https://github.com/3KUdelta/Solar_WiFi_Weather_Station/raw/master/IMG_2951.jpg)

New Blynk App Example (free widgets)
![Solar Wifi Weather Station](https://github.com/3KUdelta/Solar_WiFi_Weather_Station/raw/master/New_Blynk_App.jpeg)

Blynk template definition
![Solar Wifi Weather Station](https://github.com/3KUdelta/Solar_WiFi_Weather_Station/raw/master/Blynk_Template_Definition.png)

Monitoring your station: https://github.com/3KUdelta/Solar_Station_Watchdog
Showing the data on a LED display: https://github.com/3KUdelta/MDparola_MQTT_monitor
![LED matrix MQTT monitor](https://github.com/3KUdelta/MDparola_MQTT_monitor/raw/master/pictures/IMG_3180.JPG)

Node-Red example showing MQTT messages on dashboard.
![Solar Wifi Weather Station](https://github.com/3KUdelta/Solar_WiFi_Weather_Station/raw/master/Node-Red-Dashboard.png)

ThinkSpeak Example (ThingView iOS):
![Solar Wifi Weather Station](https://github.com/3KUdelta/Solar_WiFi_Weather_Station/raw/master/IMG_2617B43DD8C8-1.jpeg)
