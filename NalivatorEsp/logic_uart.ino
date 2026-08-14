// =============================================================================
// UART SERIAL CLI - Консоль управления и настройки через UART
// =============================================================================

String uartBuffer = "";

void printUartDivider() {
  Serial.println(F("======================================================"));
}

void printUartIpInfo() {
  printUartDivider();
  if (WiFi.getMode() == WIFI_AP) {
    Serial.println(F(" [WiFi Режим]  : ТОЧКА ДОСТУПА (Access Point - AP)"));
    Serial.println(F(" [Имя сети AP] : NALIVATOR"));
    Serial.print(F(" [IP Адрес]    : "));
    Serial.println(WiFi.softAPIP());
    Serial.print(F(" [Веб-сайт]    : http://"));
    Serial.println(WiFi.softAPIP());
  } else {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println(F(" [WiFi Режим]  : КЛИЕНТ (STA) - ПОДКЛЮЧЕНО"));
      Serial.print(F(" [Подключено к]: "));
      Serial.println(WiFi.SSID());
      Serial.print(F(" [IP Адрес]    : "));
      Serial.println(WiFi.localIP());
      Serial.print(F(" [Сигнал RSSI] : "));
      Serial.print(WiFi.RSSI());
      Serial.println(F(" dBm"));
      Serial.print(F(" [Веб-сайт]    : http://"));
      Serial.println(WiFi.localIP());
    } else {
      Serial.println(F(" [WiFi Режим]  : КЛИЕНТ (STA) - НЕ ПОДКЛЮЧЕНО"));
      Serial.print(F(" [Попытка к]   : "));
      Serial.println(WIFI_SSID.length() > 0 ? WIFI_SSID : "(не задана)");
    }
  }
  printUartDivider();
}

void printUartHelp() {
  printUartDivider();
  Serial.println(F("          КОМАНДЫ UART ДЛЯ НАЛИВАТОРА ESP32           "));
  printUartDivider();
  Serial.println(F(" status        - Показать текущий IP, Wi-Fi и MQTT статус"));
  Serial.println(F(" ip            - Быстро узнать текущий IP-адрес"));
  Serial.println(F(" scan          - Сканировать доступные сети Wi-Fi"));
  Serial.println(F(" wifi <ssid> [пароль] - Настроить и подключить Wi-Fi"));
  Serial.println(F("   Пример: wifi MyHome 12345678"));
  Serial.println(F("   Пример: wifi \"Open Cafe\" (для открытой сети)"));
  Serial.println(F(" mqtt <host> <port> [login] [pass] - Настроить MQTT"));
  Serial.println(F("   Пример: mqtt 192.168.1.50 1883 admin pass123"));
  Serial.println(F(" ap            - Принудительно включить точку NALIVATOR"));
  Serial.println(F(" restart       - Перезагрузить наливатор"));
  Serial.println(F(" ota <url>     - Запустить обновление прошивки по ссылке"));
  Serial.println(F(" help или ?    - Показать эту справку"));
  printUartDivider();
}

