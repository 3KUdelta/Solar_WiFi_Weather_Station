/*----------------------------------------------------------------------------------------------------
  Translation_EN.h - English translation
  Project: Solar Powered WiFi Weather Station V2.6+

  See Translation_DE.h for the architecture description and translator notes.
 ****************************************************************************/

#ifndef TRANSLATION_EN_H
#define TRANSLATION_EN_H

// ----- Language metadata -----
const char LANG_NAME[]            = "EN";
const char LANG_FORECAST_LABEL[]  = "Forecast";
const char LANG_ACCURACY_LABEL[]  = "Prediction accuracy";
const char LANG_PRESSURE_LABEL[]  = "Pressure";
const char LANG_FORECAST_DEFAULT[]= "Sorry, no forecast for the moment";

// ----- Precipitation words -----
//   {P} placeholder gets replaced with one of these:
const char LANG_PRECIP_P_SUMMER[] = "rain";
const char LANG_PRECIP_P_WINTER[] = "snow";
//   {E} placeholder gets replaced with one of these:
const char LANG_PRECIP_E_SUMMER[] = "showers";
const char LANG_PRECIP_E_WINTER[] = "snowfall";

// ----- Trends: index 0=RISING_FAST .. 6=FALLING_FAST -----
const char* const LANG_TRENDS[7] = {
  "rising fast",        // 0
  "rising",             // 1
  "rising slow",        // 2
  "steady",             // 3
  "falling slow",       // 4
  "falling",            // 5
  "falling fast"        // 6
};

// ----- Pressure states: index 0=STORM_LOW .. 4=STRONG_HIGH -----
const char* const LANG_PRESSURE[5] = {
  "stormy low",         // 0
  "strong low",         // 1
  "low pressure",       // 2
  "high pressure",      // 3
  "strong high"         // 4
};

// ----- Zambretti forecasts: A..Z + '0' (battery) -----
const char* const LANG_ZAMBRETTI[27] = {
  /* A */ "Settled Fine Weather",
  /* B */ "Fine Weather",
  /* C */ "Becoming Fine",
  /* D */ "Fine, Becoming Less Settled",
  /* E */ "Fine, possibly {E}",
  /* F */ "Fairly Fine, Improving",
  /* G */ "Fairly Fine, possibly {E} early",
  /* H */ "Fairly Fine, {E} later",
  /* I */ "Showery Early, Improving",
  /* J */ "Changeable Improving",
  /* K */ "Fairly Fine, {E} likely",
  /* L */ "Rather Unsettled, Clearing Later",
  /* M */ "Unsettled, Probably Improving",
  /* N */ "{E}, Bright Intervals",
  /* O */ "{E} Becoming Unsettled",
  /* P */ "Changeable, some {P}",
  /* Q */ "Unsettled, short fine Intervals",
  /* R */ "Unsettled, {P} later",
  /* S */ "Unsettled, {P} at times",
  /* T */ "Very Unsettled, Finer at times",
  /* U */ "{P} at times, Worse later",
  /* V */ "{P} at times, becoming very unsettled",
  /* W */ "{P} at Frequent Intervals",
  /* X */ "Very Unsettled, {P}",
  /* Y */ "Stormy, possibly improving",
  /* Z */ "Stormy, much {P}",
  /* 0 */ "Battery empty, please recharge!"
};

#endif // TRANSLATION_EN_H
