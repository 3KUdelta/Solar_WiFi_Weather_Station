/*----------------------------------------------------------------------------------------------------
  Translation_DE.h - German translation
  Project: Solar Powered WiFi Weather Station V2.6+
  Maintainer: Marc Stähli (Switzerland)

  Architecture notes for translators:
  -----------------------------------
  Each language file defines language-wide arrays for trends and pressure
  states, plus 27 Zambretti forecast strings (A..Z + battery message).

  Seasonal substitution (rain ↔ snow) is handled via two markers in the
  Zambretti strings. The main sketch replaces them at runtime based on
  outdoor temperature:

    {P}  -  generic precipitation noun  (e.g. "Regen" / "Schnee")
            used in mass-noun contexts: "viel {P}", "später {P}"

    {E}  -  precipitation as a discrete event  (e.g. "Regenschauer" / "Schneefall")
            used for events: "frühe {E} möglich", "{E} mit Aufhellungen"

  Strings without {P} or {E} are seasonally invariant. The choice between
  summer/winter is made each measurement cycle with hysteresis around
  1.5°C / 2.5°C to prevent flapping near freezing.

  Adding a new language:
    1. Copy this file as Translation_XX.h
    2. Translate all strings, keeping {P} and {E} markers where applicable
    3. Define LANG_PRECIP_*_SUMMER / _WINTER appropriately for your grammar
    4. In Settings26.h, set the LANGUAGE define accordingly
 ****************************************************************************/

#ifndef TRANSLATION_DE_H
#define TRANSLATION_DE_H

// ----- Language metadata -----
const char LANG_NAME[]            = "DE";
const char LANG_FORECAST_LABEL[]  = "Wettervorhersage";
const char LANG_ACCURACY_LABEL[]  = "Vorhersagegenauigkeit";
const char LANG_PRESSURE_LABEL[]  = "Luftdruck";
const char LANG_FORECAST_DEFAULT[]= "Im Moment keine Prognose möglich";

// ----- Precipitation words for seasonal substitution -----
//   {P} placeholder gets replaced with one of these:
const char LANG_PRECIP_P_SUMMER[] = "Regen";
const char LANG_PRECIP_P_WINTER[] = "Schnee";
//   {E} placeholder gets replaced with one of these:
const char LANG_PRECIP_E_SUMMER[] = "Regenschauer";
const char LANG_PRECIP_E_WINTER[] = "Schneefall";

// ----- Trend descriptions: index 0=RISING_FAST .. 6=FALLING_FAST -----
const char* const LANG_TRENDS[7] = {
  "rasch steigend",     // 0: TREND_RISING_FAST
  "steigend",           // 1: TREND_RISING
  "langsam steigend",   // 2: TREND_RISING_SLOW
  "beständig",          // 3: TREND_STEADY
  "langsam fallend",    // 4: TREND_FALLING_SLOW
  "fallend",            // 5: TREND_FALLING
  "rasch fallend"       // 6: TREND_FALLING_FAST
};

// ----- Pressure states: index 0=STORM_LOW .. 4=STRONG_HIGH -----
const char* const LANG_PRESSURE[5] = {
  "Sturmtief",                  // 0: PRESS_STORM_LOW    (< 990 hPa)
  "Ausgewachsenes Tief",        // 1: PRESS_STRONG_LOW   (990-999)
  "Tiefdruck",                  // 2: PRESS_LOW          (1000-1012)
  "Hochdruck",                  // 3: PRESS_HIGH         (1013-1024)
  "Kräftiges Hoch"              // 4: PRESS_STRONG_HIGH  (>= 1025)
};

// ----- Zambretti forecasts: A..Z + '0' (battery) -----
// Strings with {P} use the generic precipitation word.
// Strings with {E} use the discrete-event precipitation word.
// Strings without markers are seasonally invariant.
const char* const LANG_ZAMBRETTI[27] = {
  /* A */ "Beständiges Schönwetter",
  /* B */ "Schönes Wetter",
  /* C */ "Wetter wird gut",
  /* D */ "Schön, wird wechselhaft",
  /* E */ "Schön, {E} möglich",
  /* F */ "Ziemlich gut, verbessert sich",
  /* G */ "Ziemlich gut, frühe {E} möglich",
  /* H */ "Ziemlich gut, spätere {E}",
  /* I */ "Früh schauerhaft, verbessert sich",
  /* J */ "Wechselhaft, verbessert sich",
  /* K */ "Ziemlich gut, {E} möglich",
  /* L */ "Eher veränderlich, klart später auf",
  /* M */ "Veränderlich, verbessert sich wahrscheinlich",
  /* N */ "{E} mit Aufhellungen",
  /* O */ "{E}, wird veränderlich",
  /* P */ "Veränderlich mit wenig {P}",
  /* Q */ "Veränderlich, mit kurzen schönen Intervallen",
  /* R */ "Veränderlich, später {P}",
  /* S */ "Veränderlich, zeitweise {P}",
  /* T */ "Stark wechselnd, zeitweise schöner",
  /* U */ "Zeitweise {P}, verschlechtert sich",
  /* V */ "Zeitweise {P}, wird sehr unruhig",
  /* W */ "{P} in regelmässigen Abständen",
  /* X */ "Sehr veränderlich, {P}",
  /* Y */ "Stürmisch, verbessert sich wahrscheinlich",
  /* Z */ "Stürmisch, viel {P}",
  /* 0 */ "Batterie leer, bitte nachladen!"   // index 26, mapped from char '0'
};

#endif // TRANSLATION_DE_H
