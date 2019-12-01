/*----------------------------------------------------------------------------------------------------
  Project Name : Solar Powered WiFi Weather Station V2.31
  Features: temperature, dewpoint, dewpoint spread, heat index, humidity, absolute pressure, relative pressure, battery status and
  the famous Zambretti Forecaster (multi lingual)
  Authors: Keith Hungerford, Debasish Dutta and Marc Stähli
  Website : www.opengreenenergy.com
  
******* Transaltion tables ****************************************/

#if LANGUAGE == 'EN'
  const char TEXT_AIR_PRESSURE[]      = "Pressure";
  const char TEXT_RISING_FAST[]       = "rising fast";
  const char TEXT_RISING[]            = "rising";
  const char TEXT_RISING_SLOW[]       = "rising slow";
  const char TEXT_STEADY[]            = "steady";
  const char TEXT_FALLING_SLOW[]      = "falling slow";
  const char TEXT_FALLING[]           = "falling";
  const char TEXT_FALLING_FAST[]      = "falling fast";
  
  const char TEXT_ZAMBRETTI_FORECAST[]= "Forecast";
  const char TEXT_ZAMBRETTI_ACCURACY[]= "Prediction accuracy";
  const char TEXT_ZAMBRETTI_A[]       = "Settled Fine Weather";
  const char TEXT_ZAMBRETTI_B[]       = "Fine Weather";
  const char TEXT_ZAMBRETTI_C[]       = "Becoming Fine";
  const char TEXT_ZAMBRETTI_D[]       = "Fine, Becoming Less Settled";
  const char TEXT_ZAMBRETTI_E[]       = "Fine, Possibly showers";
  const char TEXT_ZAMBRETTI_F[]       = "Fairly Fine, Improving";
  const char TEXT_ZAMBRETTI_G[]       = "Fairly Fine, Possibly showers early";
  const char TEXT_ZAMBRETTI_H[]       = "Fairly Fine, Showers Later";
  const char TEXT_ZAMBRETTI_I[]       = "Showery Early, Improving";
  const char TEXT_ZAMBRETTI_J[]       = "Changeable Improving";
  const char TEXT_ZAMBRETTI_K[]       = "Fairly Fine, Showers likely";
  const char TEXT_ZAMBRETTI_L[]       = "Rather Unsettled Clearing Later";
  const char TEXT_ZAMBRETTI_M[]       = "Unsettled, Probably Improving";
  const char TEXT_ZAMBRETTI_N[]       = "Showery Bright Intervals";
  const char TEXT_ZAMBRETTI_O[]       = "Showery Becoming Unsettled";
  const char TEXT_ZAMBRETTI_P[]       = "Changeable some rain";
  const char TEXT_ZAMBRETTI_Q[]       = "Unsettled, short fine Intervals";
  const char TEXT_ZAMBRETTI_R[]       = "Unsettled, Rain later";
  const char TEXT_ZAMBRETTI_S[]       = "Unsettled, rain at times";
  const char TEXT_ZAMBRETTI_T[]       = "Very Unsettled, Finer at times";
  const char TEXT_ZAMBRETTI_U[]       = "Rain at times, Worse later";
  const char TEXT_ZAMBRETTI_V[]       = "Rain at times, becoming very unsettled";
  const char TEXT_ZAMBRETTI_W[]       = "Rain at Frequent Intervals";
  const char TEXT_ZAMBRETTI_X[]       = "Very Unsettled, Rain";
  const char TEXT_ZAMBRETTI_Y[]       = "Stormy, possibly improving";
  const char TEXT_ZAMBRETTI_Z[]       = "Stormy, much rain";
  const char TEXT_ZAMBRETTI_DEFAULT[] = "Sorry, no forecast for the moment";

