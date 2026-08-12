void Voice_command () {
 SAVEtimer.reset();
 bool fv = false;
   if (valNaliv == 28 || valNaliv == 29 || (valNaliv == 34 && player) || (valNaliv == 25 && MenuFlag != 61)  || VoiceBlock) fv = true;
      if (!fv) {
        if (player){ 
          myMP3.stop();
          playOn = true;
          player = false;
          delay (100);
          myMP3.setVolume(volume);
          delay (500);
        } 
         if (save) {
          save = false;
#if (DISPLAY_TYPE == 0)
          lcd.backlight();
#else
          lcd.display();
#endif
         check = true;  //  проверяем рюмки
#ifdef SEVE_MODE_CONDITION
         digitalWrite(SEVE_MODE_PIN, HIGH);
#endif
#ifdef TANK
          play_track(27);  // звук просыпания
#else         
          play_track(17);  // звук просыпания
#endif
          pauseVoise();
         }
      } 
switch (valNaliv) {
      case 1:  //  налей по 50
        if (!VoiceBlock && 0 < GlasCountEmply) {
            Drink = 50;
            Menu = EQUALLY;
            MenuFlag = 1;
            VoiceBlock = true;
            oled_auto();
            myMP3.playFolderTrack(folderAlisa, random(1, 3+1)); // нужно +1, чтобы сделать его всеобъемлющим
            pauseVoise();
            alise = true;
         } else Noready();
        break;

      case 2:  //  налей по 45
        if (!VoiceBlock && 0 < GlasCountEmply) {
            Drink = 45;
            Menu = EQUALLY;
            MenuFlag = 1;
            VoiceBlock = true;
            oled_auto();
            myMP3.playFolderTrack(folderAlisa, random(4, 6+1)); // нужно +1, чтобы сделать его всеобъемлющим
            pauseVoise();
            alise = true;
         } else Noready();
        break;

      case 3:  //  налей по 40
        if (!VoiceBlock && 0 < GlasCountEmply) {
            Drink = 40;
            Menu = EQUALLY;
             MenuFlag = 1;
            VoiceBlock = true;
            oled_auto();
            myMP3.playFolderTrack(folderAlisa, random(7, 9+1)); // нужно +1, чтобы сделать его всеобъемлющим
            pauseVoise();   
            alise = true;
         } else Noready();
        break;

      case 4:  //  налей по 35
        if (!VoiceBlock && 0 < GlasCountEmply) {
            Drink = 35;
            Menu = EQUALLY;
             MenuFlag = 1;
            VoiceBlock = true;
            oled_auto();
            myMP3.playFolderTrack(folderAlisa, random(10, 12+1)); // нужно +1, чтобы сделать его всеобъемлющим 
            pauseVoise();  
            alise = true;
         } else Noready();
        break;

      case 5:  //  налей по 30
        if (!VoiceBlock && 0 < GlasCountEmply) {
            Drink = 30;
            Menu = EQUALLY;
             MenuFlag = 1;
            VoiceBlock = true;
            oled_auto();
            myMP3.playFolderTrack(folderAlisa, random(13, 15+1)); // нужно +1, чтобы сделать его всеобъемлющим 
            pauseVoise();  
            alise = true;
         } else Noready();
        break;

      case 6:  //  налей по 25
        if (!VoiceBlock && 0 < GlasCountEmply) {
            Drink = 25;
            Menu = EQUALLY;
             MenuFlag = 1;
            VoiceBlock = true;
            oled_auto();
            myMP3.playFolderTrack(folderAlisa, random(16, 18+1)); // нужно +1, чтобы сделать его всеобъемлющим  
            pauseVoise(); 
            alise = true;
         } else Noready();
        break;

      case 7:  //  налей по 20
        if (!VoiceBlock && 0 < GlasCountEmply) {
            Drink = 20;
            Menu = EQUALLY;
             MenuFlag = 1;
            VoiceBlock = true;
            oled_auto();
            myMP3.playFolderTrack(folderAlisa, random(19, 21+1)); // нужно +1, чтобы сделать его всеобъемлющим  
            pauseVoise(); 
            alise = true;
         } else Noready();
        break;

      case 8:  //  выполни мультиразлив
        if (!VoiceBlock && 0 < GlasCountEmply) {
            Menu = MULTI_SPILL;
            MenuFlag = 2;
            VoiceBlock = true;
            multi_naliv();
            myMP3.playFolderTrack(folderAlisa, random(22, 24+1)); // нужно +1, чтобы сделать его всеобъемлющим 
            pauseVoise();  
            alise = true;
         } else Noready();
        break;

      case 9:  //  один за всех по 50
        if (!VoiceBlock && 1 < GlasCountEmply) {
            Drink = 50;
            Menu = MUSKETEERS;
            MenuFlag = 60;
            subMush = 1;
            VoiceBlock = true;
            oled_auto();
            myMP3.playFolderTrack(folderAlisa, random(25, 27+1)); // нужно +1, чтобы сделать его всеобъемлющим
            pauseVoise();   
            alise = true;
         } else Noready();
        break;

      case 10:  //  один за всех по 45
        if (!VoiceBlock && 1 < GlasCountEmply) {
            Drink = 45;
            Menu = MUSKETEERS;
            MenuFlag = 60;
            subMush = 1;
            VoiceBlock = true;
            oled_auto();
            myMP3.playFolderTrack(folderAlisa, random(28, 30+1)); // нужно +1, чтобы сделать его всеобъемлющим
            pauseVoise();   
            alise = true;
         } else Noready();
        break;

      case 11:  //  один за всех по 40
        if (!VoiceBlock && 1 < GlasCountEmply) {
            Drink = 40;
            Menu = MUSKETEERS;
            MenuFlag = 60;
            subMush = 1;
            VoiceBlock = true;
            oled_auto();
            myMP3.playFolderTrack(folderAlisa, random(31, 33+1)); // нужно +1, чтобы сделать его всеобъемлющим 
            pauseVoise();  
            alise = true;
         } else Noready();
        break;

      case 12:  //  один за всех по 35
        if (!VoiceBlock && 1 < GlasCountEmply) {
            Drink = 35;
            Menu = MUSKETEERS;
            MenuFlag = 60;
            subMush = 1;
            VoiceBlock = true;
            oled_auto();
            myMP3.playFolderTrack(folderAlisa, random(34, 36+1)); // нужно +1, чтобы сделать его всеобъемлющим
            pauseVoise();   
            alise = true;
         } else Noready();
        break;

      case 13:  //  один за всех по 30
        if (!VoiceBlock && 1 < GlasCountEmply) {
            Drink = 30;
            Menu = MUSKETEERS;
            MenuFlag = 60;
            subMush = 1;
            VoiceBlock = true;
            oled_auto();
            myMP3.playFolderTrack(folderAlisa, random(37, 39+1)); // нужно +1, чтобы сделать его всеобъемлющим 
            pauseVoise();  
            alise = true;
         } else Noready();
        break;

      case 14:  //  один за всех по 25
        if (!VoiceBlock && 1 < GlasCountEmply) {
            Drink = 25;
            Menu = MUSKETEERS;
            MenuFlag = 60;
            subMush = 1;
            VoiceBlock = true;
            oled_auto();
            myMP3.playFolderTrack(folderAlisa, random(40, 42+1)); // нужно +1, чтобы сделать его всеобъемлющим
            pauseVoise();   
            alise = true;
         } else Noready();
        break;

      case 15:  //  один за всех по 20
        if (!VoiceBlock && 1 < GlasCountEmply) {
            Drink = 20;
            Menu = MUSKETEERS;
            MenuFlag = 60;
            subMush = 1;
            VoiceBlock = true;
            oled_auto();
            myMP3.playFolderTrack(folderAlisa, random(43, 45+1)); // нужно +1, чтобы сделать его всеобъемлющим
            pauseVoise();   
            alise = true;
        } else Noready();
        break;


      case 16:  //  все за одного по 50
        if (!VoiceBlock && 1 < GlasCountEmply) {
             Drink = 50;
             Menu = MUSKETEERS;
            MenuFlag = 60;
            subMush = 2;
            VoiceBlock = true;
            oled_auto();
            myMP3.playFolderTrack(folderAlisa, random(46, 48+1)); // нужно +1, чтобы сделать его всеобъемлющим
            pauseVoise();   
            alise = true;
        } else Noready();
        break;

      case 17:  //  все за одного по 45
        if (!VoiceBlock && 1 < GlasCountEmply) {
             Drink = 45;
             Menu = MUSKETEERS;
            MenuFlag = 60;
            subMush = 2;
            VoiceBlock = true;
            oled_auto();
            myMP3.playFolderTrack(folderAlisa, random(49, 51+1)); // нужно +1, чтобы сделать его всеобъемлющим
            pauseVoise();   
            alise = true;
        } else Noready();
        break;

      case 18:  //  все за одного по 40
        if (!VoiceBlock && 1 < GlasCountEmply) {
             Drink = 40;
             Menu = MUSKETEERS;
            MenuFlag = 60;
            subMush = 2;
            VoiceBlock = true;
            oled_auto();
            myMP3.playFolderTrack(folderAlisa, random(52, 54+1)); // нужно +1, чтобы сделать его всеобъемлющим 
            pauseVoise();  
            alise = true;
        } else Noready();
        break;

      case 19:  //  все за одного по 35
        if (!VoiceBlock && 1 < GlasCountEmply) {
             Drink = 35;
             Menu = MUSKETEERS;
            MenuFlag = 60;
            subMush = 2;
            VoiceBlock = true;
            oled_auto();
            myMP3.playFolderTrack(folderAlisa, random(55, 57+1)); // нужно +1, чтобы сделать его всеобъемлющим
            pauseVoise();   
            alise = true;
        } else Noready();
        break;

      case 20:  //  все за одного по 30
        if (!VoiceBlock && 1 < GlasCountEmply) {
             Drink = 30;
             Menu = MUSKETEERS;
            MenuFlag = 60;
            subMush = 2;
            VoiceBlock = true;
            oled_auto();
            myMP3.playFolderTrack(folderAlisa, random(58, 60+1)); // нужно +1, чтобы сделать его всеобъемлющим
            pauseVoise();   
            alise = true;
        } else Noready();
        break;

      case 21:  //  все за одного по 25
        if (!VoiceBlock && 1 < GlasCountEmply) {
             Drink = 25;
             Menu = MUSKETEERS;
            MenuFlag = 60;
            subMush = 2;
            VoiceBlock = true;
            oled_auto();
            myMP3.playFolderTrack(folderAlisa, random(61, 63+1)); // нужно +1, чтобы сделать его всеобъемлющим 
            pauseVoise();  
            alise = true;
        } else Noready();
        break;

      case 22:  //  все за одного по 20
        if (!VoiceBlock && 1 < GlasCountEmply) {
             Drink = 20;
             Menu = MUSKETEERS;
            MenuFlag = 60;
            subMush = 2;
            VoiceBlock = true;
            oled_auto();
            myMP3.playFolderTrack(folderAlisa, random(64, 66+1)); // нужно +1, чтобы сделать его всеобъемлющим
            pauseVoise();   
            alise = true;
        } else Noready();
        break;

      case 23:  //  Включи режим бармен поровну
        if (!VoiceBlock) {
            Menu = BARMAN;
            MenuFlag = 6;
            barMan = 1;
            menu_barmen();
            myMP3.playFolderTrack(folderAlisa, random(67, 69+1)); // нужно +1, чтобы сделать его всеобъемлющим
            pauseVoise();
            if (playOn) {
              playOn = false;
              player = true;
            }   
            alise = true;
        } else Noready();
        break;

      case 24:  //  Включи режим бармен мультиразлив
        if (!VoiceBlock) {
            Menu = BARMAN;
            MenuFlag = 6;
            barMan = 2;
            menu_barmen();
            myMP3.playFolderTrack(folderAlisa, random(70, 72+1)); // нужно +1, чтобы сделать его всеобъемлющим
            pauseVoise(); 
            if (playOn) {
              playOn = false;
              player = true;
            }   
            alise = true;
        } else Noready();
        break;

      case 25:  //  Выключи режим бармен
        if (!VoiceBlock) {
            if (barMan == 1) {
               Menu = EQUALLY;
               MenuFlag = 0;
               oled_auto();
            } else if (barMan == 2) {
               Menu = MULTI_SPILL;
               MenuFlag = 0;
               multi_naliv();
            }
               myMP3.playFolderTrack(folderAlisa, random(73, 75+1)); // нужно +1, чтобы сделать его всеобъемлющим
               pauseVoise();
               if (playOn) {
                  playOn = false;
                  player = true;
               }      
               alise = true;
        } else Noready();
        break;

      case 26:  //  Включи режим 2
        if (!VoiceBlock) {
            myMP3.playFolderTrack(folderAlisa, random(85, 87+1)); // нужно +1, чтобы сделать его всеобъемлющим
            pauseVoise();
            if (playOn) {
              playOn = false;
              player = true;
            }      
            folderAlisa = 97;
        } else Noready();
        break;

      case 27:  //  Включи режим 1
        if (!VoiceBlock) {
            myMP3.playFolderTrack(folderAlisa, random(88, 90+1)); // нужно +1, чтобы сделать его всеобъемлющим   
            pauseVoise();
            if (playOn) {
              playOn = false;
              player = true;
            }   
            folderAlisa = 98;
        } else Noready();
        break;

      case 28:  //  Говори громче
            if (player) {
               volume2 = volume2 + 7;
               if (volume2 > 30) volume2 = 30;
               myMP3.setVolume(volume2);
               if (Menu == PLAYER && MenuFlag == 3) menu_play(2);
            } else { 
               volume = volume + 7;
               if (volume > 30) volume = 30;
               myMP3.setVolume(volume);
            }
        break;

      case 29:  //  Говори тише
            if (player) {
               volume2 = volume2 - 7;
               if (volume2 < 7 || 30 < volume2 ) volume2 = 7;
               myMP3.setVolume(volume2);
               if (Menu == PLAYER && MenuFlag == 3) menu_play(2);
            } else { 
               volume = volume - 7;
               if (volume < 7 || 30 < volume) volume = 7;
               myMP3.setVolume(volume);
            }
        break;

       case 30:  //  Скажи тост
        if (!VoiceBlock) {
            myMP3.stop();
            delay (100);
            myMP3.setVolume(volume);
            delay (100);
            myMP3.playFolderTrack(folderA, (TostListA[numA] + 1));
            pauseVoise();
            if (playOn) {
              playOn = false;
              player = true;
            }   
            if (++numA >= tracksA) numA = 0;
        } else Noready();
        break;

      case 31:  //  налей по 15
        if (!VoiceBlock && 0 < GlasCountEmply) {
            Drink = 15;
            Menu = EQUALLY;
             MenuFlag = 1;
            VoiceBlock = true;
            oled_auto();
            myMP3.playFolderTrack(folderAlisa, random(91, 93+1)); // нужно +1, чтобы сделать его всеобъемлющим 
            pauseVoise();  
            alise = true;
         } else Noready();
        break;

      case 32:  //  один за всех по 15
        if (!VoiceBlock && 1 < GlasCountEmply) {
            Drink = 15;
            Menu = MUSKETEERS;
            MenuFlag = 60;
            subMush = 1;
            VoiceBlock = true;
            oled_auto();
            myMP3.playFolderTrack(folderAlisa, random(94, 96+1)); // нужно +1, чтобы сделать его всеобъемлющим 
            pauseVoise();  
            alise = true;
         } else Noready();
        break;

      case 33:  //  все за одного по 15
        if (!VoiceBlock && 1 < GlasCountEmply) {
             Drink = 15;
             Menu = MUSKETEERS;
            MenuFlag = 60;
            subMush = 2;
            VoiceBlock = true;
            oled_auto();
            myMP3.playFolderTrack(folderAlisa, random(97, 99+1)); // нужно +1, чтобы сделать его всеобъемлющим 
            pauseVoise();  
            alise = true;
         } else Noready();
        break;

      case 34:  //  включи плеер
        if (!VoiceBlock && !player) {
            Menu = PLAYER;
            MenuFlag = 0;
            tracks2 = myMP3.getFolderTrackCount(folder2);
            if(tracks2 > 100) tracks2 = 100;
            nextTrack = true;
            mix_music();
            player = true;
            myMP3.setVolume(volume2);
            menu_play (0);
            alise = true;
         }
         break;

      case 35:  //  выключи плеер
        if (!VoiceBlock) {
            Menu = PLAYER;
            MenuFlag = 0;
            playOn = false;
            delay (100);
            myMP3.setVolume(volume);
            alise = true;
        }
           break;
      
      case 36:  //  пауза
        if (!VoiceBlock) {
            pause99 = !pause99;
            myMP3.pause();
            }
         break;

      case 37:  //  плей
        if (!VoiceBlock) {
            myMP3.start();
            PLAYtimer.reset();
            }
         break;

      case 38:  //  налей сколько не жалко
        if (!VoiceBlock && 0 < GlasCountEmply) {
             Drink = DrinkRnd[random(0, MAX_DRINK/5)];
            Menu = EQUALLY;
            MenuFlag = 1;
            VoiceBlock = true;
            oled_auto();
            myMP3.playFolderTrack(folderAlisa, random(22, 24+1)); // нужно +1, чтобы сделать его всеобъемлющим
            pauseVoise();   
            alise = true;
         } else Noready();
        break;

      case 39:  //  случайный налив
        if (!VoiceBlock && 0 < GlasCountEmply) { 
             for (int i = 0; i < NUM_SHOTS; i++) {
               ManDrink[i] = DrinkRnd[random(0, MAX_DRINK/5)];
             }
             Menu = MULTI_SPILL;
             MenuFlag = 2;
             VoiceBlock = true;
            multi_naliv();
            myMP3.playFolderTrack(folderAlisa, random(22, 24+1)); // нужно +1, чтобы сделать его всеобъемлющим
            pauseVoise();   
            alise = true;
         } else Noready();
        break;

      case 40:  //  смени музыку
        if (!VoiceBlock) {
            folder2 = folder2 + 1;
            if (folder2 > 12) folder2 = 10; 
            Menu = PLAYER;
            MenuFlag = 0;
            tracks2 = myMP3.getFolderTrackCount(folder2);
            if(tracks2 > 100) tracks2 = 100;
            nextTrack = true;
            mix_music();
            player = true;
            myMP3.setVolume(volume2);
            alise = true;
            }
         break;

       
   }
}