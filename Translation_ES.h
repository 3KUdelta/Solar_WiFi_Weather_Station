/*----------------------------------------------------------------------------------------------------
Translation_ES.h - Spanish translation
Project: Solar Powered WiFi Weather Station V2.6+

See Translation_DE.h for the architecture description and translator notes.
****************************************************************************/

#ifndef TRANSLATION_ES_H
#define TRANSLATION_ES_H

// ----- Language metadata -----
const char LANG_NAME[] = "ES";
const char LANG_FORECAST_LABEL[] = "Previsión";
const char LANG_ACCURACY_LABEL[] = "Fiabilidad de la previsión";
const char LANG_PRESSURE_LABEL[] = "Presión";
const char LANG_FORECAST_DEFAULT[]= "Lo sentimos, no hay previsión por el momento";

// ----- Precipitation words -----
// {P} placeholder gets replaced with one of these:
const char LANG_PRECIP_P_SUMMER[] = "lluvia";
const char LANG_PRECIP_P_WINTER[] = "nieve";
// {E} placeholder gets replaced with one of these:
const char LANG_PRECIP_E_SUMMER[] = "chubascos";
const char LANG_PRECIP_E_WINTER[] = "nevadas";

// ----- Trends: index 0=RISING_FAST .. 6=FALLING_FAST -----
const char* const LANG_TRENDS[7] = {
"subiendo rápidamente", // 0
"subiendo", // 1
"subiendo ligeramente", // 2
"estable", // 3
"bajando ligeramente", // 4
"bajando", // 5
"bajando rápidamente" // 6
};

// ----- Pressure states: index 0=STORM_LOW .. 4=STRONG_HIGH -----
const char* const LANG_PRESSURE[5] = {
"presión muy baja", // 0
"presión baja", // 1
"presión normal", // 2
"alta presión", // 3
"presión muy alta" // 4
};

// ----- Zambretti forecasts: A..Z + '0' (battery) -----
const char* const LANG_ZAMBRETTI[27] = {
/* A */ "Tiempo estable y agradable",
/* B */ "Buen tiempo",
/* C */ "El tiempo mejora",
/* D */ "Buen tiempo, pero empieza a cambiar",
/* E */ "Buen tiempo, posibles {E}",
/* F */ "Bastante bueno, con tendencia a mejorar",
/* G */ "Bastante bueno, posibles {E} por la mañana",
/* H */ "Bastante bueno, {E} más tarde",
/* I */ "{E} por la mañana, luego mejora",
/* J */ "Tiempo variable, pero mejorando",
/* K */ "Bastante bueno, con {E} probables",
/* L */ "Más bien inestable, con claros más tarde",
/* M */ "Inestable, pero probablemente mejorando",
/* N */ "{E}, con ratos de sol",
/* O */ "{E}, y luego se vuelve inestable",
/* P */ "Tiempo variable, con algo de {P}",
/* Q */ "Inestable, con breves intervalos de buen tiempo",
/* R */ "Inestable, {P} más tarde",
/* S */ "Inestable, con {P} de vez en cuando",
/* T */ "Muy inestable, pero con momentos mejores",
/* U */ "{P} de vez en cuando, luego empeora",
/* V */ "{P} de vez en cuando, volviéndose muy inestable",
/* W */ "{P} a intervalos frecuentes",
/* X */ "Muy inestable, con {P}",
/* Y */ "Tormentoso, posible mejora",
/* Z */ "Tormentoso, con mucha {P}",
/* 0 */ "Batería vacía, ¡por favor recárgala!"
};

#endif // TRANSLATION_ES_H
