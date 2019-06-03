/*----------------------------------------------------------------------------------------------------
  Project Name : Solar Powered WiFi Weather Station V2.31
  Features: temperature, dewpoint, dewpoint spread, heat index, humidity, absolute pressure, relative pressure, battery status and
  the famous Zambretti Forecaster (multi lingual)
  Authors: Keith Hungerford, Debasish Dutta and Marc Stähli
  Website : www.opengreenenergy.com
  
******* Transaltion tables ****************************************/

#if LANGUAGE == 'EN'
  const char TEXT_RISING_FAST[]       = "rising fast";
  const char TEXT_RISING[]            = "rising";
  const char TEXT_RISING_SLOW[]       = "rising slow";
  const char TEXT_STEADY[]            = "steady";
  const char TEXT_FALLING_SLOW[]      = "falling slow";
  const char TEXT_FALLING[]           = "falling";
  const char TEXT_FALLING_FAST[]      = "falling fast";

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
  const char TEXT_RISING_FAST[]       = "rasch steigend";
  const char TEXT_RISING[]            = "steigend";
  const char TEXT_RISING_SLOW[]       = "langsam steigend";
  const char TEXT_STEADY[]            = "beständig";
  const char TEXT_FALLING_SLOW[]      = "langsam fallend";
  const char TEXT_FALLING[]           = "fallend";
  const char TEXT_FALLING_FAST[]      = "rasch fallend";

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
#endif
