/*----------------------------------------------------------------------------------------------------
Translation_FR.h - French translation
Project: Solar Powered WiFi Weather Station V2.6+

See Translation_DE.h for the architecture description and translator notes.
****************************************************************************/

#ifndef TRANSLATION_FR_H
#define TRANSLATION_FR_H

// ----- Language metadata -----
const char LANG_NAME[] = "FR";
const char LANG_FORECAST_LABEL[] = "Prévisions";
const char LANG_ACCURACY_LABEL[] = "Fiabilité des prévisions";
const char LANG_PRESSURE_LABEL[] = "Pression";
const char LANG_FORECAST_DEFAULT[]= "Désolé, aucune prévision pour le moment";

// ----- Precipitation words -----
// {P} placeholder gets replaced with one of these:
const char LANG_PRECIP_P_SUMMER[] = "pluie";
const char LANG_PRECIP_P_WINTER[] = "neige";
// {E} placeholder gets replaced with one of these:
const char LANG_PRECIP_E_SUMMER[] = "averses";
const char LANG_PRECIP_E_WINTER[] = "chutes de neige";

// ----- Trends: index 0=RISING_FAST .. 6=FALLING_FAST -----
const char* const LANG_TRENDS[7] = {
"en forte hausse", // 0
"en hausse", // 1
"en légère hausse", // 2
"stable", // 3
"en légère baisse", // 4
"en baisse", // 5
"en forte baisse" // 6
};

// ----- Pressure states: index 0=STORM_LOW .. 4=STRONG_HIGH -----
const char* const LANG_PRESSURE[5] = {
"pression très basse", // 0
"pression basse", // 1
"pression normale", // 2
"haute pression", // 3
"pression très élevée" // 4
};

// ----- Zambretti forecasts: A..Z + '0' (battery) -----
const char* const LANG_ZAMBRETTI[27] = {
/* A */ "Beau temps stable",
/* B */ "Beau temps",
/* C */ "Le temps s'améliore",
/* D */ "Beau, mais commence à changer",
/* E */ "Beau, possibles {E}",
/* F */ "Assez beau, tendance à l'amélioration",
/* G */ "Assez beau, possibles {E} le matin",
/* H */ "Assez beau, {E} plus tard",
/* I */ "{E} le matin, puis amélioration",
/* J */ "Temps variable, mais en amélioration",
/* K */ "Assez beau, {E} probables",
/* L */ "Plutôt instable, éclaircies plus tard",
/* M */ "Instable, mais probablement en amélioration",
/* N */ "{E}, avec des éclaircies",
/* O */ "{E}, puis devient instable",
/* P */ "Temps variable, avec un peu de {P}",
/* Q */ "Instable, avec de courtes périodes de beau temps",
/* R */ "Instable, {P} plus tard",
/* S */ "Instable, {P} par moments",
/* T */ "Très instable, mais avec quelques embellies",
/* U */ "{P} par moments, puis dégradation",
/* V */ "{P} par moments, avec nette dégradation",
/* W */ "{P} à intervalles fréquents",
/* X */ "Très instable, avec {P}",
/* Y */ "Orageux, amélioration possible",
/* Z */ "Orageux, avec beaucoup de {P}",
/* 0 */ "Batterie vide, veuillez la recharger !"
};

#endif // TRANSLATION_FR_H