#elif LANGUAGE == 'DE'
  const char TEXT_AIR_PRESSURE[]      = "Luftdruck";
  const char TEXT_RISING_FAST[]       = "rasch steigend";
  const char TEXT_RISING[]            = "steigend";
  const char TEXT_RISING_SLOW[]       = "langsam steigend";
  const char TEXT_STEADY[]            = "beständig";
  const char TEXT_FALLING_SLOW[]      = "langsam fallend";
  const char TEXT_FALLING[]           = "fallend";
  const char TEXT_FALLING_FAST[]      = "rasch fallend";
  
  const char TEXT_ZAMBRETTI_FORECAST[]= "Wettervorhersage";
  const char TEXT_ZAMBRETTI_ACCURACY[]= "Vorhersagegenauigkeit";
  const char TEXT_ZAMBRETTI_A[]       = "Beständiges Schönwetter";
  const char TEXT_ZAMBRETTI_B[]       = "Schönes Wetter";
  const char TEXT_ZAMBRETTI_C[]       = "Wetter wird gut";
  const char TEXT_ZAMBRETTI_D[]       = "Schön, wird wechselhaft";
  const char TEXT_ZAMBRETTI_E[]       = "Schön, Regenschauer möglich";
  const char TEXT_ZAMBRETTI_F[]       = "Ziemlich gut, verbessert sich";
  const char TEXT_ZAMBRETTI_G[]       = "Ziemlich gut, frühe Regenschauer möglich";
  const char TEXT_ZAMBRETTI_H[]       = "Ziemlich gut, spätere Regenschauer";
  const char TEXT_ZAMBRETTI_I[]       = "Früh schauerhaft, verbessert sich";
  const char TEXT_ZAMBRETTI_J[]       = "Wechselhaft, verbessert sich";
  const char TEXT_ZAMBRETTI_K[]       = "Ziemlich gut, Regenschauer möglich";
  const char TEXT_ZAMBRETTI_L[]       = "Eher veränderlich, klart später auf";
  const char TEXT_ZAMBRETTI_M[]       = "Veränderlich, verbessert sich wahrscheinlich";
  const char TEXT_ZAMBRETTI_N[]       = "Regnerisch mit Aufhellungen";
  const char TEXT_ZAMBRETTI_O[]       = "Regnerisch, wird veränderlich";
  const char TEXT_ZAMBRETTI_P[]       = "Veränderlich mit wenig Regen";
  const char TEXT_ZAMBRETTI_Q[]       = "Veränderlich, mit kurzen schönen Intervallen";
  const char TEXT_ZAMBRETTI_R[]       = "Veränderlich, später Regen";
  const char TEXT_ZAMBRETTI_S[]       = "Veränderlich, zeitweise Regen";
  const char TEXT_ZAMBRETTI_T[]       = "Stark wechselnd, zeitweise schöner";
  const char TEXT_ZAMBRETTI_U[]       = "Zeitweise Regen, verschlechtert sich";
  const char TEXT_ZAMBRETTI_V[]       = "Zeitweise Regen, wird sehr unruhig";
  const char TEXT_ZAMBRETTI_W[]       = "Regen in regelmässigen Abständen";
  const char TEXT_ZAMBRETTI_X[]       = "Sehr veränderlich, Regen";
  const char TEXT_ZAMBRETTI_Y[]       = "Stürmisch, verbessert sich wahrscheinlich";
  const char TEXT_ZAMBRETTI_Z[]       = "Stürmisch, viel Regen";
  const char TEXT_ZAMBRETTI_DEFAULT[] = "Im Moment keine Prognose möglich";

#elif LANGUAGE == 'IT'
  const char TEXT_AIR_PRESSURE[]      = "Pressione atmosferica";
  const char TEXT_RISING_FAST[]       = "in rapido aumento";
  const char TEXT_RISING[]            = "in aumento";
  const char TEXT_RISING_SLOW[]       = "in lento aumento";
  const char TEXT_STEADY[]            = "stabile";
  const char TEXT_FALLING_SLOW[]      = "in lenta discesa";
  const char TEXT_FALLING[]           = "in discesa";
  const char TEXT_FALLING_FAST[]      = "in rapida discesa";


  const char TEXT_ZAMBRETTI_FORECAST[]= "Previsioni meteo";
  const char TEXT_ZAMBRETTI_ACCURACY[]= "Precisione";
  const char TEXT_ZAMBRETTI_A[]       = "Bel tempo stabile";
  const char TEXT_ZAMBRETTI_B[]       = "Bel tempo";
  const char TEXT_ZAMBRETTI_C[]       = "In miglioramento";
  const char TEXT_ZAMBRETTI_D[]       = "Bel tempo con possibile peggioramento";
  const char TEXT_ZAMBRETTI_E[]       = "Bel tempo con possibili rovesci";
  const char TEXT_ZAMBRETTI_F[]       = "Tempo abbastanza buono, in miglioramento";
  const char TEXT_ZAMBRETTI_G[]       = "Tempo abbastanza buono ma con possibili piogge a breve";
  const char TEXT_ZAMBRETTI_H[]       = "Tempo abbastanza buono ma con piogge a breve";
  const char TEXT_ZAMBRETTI_I[]       = "Piogge a breve ma in miglioramento";
  const char TEXT_ZAMBRETTI_J[]       = "Variabile";
  const char TEXT_ZAMBRETTI_K[]       = "Tempo abbastanza buono con probabili piogge";
  const char TEXT_ZAMBRETTI_L[]       = "Tempo incerto con schiarite a breve";
  const char TEXT_ZAMBRETTI_M[]       = "Tempo incerto con probabili miglioramenti";
  const char TEXT_ZAMBRETTI_N[]       = "Piovoso con schiarite ad intervalli";
  const char TEXT_ZAMBRETTI_O[]       = "Uggioso ed incerto";
  const char TEXT_ZAMBRETTI_P[]       = "Variabile con rovesci di piogge";
  const char TEXT_ZAMBRETTI_Q[]       = "Incerto con brevi intervalli di bel tempo";
  const char TEXT_ZAMBRETTI_R[]       = "Incerto con possibili rovesci a breve";
  const char TEXT_ZAMBRETTI_S[]       = "Incerto con piaggia a tratti";
  const char TEXT_ZAMBRETTI_T[]       = "Molto incerto con sprazzi di bel tempo";
  const char TEXT_ZAMBRETTI_U[]       = "Pioggia a tratti in peggioramento";
  const char TEXT_ZAMBRETTI_V[]       = "Pioggia a tratti con forte instabilità";
  const char TEXT_ZAMBRETTI_W[]       = "Pioggia a intervalli frequenti";
  const char TEXT_ZAMBRETTI_X[]       = "Molto instabile, Pioggia";
  const char TEXT_ZAMBRETTI_Y[]       = "Tempestoso con possibili miglioramenti";
  const char TEXT_ZAMBRETTI_Z[]       = "Tempestoso e moto piovoso";
  const char TEXT_ZAMBRETTI_DEFAULT[] = "Mi dispiace, nessuna previsione disponibile al momento";

