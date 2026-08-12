void oled_menu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  if(status == 0)  lcd.print(" ");
  else if (status == 1)  lcd.print("W");
  else if (status == 2)  lcd.print("H");
  lcd.setCursor(3, 0);
  lcd.print("НАЛИВАТОР+   ");
#ifdef BAT_MONITOR_ON
 #ifdef CHARG_DETECT 
  if (!flagCh) battery (flagBat);
 #else
  battery (flagBat);
 #endif
#endif
  lcd.setCursor(0, 1);
  lcd.print(">");
  lcd.setCursor(15, 1);
  lcd.print("<");
  switch (Menu) {
    case EQUALLY:
      lcd.setCursor(1, 1);
      lcd.print("НАЛИТЬ ПОРОВНУ");
      break;
    case MULTI_SPILL:
      lcd.setCursor(2, 1);
      lcd.print("МУЛЬТИРАЗЛИВ");
      break;
    case PLAYER:
      lcd.setCursor(5, 1);
      lcd.print("ПЛЕЕР");
      break;
    case SETTINGS:
      lcd.setCursor(3, 1);
      lcd.print("НАСТРОЙКИ");
      break;

    case MUSKETEERS:
      lcd.setCursor(6, 1);
      lcd.print("ИГРА");
      break;

    case BARMAN:
      lcd.setCursor(5, 1);
      lcd.print("БАРМЕН");
      break;
  }
}



void menu_nastr() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("НАСТРОЙКА");
#ifdef BAT_MONITOR_ON
 #ifdef CHARG_DETECT 
  if (!flagCh) battery (flagBat);
 #else
  battery (flagBat);
 #endif
#endif
  lcd.setCursor(0, 1);
  lcd.print(">");
  lcd.setCursor(15, 1);
  lcd.print("<");
  switch (subNastr) {
    case 1:
      lcd.setCursor(4, 1);
      lcd.print("ПРОМЫВКА");
      break;
    case 2:
      lcd.setCursor(2, 1);
      lcd.print("МУЛЬТИРАЗЛИВ");
      break;
    case 3:
      lcd.setCursor(5, 1);
      lcd.print("ТОСТЫ");
      break;

    case 4:
      lcd.setCursor(2, 1);
      lcd.print("МЕНЮ  СЕРВО");
      break;

    case 5:
      lcd.setCursor(2, 1);
      lcd.print("ЯРКОСТЬ  LED");
      break;

    case 6:
      lcd.setCursor(5, 1);
      lcd.print("ДОЛИВ");
      break;


    case 7:
      lcd.setCursor(2, 1);
      lcd.print("КАЛИБР.ПОМПЫ");
      break;

    case 8:
      lcd.setCursor(3, 1);
      lcd.print("ТАЙМЕР СНА");
      break;
#ifdef BAT_MONITOR_ON
    case 9:
      lcd.setCursor(1, 1);
      lcd.print("НАПРЯЖЕНИЕ АКБ");
      break;
#endif
  }
}



// Меню Авто режим
void oled_auto() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("HАЛИTЬ");
  lcd.setCursor(7, 0);
  lcd.print("ПО    ml");
  lcd.setCursor(10, 0);
  lcd.print(Drink, DEC);
#ifdef BAT_MONITOR_ON
 #ifdef CHARG_DETECT 
  if (!flagCh) battery (flagBat);
 #else
  battery (flagBat);
 #endif
#endif
  lcd.setCursor(0, 1);
  if (Drink < 25) lcd.print("HИ O ЧEM");       //  HИ O ЧEM
  else if (Drink < 30) lcd.print("ПO ЧУTЬ - ЧУTЬ");  //  ПO ЧУTЬ - ЧУTЬ
  else if (Drink < 40) lcd.print("B CAMЫЙ PAЗ");  //  B CAMЫЙ PAЗ
  else if (Drink < 45) lcd.print("ПО  ПОЛНОЙ");  //  ПО  ПОЛНОЙ
  else lcd.print("ДО КРАЕВ");                  //  ДО КРАЕВ

  if (MenuFlag == 61) {
    lcd.setCursor(15, 1);
    lcd.print("B");
  }
  if (MenuFlag == 60) {
    lcd.setCursor(15, 1);
    lcd.print("И");
  }
}

// Меню налива
void oled_naliv() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("НАЛИВАЮ     ml");
  lcd.setCursor(0, 1);
  lcd.print("В  -Ю РЮМКУ   ml");
}

