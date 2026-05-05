/*----------------------------------------------------------------------------------------------------
Translation_PL.h - Polish translation
Project: Solar Powered WiFi Weather Station V2.6+

See Translation_DE.h for the architecture description and translator notes.
****************************************************************************/

#ifndef TRANSLATION_PL_H
#define TRANSLATION_PL_H

// ----- Language metadata -----
const char LANG_NAME[] = "PL";
const char LANG_FORECAST_LABEL[] = "Prognoza";
const char LANG_ACCURACY_LABEL[] = "Dokładność prognozy";
const char LANG_PRESSURE_LABEL[] = "Ciśnienie";
const char LANG_FORECAST_DEFAULT[]= "Przepraszamy, obecnie brak prognozy";

// ----- Precipitation words -----
// {P} placeholder gets replaced with one of these:
const char LANG_PRECIP_P_SUMMER[] = "deszcz";
const char LANG_PRECIP_P_WINTER[] = "śnieg";
// {E} placeholder gets replaced with one of these:
const char LANG_PRECIP_E_SUMMER[] = "przelotne opady";
const char LANG_PRECIP_E_WINTER[] = "opady śniegu";

// ----- Trends: index 0=RISING_FAST .. 6=FALLING_FAST -----
const char* const LANG_TRENDS[7] = {
"szybko rośnie", // 0
"rośnie", // 1
"lekko rośnie", // 2
"stabilne", // 3
"lekko spada", // 4
"spada", // 5
"szybko spada" // 6
};

// ----- Pressure states: index 0=STORM_LOW .. 4=STRONG_HIGH -----
const char* const LANG_PRESSURE[5] = {
"bardzo niskie ciśnienie", // 0
"niskie ciśnienie", // 1
"normalne ciśnienie", // 2
"wysokie ciśnienie", // 3
"bardzo wysokie ciśnienie" // 4
};

// ----- Zambretti forecasts: A..Z + '0' (battery) -----
const char* const LANG_ZAMBRETTI[27] = {
/* A */ "Stabilna, piękna pogoda",
/* B */ "Piękna pogoda",
/* C */ "Pogoda się poprawia",
/* D */ "Pięknie, ale zaczyna się zmieniać",
/* E */ "Pięknie, możliwe {E}",
/* F */ "Dość ładnie, z tendencją poprawy",
/* G */ "Dość ładnie, możliwe {E} rano",
/* H */ "Dość ładnie, {E} później",
/* I */ "{E} rano, potem poprawa",
/* J */ "Pogoda zmienna, ale się poprawia",
/* K */ "Dość ładnie, możliwe {E}",
/* L */ "Raczej niestabilnie, później przejaśnienia",
/* M */ "Niestabilnie, ale prawdopodobnie poprawa",
/* N */ "{E}, z przejaśnieniami",
/* O */ "{E}, potem pogorszenie",
/* P */ "Pogoda zmienna, z pewnym {P}",
/* Q */ "Niestabilnie, krótkie okresy ładnej pogody",
/* R */ "Niestabilnie, {P} później",
/* S */ "Niestabilnie, {P} od czasu do czasu",
/* T */ "Bardzo niestabilnie, ale chwilami lepiej",
/* U */ "{P} od czasu do czasu, potem pogorszenie",
/* V */ "{P} od czasu do czasu, z wyraźnym pogorszeniem",
/* W */ "{P} w częstych odstępach",
/* X */ "Bardzo niestabilnie, z {P}",
/* Y */ "Burzowo, możliwa poprawa",
/* Z */ "Burzowo, z dużą ilością {P}",
/* 0 */ "Bateria rozładowana, proszę naładować!"
};

#endif // TRANSLATION_PL_H
