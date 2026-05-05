/*----------------------------------------------------------------------------------------------------
Translation_IT.h - Italian translation
Project: Solar Powered WiFi Weather Station V2.6+

See Translation_DE.h for the architecture description and translator notes.
****************************************************************************/

#ifndef TRANSLATION_IT_H
#define TRANSLATION_IT_H

// ----- Language metadata -----
const char LANG_NAME[] = "IT";
const char LANG_FORECAST_LABEL[] = "Previsioni";
const char LANG_ACCURACY_LABEL[] = "Affidabilità della previsione";
const char LANG_PRESSURE_LABEL[] = "Pressione";
const char LANG_FORECAST_DEFAULT[]= "Spiacenti, al momento nessuna previsione disponibile";

// ----- Precipitation words -----
// {P} placeholder gets replaced with one of these:
const char LANG_PRECIP_P_SUMMER[] = "pioggia";
const char LANG_PRECIP_P_WINTER[] = "neve";
// {E} placeholder gets replaced with one of these:
const char LANG_PRECIP_E_SUMMER[] = "rovesci";
const char LANG_PRECIP_E_WINTER[] = "nevicate";

// ----- Trends: index 0=RISING_FAST .. 6=FALLING_FAST -----
const char* const LANG_TRENDS[7] = {
"in rapido aumento", // 0
"in aumento", // 1
"in lieve aumento", // 2
"stabile", // 3
"in lieve calo", // 4
"in calo", // 5
"in rapido calo" // 6
};

// ----- Pressure states: index 0=STORM_LOW .. 4=STRONG_HIGH -----
const char* const LANG_PRESSURE[5] = {
"pressione molto bassa", // 0
"pressione bassa", // 1
"pressione normale", // 2
"alta pressione", // 3
"pressione molto alta" // 4
};

// ----- Zambretti forecasts: A..Z + '0' (battery) -----
const char* const LANG_ZAMBRETTI[27] = {
/* A */ "Bel tempo stabile",
/* B */ "Bel tempo",
/* C */ "Tempo in miglioramento",
/* D */ "Bel tempo, ma inizia a cambiare",
/* E */ "Bel tempo, possibili {E}",
/* F */ "Abbastanza bello, in miglioramento",
/* G */ "Abbastanza bello, possibili {E} al mattino",
/* H */ "Abbastanza bello, {E} più tardi",
/* I */ "{E} al mattino, poi in miglioramento",
/* J */ "Tempo variabile, ma in miglioramento",
/* K */ "Abbastanza bello, con {E} probabili",
/* L */ "Piuttosto instabile, poi schiarite più tardi",
/* M */ "Instabile, ma probabilmente in miglioramento",
/* N */ "{E}, con schiarite",
/* O */ "{E} e poi tempo instabile",
/* P */ "Tempo variabile, con un po' di {P}",
/* Q */ "Instabile, con brevi periodi di bel tempo",
/* R */ "Instabile, con {P} più tardi",
/* S */ "Instabile, con {P} a tratti",
/* T */ "Molto instabile, ma con momenti migliori",
/* U */ "{P} a tratti, poi peggioramento",
/* V */ "{P} a tratti, in netto peggioramento",
/* W */ "{P} frequenti",
/* X */ "Molto instabile, con {P}",
/* Y */ "Temporalesco, ma possibile miglioramento",
/* Z */ "Temporalesco, con molta {P}",
/* 0 */ "Batteria scarica, ricaricala!"
};

#endif // TRANSLATION_IT_H