void printUartStatus() {
  printUartDivider();
  Serial.println(F("             ТЕКУЩИЕ НАСТРОЙКИ И СТАТУС               "));
  printUartDivider();
  Serial.print(F(" Версия прошивки : v"));
  Serial.println(F(FIRMWARE_VERSION));
  Serial.print(F(" Дата сборки     : "));
  Serial.println(F(BUILD_TIMESTAMP));
  Serial.println(F(" ----------------------------------------------------"));
  
  // Wi-Fi
  if (WiFi.getMode() == WIFI_AP) {
    Serial.println(F(" Wi-Fi Режим     : Режим точки доступа (AP)"));
    Serial.println(F(" SSID Точки      : NALIVATOR"));
    Serial.print(F(" IP адрес        : "));
    Serial.println(WiFi.softAPIP());
    Serial.print(F(" Веб-интерфейс   : http://"));
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.print(F(" Wi-Fi Режим     : Клиент (STA), "));
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println(F("ПОДКЛЮЧЕНО"));
      Serial.print(F(" Текущая сеть    : "));
      Serial.println(WiFi.SSID());
      Serial.print(F(" IP адрес        : "));
      Serial.println(WiFi.localIP());
      Serial.print(F(" Уровень сигнала : "));
      Serial.print(WiFi.RSSI());
      Serial.println(F(" dBm"));
      Serial.print(F(" Веб-интерфейс   : http://"));
      Serial.println(WiFi.localIP());
    } else {
      Serial.println(F("ОТКЛЮЧЕНО / ПОИСК"));
    }
  }
  
  Serial.print(F(" Сохраненный SSID: "));
  Serial.println(WIFI_SSID.length() > 0 ? WIFI_SSID : "(пусто)");
  Serial.print(F(" Сохраненный Pass: "));
  Serial.println(WIFI_PASS.length() > 0 ? "********" : "(пусто)");
  
  // MQTT
  Serial.println(F(" ----------------------------------------------------"));
  Serial.print(F(" MQTT Брокер     : "));
  Serial.print(MQTT_HOST.length() > 0 ? MQTT_HOST : "(не задан)");
  Serial.print(F(":"));
  Serial.println(MQTT_PORT.length() > 0 ? MQTT_PORT : "1883");
  Serial.print(F(" MQTT Логин      : "));
  Serial.println(MQTT_LOGIN.length() > 0 ? MQTT_LOGIN : "(пусто)");
  Serial.print(F(" MQTT Статус     : "));
  Serial.println(mqttClient.connected() ? "ПОДКЛЮЧЕНО" : "ОТКЛЮЧЕНО");
  
  printUartDivider();
}

void uartScanWifi() {
  Serial.println(F("[*] Поиск сетей Wi-Fi (подождите 2-3 сек)..."));
  int n = WiFi.scanNetworks();
  if (n == 0) {
    Serial.println(F("[-] Сети Wi-Fi не найдены."));
  } else {
    Serial.print(F("[+] Найдено сетей: "));
    Serial.println(n);
    printUartDivider();
    for (int i = 0; i < n; ++i) {
      Serial.printf(" %2d: %-25s (Сигнал: %3d dBm, Защита: %s)\n", 
                    i + 1, 
                    WiFi.SSID(i).c_str(), 
                    WiFi.RSSI(i), 
                    WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? "ОТКРЫТА" : "ЗАЩИЩЕНА");
    }
    printUartDivider();
    Serial.println(F("Для подключения отправьте: wifi <ИмяСети> <Пароль>"));
  }
  WiFi.scanDelete();
}

