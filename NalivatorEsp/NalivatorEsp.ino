// ===========КОММЕРЧЕСКОЕ ИСПОЛЬЗОВАНИЕ В ЛЮБОМ ВИДЕ ЗАПРЕЩЕНО!!!===============

// ======== НАСТРОЙКИ ========
//#define STEPPER_MOTOR_USED       // Включение шагового двигателя вместо серво
#define NUM_SHOTS 5  // количество рюмок (оно же кол-во светодиодов и концевиков!), всё делалось под 6 рюмок!!!!
//#define TWO_PUMPS  // раскомментировать для варианта с двумя насосами-кранами разлива
#define FIRMWARE_BY_AIR      // Ардуино OTA, если  нужна прошивка по воздуху раскомметировать
#define DEBUG_UART 0         // отладка, 0 выключено, 1 включено
#define PRE_PAUSE 1500UL     // пауза перед движением разливного носика к рюмке к рюмке
#define POST_PAUSE 1000UL     // пауза после остановки разливного носика до включения помпы
#define NALITO_PAUSE 3000UL  // время показа меню налито, после розлива
#define MAX_DRINK 50         // максимальный объём тары мл, для корректного отображения в менюшках экрана, ставить не более 95 и кратный 5
#define MIN_DRINK 5         // минимальный объём тары мл,  кратный 5
#define MEMORY_ON // включение запись параметров в память, закомментировано - значит выключено!
#define SWITCH_LEVEL 0  // концевики. 1 - высокий сигнал при замыкании, 0 - низкий
#define PUMP_LEVEL 1    // уровень включения помпы, 1 - высокий уровень, 0 - низкий
//#define TANK   // звуковая тема для танкистов, для обычной озвучки закоментировать
#define DOP_SOUND // дополнительные звуки после выбора количества мл. , перед наливом, только в авторежиме. Закомментировать если не нужно
//#define STARTING_POS_ANGLE 0 // раскомментировать, если нужна начальная позиция разливного носика в заданной позиции в градусах от 0 до 180
#define BAT_MONITOR_ON // включение в коде всё что связано с АКБ, закомментировать если нет АКБ вообще.
#ifdef BAT_MONITOR_ON
   #define ADC_U_COEFF 5 // коэффициент делителя резисторов R10 и R9, для 12 вольт питания https://cxem.net/calc/divider_calc.php
   #define LOW_BAT_SLEEP_ON  // контроль низкого заряда акб. Закомментировать - выключить для отладки кода, если не подключен пин 34 к мониторингу АКБ, иначе сон
   #define BAT_U 8.4 // напряжение аккумулятор 4.2В - для 1-го элемента 18650, 8,4В - для двух последовательно соединенных элементов 18650
   #define CHARG_DETECT 0 // включение отображения процесса зарядки батареи (требуется подключение сигнала от контроллера заряда с индикатора зарядки на вход ESP32 CHARG_PIN) 0 - низкий уровень при зарядке для ТР5100, ТР4056; 1 - высокий уровень при зарядке для 2S/3S на IP2326
#endif
#ifdef CHARG_DETECT
   #define CHARG_PIN 6
#endif 
#define LED_TOWER  // светодиоды на башне. Раcкомментировать если есть
//#define SEVE_MODE_CONDITION  // включение пина состояния спящего режима 
#define NUM_FON_MUSIC  21 // количество треков фоновой музыки при розливе, папка 99 на флешке

#define ROUND_MATRIX // использование круглых матриц светодиодов в местах установки стопок
 #ifdef ROUND_MATRIX
   #define ROUND_MATRIX_LEDS 8 // количество диодов в круглой матрице
 #endif
//#define TTP229_USED // раскомментировать если используете 16-ти клавишную сенсорную клавиатуру, подключенную по двухпроводной линии. Пины для nano по умолчанию SCL_PIN 7 и SDO_PIN 8

