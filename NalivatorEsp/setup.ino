void setup() {

#ifndef STEPPER_MOTOR_USED
  servo.setPeriodHertz(50);
#endif 
  strip.begin(); 
  strip.show(); 
#ifdef LED_TOWER
  strip2.begin();
  strip2.show();
#endif

#ifdef BAT_MONITOR_ON
   esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_12, ADC_WIDTH_BIT_12, 0, &adc1_chars);
   adc1_config_width(ADC_WIDTH_BIT_12);
   adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_12); // ~150-2450mV
#endif


  // настройка пинов
  pinMode(PUMP1_POWER, OUTPUT);
#if (PUMP_LEVEL == 0)
  digitalWrite(PUMP1_POWER, HIGH);
#ifdef TWO_PUMPS
  pinMode(PUMP2_POWER, OUTPUT);
  digitalWrite(PUMP2_POWER, HIGH);
#endif
#else
  digitalWrite(PUMP1_POWER, LOW);
#ifdef TWO_PUMPS
  pinMode(PUMP2_POWER, OUTPUT);
  digitalWrite(PUMP2_POWER, LOW);
#endif
#endif

#ifdef CHARG_DETECT
  if (CHARG_DETECT == 0){ pinMode(CHARG_PIN, INPUT_PULLUP); 
  } else { pinMode(CHARG_PIN, INPUT);
  }
#endif 

#ifndef TTP229_USED
  Wire.begin();
  Wire.beginTransmission(PCF8574_ADRES);
  Wire.write(0b11111111);
  Wire.endTransmission();
#else
  pinMode(SCL_PIN, OUTPUT);
  pinMode(SDO_PIN, INPUT_PULLUP);
#endif
  pinMode(BUSY_PIN, INPUT_PULLUP);


#ifdef SEVE_MODE_CONDITION
  pinMode(SEVE_MODE_PIN, OUTPUT);
  digitalWrite(SEVE_MODE_PIN, HIGH);
#endif

#ifdef ENDSTOP_MOTOR_USED
  pinMode(ENDSTOP_PIN, INPUT_PULLUP);
#endif
pinMode(Pin_AP, INPUT_PULLUP);

nalivsave.begin("nalivator", false);
nalivsave.getBytes("shotPos", &shotPos, sizeof(shotPos));  // считываем из памяти положение для сервы

  Procent = 1;
  for (uint8_t i = 0; i < NUM_SHOTS; i++) {
    if (shotPos[i] < 1 || shotPos[i] > 180) shotPos[i] = Procent;  // если ячейки памяти не в интервале, то ставим начальные значения
    Procent += (180 / (NUM_SHOTS - 1)) - 1;
  }

  nalivsave.getBytes("ManDrink", &ManDrink, sizeof(ManDrink));  // считываем из памяти, дриньки для мультиразлива
  for (uint8_t i = 0; i < NUM_SHOTS; i++) {
    if (ManDrink[i] < MIN_DRINK || ManDrink[i] > MAX_DRINK) ManDrink[i] = 20;  // если ячейки памяти не в интервале, то ставим начальные значения
  }

  volume = nalivsave.getUChar("volume");  // считываем из памяти громкость
  if (volume > 30) volume = 10;

  
  bright = nalivsave.getUChar("bright");  // считываем из памяти яркость led


  speedSer = nalivsave.getUChar("speedSer");
  if (speedSer < 2 || speedSer > 30) speedSer = 10;


  sleepTime = nalivsave.getUChar("sleepTime"); // считываем из памяти время таймера сна
  if (sleepTime > 20) sleepTime = 0;
  else SAVEtimer.setInterval(sleepTime * 30000UL);

  
  time50ml_1 = nalivsave.getShort("time50ml_1");  // считываем из памяти время налива 50 мл для первой помпы.
  if (time50ml_1 < 1000 || time50ml_1 > 10000) time50ml_1 = 5000;
#ifdef TWO_PUMPS
  time50ml_2 = nalivsave.getShort("time50ml_2");  // считываем из памяти время налива 50 мл для второй помпы.
  if (time50ml_2 < 1000 || time50ml_2 > 10000) time50ml_2 = 5000;