#elif LANGUAGE == 'PL'
  const char TEXT_AIR_PRESSURE[]      = "Ciśnienie";
  const char TEXT_RISING_FAST[]       = "szybko rośnie";
  const char TEXT_RISING[]            = "rośnie";
  const char TEXT_RISING_SLOW[]       = "powoli rośnie";
  const char TEXT_STEADY[]            = "stałe";
  const char TEXT_FALLING_SLOW[]      = "powoli spada";
  const char TEXT_FALLING[]           = "spada";
  const char TEXT_FALLING_FAST[]      = "szybko spada";

  const char TEXT_ZAMBRETTI_FORECAST[]= "Prognoza";
  const char TEXT_ZAMBRETTI_ACCURACY[]= "Dokładność";
  const char TEXT_ZAMBRETTI_A[]       = "Dobra pogoda, stabilna";
  const char TEXT_ZAMBRETTI_B[]       = "Dobra pogoda";
  const char TEXT_ZAMBRETTI_C[]       = "Poprawia się";
  const char TEXT_ZAMBRETTI_D[]       = "Dobra, pogarsza się";
  const char TEXT_ZAMBRETTI_E[]       = "Dobra, możliwy deszcz";
  const char TEXT_ZAMBRETTI_F[]       = "Całkiem dobra, polepsza się";
  const char TEXT_ZAMBRETTI_G[]       = "Całkiem dobra, wkrótce możliwe deszcze";
  const char TEXT_ZAMBRETTI_H[]       = "Całkiem dobra, później deszcze";
  const char TEXT_ZAMBRETTI_I[]       = "Wkrótce deszczowo, polepsza się";
  const char TEXT_ZAMBRETTI_J[]       = "Zmienna, polepsza się";
  const char TEXT_ZAMBRETTI_K[]       = "Całkiem dobra, możliwe deszcze";
  const char TEXT_ZAMBRETTI_L[]       = "Raczej niepewna, później rozpogodzi się";
  const char TEXT_ZAMBRETTI_M[]       = "Niepewna, możliwe rozpogodzenia";
  const char TEXT_ZAMBRETTI_N[]       = "Deszcze z przejaśnieniami";
  const char TEXT_ZAMBRETTI_O[]       = "Deszczowo, staje się niepewna";
  const char TEXT_ZAMBRETTI_P[]       = "Zmienna, niewielkie opady";
  const char TEXT_ZAMBRETTI_Q[]       = "Niepewna, krótkie przejaśnienia";
  const char TEXT_ZAMBRETTI_R[]       = "Niepewna, później deszcz";
  const char TEXT_ZAMBRETTI_S[]       = "Niepewna, okresowe opady";
  const char TEXT_ZAMBRETTI_T[]       = "Bardzo niepewna, z przejaśnieniami";
  const char TEXT_ZAMBRETTI_U[]       = "Okresowe opady, później się pogorszy";
  const char TEXT_ZAMBRETTI_V[]       = "Okresowe opady, staje się bardzo niepewna";
  const char TEXT_ZAMBRETTI_W[]       = "Częste opady";
  const char TEXT_ZAMBRETTI_X[]       = "Bardzo niepewna, deszcz";
  const char TEXT_ZAMBRETTI_Y[]       = "Burzowa, możliwe polepszenie";
  const char TEXT_ZAMBRETTI_Z[]       = "Burzowa, duże opady deszczu";
  const char TEXT_ZAMBRETTI_DEFAULT[] = "Obecnie brak prognozy";

#endif
