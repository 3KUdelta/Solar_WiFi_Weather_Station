/*----------------------------------------------------------------------------------------------------
  Project Name : Solar Powered WiFi Weather Station V2.4x
  Features: temperature, dewpoint, dewpoint spread, heat index, humidity, absolute pressure, relative pressure, battery status and
  the famous Zambretti Forecaster (multi lingual)
  Authors: Keith Hungerford, Debasish Dutta and Marc Stähli
  Website : www.opengreenenergy.com

******* Transaltion tables ****************************************/
//output variables pressure
String TEXT_AIR_PRESSURE;
String TEXT_RISING_FAST;
String TEXT_RISING;
String TEXT_RISING_SLOW;
String TEXT_STEADY;
String TEXT_FALLING_SLOW;
String TEXT_FALLING;
String TEXT_FALLING_FAST;

String TEXT_STRONG_HIGH;
String TEXT_HIGHPRESSURE;
String TEXT_LOWPRESSURE;
String TEXT_STRONG_LOW;
String TEXT_STORM_LOW;

//output variables forecast in words
String TEXT_ZAMBRETTI_FORECAST;
String TEXT_ZAMBRETTI_ACCURACY;
String TEXT_ZAMBRETTI_A;
String TEXT_ZAMBRETTI_B;
String TEXT_ZAMBRETTI_C;
String TEXT_ZAMBRETTI_D;
String TEXT_ZAMBRETTI_E;
String TEXT_ZAMBRETTI_F;
String TEXT_ZAMBRETTI_G;
String TEXT_ZAMBRETTI_H;
String TEXT_ZAMBRETTI_I;
String TEXT_ZAMBRETTI_J;
String TEXT_ZAMBRETTI_K;
String TEXT_ZAMBRETTI_L;
String TEXT_ZAMBRETTI_M;
String TEXT_ZAMBRETTI_N;
String TEXT_ZAMBRETTI_O;
String TEXT_ZAMBRETTI_P;
String TEXT_ZAMBRETTI_Q;
String TEXT_ZAMBRETTI_R;
String TEXT_ZAMBRETTI_S;
String TEXT_ZAMBRETTI_T;
String TEXT_ZAMBRETTI_U;
String TEXT_ZAMBRETTI_V;
String TEXT_ZAMBRETTI_W;
String TEXT_ZAMBRETTI_X;
String TEXT_ZAMBRETTI_Y;
String TEXT_ZAMBRETTI_Z;
String TEXT_ZAMBRETTI_0;
String TEXT_ZAMBRETTI_DEFAULT;