#ifndef STEPPER_MOTOR_USED
//            ============ НАСТРОЙКИ ДЛЯ SERVO ==========
// изменить значения если серво не доезжает до нужных углов, не всегда эффективно!
   #define SERVO_MIN 544 // уменьшить если не доезжает до 0° (544 по умолчанию)
   #define SERVO_MAX 2600 // увеличить если не доезжает до 180° (2400 по умолчанию )
   //#define SERVO_CHANGE_DIRECTION // раскомментировать, отзеркалить движение серво
   #define SERVO_DETACH_PIN_ON // отключение генерации сигнала на пине серво в простое и между наливами наливатора
   #ifdef SERVO_CHANGE_DIRECTION
      #define INITAL_ANGLE_SERVO 180  // начальный угол на который становится серво, если включен режим зеркало, подбирать в меньшую сторону, если упирается серво.
   #endif
#else
//            ============ НАСТРОЙКИ ДЛЯ ШАГОВИКА ==========
#define NUMBER_OF_STEPS_PER_TURN 400  // количество шагов на оборот  шаговика, читать тут https://alexgyver.ru/gyverstepper/
#define SPEED_MULTIPLIER 10            // множитель скорости, если нельзя настроить приемлемую скорость для шаговика в настройках наливатора
#define STEPPER_RETURN_ANGLE -180      // угол для возврата в нулевую позицию краника разлива. Если нулевую позицию искать упором, то делать как можно меньше
//#define INVERT_MOTOR // инвертировать вращение шаговика
#define ENDSTOP_MOTOR_USED             // закомментировать, без концевика нулевого положения, нулевая позиция ищется упором во что либо
#define CHECKING_THE_ZERO_POSITION     // проверка нулевой позиции перед каждым розливом, если закомментировано, то нулевая позиция проверяется только при включении
#endif
//            ============ НАСТРОЙКИ ИЛЛЮМИНАЦИИ ==========
// цвет статусов налитых-пустых установленных рюмок, например здесь https://colorscheme.ru/color-names.html можно посмотреть и выставить понравившиеся
#define COLOR_POURET_GLASS 0, 255, 0   // цвет налитой рюмки R G B , здесь зелёный
#define COLOR_EMPTY_GLASS_1 255, 0, 0  // цвет пустой рюмки для первой помпы,  R G B , здесь красный
#define COLOR_EMPTY_SPACE 5, 0, 8      // цвет пустого места, без рюмок ,  R G B , здесь чёрный, т.е не светится
#ifdef TWO_PUMPS
   #define COLOR_EMPTY_GLASS_2 0, 0, 255  // цвет пустой рюмки для второй помпы,  R G B , здесь синий
#endif

// настройки для иллюминации наливающейся рюмки
#define INTENSITY_COLOR_CHANGE_GLASS 9362     // можно поменять , интенсивность смены цвета при розливе на рюмке
#define GLASS_RAINBOW_ILLUMINATION_TIMER 100  //  мс,  время переключения следующего цвета светодиода при наливе рюмки

// настройки для иллюминации стола после разлива
#define TABLE_RAINBOW_STEP 6425              // увеличивает шаг радуги стола после розлива, можно поиграться значением
#define NUMBER_ILLUMINATION_PASSES 50        // условное число проходов иллюминации стола после налива
#define TABLE_RAINBOW_ILLUMINATION_TIMER 25  //  мс, время последовательного переключения цвета светодиодов при радуге стола
#define TABLE_RAINBOW_READY_GLASS_TIMER 25 //  мс, время последовательного переключения цвета светодиодов наполненной рюмки для отключения смены цвета закоментировать

//            =========== MP3 Player ===========


//#define PLAYER_16_PIN  // раскомментировать для 16-пинового плеера
#define PLAYER_SERIAL_TIMEOUT 500  //таймаут(мс) ожидания данных с сериал порта плеера, если не верно читает количество треков или глюки, пробуем увеличивать сразу до 2000 мс
//#define SENDING_MULTIPLE_COMMANDS  // если раскомментировано то плееру посылается по три команды за раз, может избавить от глюков
//#define PLAYER_MH2024K_24SS  // тормозной плеер, PLAYER_SERIAL_TIMEOUT увеличиваем до 1500, вводятся задержки для нормальной работы плеера, в работе наливатора будут некоторые затупы