// Меню налито
void oled_nalito() {
  lcd.clear();
  if (MenuFlag == 2) {
    lcd.setCursor(5, 0);
  } else {
    lcd.setCursor(7, 0);
    lcd.print("ПО    ml");
    lcd.setCursor(10, 0);
    lcd.print(Drink, DEC);
    lcd.setCursor(0, 0);
  }
  lcd.print("HАЛИТО");
  lcd.setCursor(0, 1);
  lcd.print("   В   РЮМ");
  lcd.setCursor(5, 1);
  lcd.print(DrinkCount, DEC);
  lcd.setCursor(10, 1);
  if (DrinkCount == 1) lcd.print("КУ     ");       //  КУ
  else if (DrinkCount <= 4) lcd.print("КИ     ");  //  КИ
  else lcd.print("ОК     ");                       //  ОК
}

// Меню настройки серво
void servo_calibr(uint8_t subServo) {
  if (subServo == 0) {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("-я рюмка");
    lcd.setCursor(3, 1);
    lcd.print("поз.:");
  }
  if (subServo == 1) {
    lcd.setCursor(3, 0);
    lcd.print((count + 1), DEC);
  }
  if (subServo == 2 || subServo == 1) {
    lcd.setCursor(10, 1);
    lcd.print("   ");
    lcd.setCursor(9, 1);
    lcd.print(servoPos, DEC);
  }
}


void multi_naliv() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("1-   2-   3-");
#ifdef BAT_MONITOR_ON
 #ifdef CHARG_DETECT 
  if (!flagCh) battery (flagBat);
 #else
  battery (flagBat);
 #endif
#endif
  lcd.setCursor(2, 0);
  lcd.print(ManDrink[0], DEC);
  lcd.setCursor(7, 0);
  lcd.print(ManDrink[1], DEC);
  lcd.setCursor(12, 0);
  lcd.print(ManDrink[2], DEC);
  lcd.setCursor(0, 1);
  lcd.print("4-   5-   6-");
  lcd.setCursor(2, 1);
  lcd.print(ManDrink[3], DEC);
  lcd.setCursor(7, 1);
  lcd.print(ManDrink[4], DEC);
  lcd.setCursor(12, 1);
  lcd.print(ManDrink[5], DEC);
  if (MenuFlag == 61) {
    lcd.setCursor(15, 1);
    lcd.print("B");
  }
#ifdef TWO_PUMPS
  lcd.setCursor(1, 0);
  if (pourFrom[0] == 1) {
    lcd.setCursor(1, 0);
    lcd.print("=");
  }
  if (pourFrom[1] == 1) {
    lcd.setCursor(6, 0);
    lcd.print("=");
  }
  if (pourFrom[2] == 1) {
    lcd.setCursor(11, 0);
    lcd.print("=");
  }
  if (pourFrom[3] == 1) {
    lcd.setCursor(1, 1);
    lcd.print("=");
  }
  if (pourFrom[4] == 1) {
    lcd.setCursor(6, 1);
    lcd.print("=");
  }
  if (pourFrom[5] == 1) {
    lcd.setCursor(11, 1);
    lcd.print("=");
  }

#endif
}

void multi_naliv2() {
  lcd.clear();
  lcd.setCursor(0, 1);
  if (ManDrink[(ManRum - 1)] < 25) lcd.print("НИ О ЧЕМ");       //  НИ О ЧЕМ
  else if (ManDrink[(ManRum - 1)] < 30) lcd.print("ПО ЧУТЬ - ЧУТЬ");  //  ПО ЧУТЬ - ЧУТЬ
  else if (ManDrink[(ManRum - 1)] < 40) lcd.print("В САМЫЙ  РАЗ");  //  В САМЫЙ  РАЗ
  else if (ManDrink[(ManRum - 1)] < 45) lcd.print("ПО  ПОЛНОЙ");  //  ПО  ПОЛНОЙ
  else lcd.print("ДО КРАЕВ");                                   //  ДО КРАЕВ

  lcd.setCursor(0, 0);
  lcd.print(ManRum, DEC);
  lcd.setCursor(1, 0);
  lcd.print("-Я РЮМКА:   ml");
  lcd.setCursor(10, 0);
  lcd.print(ManDrink[(ManRum - 1)], DEC);
#ifdef TWO_PUMPS
  multi_naliv_two_pump();
#endif
}
#ifdef TWO_PUMPS
void multi_naliv_two_pump() {
  lcd.setCursor(15, 1);
  if (pourFrom[ManRum - 1] == 0) lcd.print("1");       // 1
  else if (pourFrom[ManRum - 1] == 1) lcd.print("2");  // 2
}
#endif

void menu_tost() {
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("ТОСТЫ");
  switch (subMenu) {
    case 1:
      lcd.setCursor(3, 1);
      lcd.print("ГР.ОЗВУЧКИ");
      break;

    case 2:
      lcd.setCursor(0, 1);
      lcd.print("ПЕРЕМЕШАТЬ ТРЕКИ");
      break;

    case 3:
      lcd.setCursor(0, 1);
      lcd.print("ПАПКА С ТРЕКАМИ");
      break;

    case 4:
      lcd.setCursor(5, 1);
      lcd.print("БАРМЕН");
      break;
  }
}

