/*----------------------------------------------------------------------------------------------------
Translation_NO.h - Norwegian translation
Project: Solar Powered WiFi Weather Station V2.6+

See Translation_DE.h for the architecture description and translator notes.
****************************************************************************/

#ifndef TRANSLATION_NO_H
#define TRANSLATION_NO_H

// ----- Language metadata -----
const char LANG_NAME[] = "NO";
const char LANG_FORECAST_LABEL[] = "Værprognose";
const char LANG_ACCURACY_LABEL[] = "Prognosens pålitelighet";
const char LANG_PRESSURE_LABEL[] = "Lufttrykk";
const char LANG_FORECAST_DEFAULT[]= "Beklager, ingen prognose akkurat nå";

// ----- Precipitation words -----
// {P} placeholder gets replaced with one of these:
const char LANG_PRECIP_P_SUMMER[] = "regn";
const char LANG_PRECIP_P_WINTER[] = "snø";
// {E} placeholder gets replaced with one of these:
const char LANG_PRECIP_E_SUMMER[] = "byger";
const char LANG_PRECIP_E_WINTER[] = "snøbyger";

// ----- Trends: index 0=RISING_FAST .. 6=FALLING_FAST -----
const char* const LANG_TRENDS[7] = {
"stiger raskt", // 0
"stiger", // 1
"stiger svakt", // 2
"stabilt", // 3
"faller svakt", // 4
"faller", // 5
"faller raskt" // 6
};

// ----- Pressure states: index 0=STORM_LOW .. 4=STRONG_HIGH -----
const char* const LANG_PRESSURE[5] = {
"veldig lavt trykk", // 0
"lavt trykk", // 1
"normalt trykk", // 2
"høyt trykk", // 3
"veldig høyt trykk" // 4
};

// ----- Zambretti forecasts: A..Z + '0' (battery) -----
const char* const LANG_ZAMBRETTI[27] = {
/* A */ "Stabilt fint vær",
/* B */ "Fint vær",
/* C */ "Været bedrer seg",
/* D */ "Fint, men begynner å endre seg",
/* E */ "Fint, mulige {E}",
/* F */ "Ganske fint, bedring på gang",
/* G */ "Ganske fint, mulige {E} om morgenen",
/* H */ "Ganske fint, {E} senere",
/* I */ "{E} om morgenen, deretter bedring",
/* J */ "Variabelt, men bedrer seg",
/* K */ "Ganske fint, {E} sannsynlig",
/* L */ "Litt ustabilt, opphold senere",
/* M */ "Ustabilt, men trolig bedring",
/* N */ "{E}, med opphold",
/* O */ "{E}, deretter ustabilt",
/* P */ "Variabelt, litt {P}",
/* Q */ "Ustabilt, korte fine perioder",
/* R */ "Ustabilt, {P} senere",
/* S */ "Ustabilt, {P} av og til",
/* T */ "Veldig ustabilt, men noen bedringer",
/* U */ "{P} av og til, deretter forverring",
/* V */ "{P} av og til, sterkt ustabilt",
/* W */ "{P} ofte",
/* X */ "Veldig ustabilt, med {P}",
/* Y */ "Stormfullt, mulig bedring",
/* Z */ "Stormfullt, mye {P}",
/* 0 */ "Batteri tomt, lad opp!"
};

#endif // TRANSLATION_NO_H
