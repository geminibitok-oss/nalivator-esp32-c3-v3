

// наливайка, опрос концевиков
void flowTick() {
  static bool swState = false;
    for (uint8_t i = 0; i < NUM_SHOTS; i++) {
#if (SWITCH_LEVEL == 0)
      swState = !bitRead(reedPins, i);
#else
      swState = bitRead(reedPins, i);
#endif

      if (shotStates[i] == NO_GLASS && swState && readySystem) {  // поставили пустую рюмку
        shotStates[i] = EMPTY;                                    // флаг на заправку
#ifndef ROUND_MATRIX
#ifdef TWO_PUMPS
        if (pourFrom[i] == 0) strip.setPixelColor(i, COLOR_EMPTY_GLASS_1);       // подсветили
        else if (pourFrom[i] == 1) strip.setPixelColor(i, COLOR_EMPTY_GLASS_2);  // подсветили
#else
        strip.setPixelColor(i, COLOR_EMPTY_GLASS_1);  // подсветили
#endif
        LEDchanged = true;
#endif

        //DEBUG("set glass");
        //DEBUG(i);
#ifdef TANK
        if (!systemON && !save && !playMush) play_track(i + 40);  //  трек в папке mp3, с 40 по 45
#else
        if (!systemON && !save && !playMush) play_track(i + 20);  //  трек в папке mp3, с 20 по 25
#endif
        yesGlass++;
        GlasCountEmply++;
        mqttClient.publish(Nalivator_Send, 1, false, "1");  // передаём от наливатора о постановке рюмки
        SAVEtimer.reset();
        if (save) {
#ifdef SEVE_MODE_CONDITION
          digitalWrite(SEVE_MODE_PIN, HIGH);
          delay(100);
#endif

#ifdef TANK
          play_track(27);  // звук просыпания
#else         
          play_track(17);  // звук просыпания
#endif
          save = false;
          enc.rst();
#if (DISPLAY_TYPE == 0)
          lcd.backlight();
#else
          lcd.display();
#endif
          check = true;  //  проверяем рюмки
          delay(2000);
        }
        if (MenuFlag == 61 && !systemON) {
          oled_naliv();
          systemON = true;

          if (ledShowOn == 1) {
            if (ledShow) check = true;
#ifdef LED_TOWER
            TOWERtimer.setInterval(50);
            if (mig || rainbow) {
              mig = false;
              rainbow = false;
              clearLed = true;
              TOWERtimer.reset();
            }
#endif
          }
        }
#ifdef TWO_PUMPS
        if (MenuFlag == 20 && yesGlass != 0) kalibr_pump(1);
#endif
      }
      if (shotStates[i] != NO_GLASS && !swState) {  // убрали пустую/полную рюмку
        if (shotStates[i] == EMPTY && GlasCountEmply !=0) GlasCountEmply--;
        shotStates[i] = NO_GLASS;                   // статус - нет рюмки

        if (!ledShow) {
#ifndef ROUND_MATRIX
          strip.setPixelColor(i, COLOR_EMPTY_SPACE);  // чёрный или на выбор
          LEDchanged = true;
#else
          for (uint8_t leds = 0; leds < ROUND_MATRIX_LEDS; leds++) {
            strip.setPixelColor(i * ROUND_MATRIX_LEDS + leds, COLOR_EMPTY_SPACE);
          }
#endif
        }

        if (i == curPumping) {  // убрали во время заправки!
#ifdef TWO_PUMPS
          if (pourFrom[curPumping] == 0) pump1OFF();       // помпа выкл
          else if (pourFrom[curPumping] == 1) pump2OFF();  // помпа выкл
#else
          pump1OFF();  // помпа выкл
#endif  // помпу выкл
          moving = false;
          if (MenuFlag == 20 || promivka) {
            if (GlasCountEmply !=0) GlasCountEmply--;
            curPumping = -1;
            systemState = WAIT;  // режим работы - ждать
            WAITtimer.reset();
          } else {
            systemState = TOOK;            // режим работы - взял рюмку во время разлива
            if (GlasCountEmply !=0) GlasCountEmply--;
            PAUSEtimer.setInterval(2000);  // время которое нужно для полного проговаривания трека Моргунова
            PAUSEtimer.reset();
#ifdef TANK
            play_track(12);  //  трек в папке mp3,  кто то снял рюмку при наливе
#else 
            play_track(2);  //  трек в папке mp3,  кто то снял рюмку при наливе
#endif
            VoiceBlock = false;
          }
        }
        //DEBUG("take glass");
        //DEBUG(i);
        yesGlass--;
        if (yesGlass == 0) mqttClient.publish(Nalivator_Send, 1, false, "0");  // передаём от наливатора о снятии последней рюмки
#ifdef TWO_PUMPS
        if (MenuFlag == 20) kalibr_pump(0);
#endif
      }
    }
}

