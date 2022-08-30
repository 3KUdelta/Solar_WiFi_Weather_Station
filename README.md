# Zambretti Forecaster on Solar WiFi Weather Station
Based on the work of Open Green Energy.
https://www.instructables.com/id/Solar-Powered-WiFi-Weather-Station-V20/
Authors of the base code: Keith Hungerford and Debasish Dutta - Excellent work, gentlemen!

Major changes:
- simplified, restructured code (used Adafruit libraries for BME280 instead, sorry for this Keith)
- added relative pressure, dewpoint, dewpoint spread and heatindex calculations
- allow Blynk (**deprecated by Dec 31, 2022**), ThingSpeak and MQTT data transmission
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

Print the box yourself: https://www.thingiverse.com/thing:3551386

## BLYNK legacy (used in this code) will get deprecated! Legacy server shutdown - December 31, 2022 (https://blynk.io/blog/what-will-happen-to-the-legacy-blynk-platform). I do not plan to update to new Blynk - maybe someone here in the community?

[![Solar Wifi Weather Station](https://github.com/3KUdelta/Solar_WiFi_Weather_Station/blob/master/IMG_2951.jpg)](https://github.com/3KUdelta/Solar_WiFi_Weather_Station)

Showing the data on a LED display: https://github.com/3KUdelta/MDparola_MQTT_monitor
[![LED matrix MQTT monitor](https://github.com/3KUdelta/MDparola_MQTT_monitor/blob/master/pictures/IMG_3180.JPG)](https://github.com/3KUdelta/MDparola_MQTT_monitor)

Node-Red example showing MQTT messages on dashboard.
[![Solar Wifi Weather Station](https://github.com/3KUdelta/Solar_WiFi_Weather_Station/blob/master/Node-Red-Dashboard.png)](https://github.com/3KUdelta/Solar_WiFi_Weather_Station)

ThinkSpeak Example (ThingView iOS):
[![Solar Wifi Weather Station](https://github.com/3KUdelta/Solar_WiFi_Weather_Station/blob/master/IMG_2617B43DD8C8-1.jpeg)](https://github.com/3KUdelta/Solar_WiFi_Weather_Station)
