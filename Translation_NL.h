/*----------------------------------------------------------------------------------------------------
Translation_NL.h - Dutch translation
Project: Solar Powered WiFi Weather Station V2.6+

See Translation_DE.h for the architecture description and translator notes.
****************************************************************************/

#ifndef TRANSLATION_NL_H
#define TRANSLATION_NL_H

// ----- Language metadata -----
const char LANG_NAME[] = "NL";
const char LANG_FORECAST_LABEL[] = "Voorspelling";
const char LANG_ACCURACY_LABEL[] = "Betrouwbaarheid voorspelling";
const char LANG_PRESSURE_LABEL[] = "Luchtdruk";
const char LANG_FORECAST_DEFAULT[]= "Sorry, momenteel geen voorspelling";

// ----- Precipitation words -----
// {P} placeholder gets replaced with one of these:
const char LANG_PRECIP_P_SUMMER[] = "regen";
const char LANG_PRECIP_P_WINTER[] = "sneeuw";
// {E} placeholder gets replaced with one of these:
const char LANG_PRECIP_E_SUMMER[] = "buien";
const char LANG_PRECIP_E_WINTER[] = "sneeuwval";

// ----- Trends: index 0=RISING_FAST .. 6=FALLING_FAST -----
const char* const LANG_TRENDS[7] = {
"snel stijgend", // 0
"stijgend", // 1
"licht stijgend", // 2
"stabiel", // 3
"licht dalend", // 4
"dalend", // 5
"snel dalend" // 6
};

// ----- Pressure states: index 0=STORM_LOW .. 4=STRONG_HIGH -----
const char* const LANG_PRESSURE[5] = {
"zeer lage druk", // 0
"lage druk", // 1
"normale druk", // 2
"hoge druk", // 3
"zeer hoge druk" // 4
};

// ----- Zambretti forecasts: A..Z + '0' (battery) -----
const char* const LANG_ZAMBRETTI[27] = {
/* A */ "Stabiel goed weer",
/* B */ "Goed weer",
/* C */ "Weer trekt bij",
/* D */ "Goed, maar begint te veranderen",
/* E */ "Goed, mogelijk {E}",
/* F */ "Redelijk goed, in verbetering",
/* G */ "Redelijk goed, mogelijk {E} 's ochtends",
/* H */ "Redelijk goed, {E} later",
/* I */ "{E} 's ochtends, dan verbetering",
/* J */ "Wisselend, maar verbeterend",
/* K */ "Redelijk goed, {E} waarschijnlijk",
/* L */ "Iets onstabiel, later opklaringen",
/* M */ "Onstabiel, maar waarschijnlijk beter",
/* N */ "{E}, met opklaringen",
/* O */ "{E}, dan onstabiel",
/* P */ "Wisselend, met wat {P}",
/* Q */ "Onstabiel, korte periodes goed weer",
/* R */ "Onstabiel, {P} later",
/* S */ "Onstabiel, {P} af en toe",
/* T */ "Zeer onstabiel, soms beter",
/* U */ "{P} af en toe, dan verslechtering",
/* V */ "{P} af en toe, sterk verslechterend",
/* W */ "{P} frequent",
/* X */ "Zeer onstabiel, met {P}",
/* Y */ "Stormachtig, mogelijk verbetering",
/* Z */ "Stormachtig, veel {P}",
/* 0 */ "Batterij leeg, opladen aub!"
};

#endif // TRANSLATION_NL_H