void num_folder(uint8_t subFolder) {
  if (subFolder == 0) {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("ПАПКА");
    lcd.setCursor(2, 1);
    lcd.print("ТРЕКОВ");
  }
  if (subFolder == 1 || subFolder == 0) {
    lcd.setCursor(11, 0);
    lcd.print(folder, DEC);
    lcd.setCursor(11, 1);
    if (tracks < 1) {
      lcd.print("НЕТ");
    } else {
      if (subFolder != 0) lcd.print("   ");  // пробел 3
      lcd.setCursor(11, 1);
      lcd.print(tracks, DEC);
    }
  }
}
void menu_play(uint8_t subPlay) {
  if (subPlay == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ДИР:");
    lcd.setCursor(9, 0);
    lcd.print("ГР:");
#ifdef BAT_MONITOR_ON
 #ifdef CHARG_DETECT 
  if (!flagCh) battery (flagBat);
 #else
  battery (flagBat);
 #endif
#endif
    lcd.setCursor(0, 1);
    lcd.print("ТР:");
    lcd.setCursor(11, 1);
    if (player) lcd.print("PLAY");  //  PLAY
  }
  if (subPlay == 1 || subPlay == 0) {
    lcd.setCursor(4, 0);
    lcd.print(folder2, DEC);
    lcd.setCursor(3, 1);
    if (tracks2 < 1) {
      lcd.print("НЕТ");
    } else {
      if (subPlay != 0) lcd.print("   ");  // пробел 3
      lcd.setCursor(3, 1);
      lcd.print(tracks2, DEC);
    }
  }
  if (subPlay == 2 || subPlay == 0) {
    if (volume2 == 9) {
      lcd.setCursor(13, 0);
      lcd.print(" ");
    }
    lcd.setCursor(12, 0);
    lcd.print(volume2, DEC);
  }
  if (subPlay == 3 || subPlay == 0) {
    switch (folTra) {
      case 1: lcd.setCursor(6, 0); break;   // галочка на выборе папки
      case 2: lcd.setCursor(10, 1); break;  // галочка на треках
      case 3: lcd.setCursor(8, 0); break;   //  галочка на вкл-выкл рандом
      case 4: lcd.setCursor(14, 0); break;  // галочка на громкости
      case 5: lcd.setCursor(15, 1); break;  //  галочка на вкл-выкл play
    }
    lcd.print("<");
  }
  if (subPlay == 4 || subPlay == 1 || subPlay == 0) {
    if (subPlay != 0) {
      lcd.setCursor(7, 1);
      lcd.print("   ");
    }
    if (tracks2 > 0) {
      lcd.setCursor(7, 1);
      lcd.print((MusicList[num2] + 1), DEC);
    }
  }
  if (subPlay == 5 || subPlay == 0) {
    lcd.setCursor(7, 0);
    if (mixMusic == 1) lcd.print("R");  // R
    else lcd.print("L");                // L
  }
}



void menu_servo() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("НАСТРОЙКИ  СЕРВО");
  switch (subMenu) {
    case 1:
      lcd.setCursor(3, 1);
      lcd.print("КАЛИБРОВКА");
      break;

    case 2:
      lcd.setCursor(4, 1);
      lcd.print("СКОРОСТЬ");
      break;
  }
}


void menu_vol(uint8_t subVol) {
  if (subVol == 0) {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("ГРОМКОСТЬ");
  }
  if (subVol == 1 || subVol == 0) {
    if (volume == 9) {
      lcd.setCursor(14, 1);
      lcd.print(" ");
    }
    lcd.setCursor(13, 1);
    lcd.print(volume, DEC);
  }
}


void menu_brigh(uint8_t subBrigh) {
  if (subBrigh == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ЯРКОСТЬ");
    lcd.setCursor(10, 0);
    lcd.print("SHOW");
  }
  if (subBrigh == 1 || subBrigh == 0) {
    lcd.setCursor(2, 1);
    lcd.print("   ");
    lcd.setCursor(2, 1);
    lcd.print((map(bright, 0, 254, 0, 99)), DEC);
  }
  if (subBrigh == 2 || subBrigh == 0) {
    lcd.setCursor(10, 1);
    if (ledShowOn == 1) lcd.print("ВКЛ.");  // ВКЛ.
    else lcd.print("ВЫКЛ.");                 // ВЫКЛ.
  }
}

void menu_promivka(uint8_t subPromivka) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("П Р О М Ы В К А");
  if (subPromivka == 1) {
    lcd.setCursor(2, 1);
    lcd.print(">>>>>>>>>>>>");
  } else if (subPromivka == 2) {
    lcd.setCursor(5, 1);
    lcd.print(Procent, DEC);
    lcd.setCursor(10, 1);
    lcd.print("мс");
  }
}