// поиск и заливка
void flowRoutnie() {
  if (systemState == SEARCH) {  // если поиск рюмки
    if (!moving) {
      noGlass = true;
      for (int8_t i = 0; i < NUM_SHOTS; i++) {
        if (shotStates[i] == EMPTY && i != curPumping) {  // поиск рюмки
          if (MenuFlag == 60) {
            for (int8_t j = randomGlass; j < NUM_SHOTS; j++) {
              if (rumka[j] != -1) {
                randomGlass = j;
                break;
              }
            }
            if (subMush == 1) {
              if (i == rumka[randomGlass]) {
                noGlass = false;  // один за всех
              }
            } else if (subMush == 2) {
              if (i == rumka[randomGlass]) {
                if (randomGlass < NUM_SHOTS - 1) randomGlass++;
                noGlass = false;  // все за одного
              }
            }
          } else {
            noGlass = false;
          }

          if (!noGlass) {  //  нашли хоть одну рюмку
            if (flagSystemON) {
              mqttClient.publish(Nalivator_Send, 1, false, "3");  // передаём от наливатора о работе системы
              flagSystemON = false;
            }

#ifdef DOP_SOUND
            if (dopSound) dop_sound();
#endif
#ifdef TANK
            if (digitalRead(BUSY_PIN) && !promivka && MenuFlag != 20) {
              play_track(53);  //  трек в папке mp3 ,  движения башни танка
            }
#else
            if ((DrinkCount == 0 || digitalRead(BUSY_PIN)) && !promivka && MenuFlag != 20) {
              if (track_fonmusic > 0) myMP3.playFolderTrack(99, (FonMusic[num3] + 1));  //  треки в папке 99 , мелодии во время разлива
              else play_track(18);                                                      //  трек в папке mp3 , мелодия во время разлива
            }
#endif
            curPumping = i;                       // запоминаем выбор
            systemState = MOVING;                 // режим - движение
            shotStates[curPumping] = IN_PROCESS;  // стакан в режиме заполнения

            if (needZero) {
#ifdef SERVO_DETACH_PIN_ON
              servo.attach(SERVO_PIN, SERVO_MIN, SERVO_MAX);
#endif
#ifdef STEPPER_MOTOR_USED
              stepper.enable();  // включаем питание шаговика
#ifdef CHECKING_THE_ZERO_POSITION
              Zero_Pos();  // едем до концевика, ищем домашнюю позицию разливного носика
#endif
#endif
              needZero = false;
            }

            break;
          }
        }
      }
    }
    if (noGlass) {  // если не нашли ни одной рюмки
#ifdef TANK
      if (muveBack) {
        play_track(53);  //  трек в папке mp3 ,  движения башни танка
        muveBack = false;
      }
#endif

      readySystem = false;

#ifdef STARTING_POS_ANGLE
      servo_move(STARTING_POS_ANGLE);  // цель - заданная позиция
#else
      servo_move(0);                               // цель - 0
#endif
      if (!moving) {  // едем до упора
        if (DrinkCount > 0 && !promivka && MenuFlag != 20) {
          if (noTostBarmen == 0 || MenuFlag != 61) {
            PAUSEtimer.setInterval(NALITO_PAUSE);
            oled_nalito();  // Выводим на экран налито ...
            tost = true;
            if (player) {
              playOn = true;
              player = false;
            }
          } else {
            PAUSEtimer.setInterval(100);
            returnMenu = true;
          }
          if (!player && volume != 0 && !digitalRead(BUSY_PIN)) {
            myMP3.stop();
            delay(100);
            myMP3.setVolume(volume);
          }

          if (ledShowOn == 1) {
            ledShow = true;
            LEDtimer.setInterval(TABLE_RAINBOW_ILLUMINATION_TIMER);
#ifdef LED_TOWER
            TOWERtimer.setInterval(20);
            rainbow = true;
            clearLed = true;
            TOWERtimer.reset();
#endif
          }
          mqttClient.publish(Nalivator_Send, 1, false, "2");  // передаём от наливатора, что все рюмки наполнены
          if (track_fonmusic > 0 && ++num3 >= track_fonmusic) num3 = 0;
        } else {
          PAUSEtimer.setInterval(NALITO_PAUSE);
          lcd.clear();
          if (yesGlass == 0 || promivka || MenuFlag == 20) {
            lcd.setCursor(3, 1);
            lcd.print("НЕТ РЮМОК!");
          } else if (MenuFlag == 60) {
            lcd.setCursor(3, 1);
            lcd.print("МАЛО РЮМОК!");
          } else {
            lcd.setCursor(1, 1);
            lcd.print("СНИМИТЕ РЮМКИ!");
          }
          returnMenu = true;
          VoiceBlock = false;
#ifdef TANK
          play_track(13);  //  трек в папке mp3,  типа предупреждение нет рюмок
#else
          play_track(3);  //  трек в папке mp3,  типа предупреждение нет рюмок
#endif
          delay (500);
          if (playOn) {
            playOn = false;
            player = true;
          }

#ifdef LED_TOWER
          if (ledShowOn == 1) {
            TOWERtimer.setInterval(50);
            mig = true;
            rainbow = false;
            clearLed = true;
            TOWERtimer.reset();
          }
#endif
        }
        DrinkCount = 0;
        systemON = false;
        flagSystemON = true;
        curPumping = -1;  // снимаем выбор рюмки
#ifdef TANK
        muveBack = true;
#endif
#ifdef DOP_SOUND
        if (dopSound) dopSound = false;
#endif
        //DEBUG("no glass");
        PAUSEtimer.reset();
#ifdef SERVO_DETACH_PIN_ON
        servo.detach();
#endif
#ifdef STEPPER_MOTOR_USED
        stepper.disable();  // отключаем питание шаговика
#endif
        if (!promivka && MenuFlag != 20) needZero = true;
      }
    }



  } else if (systemState == MOVING) {  // движение к рюмке
    if (flag) {
      if (!promivka) oled_naliv();
      else menu_promivka(1);
      flag = false;
    }
    servo_move(shotPos[curPumping]);
    if (!moving) {            // если приехали
      systemState = PUMPING;  // режим - наливание
      if (!promivka) {
#ifdef TWO_PUMPS
        if (pourFrom[curPumping] == 0) TIMEProcent.setInterval(time50ml_1 / 50);       // перенастроили таймер для первой помпы
        else if (pourFrom[curPumping] == 1) TIMEProcent.setInterval(time50ml_2 / 50);  // перенастроили таймер для второй помпы
#else
        TIMEProcent.setInterval(time50ml_1 / 50);  // перенастроили таймер для первой помпы
#endif
      } else TIMEProcent.setInterval(1);

      Procent = 0;
      TIMEProcent.reset();
#ifdef TWO_PUMPS
      if (pourFrom[curPumping] == 0) pump1ON();       // НАЛИВАЙ!
      else if (pourFrom[curPumping] == 1) pump2ON();  // НАЛИВАЙ!
#else
      pump1ON();                                   // НАЛИВАЙ!
#endif
      if (!promivka && MenuFlag != 20) {
        DrinkCount++;
        lcd.setCursor(11, 0);
        lcd.print("   ");
        lcd.setCursor(11, 0);
        if (MenuFlag == 1 || MenuFlag == 60) {
          lcd.print(Drink, DEC);
        } else if (MenuFlag == 2) {
          lcd.print(ManDrink[curPumping], DEC);
        } else if (MenuFlag == 61) {
          if (barMan == 1) lcd.print(Drink, DEC);
          else if (barMan == 2) lcd.print(ManDrink[curPumping], DEC);
        }
        lcd.setCursor(2, 1);
        lcd.print((curPumping + 1), DEC);
        lcd.setCursor(11, 1);
        lcd.print("   ");
#ifdef TANK
        play_track(51);  //выстрел танка
#endif
      }
      //DEBUG("fill glass");
    }




  } else if (systemState == PUMPING) {  // если качаем
    if (TIMEProcent.isReady()) {
      Procent++;
      if (!promivka) {
        if ((MenuFlag == 1 || MenuFlag == 20 || MenuFlag == 60) && Procent >= Drink) {
          readyDrink = true;
        } else if (MenuFlag == 2 && Procent >= ManDrink[curPumping]) {
          readyDrink = true;
        } else if (MenuFlag == 61) {
          if (barMan == 1 && Procent >= Drink) readyDrink = true;
          else if (barMan == 2 && Procent >= ManDrink[curPumping]) readyDrink = true;
        }
        lcd.setCursor(12, 1);
        lcd.print(Procent, DEC);
      } else {
        if (MenuFlag == 11 && (Procent >= 7000 || !enc.isHold())) readyDrink = true;
#ifdef BUTTON_TOWER
        else if (MenuFlag != 11 && (Procent >= 7000 || !stateBut)) readyDrink = true;
#endif
      }
    }
    if (readyDrink) {  // если налили
    if (GlasCountEmply !=0) GlasCountEmply--;
#ifdef TWO_PUMPS
      if (pourFrom[curPumping] == 0) pump1OFF();       // помпа выкл
      else if (pourFrom[curPumping] == 1) pump2OFF();  // помпа выкл
#else
      pump1OFF();                                  // помпа выкл
#endif
      shotStates[curPumping] = READY;  // налитая рюмка, статус: готов
#ifndef ROUND_MATRIX
      strip.setPixelColor(curPumping, COLOR_POURET_GLASS);  // подсветили
      strip.show();
#endif
      curPumping = -1;     // снимаем выбор рюмки
      systemState = WAIT;  // режим работы - ждать
      WAITtimer.reset();
      readyDrink = false;
      //DEBUG("wait");
    }



  } else if (systemState == WAIT) {  // ждём
    if (WAITtimer.isReady()) {
      systemState = SEARCH;
      //DEBUG("search");
      if (promivka) {
        systemON = false;
        flagSystemON = true;
        if (MenuFlag == 11) {
          menu_promivka(2);
        } else {
          do {
#ifdef STARTING_POS_ANGLE
            servo_move(STARTING_POS_ANGLE);  // цель - заданная позиция
#else
            servo_move(0);                         // цель - 0
#endif
          } while (moving);
#ifdef SERVO_DETACH_PIN_ON
          servo.detach();
#endif

#ifdef STEPPER_MOTOR_USED
          stepper.disable();  // отключаем питание шаговика
#endif
          needZero = true;
          returnMenu = true;
        }
        promivka = false;
      }
      if (MenuFlag == 20) {
        systemON = false;
        flagSystemON = true;
      }
    }



  } else if (systemState == TOOK) {
    if (PAUSEtimer.isReady()) {
      systemState = WAIT;
    }
  }
}