#endif

  mixTracks = nalivsave.getUChar("mixTracks");  // считываем из памяти флаг перемешивания тостов
  if (mixTracks > 1) mixTracks = 0;

  
  folder = nalivsave.getUChar("folder");  // считываем из памяти номер папки треков тостов
  if (folder < 1 || folder > 9) folder = 1;
  oldFolder = folder;

  
  noDoliv = nalivsave.getUChar("noDoliv");  // считываем из памяти флаг долива
  if (noDoliv > 1) noDoliv = 1;

  
  volume2 = nalivsave.getUChar("volume2");  // считываем из памяти громкость для плеера
  if (volume2 > 30) volume2 = 10;

  
  mixMusic = nalivsave.getUChar("mixMusic");  // считываем из памяти флаг перемешивания песенок
  if (mixMusic > 1) mixMusic = 1;

  
  ledShowOn = nalivsave.getUChar("ledShowOn");  // считываем из памяти флаг включения иллюминации стола
  if (ledShowOn > 1) ledShowOn = 1;
  if (ledShowOn == 1) {
    ledShow = true;
#ifdef LED_TOWER
    rainbow = true;
#endif
  } else {
    LEDtimer.setInterval(GLASS_RAINBOW_ILLUMINATION_TIMER);
    check = true;
  }

  
  noTostBarmen = nalivsave.getUChar("noTostBarmen");  // считываем из памяти флаг тостов в  бармене
  if (noTostBarmen > 1) noTostBarmen = 1;

#ifdef TWO_PUMPS
  nalivsave.getBytes("pourFrom", &pourFrom, sizeof(pourFrom)); // считываем из памяти номер разливного крана, если включена опция двух кранов
  for (uint8_t i = 0; i < NUM_SHOTS; i++) {
    if (pourFrom[i] > 1) pourFrom[i] = 0;  // если ячейки памяти не в интервале, то ставим начальные значения
  }
#endif

for (uint8_t i = 0; i < (MAX_DRINK/5); i++) DrinkRnd[i] = i*5+5;// заполняем массив значениями кратными 5

  Procent = 0;

  WIFI_SSID = nalivsave.getString("wifi_ssid");  //
  WIFI_PASS = nalivsave.getString("wifi_pass");  //
  MQTT_LOGIN = nalivsave.getString("mqtt_login");  //
  MQTT_PASS = nalivsave.getString("mqtt_pass");  //
  MQTT_HOST = nalivsave.getString("mqtt_host");  //
  MQTT_PORT = nalivsave.getString("1234");  //////////////////////////////////////////////////////////////////////////////////////////////////////  

  nalivsave.end();

  Serial.begin(115200);
  delay(100);
  Serial.println(F("\n\n======================================================"));
  Serial.print(F("        NALIVATOR ESP32-C3 | Прошивка v"));
  Serial.println(F(FIRMWARE_VERSION));
  Serial.print(F("        Сборка: "));
  Serial.println(F(BUILD_TIMESTAMP));
  Serial.println(F("  Для справки по командам отправьте: help или ?       "));
  Serial.println(F("======================================================"));


mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(5000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(5000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));


  WiFi.onEvent(WiFiEvent);
  mqttClient.setCredentials(MQTT_LOGIN.c_str(), MQTT_PASS.c_str());
  mqttClient.setKeepAlive (120);
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.setServer(MQTT_HOST.c_str(), atoi(MQTT_PORT.c_str()));
    
    Serial.print("WIFI_SSID - ");
    Serial.println(WIFI_SSID);
    Serial.print("WIFI_PASS - ");
    Serial.println(WIFI_PASS);
    Serial.print("MQTT_LOGIN - ");
    Serial.println(MQTT_LOGIN);
    Serial.print("MQTT_PASS - ");
    Serial.println(MQTT_PASS);
    Serial.print("MQTT_HOST - ");
    Serial.println(MQTT_HOST);
    Serial.print("MQTT_PORT - ");/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.println(MQTT_PORT);////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  connectToWifi();

   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", index_html, processor);
  });

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    nalivsave.begin("nalivator", false);
    if (request->hasParam(PARAM_INPUT_1)) {
      WIFI_SSID = request->getParam(PARAM_INPUT_1)->value();
      nalivsave.putString("wifi_ssid", WIFI_SSID);
    }
    if (request->hasParam(PARAM_INPUT_2)) {
      WIFI_PASS = request->getParam(PARAM_INPUT_2)->value();
      nalivsave.putString("wifi_pass", WIFI_PASS);
    }
    if (request->hasParam(PARAM_INPUT_3)) {
      MQTT_LOGIN = request->getParam(PARAM_INPUT_3)->value();
      nalivsave.putString("mqtt_login", MQTT_LOGIN);
    }
    if (request->hasParam(PARAM_INPUT_4)) {
      MQTT_PASS = request->getParam(PARAM_INPUT_4)->value();
      nalivsave.putString("mqtt_pass", MQTT_PASS);
    }
    if (request->hasParam(PARAM_INPUT_5)) {
      MQTT_HOST = request->getParam(PARAM_INPUT_5)->value();
      nalivsave.putString("mqtt_host", MQTT_HOST);
    }
    if (request->hasParam(PARAM_INPUT_6)) {/////////////////////////////////////////////////////////////////////////////////////////////
      MQTT_PORT = request->getParam(PARAM_INPUT_6)->value();
      nalivsave.putString("1234", MQTT_PORT);
    }
      request->onDisconnect([](){ 
        ESP.restart();
        });
    request->send(200, "text/html;charset=utf-8", "<center><br><br><h1>ПЕРЕЗАГРУЗКА НАЛИВАТОРА</h1><br><br><a href=\"/\"><h2>НАЗАД</h2></a></center>");
    nalivsave.end();                               
  });

  server.on("/ota", HTTP_GET, [] (AsyncWebServerRequest *request) {
    if (request->hasParam("ota_url")) {
      otaUrl = request->getParam("ota_url")->value();
      startOtaUpdate = true;
      request->send(200, "text/html;charset=utf-8", "<center><br><br><h1>ОБНОВЛЕНИЕ ЗАПУЩЕНО</h1><p style=\"font-size: 24px\">Устройство скачивает прошивку... Не выключайте питание!</p><br><br><a href=\"/\"><h2>НАЗАД</h2></a></center>");
    } else {
      request->send(400, "text/plain", "Error: No URL provided");
    }
  });

  server.onNotFound(notFound);
  server.begin();

