#define _ASYNC_MQTT_LOGLEVEL_ 0

const char* Nalivator = "nalivator/alise";      // топик наливатора для приёма
const char* Nalivator_Send = "nalivator/send";  // топик наливатора для передачи

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
AsyncWebServer server(80);

extern "C" {
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
}
#include <AsyncMQTT_ESP32.h>


AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;

const char* PARAM_INPUT_1 = "wifi_ssid";
const char* PARAM_INPUT_2 = "wifi_pass";
const char* PARAM_INPUT_3 = "mqtt_login";
const char* PARAM_INPUT_4 = "mqtt_pass";
const char* PARAM_INPUT_5 = "mqtt_host";
const char* PARAM_INPUT_6 = "1234"; /////////////////////////////////////////////////////////////

String WIFI_SSID;
String WIFI_PASS;
String MQTT_LOGIN;
String MQTT_PASS;
String MQTT_HOST;
String MQTT_PORT;
bool startOtaUpdate = false;
String otaUrl = "";

//////////////////////////////////////////////////////////////////////////////////////////////////////////

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="ru">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>НАЛИВАТОР - Настройки</title>
  <style>
    body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background: #121212; color: #e0e0e0; margin: 0; padding: 20px; display: flex; justify-content: center; }
    .container { background: #1e1e1e; padding: 25px; border-radius: 12px; box-shadow: 0 8px 16px rgba(0,0,0,0.6); max-width: 450px; width: 100%; }
    h2 { text-align: center; color: #4CAF50; border-bottom: 2px solid #333; padding-bottom: 15px; margin-top: 0; }
    h3 { color: #81c784; margin-top: 25px; margin-bottom: 10px; font-size: 18px; }
    label { display: block; margin-top: 10px; font-size: 14px; color: #bbb; }
    input[type="text"], input[type="number"], input[type="url"] { width: 100%; padding: 12px; margin-top: 5px; border: 1px solid #333; border-radius: 6px; background: #2c2c2c; color: #fff; box-sizing: border-box; font-size: 16px; transition: border 0.3s; }
    input[type="text"]:focus, input[type="number"]:focus, input[type="url"]:focus { border-color: #4CAF50; outline: none; }
    input[type="submit"] { background: #4CAF50; color: white; border: none; padding: 14px; width: 100%; border-radius: 6px; font-size: 16px; font-weight: bold; cursor: pointer; margin-top: 25px; transition: background 0.3s; }
    input[type="submit"]:hover { background: #45a049; }
    .btn-ota { background: #2196F3 !important; }
    .btn-ota:hover { background: #1976D2 !important; }
  </style>
</head>
<body>
  <div class="container">
    <h2>НАЛИВАТОР</h2>
    <form action="/get">
      <h3>Параметры WiFi</h3>
      <label>SSID сети</label>
      <input type="text" value="%wifi_ssid%" name="wifi_ssid">
      <label>Пароль</label>
      <input type="text" value="%wifi_pass%" name="wifi_pass">
      
      <h3>Параметры MQTT</h3>
      <label>Логин</label>
      <input type="text" value="%mqtt_login%" name="mqtt_login">
      <label>Пароль</label>
      <input type="text" value="%mqtt_pass%" name="mqtt_pass">
      <label>IP Адрес брокера</label>
      <input type="text" value="%mqtt_host%" name="mqtt_host">
      <label>Порт</label>
      <input type="number" value="%1234%" name="1234">
      
      <input type="submit" value="Сохранить настройки">
    </form>

    <form action="/ota" method="GET">
      <h3>Обновление (OTA GitHub)</h3>
      <label>Прямая ссылка на .bin файл (raw url)</label>
      <input type="url" name="ota_url" value="https://raw.githubusercontent.com/geminibitok-oss/nalivator-esp32-c3-v3/firmware/firmware.bin" placeholder="https://raw.githubusercontent.com/geminibitok-oss/nalivator-esp32-c3-v3/firmware/firmware.bin" required>
      <input type="submit" value="Обновить прошивку" class="btn-ota">
    </form>
  </div>
</body>
</html>)rawliteral";

////////////////////////////////////////////////////////////////////////////////////////////////////////

void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
}

String processor(const String& var) {
  if (var == "wifi_ssid") {
    return WIFI_SSID;
  }
  if (var == "wifi_pass") {
    return WIFI_PASS;
  }
  if (var == "mqtt_login") {
    return MQTT_LOGIN;
  }
  if (var == "mqtt_pass") {
    return MQTT_PASS;
  }
  if (var == "mqtt_host") {
    return MQTT_HOST;
  }
  if (var == "1234") {///////////////////////////////////////////////////////////////////////////////
  return MQTT_PORT;
  }
  return String();
}