//          ============= ТИП ДИПЛЕЯ =================
#define DISPLAY_TYPE 0 // Выбор типа дисплея 0-LCD1602, 1-OLED1602

//           =========== ЛИБЫ ===========
#include <esp_task_wdt.h>

#include <Preferences.h>
Preferences nalivsave;
#define _LCD_TYPE 1
#include <LCD_1602_RUS_ALL.h>
#include <DFMiniMp3.h>

class Mp3Notify {
public:
  static void PrintlnSourceAction(DfMp3_PlaySources source, const char* action) {}
  static void OnError(uint16_t errorCode) {}
  static void OnPlayFinished(DfMp3_PlaySources source, uint16_t track) {}
  static void OnPlaySourceOnline(DfMp3_PlaySources source) {}
  static void OnPlaySourceInserted(DfMp3_PlaySources source) {}
  static void OnPlaySourceRemoved(DfMp3_PlaySources source) {}
};

#include "webpage.h" 
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <WiFiClientSecure.h>
#include "encMinim.h"
#ifndef STEPPER_MOTOR_USED
  #include <ESP32Servo.h>
  Servo servo;
#else
  #define GS_NO_ACCEL  // отключить модуль движения с ускорением (уменьшить вес кода)
  #include "GyverStepper2.h"
#endif

#include "timerMinim.h"
#include <Adafruit_NeoPixel.h>
#ifdef FIRMWARE_BY_AIR
  #include <ArduinoOTA.h>
#endif



HardwareSerial mp3Serial(0);

#ifndef PLAYER_16_PIN
   DFMiniMp3<HardwareSerial, Mp3Notify, Mp3ChipOriginal> myMP3(mp3Serial);
#else
   DFMiniMp3<HardwareSerial, Mp3Notify, Mp3ChipMH2024K16SS> myMP3(mp3Serial);
#endif

#ifndef TTP229_USED
   #define PCF8574_ADRES 0x20 // такой адрес получается, когда все перемычки запаяны
   //#define BUTTON_TOWER  // сенсорная кнопка на башне. Раскомментировать если есть
#else
   #define SCL_PIN 9  // пин scl модуля ttp229, если выбран
   #define SDO_PIN 8  // пин sdo модуля ttp229, если выбран
#endif
#ifndef STEPPER_MOTOR_USED
   #define SERVO_PIN 4  // пин серво.
#else
// пины для использования шагового двигателя
   #define STEP_PIN   // пин step драйвера шагового двигателя
   #define DIR_PIN    // пин dir драйвера шагового двигателя
   #define EN_PIN     // пин en драйвера шагового двигателя
   #ifdef ENDSTOP_MOTOR_USED
      #define ENDSTOP_PIN   // пин концевика нулевой позиции шагового двигателя
   #endif
#endif

#define PUMP1_POWER 10  // помпа 1

#ifdef TWO_PUMPS
   #define PUMP2_POWER   // помпа 2
#endif
#define LED_PIN 5  // пин ленты
#ifdef LED_TOWER
   #define LED_PIN2 6  // пин второй ленты
#endif

#ifdef BAT_MONITOR_ON
   #define BAT_PIN 3  // пин(6 канал) замера напряжения акб,  0-4095
   #include "driver/adc.h"
   #include "esp_adc_cal.h"
   static esp_adc_cal_characteristics_t adc1_chars;
#endif
#define BUSY_PIN 7  // пин готовности DF плеера
// ПИНЫ ЭНКОДЕРА
#define CLK 0  //
#define DT 1   //
#define SW 2   // кнопка энкодера
#ifdef BUTTON_TOWER
   #define BUT_TOWER_PIN 7  // пин кнопки на башне подключен к PCF8574
#endif
#ifdef SEVE_MODE_CONDITION
   #define SEVE_MODE_PIN ????  // пин режима энеогосбережения
#endif
#define Pin_AP 2


// настройка последовательности пунктов главного меню, тасуем на своё усмотрение, меняем цифры местами от 0 до 5