void parseUartCommand(String cmd) {
  cmd.trim();
  if (cmd.length() == 0) return;

  Serial.println();
  Serial.print(F("> "));
  Serial.println(cmd);

  String lowerCmd = cmd;
  lowerCmd.toLowerCase();

  if (lowerCmd == "help" || lowerCmd == "?" || lowerCmd == "h") {
    printUartHelp();
  } 
  else if (lowerCmd == "status" || lowerCmd == "info") {
    printUartStatus();
  } 
  else if (lowerCmd == "ip") {
    printUartIpInfo();
  } 
  else if (lowerCmd == "scan") {
    uartScanWifi();
  } 
  else if (lowerCmd.startsWith("wifi ") || lowerCmd.startsWith("set wifi ")) {
    // Извлекаем аргументы
    int firstSpace = cmd.indexOf(' ');
    String args = cmd.substring(firstSpace + 1);
    args.trim();
    if (args.startsWith("wifi ")) {
      args = args.substring(5);
      args.trim();
    }

    String newSsid = "";
    String newPass = "";

    // Поддержка кавычек для имени сети с пробелами: wifi "My Network" password
    if (args.startsWith("\"")) {
      int endQuote = args.indexOf('"', 1);
      if (endQuote != -1) {
        newSsid = args.substring(1, endQuote);
        newPass = args.substring(endQuote + 1);
        newPass.trim();
      }
    }

    if (newSsid.length() == 0) {
      int spaceIdx = args.indexOf(' ');
      if (spaceIdx != -1) {
        newSsid = args.substring(0, spaceIdx);
        newPass = args.substring(spaceIdx + 1);
        newPass.trim();
      } else {
        newSsid = args;
        newPass = "";
      }
    }

    if (newSsid.length() > 0) {
      WIFI_SSID = newSsid;
      WIFI_PASS = newPass;

      nalivsave.begin("nalivator", false);
      nalivsave.putString("wifi_ssid", WIFI_SSID);
      nalivsave.putString("wifi_pass", WIFI_PASS);
      nalivsave.end();

      Serial.println(F("[OK] Настройки Wi-Fi сохранены!"));
      Serial.print(F("     SSID  : ")); Serial.println(WIFI_SSID);
      Serial.print(F("     Пароль: ")); Serial.println(WIFI_PASS.length() > 0 ? "********" : "(без пароля)");
      Serial.println(F("[*] Подключение к Wi-Fi..."));

      countWifi = 0;
      WiFi.disconnect();
      WiFi.mode(WIFI_STA);
      WiFi.begin(WIFI_SSID.c_str(), WIFI_PASS.c_str());
    } else {
      Serial.println(F("[ОШИБКА] Укажите имя сети! Формат: wifi <SSID> [Пароль]"));
    }
  } 
  else if (lowerCmd.startsWith("mqtt ") || lowerCmd.startsWith("set mqtt ")) {
    int firstSpace = cmd.indexOf(' ');
    String args = cmd.substring(firstSpace + 1);
    args.trim();
    if (args.startsWith("mqtt ")) {
      args = args.substring(5);
      args.trim();
    }

    // Парсим: <host> <port> [login] [pass]
    String parts[4];
    int partCount = 0;
    int curPos = 0;
    while (curPos < args.length() && partCount < 4) {
      while (curPos < args.length() && args.charAt(curPos) == ' ') curPos++;
      if (curPos >= args.length()) break;
      int nextSpace = args.indexOf(' ', curPos);
      if (nextSpace == -1) {
        parts[partCount++] = args.substring(curPos);
        break;
      } else {
        parts[partCount++] = args.substring(curPos, nextSpace);
        curPos = nextSpace + 1;
      }
    }

    if (partCount >= 2) {
      MQTT_HOST = parts[0];
      MQTT_PORT = parts[1];
      if (partCount >= 3) MQTT_LOGIN = parts[2];
      if (partCount >= 4) MQTT_PASS = parts[3];

      nalivsave.begin("nalivator", false);
      nalivsave.putString("mqtt_host", MQTT_HOST);
      nalivsave.putString("1234", MQTT_PORT);
      nalivsave.putString("mqtt_login", MQTT_LOGIN);
      nalivsave.putString("mqtt_pass", MQTT_PASS);
      nalivsave.end();

      mqttClient.setCredentials(MQTT_LOGIN.c_str(), MQTT_PASS.c_str());
      mqttClient.setServer(MQTT_HOST.c_str(), atoi(MQTT_PORT.c_str()));

      Serial.println(F("[OK] Параметры MQTT сохранены!"));
      Serial.print(F("     Брокер: ")); Serial.print(MQTT_HOST); Serial.print(F(":")); Serial.println(MQTT_PORT);
      Serial.print(F("     Логин : ")); Serial.println(MQTT_LOGIN);
      
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println(F("[*] Переподключение к MQTT..."));
        connectToMqtt();
      }
    } else {
      Serial.println(F("[ОШИБКА] Формат команды: mqtt <host> <port> [login] [pass]"));
    }
  } 
  else if (lowerCmd == "ap") {
    Serial.println(F("[*] Принудительное переключение в режим точки доступа (AP)..."));
    xTimerStop(wifiReconnectTimer, 0);
    WiFi.mode(WIFI_AP);
    WiFi.softAP("NALIVATOR");
    printUartIpInfo();
  } 
  else if (lowerCmd == "restart" || lowerCmd == "reboot") {
    Serial.println(F("[*] Перезагрузка ESP32..."));
    delay(500);
    ESP.restart();
  } 
  else if (lowerCmd.startsWith("ota ")) {
    String url = cmd.substring(4);
    url.trim();
    if (url.length() > 0) {
      otaUrl = url;
      startOtaUpdate = true;
      Serial.println(F("[*] Запущено OTA обновление..."));
    } else {
      Serial.println(F("[ОШИБКА] Укажите URL прошивки!"));
    }
  } 
  else {
    Serial.print(F("[?] Неизвестная команда: "));
    Serial.println(cmd);
    Serial.println(F("Отправьте 'help' для списка доступных команд."));
  }
}

void uartTick() {
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\r' || c == '\n') {
      if (uartBuffer.length() > 0) {
        parseUartCommand(uartBuffer);
        uartBuffer = "";
      }
    } else {
      if (uartBuffer.length() < 256) {
        uartBuffer += c;
      }
    }
  }
}