void translate() {

  if (LANGUAGE == "EN") {
    TEXT_AIR_PRESSURE      = "Pressure";
    TEXT_RISING_FAST       = "rising fast";
    TEXT_RISING            = "rising";
    TEXT_RISING_SLOW       = "rising slow";
    TEXT_STEADY            = "steady";
    TEXT_FALLING_SLOW      = "falling slow";
    TEXT_FALLING           = "falling";
    TEXT_FALLING_FAST      = "falling fast";

    TEXT_STRONG_HIGH       = "strong high";
    TEXT_HIGHPRESSURE      = "high pressure";
    TEXT_LOWPRESSURE       = "low pressure";
    TEXT_STRONG_LOW        = "strong low";
    TEXT_STORM_LOW         = "stormy low";

    TEXT_ZAMBRETTI_FORECAST = "Forecast";
    TEXT_ZAMBRETTI_ACCURACY = "Prediction accuracy";
    TEXT_ZAMBRETTI_A       = "Settled Fine Weather";
    TEXT_ZAMBRETTI_B       = "Fine Weather";
    TEXT_ZAMBRETTI_C       = "Becoming Fine";
    TEXT_ZAMBRETTI_D       = "Fine, Becoming Less Settled";
    TEXT_ZAMBRETTI_E       = "Fine, Possibly showers";
    TEXT_ZAMBRETTI_G       = "Fairly Fine, Possibly showers early";
    TEXT_ZAMBRETTI_H       = "Fairly Fine, Showers Later";
    TEXT_ZAMBRETTI_I       = "Showery Early, Improving";
    TEXT_ZAMBRETTI_J       = "Changeable Improving";
    TEXT_ZAMBRETTI_K       = "Fairly Fine, Showers likely";
    TEXT_ZAMBRETTI_L       = "Rather Unsettled Clearing Later";
    TEXT_ZAMBRETTI_M       = "Unsettled, Probably Improving";
    TEXT_ZAMBRETTI_N       = "Showery Bright Intervals";
    TEXT_ZAMBRETTI_O       = "Showery Becoming Unsettled";
    TEXT_ZAMBRETTI_Q       = "Unsettled, short fine Intervals";
    TEXT_ZAMBRETTI_R       = "Unsettled, Rain later";
    TEXT_ZAMBRETTI_S       = "Unsettled, rain at times";
    TEXT_ZAMBRETTI_T       = "Very Unsettled, Finer at times";
    TEXT_ZAMBRETTI_U       = "Rain at times, Worse later";
    TEXT_ZAMBRETTI_V       = "Rain at times, becoming very unsettled";
    TEXT_ZAMBRETTI_W       = "Rain at Frequent Intervals";
    TEXT_ZAMBRETTI_X       = "Very Unsettled, Rain";
    TEXT_ZAMBRETTI_Y       = "Stormy, possibly improving";
    TEXT_ZAMBRETTI_Z       = "Stormy, much rain";
    TEXT_ZAMBRETTI_0       = "Battery empty, please recharge!";
    TEXT_ZAMBRETTI_DEFAULT = "Sorry, no forecast for the moment";
  }

  if (LANGUAGE == "DE") {
    TEXT_AIR_PRESSURE      = "Luftdruck";
    TEXT_RISING_FAST       = "rasch steigend";
    TEXT_RISING            = "steigend";
    TEXT_RISING_SLOW       = "langsam steigend";
    TEXT_STEADY            = "beständig";
    TEXT_FALLING_SLOW      = "langsam fallend";
    TEXT_FALLING           = "fallend";
    TEXT_FALLING_FAST      = "rasch fallend";

    TEXT_STRONG_HIGH       = "Kräftiges Hoch";
    TEXT_HIGHPRESSURE      = "Hochdruck";
    TEXT_LOWPRESSURE       = "Tiefdruck";
    TEXT_STRONG_LOW        = "Ausgewachsenes Tief";
    TEXT_STORM_LOW         = "Sturmtief";

    TEXT_ZAMBRETTI_FORECAST = "Wettervorhersage";
    TEXT_ZAMBRETTI_ACCURACY = "Vorhersagegenauigkeit";
    TEXT_ZAMBRETTI_A       = "Beständiges Schönwetter";
    TEXT_ZAMBRETTI_B       = "Schönes Wetter";
    TEXT_ZAMBRETTI_C       = "Wetter wird gut";
    TEXT_ZAMBRETTI_D       = "Schön, wird wechselhaft";
    TEXT_ZAMBRETTI_E       = "Schön, Regenschauer möglich";
    TEXT_ZAMBRETTI_F       = "Ziemlich gut, verbessert sich";
    TEXT_ZAMBRETTI_G       = "Ziemlich gut, frühe Regenschauer möglich";
    TEXT_ZAMBRETTI_H       = "Ziemlich gut, spätere Regenschauer";
    TEXT_ZAMBRETTI_I       = "Früh schauerhaft, verbessert sich";
    TEXT_ZAMBRETTI_J       = "Wechselhaft, verbessert sich";
    TEXT_ZAMBRETTI_K       = "Ziemlich gut, Regenschauer möglich";
    TEXT_ZAMBRETTI_L       = "Eher veränderlich, klart später auf";
    TEXT_ZAMBRETTI_M       = "Veränderlich, verbessert sich wahrscheinlich";
    TEXT_ZAMBRETTI_N       = "Regnerisch mit Aufhellungen";
    TEXT_ZAMBRETTI_O       = "Regnerisch, wird veränderlich";
    TEXT_ZAMBRETTI_P       = "Veränderlich mit wenig Regen";
    TEXT_ZAMBRETTI_Q       = "Veränderlich, mit kurzen schönen Intervallen";
    TEXT_ZAMBRETTI_R       = "Veränderlich, später Regen";
    TEXT_ZAMBRETTI_S       = "Veränderlich, zeitweise Regen";
    TEXT_ZAMBRETTI_T       = "Stark wechselnd, zeitweise schöner";
    TEXT_ZAMBRETTI_U       = "Zeitweise Regen, verschlechtert sich";
    TEXT_ZAMBRETTI_V       = "Zeitweise Regen, wird sehr unruhig";
    TEXT_ZAMBRETTI_W       = "Regen in regelmässigen Abständen";
    TEXT_ZAMBRETTI_X       = "Sehr veränderlich, Regen";
    TEXT_ZAMBRETTI_Y       = "Stürmisch, verbessert sich wahrscheinlich";
    TEXT_ZAMBRETTI_Z       = "Stürmisch, viel Regen";
    TEXT_ZAMBRETTI_0       = "Batterie leer, bitte nachladen!";
    TEXT_ZAMBRETTI_DEFAULT = "Im Moment keine Prognose möglich";
  }

  if (LANGUAGE == "DW") {   // German - winter version
    TEXT_AIR_PRESSURE      = "Luftdruck";
    TEXT_RISING_FAST       = "rasch steigend";
    TEXT_RISING            = "steigend";
    TEXT_RISING_SLOW       = "langsam steigend";
    TEXT_STEADY            = "beständig";
    TEXT_FALLING_SLOW      = "langsam fallend";
    TEXT_FALLING           = "fallend";
    TEXT_FALLING_FAST      = "rasch fallend";

    TEXT_STRONG_HIGH       = "Kräftiges Hoch";
    TEXT_HIGHPRESSURE      = "Hochdruck";
    TEXT_LOWPRESSURE       = "Tiefdruck";
    TEXT_STRONG_LOW        = "Ausgewachsenes Tief";
    TEXT_STORM_LOW         = "Sturmtief";

    TEXT_ZAMBRETTI_FORECAST = "Wettervorhersage";
    TEXT_ZAMBRETTI_ACCURACY = "Vorhersagegenauigkeit";
    TEXT_ZAMBRETTI_A       = "Beständig schönes Winterwetter";
    TEXT_ZAMBRETTI_B       = "Schönes Winterwetter";
    TEXT_ZAMBRETTI_C       = "Wetter wird gut";
    TEXT_ZAMBRETTI_D       = "Schön, wird wechselhaft";
    TEXT_ZAMBRETTI_E       = "Schön, Schneefall möglich";
    TEXT_ZAMBRETTI_F       = "Ziemlich gut, verbessert sich";
    TEXT_ZAMBRETTI_G       = "Ziemlich gut, früher Schneefall möglich";
    TEXT_ZAMBRETTI_H       = "Ziemlich gut, späterer Schneefall";
    TEXT_ZAMBRETTI_I       = "Früh schauerhaft, verbessert sich";
    TEXT_ZAMBRETTI_J       = "Wechselhaft, verbessert sich";
    TEXT_ZAMBRETTI_K       = "Ziemlich gut, Schneefall möglich";
    TEXT_ZAMBRETTI_L       = "Eher veränderlich, klart später auf";
    TEXT_ZAMBRETTI_M       = "Veränderlich, verbessert sich wahrscheinlich";
    TEXT_ZAMBRETTI_N       = "Schneefall mit Aufhellungen";
    TEXT_ZAMBRETTI_O       = "Schnee möglich, wird veränderlich";
    TEXT_ZAMBRETTI_P       = "Veränderlich mit wenig Schnee";
    TEXT_ZAMBRETTI_Q       = "Veränderlich, mit kurzen schönen Intervallen";
    TEXT_ZAMBRETTI_R       = "Veränderlich, später Schnee";
    TEXT_ZAMBRETTI_S       = "Veränderlich, zeitweise Schnee";
    TEXT_ZAMBRETTI_T       = "Stark wechselnd, zeitweise schöner";
    TEXT_ZAMBRETTI_U       = "Zeitweise Schnee, verschlechtert sich";
    TEXT_ZAMBRETTI_V       = "Zeitweise Schnee, wird sehr unruhig";
    TEXT_ZAMBRETTI_W       = "Schnee in regelmässigen Abständen";
    TEXT_ZAMBRETTI_X       = "Sehr veränderlich, Schnee";
    TEXT_ZAMBRETTI_Y       = "Stürmisch, verbessert sich wahrscheinlich";
    TEXT_ZAMBRETTI_Z       = "Stürmisch, viel Schnee";
    TEXT_ZAMBRETTI_0       = "Batterie leer, bitte nachladen!";
    TEXT_ZAMBRETTI_DEFAULT = "Im Moment keine Prognose moeglich";
  }
}
