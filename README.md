# Zambretti Forecaster on Solar WiFi Weather Station
Based on the work of Open Green Energy.
https://www.instructables.com/id/Solar-Powered-WiFi-Weather-Station-V20/
Authors of the base code: Keith Hungerford and Debasish Dutta - Excellent work, gentlemen!

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
- available languages (a big thank you to the contributors!)
  - English
  - German
  - Italian (Chak10)
  - Polish (TomaszDom)
  - Romanian (zangaby)
  - French (Ludestru)
  - Spanish (Fedecatt)
  - Turkish (Mert Sarac)
  - Dutch (Rickthefrog)
  - Norwegian (solbero)

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

Changes in V2.4
- updated Blynk (simple code changes - needs more to do on the Blynk server side)
- reformatted data into json
- decide in settings.h if you want MQTT or not
- non-blocking MQTT connector if broker is not available
- minor bug fixes
  
Print the box yourself: https://www.thingiverse.com/thing:3551386

[![Solar Wifi Weather Station](https://github.com/3KUdelta/Solar_WiFi_Weather_Station/blob/master/IMG_2951.jpg)](https://github.com/3KUdelta/Solar_WiFi_Weather_Station)

New Blynk App Example (free widgets)
[![Solar Wifi Weather Station](https://github.com/3KUdelta/Solar_WiFi_Weather_Station/blob/master/New_Blynk_App.jpeg)](https://github.com/3KUdelta/Solar_WiFi_Weather_Station)

Blynk template definition
[![Solar Wifi Weather Station](https://github.com/3KUdelta/Solar_WiFi_Weather_Station/blob/master/Blynk_Template_Definition.png)](https://github.com/3KUdelta/Solar_WiFi_Weather_Station)

Showing the data on a LED display: https://github.com/3KUdelta/MDparola_MQTT_monitor
[![LED matrix MQTT monitor](https://github.com/3KUdelta/MDparola_MQTT_monitor/blob/master/pictures/IMG_3180.JPG)](https://github.com/3KUdelta/MDparola_MQTT_monitor)

Node-Red example showing MQTT messages on dashboard.
[![Solar Wifi Weather Station](https://github.com/3KUdelta/Solar_WiFi_Weather_Station/blob/master/Node-Red-Dashboard.png)](https://github.com/3KUdelta/Solar_WiFi_Weather_Station)

ThinkSpeak Example (ThingView iOS):
[![Solar Wifi Weather Station](https://github.com/3KUdelta/Solar_WiFi_Weather_Station/blob/master/IMG_2617B43DD8C8-1.jpeg)](https://github.com/3KUdelta/Solar_WiFi_Weather_Station)
