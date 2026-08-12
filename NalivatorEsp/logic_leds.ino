void CvetoMuzik() {
  static uint8_t led = 0;
  static uint8_t count = 0;
  static uint32_t counter = 0;
#ifdef TABLE_RAINBOW_READY_GLASS_TIMER
  static uint32_t counter1 = 0;
#endif
#ifdef LED_TOWER
  static bool flag = false;
  static uint8_t count2 = 0;
  static uint32_t count3 = 0;
  static uint8_t count4 = 0;
#endif
#ifdef ROUND_MATRIX
  static uint8_t count5 = 0;
#endif

#ifndef ROUND_MATRIX
  if ((ledShow || systemState == PUMPING) && LEDtimer.isReady()) {
    if (systemState == PUMPING) {
      strip.setPixelColor(curPumping, strip.ColorHSV(counter));  // радуга при наливе рюмки
      counter += INTENSITY_COLOR_CHANGE_GLASS;                   // можно поменять , интенсивность смены цвета при розливе на рюмке
      if (counter > 65535) counter = INTENSITY_COLOR_CHANGE_GLASS;
      strip.show();
    } else {
      strip.setPixelColor(led, strip.ColorHSV(counter + led * TABLE_RAINBOW_STEP));  //  умножение  led  увеличивает шаг радуги стола, можно поиграться значением
      counter += 1285;
      strip.show();

      if (++led >= NUM_SHOTS) {
        led = 0;
        if (++count == NUMBER_ILLUMINATION_PASSES) {  // количество проходов иллюминации после налива
          count = 0;
          if (MenuFlag != 15 && !playMush) check = true;  // если не в меню настроек, то проверяем рюмки
        }
      }
    }
  }


  if (check) {  // проверка рюмок
    for (led = 0; led < NUM_SHOTS; led++) {
#ifdef TWO_PUMPS
      if (shotStates[led] == EMPTY) {
        if (pourFrom[led] == 0) strip.setPixelColor(led, COLOR_EMPTY_GLASS_1);       // подсветили
       else if (pourFrom[led] == 1) strip.setPixelColor(led, COLOR_EMPTY_GLASS_2);  // подсветили
      }
#else
      if (shotStates[led] == EMPTY) strip.setPixelColor(led, COLOR_EMPTY_GLASS_1);  // налитая рюмка, статус: готов
#endif
#ifndef TABLE_RAINBOW_READY_GLASS_TIMER
      else if (shotStates[led] == READY)
      strip.setPixelColor(led, COLOR_POURET_GLASS); // налитая рюмка, статус: готов
#endif
      else if (shotStates[led] == NO_GLASS) strip.setPixelColor(led, COLOR_EMPTY_SPACE);
    }
    LEDtimer.setInterval(GLASS_RAINBOW_ILLUMINATION_TIMER);
    LEDtimer.reset();
    LEDchanged = true;
    check = false;
    ledShow = false;
    led = 0;
    count = 0;
    counter = 0;
  }

#ifdef TABLE_RAINBOW_READY_GLASS_TIMER
 if (!ledShow && !save && LEDtimer1.isReady() && systemState != PUMPING){
   for (int i = 0; i < NUM_SHOTS; i++) {
       if (shotStates[i] == READY) strip.setPixelColor(i, strip.ColorHSV(counter1+i*200*64));
    }
   counter1 += 64;                   // можно поменять , интенсивность смены цвета при розливе на рюмке
   if (counter1 > 65535) counter = 64;
   LEDtimer1.setInterval(TABLE_RAINBOW_READY_GLASS_TIMER);
   LEDtimer1.reset();
   LEDchanged = true;
 }
   // отрисовка светодиодов по флагу
   if (!ledShow && LEDchanged && !save && (LEDtimer.isReady() || LEDtimer1.isReady())) {
     LEDchanged = false;
     strip.show();
   }
#else
  // отрисовка светодиодов по флагу
   if (!ledShow && LEDchanged && !save && (LEDtimer.isReady())) {
     LEDchanged = false;
     strip.show();
   }
#endif

#else


  if (ledShow && LEDtimer.isReady()) {
    for (uint8_t leds = 0; leds < NUM_SHOTS; leds++) {
      strip.setPixelColor(led + leds * ROUND_MATRIX_LEDS, strip.ColorHSV(counter + led * TABLE_RAINBOW_STEP));  //  умножение  led  увеличивает шаг радуги стола, можно поиграться значением
    }
    counter += 1285;
    strip.show();

    if (++led >= ROUND_MATRIX_LEDS) {
      led = 0;
      if (++count == NUMBER_ILLUMINATION_PASSES) {  // количество проходов иллюминации после налива
        count = 0;
        if (MenuFlag != 15 && !playMush) check = true;  // если не в меню настроек, то проверяем рюмки
      }
    }
  }

  if (check) {  // проверка рюмок
    strip.clear();
    for (led = 0; led < NUM_SHOTS; led++) {
      if (shotStates[led] == NO_GLASS) {
        for (uint8_t leds = 0; leds < ROUND_MATRIX_LEDS; leds++) {
          strip.setPixelColor(led * ROUND_MATRIX_LEDS + leds, COLOR_EMPTY_SPACE);
        }
      }
    }
    strip.show();
    LEDtimer.setInterval(GLASS_RAINBOW_ILLUMINATION_TIMER);
    LEDtimer.reset();
    check = false;
    ledShow = false;
    led = 0;
    count = 0;
    counter = 0;
  }


#ifdef BAT_MONITOR_ON
 #ifdef TABLE_RAINBOW_READY_GLASS_TIMER 
  if (!ledShow && !save && !lowBat && LEDtimer1.isReady()){
    counter1 += 64;                   // можно поменять , интенсивность смены цвета при розливе на рюмке
    if (counter1 > 65535) counter = 64;
    LEDtimer1.setInterval(TABLE_RAINBOW_READY_GLASS_TIMER);
    LEDtimer1.reset();
  }
 #endif
  if (!ledShow && !save && !lowBat && LEDtimer.isReady()) {
#else
 #ifdef TABLE_RAINBOW_READY_GLASS_TIMER 
  if (!ledShow && !save && LEDtimer1.isReady()){
    counter1 += 64;                   // можно поменять , интенсивность смены цвета при розливе на рюмке
    if (counter1 > 65535) counter = 64;
    LEDtimer1.setInterval(TABLE_RAINBOW_READY_GLASS_TIMER);
    LEDtimer1.reset();
  }
 #endif 
  if (!ledShow && !save && LEDtimer.isReady()) {
#endif

    for (led = 0; led < NUM_SHOTS; led++) {
      if (shotStates[led] == EMPTY) {
        strip.setPixelColor(led * ROUND_MATRIX_LEDS + count + ROUND_MATRIX_LEDS / 2, 0, 0, 0);
        strip.setPixelColor(led * ROUND_MATRIX_LEDS + count, 0, 0, 0);
        strip.setPixelColor(led * ROUND_MATRIX_LEDS + count + 1, COLOR_EMPTY_GLASS_1);
        if (count == 3) {
          strip.setPixelColor(led * ROUND_MATRIX_LEDS, COLOR_EMPTY_GLASS_1);
        } else {
          strip.setPixelColor(led * ROUND_MATRIX_LEDS + count + 1 + ROUND_MATRIX_LEDS / 2, COLOR_EMPTY_GLASS_1);
        }
    #ifdef TABLE_RAINBOW_READY_GLASS_TIMER 
      } else if (shotStates[led] == READY) {
        strip.setPixelColor(led * ROUND_MATRIX_LEDS + count + ROUND_MATRIX_LEDS / 2, 0, 0, 0);
        strip.setPixelColor(led * ROUND_MATRIX_LEDS + count, 0, 0, 0);
        strip.setPixelColor(led * ROUND_MATRIX_LEDS + count + 1, strip.ColorHSV(counter1+led*200*64));
        if (count == 3) {
          strip.setPixelColor(led * ROUND_MATRIX_LEDS, strip.ColorHSV(counter1+led*200*64));
        } else {
          strip.setPixelColor(led * ROUND_MATRIX_LEDS + count + 1 + ROUND_MATRIX_LEDS / 2, strip.ColorHSV(counter1+led*200*64));
        }
    #else
        } else if (shotStates[led] == READY) {
        strip.setPixelColor(led * ROUND_MATRIX_LEDS + count + ROUND_MATRIX_LEDS / 2, 0, 0, 0);
        strip.setPixelColor(led * ROUND_MATRIX_LEDS + count, 0, 0, 0);
        strip.setPixelColor(led * ROUND_MATRIX_LEDS + count + 1, COLOR_POURET_GLASS);
        if (count == 3) {
          strip.setPixelColor(led * ROUND_MATRIX_LEDS, COLOR_POURET_GLASS);
        } else {
          strip.setPixelColor(led * ROUND_MATRIX_LEDS + count + 1 + ROUND_MATRIX_LEDS / 2, COLOR_POURET_GLASS);
        }
    #endif
      } else if (shotStates[led] == IN_PROCESS) {

        strip.setPixelColor(led * ROUND_MATRIX_LEDS + count5, 255, 255, 0);                                          // светодиод 1
        if (count5 + 1 < ROUND_MATRIX_LEDS) strip.setPixelColor(led * ROUND_MATRIX_LEDS + count5 + 1, 255, 255, 0);  // светодиод 2
        else strip.setPixelColor(led * ROUND_MATRIX_LEDS + count5 - 7, 255, 255, 0);                                 // светодиод 2

        if (count5 + 2 < ROUND_MATRIX_LEDS) strip.setPixelColor(led * ROUND_MATRIX_LEDS + count5 + 2, 0, 0, 255);  // светодиод 3
        else strip.setPixelColor(led * ROUND_MATRIX_LEDS + count5 - 6, 0, 0, 255);                                 // светодиод 3

        if (count5 + 3 < ROUND_MATRIX_LEDS) strip.setPixelColor(led * ROUND_MATRIX_LEDS + count5 + 3, 0, 0, 255);  // светодиод 4
        else strip.setPixelColor(led * ROUND_MATRIX_LEDS + count5 - 5, 0, 0, 255);                                 // светодиод 4

        if (count5 + 4 < ROUND_MATRIX_LEDS) strip.setPixelColor(led * ROUND_MATRIX_LEDS + count5 + 4, 255, 0, 0);  // светодиод 5
        else strip.setPixelColor(led * ROUND_MATRIX_LEDS + count5 - 4, 255, 0, 0);                                 // светодиод 5

        if (count5 + 5 < ROUND_MATRIX_LEDS) strip.setPixelColor(led * ROUND_MATRIX_LEDS + count5 + 5, 255, 0, 0);  // светодиод 6
        else strip.setPixelColor(led * ROUND_MATRIX_LEDS + count5 - 3, 255, 0, 0);                                 // светодиод 6

        if (count5 + 6 < ROUND_MATRIX_LEDS) strip.setPixelColor(led * ROUND_MATRIX_LEDS + count5 + 6, 0, 255, 0);  // светодиод 7
        else strip.setPixelColor(led * ROUND_MATRIX_LEDS + count5 - 2, 0, 255, 0);                                 // светодиод 7

        if (count5 + 7 < ROUND_MATRIX_LEDS) strip.setPixelColor(led * ROUND_MATRIX_LEDS + count5 + 7, 0, 255, 0);  // светодиод 8
        else strip.setPixelColor(led * ROUND_MATRIX_LEDS + count5 - 1, 0, 255, 0);                                 // светодиод 8
      }
    }
    if (++count >= ROUND_MATRIX_LEDS / 2) {
      count = 0;
    }
    if (++count5 >= ROUND_MATRIX_LEDS) {
      count5 = 0;
    }

    strip.show();
  }

 

#endif
  // Башня LED
#ifdef LED_TOWER
  if (mig && !clearLed && TOWERtimer.isReady()) {

    if (flag) {
      strip2.setPixelColor(count2, 255, 0, 0);
      strip2.setPixelColor(count2 + (NUMLEDS / 2), 0, 0, 255);
    } else {
      strip2.setPixelColor(count2 + (NUMLEDS / 2), 255, 0, 0);
      strip2.setPixelColor(count2, 0, 0, 255);
    }
    strip2.show();
    if (++count2 >= NUMLEDS / 2) {
      count2 = 0;
      flag = !flag;
    }

    if (++count3 > 75) {  // продолжительность мигалки у.е.
      mig = false;
      clearLed = true;
      TOWERtimer.reset();
    }
  }


  if ((MenuFlag == 15 || rainbow) && !clearLed && TOWERtimer.isReady()) {
    strip2.setPixelColor(count2, strip2.ColorHSV(count3 + count2 * 12850));  //  умножение  count2  увеличивает шаг радуги башни, можно поиграться значением
    count3 += 1285;
    strip2.show();
    if (++count2 >= NUMLEDS) {
      count2 = 0;
      if (!playMush && MenuFlag != 15 && ++count4 > 20) {  // продолжительность радуги у.е.
        rainbow = false;
        clearLed = true;
        TOWERtimer.reset();
      }
    }
  }

  if (systemON && (ledShowOn == 1) && !clearLed && !promivka && MenuFlag != 20 && TOWERtimer.isReady()) {
    if (++count2 >= 7) count2 = 0;
    if (++count4 >= 21) count4 = 0;
    strip2.clear();
    for (count3 = count2; count3 < NUMLEDS; count3 += 7) {
      if (count4 < 7) {
        strip2.setPixelColor(count3, 255, 0, 0);
      } else if (count4 < 14) {
        strip2.setPixelColor(count3, 0, 255, 0);
      } else if (count4 < 21) {
        strip2.setPixelColor(count3, 0, 0, 255);
      }
    }
    strip2.show();
  }

  if (clearLed && TOWERtimer.isReady()) {
    clearLed = false;
    count2 = 0;
    count3 = 0;
    count4 = 0;
    strip2.clear();
    strip2.show();
    TOWERtimer.reset();
  }


#endif
}
