#pragma once
#include <Arduino.h>

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
const char* PARAM_INPUT_6 = "1234";

String WIFI_SSID;
String WIFI_PASS;
String MQTT_LOGIN;
String MQTT_PASS;
String MQTT_HOST;
String MQTT_PORT;
bool startOtaUpdate = false;
String otaUrl = "";

#include "version.h"

const char index_html[] PROGMEM = R"rawliteral(<!DOCTYPE HTML>
<html lang="ru">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>НАЛИВАТОР ESP32</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; }
    body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif; background: #0f172a; color: #f8fafc; min-height: 100vh; padding: 20px; display: flex; justify-content: center; }
    .container { background: #1e293b; padding: 24px; border-radius: 16px; box-shadow: 0 10px 25px rgba(0,0,0,0.5); max-width: 480px; width: 100%; }
    
    .header { text-align: center; border-bottom: 1px solid #334155; padding-bottom: 16px; margin-bottom: 20px; }
    .header h2 { font-size: 24px; color: #38bdf8; font-weight: 700; }
    .version-badge { display: inline-block; background: #0369a1; color: #e0f2fe; font-size: 13px; font-weight: 600; padding: 4px 10px; border-radius: 12px; margin-top: 6px; }
    
    .section-title { font-size: 16px; font-weight: 600; color: #94a3b8; margin: 20px 0 12px; text-transform: uppercase; letter-spacing: 0.5px; border-left: 3px solid #38bdf8; padding-left: 8px; }
    
    label { display: block; margin-top: 10px; font-size: 13px; color: #cbd5e1; font-weight: 500; }
    input[type="text"], input[type="password"], input[type="number"], select {
      width: 100%; padding: 12px; margin-top: 5px; border: 1px solid #475569; border-radius: 8px; background: #0f172a; color: #fff; font-size: 15px; outline: none; transition: border-color 0.2s;
    }
    input:focus, select:focus { border-color: #38bdf8; }
    
    .btn { display: block; width: 100%; border: none; padding: 13px; border-radius: 8px; font-size: 15px; font-weight: 600; cursor: pointer; margin-top: 18px; transition: background 0.2s, transform 0.1s; text-align: center; text-decoration: none; }
    .btn:active { transform: scale(0.98); }
    .btn-save { background: #10b981; color: white; }
    .btn-save:hover { background: #059669; }
    .btn-ota { background: #0284c7; color: white; }
    .btn-ota:hover { background: #0369a1; }
    .btn-check { background: #6366f1; color: white; margin-top: 8px; }
    .btn-check:hover { background: #4f46e5; }
    
    .card { background: #0f172a; padding: 14px; border-radius: 10px; border: 1px solid #334155; margin-top: 10px; }
    .status-text { font-size: 13px; color: #94a3b8; line-height: 1.5; }
    .status-highlight { color: #38bdf8; font-weight: 600; }
    
    .release-item { background: #1e293b; border: 1px solid #475569; border-radius: 8px; padding: 10px; margin-top: 8px; }
    .release-title { font-weight: 600; font-size: 14px; color: #f1f5f9; display: flex; justify-content: space-between; }
    .release-date { font-size: 11px; color: #94a3b8; }
  </style>
</head>
<body>
  <div class="container">
    <div class="header">
      <h2>🍸 НАЛИВАТОР ESP32</h2>
      <div class="version-badge">Текущая версия: v%firmware_ver%</div>
    </div>

    <!-- СЕКЦИЯ ОБНОВЛЕНИЯ ПРОШИВКИ -->
    <div class="section-title">Обновление прошивки (GitHub OTA)</div>
    <div class="card">
      <p class="status-text">Проверка новых версий из репозитория GitHub в один клик:</p>
      <button type="button" class="btn btn-check" id="checkBtn" onclick="checkGitHubReleases()">🔍 Проверить новые версии на GitHub</button>
      
      <div id="releasesArea" style="margin-top: 12px; display: none;">
        <label>Доступные версии прошивки:</label>
        <select id="releaseSelect" onchange="onSelectRelease()"></select>
        <div id="releaseNotes" style="font-size: 12px; color: #94a3b8; margin-top: 6px;"></div>
      </div>
    </div>

    <form action="/ota" method="GET" style="margin-top: 12px;">
      <label>URL адрес прошивки (.bin):</label>
      <input type="text" id="ota_url" name="ota_url" value="https://raw.githubusercontent.com/geminibitok-oss/nalivator-esp32-c3-v3/firmware/firmware.bin" required>
      <button type="submit" class="btn btn-ota" onclick="return confirm('Начать обновление прошивки? Не выключайте питание устройства!')">🚀 Установить выбранную прошивку</button>
    </form>

    <!-- НАСТРОЙКИ СЕТИ И MQTT -->
    <div class="section-title" style="margin-top: 28px;">Параметры Wi-Fi</div>
    <form action="/get">
      <label>Имя сети (SSID):</label>
      <input type="text" value="%wifi_ssid%" name="wifi_ssid" required>
      
      <label>Пароль от Wi-Fi:</label>
      <input type="password" value="%wifi_pass%" name="wifi_pass" placeholder="Пароль">

      <div class="section-title" style="margin-top: 24px;">Параметры MQTT</div>
      <label>Логин брокера:</label>
      <input type="text" value="%mqtt_login%" name="mqtt_login">
      
      <label>Пароль брокера:</label>
      <input type="password" value="%mqtt_pass%" name="mqtt_pass">
      
      <label>IP / Host брокера:</label>
      <input type="text" value="%mqtt_host%" name="mqtt_host">
      
      <label>Порт:</label>
      <input type="number" value="%1234%" name="1234">

      <button type="submit" class="btn btn-save">💾 Сохранить и применить</button>
    </form>
  </div>

  <script>
    // GitHub репозиторий для проверки релизов
    const GITHUB_REPO = "geminibitok-oss/nalivator-esp32-c3-v3";

    async function checkGitHubReleases() {
      const btn = document.getElementById("checkBtn");
      btn.innerText = "⏳ Загрузка списка релизов...";
      btn.disabled = true;

      try {
        const response = await fetch(`https://api.github.com/repos/${GITHUB_REPO}/releases`);
        if (!response.ok) throw new Error("Не удалось получить релизы с GitHub");
        const releases = await response.json();

        if (releases.length === 0) {
          alert("Релизов на GitHub пока не найдено.");
          btn.innerText = "🔍 Проверить новые версии на GitHub";
          btn.disabled = false;
          return;
        }

        const select = document.getElementById("releaseSelect");
        select.innerHTML = "";
        window.ghReleases = releases;

        releases.forEach((rel, index) => {
          // Ищем bin-файл в активах релиза
          const binAsset = rel.assets && rel.assets.find(a => a.name.endsWith(".bin"));
          const downloadUrl = binAsset ? binAsset.browser_download_url : `https://raw.githubusercontent.com/${GITHUB_REPO}/${rel.tag_name}/firmware.bin`;
          
          const opt = document.createElement("option");
          opt.value = downloadUrl;
          opt.innerText = `${rel.name || rel.tag_name} (${new Date(rel.published_at).toLocaleDateString()})${index === 0 ? ' [ПОСЛЕДНИЙ]' : ''}`;
          opt.dataset.notes = rel.body || "Нет описания";
          select.appendChild(opt);
        });

        // Добавим ветку firmware по умолчанию
        const defaultOpt = document.createElement("option");
        defaultOpt.value = `https://raw.githubusercontent.com/${GITHUB_REPO}/firmware/firmware.bin`;
        defaultOpt.innerText = "Ветка firmware (latest build auto-stream)";
        defaultOpt.dataset.notes = "Автоматическая сборка из ветки firmware";
        select.appendChild(defaultOpt);

        document.getElementById("releasesArea").style.display = "block";
        onSelectRelease();
        btn.innerText = "✅ Список версий обновлен";
      } catch (err) {
        alert("Ошибка при запросе к GitHub: " + err.message + "\nУбедитесь, что устройство или ваш телефон подключены к интернету.");
        btn.innerText = "🔍 Проверить новые версии на GitHub";
      } finally {
        btn.disabled = false;
      }
    }

    function onSelectRelease() {
      const select = document.getElementById("releaseSelect");
      const chosenUrl = select.value;
      document.getElementById("ota_url").value = chosenUrl;
      const selectedOption = select.options[select.selectedIndex];
      if (selectedOption && selectedOption.dataset.notes) {
        document.getElementById("releaseNotes").innerText = "Описание: " + selectedOption.dataset.notes.substring(0, 150);
      }
    }
  </script>
</body>
</html>)rawliteral";

void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
}

String processor(const String& var) {
  if (var == "firmware_ver") {
    return String(FIRMWARE_VERSION);
  }
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
  if (var == "1234") {
    return MQTT_PORT;
  }
  return String();
}
