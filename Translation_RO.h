/*----------------------------------------------------------------------------------------------------
Translation_RO.h - Romanian translation
Project: Solar Powered WiFi Weather Station V2.6+

See Translation_DE.h for the architecture description and translator notes.
****************************************************************************/

#ifndef TRANSLATION_RO_H
#define TRANSLATION_RO_H

// ----- Language metadata -----
const char LANG_NAME[] = "RO";
const char LANG_FORECAST_LABEL[] = "Prognoză";
const char LANG_ACCURACY_LABEL[] = "Precizia prognozei";
const char LANG_PRESSURE_LABEL[] = "Presiune";
const char LANG_FORECAST_DEFAULT[]= "Ne pare rău, momentan nu există prognoză";

// ----- Precipitation words -----
// {P} placeholder gets replaced with one of these:
const char LANG_PRECIP_P_SUMMER[] = "ploaie";
const char LANG_PRECIP_P_WINTER[] = "zăpadă";
// {E} placeholder gets replaced with one of these:
const char LANG_PRECIP_E_SUMMER[] = "averse";
const char LANG_PRECIP_E_WINTER[] = "ninsoare";

// ----- Trends: index 0=RISING_FAST .. 6=FALLING_FAST -----
const char* const LANG_TRENDS[7] = {
"în creștere rapidă", // 0
"în creștere", // 1
"în ușoară creștere", // 2
"constant", // 3
"în ușoară scădere", // 4
"în scădere", // 5
"în scădere rapidă" // 6
};

// ----- Pressure states: index 0=STORM_LOW .. 4=STRONG_HIGH -----
const char* const LANG_PRESSURE[5] = {
"presiune foarte joasă", // 0
"presiune joasă", // 1
"presiune normală", // 2
"presiune ridicată", // 3
"presiune foarte ridicată" // 4
};

// ----- Zambretti forecasts: A..Z + '0' (battery) -----
const char* const LANG_ZAMBRETTI[27] = {
/* A */ "Vreme stabilă și frumoasă",
/* B */ "Vreme frumoasă",
/* C */ "Vremea se îmbunătățește",
/* D */ "Frumos, dar începe să se schimbe",
/* E */ "Frumos, posibil {E}",
/* F */ "Destul de frumos, cu tendință de îmbunătățire",
/* G */ "Destul de frumos, posibil {E} dimineața",
/* H */ "Destul de frumos, {E} mai târziu",
/* I */ "{E} dimineața, apoi vremea se îmbunătățește",
/* J */ "Vreme schimbătoare, dar în îmbunătățire",
/* K */ "Destul de frumos, cu {E} probabil",
/* L */ "Mai degrabă instabil, dar se limpezește mai târziu",
/* M */ "Instabil, dar probabil în îmbunătățire",
/* N */ "{E}, cu perioade senine",
/* O */ "{E} și apoi devine instabil",
/* P */ "Vreme schimbătoare, cu ceva {P}",
/* Q */ "Instabil, cu intervale scurte de vreme bună",
/* R */ "Instabil, cu {P} mai târziu",
/* S */ "Instabil, cu {P} din când în când",
/* T */ "Foarte instabil, dar cu momente mai bune",
/* U */ "{P} din când în când, apoi vremea se înrăutățește",
/* V */ "{P} din când în când, devenind foarte instabil",
/* W */ "{P} la intervale frecvente",
/* X */ "Foarte instabil, cu {P}",
/* Y */ "Furtunos, dar posibil în îmbunătățire",
/* Z */ "Furtunos, cu multă {P}",
/* 0 */ "Bateria este descărcată, te rog reîncarcă!"
};

#endif // TRANSLATION_RO_H
