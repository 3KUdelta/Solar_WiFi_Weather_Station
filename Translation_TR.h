/*----------------------------------------------------------------------------------------------------
Translation_TR.h - Turkish translation
Project: Solar Powered WiFi Weather Station V2.6+

See Translation_DE.h for the architecture description and translator notes.
****************************************************************************/

#ifndef TRANSLATION_TR_H
#define TRANSLATION_TR_H

// ----- Language metadata -----
const char LANG_NAME[] = "TR";
const char LANG_FORECAST_LABEL[] = "Tahmin";
const char LANG_ACCURACY_LABEL[] = "Tahmin doğruluğu";
const char LANG_PRESSURE_LABEL[] = "Basınç";
const char LANG_FORECAST_DEFAULT[]= "Üzgünüz, şu anda tahmin yok";

// ----- Precipitation words -----
// {P} placeholder gets replaced with one of these:
const char LANG_PRECIP_P_SUMMER[] = "yağmur";
const char LANG_PRECIP_P_WINTER[] = "kar";
// {E} placeholder gets replaced with one of these:
const char LANG_PRECIP_E_SUMMER[] = "sağanak";
const char LANG_PRECIP_E_WINTER[] = "kar yağışı";

// ----- Trends: index 0=RISING_FAST .. 6=FALLING_FAST -----
const char* const LANG_TRENDS[7] = {
"hızla yükseliyor", // 0
"yükseliyor", // 1
"hafif yükseliyor", // 2
"sabit", // 3
"hafif düşüyor", // 4
"düşüyor", // 5
"hızla düşüyor" // 6
};

// ----- Pressure states: index 0=STORM_LOW .. 4=STRONG_HIGH -----
const char* const LANG_PRESSURE[5] = {
"çok düşük basınç", // 0
"düşük basınç", // 1
"normal basınç", // 2
"yüksek basınç", // 3
"çok yüksek basınç" // 4
};

// ----- Zambretti forecasts: A..Z + '0' (battery) -----
const char* const LANG_ZAMBRETTI[27] = {
/* A */ "Sürekli güzel hava",
/* B */ "Güzel hava",
/* C */ "Hava düzeliyor",
/* D */ "Güzel, ama değişmeye başlıyor",
/* E */ "Güzel, {E} ihtimali var",
/* F */ "Oldukça güzel, iyileşme eğiliminde",
/* G */ "Oldukça güzel, sabah {E} ihtimali var",
/* H */ "Oldukça güzel, daha sonra {E}",
/* I */ "Sabah {E}, ardından hava düzeliyor",
/* J */ "Değişken hava, ama iyileşiyor",
/* K */ "Oldukça güzel, {E} olası",
/* L */ "Biraz dengesiz, sonra açılıyor",
/* M */ "Dengesiz, ama muhtemelen düzeliyor",
/* N */ "{E}, aralarda güneş açıyor",
/* O */ "{E}, ardından dengesizleşiyor",
/* P */ "Değişken hava, biraz {P}",
/* Q */ "Dengesiz, kısa süreli güzel hava araları",
/* R */ "Dengesiz, daha sonra {P}",
/* S */ "Dengesiz, zaman zaman {P}",
/* T */ "Çok dengesiz, ama zaman zaman daha iyi",
/* U */ "Zaman zaman {P}, sonra kötüleşiyor",
/* V */ "Zaman zaman {P}, çok dengesiz hale geliyor",
/* W */ "Sık aralıklarla {P}",
/* X */ "Çok dengesiz, {P}",
/* Y */ "Fırtınalı, düzelme ihtimali var",
/* Z */ "Fırtınalı, bol {P}",
/* 0 */ "Pil boş, lütfen şarj edin!"
};

#endif // TRANSLATION_TR_H