void doliv(uint8_t subDoliv) {
  if (subDoliv == 0) {
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("ДОЛИВ");
  }
  if (subDoliv == 1 || subDoliv == 0) {
    lcd.setCursor(5, 1);
    if (noDoliv == 0) lcd.print("ВКЛ.");  // ВКЛ.
    else lcd.print("ВЫКЛ.");               // ВЫКЛ.
  }
}

void mix_track() {
  lcd.clear();
  lcd.setCursor(6, 1);
  if (mixTracks == 1) lcd.print("ВКЛ.");  // ВКЛ.
  else lcd.print("ВЫКЛ.");                 // ВЫКЛ.
}

void no_tost_barman() {
  lcd.clear();
  lcd.setCursor(6, 1);
  if (noTostBarmen == 0) lcd.print("ВКЛ.");  // ВКЛ.
  else lcd.print("ВЫКЛ.");                    // ВЫКЛ.
}

void servo_speed(uint8_t subSS) {
  if (subSS == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("СКОРОСТЬ");
  }
  if (subSS == 1 || subSS == 0) {
    if (speedSer == 23) {
      lcd.setCursor(13, 0);
      lcd.print(" ");
    }
    lcd.setCursor(12, 0);
#ifndef STEPPER_MOTOR_USED
    lcd.print((map(speedSer, 2, 30, 30, 2)), DEC);
#else
    if (speedSer == 9) {
      lcd.setCursor(13, 0);
      lcd.print(" ");
      lcd.setCursor(12, 0);
    }
    lcd.print(speedSer, DEC);
#endif
  }
}
#ifdef BAT_MONITOR_ON
void bat_volt() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("НАПРЯЖЕНИЕ");
  lcd.setCursor(5, 1);
  lcd.print((float)v_akky / 1000.0f ,2);
  lcd.setCursor(10, 1);
  lcd.print("V");
}

#endif

void sleep_time(uint8_t subTime) {
  if (subTime == 0) {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("ЗАСНУТЬ ЧЕРЕЗ");
  }
  if (subTime == 1 || subTime == 0) {
    if (sleepTime == 0) {
      if (subTime != 0) {
        lcd.setCursor(10, 1);
        lcd.print("   ");
      }
      lcd.setCursor(5, 1);
      lcd.print("ВЫКЛ.");
    } else {
      lcd.setCursor(5, 1);
      lcd.print((map(sleepTime, 0, 20, 0, 10)), DEC);
      lcd.setCursor(6, 1);
      if (sleepTime % 2 != 0) lcd.print(".30");  //  .30
      else lcd.print(".00");                     //  .00
      lcd.setCursor(9, 1);
      lcd.print("МИН.");
    }
  }
}


void kalibr_pump(uint8_t subPump) {
  if (subPump == 0) {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("НАЛИВ 50ml ЗА:");
    lcd.setCursor(8, 1);
    lcd.print("мс");
  }
  if (subPump == 1 || subPump == 0) {
#ifdef TWO_PUMPS
    if (yesGlass != 0) {
      for (uint8_t i = 0; i < NUM_SHOTS; i++) {
        if (shotStates[i] == EMPTY) {
          pumpNumber = pourFrom[i];
          break;
        }
      }
      if (pumpNumber == 0) {
        lcd.setCursor(0, 1);
        lcd.print("1");
        lcd.setCursor(3, 1);
        lcd.print(time50ml_1, DEC);

      } else if (pumpNumber == 1) {
        lcd.setCursor(0, 1);
        lcd.print("2");
        lcd.setCursor(3, 1);
        lcd.print(time50ml_2, DEC);
      }
    }
#else
    lcd.setCursor(3, 1);
    lcd.print(time50ml_1, DEC);
#endif
    lcd.setCursor(12, 1);
    lcd.print("  ml");
  }
}



void mushket() {
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("ИГРА");
#ifdef BAT_MONITOR_ON
  lcd.setCursor(15, 0);
  lcd.write(7);
#endif
  switch (subMush) {
    case 1:
      lcd.setCursor(2, 1);
      lcd.print("ОДИН ЗА ВСЕХ");
      break;
    case 2:
      lcd.setCursor(1, 1);
      lcd.print("ВСЕ ЗА ОДНОГО");
      break;
  }
}

void menu_barmen() {
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("БАРМЕН");
#ifdef BAT_MONITOR_ON
 #ifdef CHARG_DETECT 
  if (!flagCh) battery (flagBat);
 #else
  battery (flagBat);
 #endif
#endif
  switch (barMan) {
    case 1:
      lcd.setCursor(1, 1);
      lcd.print("НАЛИТЬ ПОРОВНУ");
      break;
    case 2:
      lcd.setCursor(2, 1);
      lcd.print("МУЛЬТИРАЗЛИВ");
      break;
  }
}