#ifndef STEPPER_MOTOR_USED
void servo_move(uint8_t target) {
  static uint32_t prevServoTime = 0;
#ifdef SERVO_CHANGE_DIRECTION
  target = INITAL_ANGLE_SERVO - target;
#endif
#ifdef STARTING_POS_ANGLE
#ifdef SERVO_CHANGE_DIRECTION
  static uint8_t pos = INITAL_ANGLE_SERVO - STARTING_POS_ANGLE;
#else
  static uint8_t pos = STARTING_POS_ANGLE;
#endif
#else
#ifdef SERVO_CHANGE_DIRECTION
  static uint8_t pos = INITAL_ANGLE_SERVO;
#else
  static uint8_t pos = 0;
#endif
#endif
  static bool deadTime = false;
  if (!moving) {
    if (pos != target) {
      moving = true;
      deadTime = false;
      prevServoTime = millis();
    }
  }
  if (moving && !deadTime && millis() - prevServoTime >= speedSer) {
    prevServoTime = millis();
    if (pos > target) pos--;
    else if (pos < target) pos++;
    servo.write(pos);
    if (pos == target) {
      deadTime = true;
#ifdef TANK
      if (!playMush && !promivka && Menu != SETTINGS) play_track(52);  //клик башни
#endif
    }
  }

#ifndef PLAYER_MH2024K_24SS
  if (deadTime && millis() - prevServoTime >= POST_PAUSE) {  // пауза после остановки сервы до включения помпы
#else
  if (deadTime && millis() - prevServoTime >= POST_PAUSE+1500) {  // пауза после остановки сервы до включения помпы
#endif
    deadTime = false;
    moving = false;
  }
}
#else
void servo_move(uint8_t target) {
  static bool deadTime = false;
  static uint32_t prevServoTime = 0;
#ifdef STARTING_POS_ANGLE
  static uint8_t pos = STARTING_POS_ANGLE;
#else
  static uint8_t pos = 0;
#endif
  if (!moving) {
    if (pos != target) {
      moving = true;
      deadTime = false;
      pos = target;
      stepper.setTargetDeg(pos);
    }
  }
  if (moving && !deadTime) {
    stepper.tick();
    if (stepper.ready()) {
      deadTime = true;
      prevServoTime = millis();
#ifdef TANK
      if (!playMush && !promivka && Menu != SETTINGS) play_track(52);  //клик башни
#endif
    }
  }
#ifndef PLAYER_MH2024K_24SS
  if (deadTime && millis() - prevServoTime >= POST_PAUSE) {  // пауза после остановки шаговика до включения помпы
#else 
  if (deadTime && millis() - prevServoTime >= POST_PAUSE+1500) {  // пауза после остановки шаговика до включения помпы
#endif
    deadTime = false;
    moving = false;
  }
}
#endif

void energy_saving() {
  if (sleepTime != 0 && !systemON && !tost && !save && SAVEtimer.isReady()) {
#ifdef TANK
          play_track(26);  // звук засыпания
#else         
          play_track(16);  // звук засыпания
#endif
#if (DISPLAY_TYPE == 0)
    lcd.noBacklight();
#else
    lcd.noDisplay();
#endif
    save = true;
    strip.clear();
    strip.show();
#ifdef SEVE_MODE_CONDITION
    delay(1000);
    digitalWrite(SEVE_MODE_PIN, LOW);
#endif
  }
}


void ret_menu() {
  if (returnMenu && PAUSEtimer.isReady()) {
    if (MenuFlag == 1 || MenuFlag == 60) {
      oled_auto();  // выходим в меню автоналива
    } else if (MenuFlag == 2) {
      multi_naliv();  // выходим в меню мультиналива
    } else if (promivka) {
      menu_promivka(0);
    } else if (MenuFlag == 20) {
      kalibr_pump(0);
    } else if (MenuFlag == 61) {
      if (barMan == 1) oled_auto();
      else if (barMan == 2) multi_naliv();
    }

    returnMenu = false;
    promivka = false;
    readySystem = true;
  }
}

#ifdef BAT_MONITOR_ON
void bat_tery() {
  static uint32_t prevBatTime = 0;
  static uint8_t flag = 0;
  static int16_t voltage = 1550;
  if (!systemON && !ledShow && !playMush && millis() - prevBatTime >= 1000UL) {
    prevBatTime = millis();
    voltage = ((voltage * 4) + (esp_adc_cal_raw_to_voltage(adc1_get_raw(ADC1_CHANNEL_3), &adc1_chars))) / 5;
    v_akky = voltage * ADC_U_COEFF;
    //Serial.println(voltage);
    if (v_akky <= BAT_U * 797.7) {  // 6,7/3,35 вольт
      flagBat = 0;
#ifdef LOW_BAT_SLEEP_ON
      if (player) myMP3.stop();
#if (DISPLAY_TYPE == 0)
    lcd.noBacklight();
#else
    lcd.noDisplay();
#endif
      delay (100);
      myMP3.setVolume(27);
      delay (100);
      myMP3.playMp3FolderTrack(54);
      delay (800);
      while (!digitalRead(BUSY_PIN));
      delay (2000);
      myMP3.setVolume(volume);
      if (!lowBat) {
        lowBat = true;  // гасим всё
          if (player) {
          playOn = false;
          player = false;
          myMP3.stop();
        }
        strip.clear();
        strip.show();
      }
#endif
    } else if (v_akky <= BAT_U * 833.4) {  // 7.0/3,5 вольта
      flagBat = 1;
    } else if (v_akky <= BAT_U * 857.2) {  // 7.2/3,6 вольт
      flagBat = 2;
    } else if (v_akky <= BAT_U * 881.0) {  // 7.4/3,7 вольта
      flagBat = 3;
    } else if (v_akky <= BAT_U * 928.6) {  // 7,8/3,9 вольта
      flagBat = 4;
    } else if (v_akky <= BAT_U * 988.1) {  // 8,3/4,2 вольта
      flagBat = 5;
    } else {  // больше 8,3/4,2 вольт от сети
      flagBat = 6;
    }
#ifdef LOW_BAT_SLEEP_ON
      if (lowBat && 0 < flagBat) {
        if (save)  save = false;
        lowBat = false;
#if (DISPLAY_TYPE == 0)
          lcd.backlight();
#else
          lcd.display();
#endif
        check = true;  //  проверяем рюмки
      }
#endif
  #ifdef CHARG_DETECT
     if (CHARG_DETECT == 0) {
       flagCh = !digitalRead (CHARG_PIN);
     } else { flagCh = digitalRead (CHARG_PIN);
     }
     if (!flagCh) {
         if (!lowBat && (flagBat != flag || BATtimer.isReady()) && (MenuFlag <= 6 || MenuFlag == 60 || MenuFlag == 61) && !tost && !returnMenu) {
           flag = flagBat;
           BATtimer.reset();
           battery(flagBat);
        }
     }
  #else
    if (!lowBat && flagBat != flag && (MenuFlag <= 6 || MenuFlag == 60 || MenuFlag == 61) && !tost && !returnMenu) {
      flag = flagBat;
      battery (flag);
    }
  #endif
    if (MenuFlag == 18) {
      lcd.setCursor(5, 1);
      lcd.print(v_akky / 1000.0f, 2);
    }
  }
}
#endif
void mix() {
  if (tracks > 0) {
    for (uint8_t i = 0; i < tracks; i++) TostList[i] = i;  // заполняем массив тостов, последовательно значениями
    num = 0;
    if (mixTracks == 1) {
      for (uint8_t i = tracks - 1; i > 0; i--) {  // перемешиваем значения случайно в массиве так, что они не повторяются
        uint8_t idx = random(i + 1);
        uint8_t t = TostList[i];
        TostList[i] = TostList[idx];
        TostList[idx] = t;
      }
    }
  }
}

void mixA() {
  if (tracksA > 0) {
    for (uint8_t i = 0; i < tracksA; i++) TostListA[i] = i;  // заполняем массив тостов Алисы, последовательно значениями
    numA = 0;
    if (mixTracks == 1) {
      for (uint8_t i = tracksA - 1; i > 0; i--) {  // перемешиваем значения случайно в массиве так, что они не повторяются
        uint8_t idx = random(i + 1);
        uint8_t t = TostListA[i];
        TostListA[i] = TostListA[idx];
        TostListA[idx] = t;
      }
    }
  }
}

void mix_music() {
  if (tracks2 > 0) {
    for (uint8_t i = 0; i < tracks2; i++) MusicList[i] = i;  // заполняем массив песенок, последовательно значениями
    if (mixMusic == 1) {
      for (uint8_t i = tracks2 - 1; i > 0; i--) {  // перемешиваем значения случайно в массиве так, что они не повторяются
        uint8_t idx = random(i + 1);
        uint8_t t = MusicList[i];
        MusicList[i] = MusicList[idx];
        MusicList[idx] = t;
      }
    }
  }
}

void mix_fonmusic() {
  if (track_fonmusic > 0) {
    for (uint8_t i = 0; i < track_fonmusic; i++) FonMusic[i] = i;  // заполняем массив  последовательно значениями
    for (uint8_t i = track_fonmusic - 1; i > 0; i--) {             // перемешиваем значения случайно в массиве так, что они не повторяются
      uint8_t idx = random(i + 1);
      uint8_t t = FonMusic[i];
      FonMusic[i] = FonMusic[idx];
      FonMusic[idx] = t;
    }
  }
}

void play_next() {
  if (player && !pause99 && PLAYtimer.isReady() && !nextTrack) {
    if (digitalRead(BUSY_PIN)) {
      if (playOn) {
        playOn = false;
      }
      nextTrack = true;
      num2++;
    }
  }
  if (nextTrack) {
    PLAYtimer.reset();
    nextTrack = false;
    pause99 = false;
    oldNum = num2;
    if (num2 <= tracks2 - 1) {
      delay (100);
      myMP3.setVolume(volume2);
      delay (100);
      myMP3.playFolderTrack(folder2, (MusicList[num2] + 1));
      if (!player) {
        player = true;
        if (MenuFlag == 3) menu_play(0);
      } else {
        if (MenuFlag == 3) menu_play(4);
      }
      } else {
      player = false;
      num2 = 0;
      myMP3.stop();
      delay(100);
      if (MenuFlag == 3) menu_play(0);
      myMP3.setVolume(volume);
    }
  }
}




#ifdef BUTTON_TOWER
void button_tower() {
  static uint32_t butTowTime = 0;
  static uint8_t clickCount = 0;
  static bool lastStateBut = false;
  static bool flagDubl = false;
  if ((MenuFlag == 1 || MenuFlag == 2 || MenuFlag == 60) && ((!systemON && !returnMenu && !save && !playMush && MenuFlag != 61) || promivka)) {
    stateBut = bitRead(reedPins, BUT_TOWER_PIN);
    if (stateBut != lastStateBut) {
      lastStateBut = stateBut;
      if (!lastStateBut) {
        if (tost) {
          noTost = true;
        } else {
          clickCount++;
          butTowTime = millis();
          flagDubl = true;
        }
      }
    }
    if (flagDubl && millis() - butTowTime >= 400) {  // двойной клик на башне
      flagDubl = false;
      if (clickCount >= 2) {
        if (MenuFlag != 60) {
#ifdef DOP_SOUND
          if (MenuFlag == 1) dopSound = true;
#endif
          systemON = true;
          flag = true;
          if (noDoliv >= 1) readySystem = false;
          if (ledShowOn == 1) {
            if (ledShow) check = true;
#ifdef LED_TOWER
            TOWERtimer.setInterval(50);
            if (mig || rainbow) {
              mig = false;
              rainbow = false;
              clearLed = true;
              TOWERtimer.reset();
            }
#endif
          }
        } else if (MenuFlag == 60) {
          flagMush = true;
        }
        SAVEtimer.reset();
      }
      clickCount = 0;
    }
  }
}
#endif



void play_track(uint8_t tr) {
  if (!player && volume != 0) myMP3.playMp3FolderTrack(tr);
}

#ifdef DOP_SOUND
void dop_sound() {
  if (!player && volume != 0 && digitalRead(BUSY_PIN)) {
    if (Drink <= 20) myMP3.playMp3FolderTrack(30);
    else if (Drink <= 30) myMP3.playMp3FolderTrack(31);
    else if (Drink <= 35) myMP3.playMp3FolderTrack(32);
    else if (Drink <= 40) myMP3.playMp3FolderTrack(33);
    else if (Drink <= 45) myMP3.playMp3FolderTrack(34);
    else if (Drink <= 50) myMP3.playMp3FolderTrack(35);
#ifdef PLAYER_MH2024K_24SS
    delay (2000);
#else
    delay(500);
#endif
    while (!digitalRead(BUSY_PIN)) CvetoMuzik();
      delay(500);
  }
  dopSound = false;
}
#endif

void play_mushket() {
  static bool muveServo = false;
  static uint8_t countRand = 0;
  if (flagMush) {
    flagMush = false;
    countRand = 0;
    for (uint8_t i = 0; i < NUM_SHOTS; i++) {
      if (shotStates[i] == EMPTY) countRand++;  // подсчёт стопок готовых к наливу
      rumka[i] = i;                             // заполняем массив последовательно позициями  рюмок
    }
    if (countRand > 1) {
      playMush = true;
      if (volume != 0) {
        if (ledShowOn == 1) {
          ledShow = true;
          LEDtimer.setInterval(TABLE_RAINBOW_ILLUMINATION_TIMER);
#ifdef LED_TOWER
          TOWERtimer.setInterval(20);
          rainbow = true;
#endif
        }

        if (player) {
          playOn = true;
          player = false;
          myMP3.stop();
          delay(100);
        }
        myMP3.setVolume(volume);
        delay(100);
        myMP3.playMp3FolderTrack(15);  // 0015 трек в папке MP3, рулетка
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("КРУЧУ ВЕРЧУ");
        lcd.setCursor(2, 1);
        lcd.print("НАЛИТЬ ХОЧУ");
#ifndef PLAYER_MH2024K_24SS
        delay(500);
#else
        delay(2000);
#endif
      }
#ifdef SERVO_DETACH_PIN_ON
      servo.attach(SERVO_PIN, SERVO_MIN, SERVO_MAX);
#endif
#ifdef STEPPER_MOTOR_USED
      stepper.enable();  // включаем питание шаговика
#ifdef CHECKING_THE_ZERO_POSITION
      Zero_Pos();  // едем до концевика, ищем домашнюю позицию разливного носика
#endif
#endif
      needZero = false;

    } else {
      systemON = true;
      for (uint8_t i = 0; i < NUM_SHOTS; i++) rumka[i] = -1;
    }
  }

  if (playMush) {
    if (!muveServo) {
      servo_move(shotPos[NUM_SHOTS - 1] - 1);
      if (!moving) muveServo = true;
    } else {
      servo_move(shotPos[0] + 1);
      if (!moving) muveServo = false;
    }
    if (digitalRead(BUSY_PIN) || player) {
      moving = false;
      muveServo = false;
      systemON = true;
      flag = true;  // флаг ,если пустые рюмки найдены показываем меню налива
      readySystem = false;
      countRand = 0;
      for (int8_t i = NUM_SHOTS - 1; i >= 0; i--) {
        int8_t idx = random(i + 1);
        int8_t t = rumka[i];
        rumka[i] = rumka[idx];
        rumka[idx] = t;
        if (shotStates[i] == EMPTY) countRand++;  // снова подсчёт стопок готовых к наливу, вдруг кто снял во время трека рулетки
      }
      randomGlass = 1;
      for (uint8_t i = 0; i < NUM_SHOTS; i++) {
        if (shotStates[rumka[i]] != EMPTY) rumka[i] = -1;  // убираем позиции из разлива, где нет тары
        if (countRand > 1 && subMush == 2) {
          if (i == randomGlass && rumka[i] == -1) randomGlass++;
          else if (i == randomGlass) rumka[i] = -1;  // убираем из разлива одну из установленных рюмок
        }
      }
      playMush = false;
      randomGlass = 0;
      if (ledShowOn == 1) {
        if (ledShow) check = true;
        LEDtimer.setInterval(GLASS_RAINBOW_ILLUMINATION_TIMER);
#ifdef LED_TOWER
        TOWERtimer.setInterval(50);
        if (mig || rainbow) {
          mig = false;
          rainbow = false;
          clearLed = true;
          TOWERtimer.reset();
        }
#endif
      }
    }
  }
}

void move_enc(uint8_t* var, int16_t shift, int16_t lowLimit, int16_t upLimit, bool cycle) {
  int16_t value = (int16_t)*var;
  value += shift;
  if (value > upLimit) value = (cycle) ? lowLimit : upLimit;
  if (value < lowLimit) value = (cycle) ? upLimit : lowLimit;
  *var = (uint8_t)value;
}



void move_enc(int16_t* var, int16_t shift, int16_t lowLimit, int16_t upLimit, bool cycle) {
  *var += shift;
  if (*var > upLimit) *var = (cycle) ? lowLimit : upLimit;
  if (*var < lowLimit) *var = (cycle) ? upLimit : lowLimit;
}

#ifdef TTP229_USED
void Read_Keypad() {
  for (uint8_t j = 0; j < NUM_SHOTS; j++) {
    digitalWrite(SCL_PIN, LOW);
    if (!digitalRead(SDO_PIN)) bitWrite(reedPins, j, 1);
    else bitWrite(reedPins, j, 0);
    digitalWrite(SCL_PIN, HIGH);
  }
}
#endif
#ifdef STEPPER_MOTOR_USED
void Zero_Pos() {
#ifdef ENDSTOP_MOTOR_USED
  if (digitalRead(ENDSTOP_PIN)) {                // если концевик нулевого положения не нажат
    stepper.setTargetDeg(STEPPER_RETURN_ANGLE);  // ставим носик в нулевую позицию
    while (digitalRead(ENDSTOP_PIN)) {           // пока концевик не нажат
      stepper.tick();                            // крутим
      if (stepper.ready()) {
        enc.singl_klick();
        break;
      }
    }
    // концевик нажался - покидаем цикл
    stepper.brake();  // тормозим, приехали
  }
#else
  stepper.setTargetDeg(STEPPER_RETURN_ANGLE);  // ставим носик в нулевую позицию
  while (!stepper.ready()) {                   // пока не доехал до упора
    stepper.tick();                            // крутим
  }
#endif
  stepper.reset();  // сбрасываем координаты в 0
    #ifdef STARTING_POS_ANGLE
    stepper.setTargetDeg(STARTING_POS_ANGLE);  // ставим носик в нужную позицию
    while (!stepper.ready()) {                   // пока не доехал до нужной позиции
    stepper.tick();                            // крутим
    }
    #endif 
}
#endif

void Noready () {
            if (yesGlass == 0) { //НЕТ РЮМОК
            myMP3.playFolderTrack(folderAlisa, random(76, 78+1));  // нужно +1, чтобы сделать его всеобъемлющим  
            }else if (GlasCountEmply == 0) {//СНИМИТЕ РЮМКИ
            myMP3.playFolderTrack(folderAlisa, random(79, 81+1));  // нужно +1, чтобы сделать его всеобъемлющим  
            }else if (GlasCountEmply == 1){ //ДОБАВЬТЕ РЮМКИ
            myMP3.playFolderTrack(folderAlisa, random(82, 84+1)); // нужно +1, чтобы сделать его всеобъемлющим  
            }
            pauseVoise();
            if (playOn) {
              playOn = false;
              player = true;
            }
        //  else //НАЛИВАТОР В РАБОТЕ
        //    {
        //    myMP3.playFolderTrack(folderAlisa, random(82, 84+1)); // нужно +1, чтобы сделать его всеобъемлющим  
        //    }
}

#ifdef BAT_MONITOR_ON
void battery (uint8_t fBat){

      switch (fBat) {
        case 0: lcd.createChar(7, battery0); break;
        case 1: lcd.createChar(7, battery1); break;
        case 2: lcd.createChar(7, battery2); break;
        case 3: lcd.createChar(7, battery3); break;
        case 4: lcd.createChar(7, battery4); break;
        case 5: lcd.createChar(7, battery5); break;
        case 6: lcd.createChar(7, power); break;
      }
      lcd.setCursor(15, 0);
      lcd.write(7);
}


#ifdef CHARG_DETECT
void charge(){
  if (CHARGtimer.isReady()){
      battery(fB);
      CHARGtimer.reset();
      fB++;
      if (fB > 5) fB = 0;
  }
}
#endif
#endif

void pauseVoise(){   // время которое нужно для полного воспроизведения трека//////////////////////////////////////////////////////////////////////////////////////////
      delay(500);
      while (!digitalRead(BUSY_PIN)){
         CvetoMuzik();
         esp_task_wdt_reset();
      }
      delay(500);
}