#ifdef FIRMWARE_BY_AIR
  ArduinoOTA.begin();
#endif  

  myMP3.begin(9600, PLAYER_SERIAL_TIMEOUT);  // скорость порта, таймаут порта мс.
  delay(1000);
  myMP3.setEq(DfMp3_Eq_Normal);
  delay(100);                                  //Между двумя командами необходимо делать задержку 100 миллисекунд, в противном случае некоторые команды могут работать не стабильно.
  tracks = myMP3.getFolderTrackCount(folder);  // считываем количество треков в папке 01-09 в корне флешки, не больше 100
  delay(100);
  if (tracks > 100) tracks = 100;
  delay(100);
  tracksA = myMP3.getFolderTrackCount(folderA);// считываем количество треков в папке c тоставми Алисы в корне флешки, не больше 100
  if (tracksA > 100) tracksA = 100;
  myMP3.setVolume(volume);
  delay(100);
  track_fonmusic = myMP3.getFolderTrackCount(99);
  delay(100);
  if (track_fonmusic > NUM_FON_MUSIC) track_fonmusic = NUM_FON_MUSIC;
#ifdef TANK
  play_track(50);  //танк завёлся
#else
  play_track(1);  // Проигрываем "mp3/0001.mp3"(0001_get started!.mp3)
#endif
  lcd.init();  // Инициализация дисплея
#if (DISPLAY_TYPE == 0)
  lcd.backlight();
#else
  lcd.display();
#endif
#ifdef TANK
  lcd.setCursor(6, 0);
  lcd.print("ТАНК");
  lcd.setCursor(3, 1);
  lcd.print("НАЛИВАТОР+");
#else
  lcd.setCursor(7, 0);
  lcd.print("Ну,");
  lcd.setCursor(1, 1);
  lcd.print("НАА-ЧАА-ЛИИИИ!");
#endif
  strip.setBrightness(bright);
#ifdef LED_TOWER
  strip2.setBrightness(bright);
#endif
#ifndef STEPPER_MOTOR_USED
  servo.attach(SERVO_PIN, SERVO_MIN, SERVO_MAX);
#ifdef STARTING_POS_ANGLE
#ifdef SERVO_CHANGE_DIRECTION
  servo.write(INITAL_ANGLE_SERVO - STARTING_POS_ANGLE);
#else
  servo.write(STARTING_POS_ANGLE);
#endif
#else
#ifdef SERVO_CHANGE_DIRECTION
  servo.write(INITAL_ANGLE_SERVO);
#else
  servo.write(0);
#endif
#endif
  delay(2000);
#ifdef SERVO_DETACH_PIN_ON
  servo.detach();
#endif
#else
#ifdef INVERT_MOTOR
  stepper.reverse(true);
#endif
  stepper.setMaxSpeedDeg(speedSer * SPEED_MULTIPLIER);  // скорость движения шаговика
  stepper.enable();                                     // включаем питание шаговика
  Zero_Pos();                                           // едем до концевика, ищем домашнюю позицию разливного носика
  stepper.disable();                                    // отключаем питание шаговика
  delay(2000);
#endif
  oled_menu();
  mix();
  mix_fonmusic();
  mixA();

}