#define EQUALLY 0      // меню поровну
#define MULTI_SPILL 1  // меню мультиразлив
#define BARMAN 2       // меню бармена
#define MUSKETEERS 3   // меню мушкетёров
#define PLAYER 4       // меню плеера
#define SETTINGS 5     // меню настройки


//               =========== ДАТА ===========
#ifndef ROUND_MATRIX
Adafruit_NeoPixel strip(NUM_SHOTS, LED_PIN, NEO_GRB + NEO_KHZ800);
#else
Adafruit_NeoPixel strip(NUM_SHOTS * ROUND_MATRIX_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
#endif
#ifdef LED_TOWER
#define NUMLEDS 16                          // количество светиков во второй ленте
Adafruit_NeoPixel strip2(NUMLEDS, LED_PIN2, NEO_GRB + NEO_KHZ800);
timerMinim TOWERtimer(20);
bool mig = false;
bool rainbow = false;
bool clearLed = false;
#endif
#ifdef BUTTON_TOWER
bool stateBut = false;
#endif
encMinim enc(CLK, DT, SW, 0);   // пин clk, пин dt, пин sw, направление (0/1)
LCD_1602_RUS lcd(0x27, 16, 2);  //Адрес дисплея обычно 0x27 или 0x3F, подключение экрана esp32 21-SDA и 22-SCL
#ifdef STEPPER_MOTOR_USED
GStepper2< STEPPER2WIRE> stepper(NUMBER_OF_STEPS_PER_TURN, STEP_PIN, DIR_PIN, EN_PIN);
//GStepper2< STEPPER4WIRE> stepper(шаговНаОборот, pin1, pin2, pin3, pin4);             // драйвер 4 пин
//GStepper2< STEPPER4WIRE> stepper(шаговНаОборот, pin1, pin2, pin3, pin4, en);         // драйвер 4 пин + enable
#endif


#ifdef TABLE_RAINBOW_READY_GLASS_TIMER
   timerMinim LEDtimer1(TABLE_RAINBOW_READY_GLASS_TIMER);
#endif
timerMinim LEDtimer(TABLE_RAINBOW_ILLUMINATION_TIMER);
timerMinim FLOWdebounce(20);
timerMinim WAITtimer(PRE_PAUSE);
timerMinim TIMEProcent(2000);
timerMinim SAVEtimer(30000);  // таймер спящего режима
timerMinim PAUSEtimer(4000);  // таймер паузы
timerMinim PLAYtimer(2000);   // таймер переключения треков
#ifdef CHARG_DETECT
   timerMinim BATtimer(5000); //таймер обновления статуса батреи при включенной детекции зарядки
   timerMinim CHARGtimer(500); // таймер процесса заряда
#endif 

int16_t time50ml_1;                        // время налива 50 мл для первой помпы
uint8_t shotPos[] = { 0, 0, 0, 0, 0, 0 };  // позиция рюмок, в сетапе считается из памяти.
uint8_t bright = 0;                        // яркость led, считается из памяти
int16_t Procent = 0;
int8_t count = -1;
int8_t curPumping = -1;
uint8_t servoPos = 0;
uint8_t volume;
uint8_t volume2;
uint8_t subNastr = 1;
uint8_t subMush = 1;
uint8_t subMenu = 1;
uint8_t yesGlass = 0;
uint8_t randomGlass = 0;
uint8_t speedSer = 10;
uint8_t tracks = 0;
uint8_t tracks2 = 0;
uint8_t tracksA = 0; // количество треков тостов Алисы
uint8_t track_fonmusic = 0;
static int16_t valNaliv = 0;
uint8_t Menu = 0;
uint8_t MenuFlag = 0;       // Здесь храниться уровень меню.
uint8_t Drink = MIN_DRINK;  //
uint8_t DrinkCount = 0;     //счётчик налитых рюмок
uint8_t GlasCountEmply = 0; //счетчик пустых рюмок
uint8_t folder;
uint8_t folderA = 9; //Папка с тостами Алисы
uint8_t oldFolder;
uint8_t folder2 = 10;
uint8_t folderAlisa = 98; ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t num = 0;
uint8_t numA = 0; // Номер тоста алисы
uint8_t num2 = 0;
uint8_t num3 = 0;
uint8_t sleepTime = 0;
uint8_t ManDrink[] = { 0, 0, 0, 0, 0, 0 };
uint8_t TostList[100];   // массив номеров треков тостов, в нём перемешаем всё, максимум 100 тостов. нужно ли столько?
uint8_t TostListA[100];   // массив номеров треков тостов Алисы, в нём перемешаем всё, максимум 100 тостов.
uint8_t MusicList[100];  // массив номеров песенок, в нём перемешаем всё, максимум 100
uint8_t FonMusic[NUM_FON_MUSIC];  // массив фоновых мелодий при розливе, в нём перемешаем всё
uint8_t DrinkRnd[MAX_DRINK/5]; //массив объема налива кратного 5
uint8_t ManRum = 1;
uint8_t mixTracks = 0;
uint8_t mixMusic = 0;
uint8_t folTra = 1;  //
uint8_t oldNum = 1;  //
uint8_t barMan = 1;
uint8_t ledShowOn;
#ifndef ROUND_MATRIX
  bool LEDchanged = false;
#endif
bool pumping = false;
bool promivka = false;
bool ledShow = false;
bool moving = false;
bool tost = false;
bool returnMenu = false;
bool check = false;
bool save = false;
bool systemON = false;
bool noGlass = true;
bool readyDrink = false;  // стопки налиты
bool flag = false;        // флаг что нужно показать меню налива
bool player = false;      //
bool nextTrack = false;
bool playOn = false;  // флаг что плеер был включен
bool VoiceBlock = false; // флаг блокировки голосовых команд
bool pause99 = false;
bool noTost = false;  // флаг отмены тоста при нажатии на кнопку энкодера или кнопку на башне
bool playMush = false;
bool flagMush = false;
#ifdef CHARG_DETECT
   bool flagCh = false; // флаг отображения процесса зарядки
   uint8_t fB = 0;
#endif
bool flagSystemON = true;
uint8_t noDoliv;
uint8_t noTostBarmen;
int8_t rumka[NUM_SHOTS];  // массив рандомных позиций рюмок
bool readySystem = true;
bool alise = false;
int8_t drift = 0;
#ifdef TANK
   bool muveBack = true;
#endif
#ifdef DOP_SOUND
   bool dopSound = false;
#endif
#ifdef TWO_PUMPS
   uint8_t pourFrom[] = { 0, 0, 0, 0, 0, 0 };
   int16_t time50ml_2;  // время налива 50 мл для второй помпы
   uint8_t pumpNumber = 0;
#endif
uint8_t reedPins = 0;
bool needZero = true;


uint8_t status = 0;  // 0 - нет подключения, 1 - подключено к wifi, 2 - подключено к wifi и к mqtt
uint8_t old_status = 10;
uint8_t countWifi = 0;

enum { NO_GLASS,
       EMPTY,
       IN_PROCESS,
       READY } shotStates[NUM_SHOTS];
enum { SEARCH,
       MOVING,
       WAIT,
       PUMPING,
       TOOK } systemState;


// свои символы
#ifdef BAT_MONITOR_ON
   static uint8_t flagBat = 0;
   float v_akky;
   bool lowBat = false;                                                                       // флаг низкого заряда акб
   uint8_t battery0[8] = { B01110, B11111, B10001, B10001, B10001, B10001, B10001, B11111 };  // пустая батарея
   uint8_t battery1[8] = { B01110, B11111, B10001, B10001, B10001, B10001, B11111, B11111 };  //  батарея одно деление
   uint8_t battery2[8] = { B01110, B11111, B10001, B10001, B10001, B11111, B11111, B11111 };  //  батарея два деления
   uint8_t battery3[8] = { B01110, B11111, B10001, B10001, B11111, B11111, B11111, B11111 };  //  батарея три деления
   uint8_t battery4[8] = { B01110, B11111, B10001, B11111, B11111, B11111, B11111, B11111 };  //  батарея четыре деления
   uint8_t battery5[8] = { B01110, B11111, B11111, B11111, B11111, B11111, B11111, B11111 };  //  батарея полная
   uint8_t power[8] = { B01010, B01010, B11111, B11111, B11111, B01110, B00100, B00100 };     // питание от сети
#endif


// =========== МАКРО ===========
#if (PUMP_LEVEL == 0)
   #define pump1ON() digitalWrite(PUMP1_POWER, 0)
   #define pump1OFF() digitalWrite(PUMP1_POWER, 1)
   #ifdef TWO_PUMPS
      #define pump2ON() digitalWrite(PUMP2_POWER, 0)
      #define pump2OFF() digitalWrite(PUMP2_POWER, 1)
   #endif
#else
   #define pump1ON() digitalWrite(PUMP1_POWER, 1)
   #define pump1OFF() digitalWrite(PUMP1_POWER, 0)
   #ifdef TWO_PUMPS
      #define pump2ON() digitalWrite(PUMP2_POWER, 1)
      #define pump2OFF() digitalWrite(PUMP2_POWER, 0)
   #endif
#endif

#if (DEBUG_UART == 1)
   #define DEBUG(x) Serial.println(x)
#else
   #define DEBUG(x)
#endif

void connectToWifi() {
  if(WIFI_SSID == "" || WIFI_PASS == "" || !digitalRead(Pin_AP) || countWifi >= 12) {
   WiFi.mode(WIFI_AP);
   WiFi.softAP("NALIVATOR");
   xTimerStop(wifiReconnectTimer, 0);  // стоп таймера переподключения к wifi
   Serial.println(" Wi-Fi soft AP Nalivator");
  } else {
  WiFi.mode(WIFI_STA);
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID.c_str(), WIFI_PASS.c_str());
  countWifi++;
 }
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event) {
  switch (event) {

    case ARDUINO_EVENT_WIFI_READY:
      Serial.println("WiFi ready");
      break;

    case ARDUINO_EVENT_WIFI_STA_START:
      Serial.println("WiFi STA starting");
      break;

    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
      Serial.println("WiFi STA connected");
      break;    

    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      xTimerStop(wifiReconnectTimer, 0);  // стоп таймера переподключения к wifi
      countWifi = 0;
      connectToMqtt();
      status = 1;
      if(MenuFlag == 0){
        lcd.setCursor(0, 0);
        lcd.print("W");
      }
      break;    

    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      xTimerStop(mqttReconnectTimer, 0);  // остнаовка таймера подключения к mqtt, если пропало соединение с wifi
      if( xTimerIsTimerActive( wifiReconnectTimer ) == false ) xTimerStart(wifiReconnectTimer, 0);  // старт таймера переподключения к wifi
      status = 0;
      if(old_status != status && MenuFlag == 0){
        old_status = status;
         lcd.setCursor(0, 0);
         lcd.print(" ");
      }
      break;
    default:
      break;
  }
}

void onMqttConnect(bool sessionPresent) {
  Serial.print("Connected to MQTT broker: ");
  xTimerStop(mqttReconnectTimer, 0);  // остнаовка таймера подключения к mqtt, если подключились
  Serial.print(MQTT_HOST);
  Serial.print(", port: ");
  Serial.println(MQTT_PORT);
  mqttClient.subscribe(Nalivator, 1);
  status = 2;
  countWifi = 0;
  if(MenuFlag == 0){
    lcd.setCursor(0, 0);
    lcd.print("H");
  }
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  (void)reason;

  Serial.println("Disconnected from MQTT.");
  status = 1;
    if(old_status != status && MenuFlag == 0){
    old_status = status;
    lcd.setCursor(0, 0);
    lcd.print("W");
  }
  if (WiFi.isConnected() && MQTT_HOST != "" && countWifi <= 3) {
    if( xTimerIsTimerActive( mqttReconnectTimer ) == false ) xTimerStart(mqttReconnectTimer, 0);
    countWifi++;
  }
}


