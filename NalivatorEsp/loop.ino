
void performOtaUpdate(String url) {
    #if (DISPLAY_TYPE == 0)
      lcd.backlight();
    #else
      lcd.display();
    #endif
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("OTA UPDATE...");
    Serial.println("Starting OTA from: " + url);

    WiFiClientSecure client;
    client.setInsecure(); // Ignore SSL cert checks for GitHub raw links
    httpUpdate.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

    t_httpUpdate_return ret = httpUpdate.update(client, url);

    switch (ret) {
        case HTTP_UPDATE_FAILED:
            Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
            lcd.setCursor(0, 1);
            lcd.print("FAILED!");
            delay(3000);
            break;
        case HTTP_UPDATE_NO_UPDATES:
            Serial.println("HTTP_UPDATE_NO_UPDATES");
            lcd.setCursor(0, 1);
            lcd.print("NO UPDATES");
            delay(3000);
            break;
        case HTTP_UPDATE_OK:
            Serial.println("HTTP_UPDATE_OK");
            break;
    }
}

void loop() {
    uartTick();

    if (startOtaUpdate) {
        startOtaUpdate = false;
        performOtaUpdate(otaUrl);
    }
#ifdef FIRMWARE_BY_AIR
  ArduinoOTA.handle();
#endif
#ifdef BAT_MONITOR_ON
  if (!lowBat) {
#endif
    encTick();
    if (FLOWdebounce.isReady()) {
#ifdef TTP229_USED
      Read_Keypad();
#else
    Wire.beginTransmission(PCF8574_ADRES);
    Wire.endTransmission();
    Wire.requestFrom(PCF8574_ADRES, 1);
    reedPins = Wire.read();
#endif
      flowTick();
    }
    if (systemON) {
      flowRoutnie();  // если активны - ищем рюмки и всё такое
    }
    CvetoMuzik();
    play_mushket();
    energy_saving();
    Tost();
    ret_menu();
    play_next();
#ifdef BUTTON_TOWER
    button_tower();
#endif
#ifdef BAT_MONITOR_ON
  }
   #ifdef CHARG_DETECT
     if (flagCh)  charge();
   #endif
  bat_tery();
#endif
}
