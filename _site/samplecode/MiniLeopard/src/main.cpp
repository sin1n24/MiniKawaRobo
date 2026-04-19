/* © 2024 sin1 Released under the MIT license https://opensource.org/licenses/mit-license.php

v08:ESP-NOW検証FW互換から移植、10回連続送信とDeepSleep削除、A短押しでBAT問合せ
v09:BTN_FLAG実装、MACアドレス保存実装
v10:MACアドレス登録用ENQとACK実装、A長押しで登録→再起動
v11:デフォルトrecv_mode = false、2度押しで切替→再起動、trueでA短押しでサーボ動作確認
v12:blynk,flex,matrix-led追加、他モードにもdeadband拡張、AtomS3でBLE非コンパイル、
v13:Avator追加、matrix逆向き追加
v14:matrix表示中スキップ対応、board非対応モードスキップ、AtomLite対応、AtomS3でDot表示
v15:AtomS3R追加(Sv専用)、カメラは未達
v16:Julesで軽めのリファクタリング。
v17:コントローラーにL/R追加。ボタン操作を表情のみに変更。MODE_BLYNC削除。
v18:ニュートラル変更、UGOKUでデッドバンド削除＆Avotor追加
TODO:LPD32PCBとの統合
*/
char version[8] = "v18";

#include <M5unified.h>
#include <esp_now.h>
#include <WiFi.h>
#include "FS.h"
#include "SPIFFS.h"
#include "fcled.h"

// Forward declarations for setup and loop helpers
void setupBlynk();
void setupFlex();
void setupEspNow();
void loopBlynk();
void loopFlex();
void loopEspNowRobot();
void loopEspNowController();

#include <Adafruit_NeoPixel.h>

// --- Constants ---

// -- Hardware --
constexpr int NEOPIXEL_PIN = 27;
constexpr int NEOPIXEL_COUNT = 25;
constexpr float ADC_MAX_VALUE = 4095.0f;

// -- Pinout --
// M5AtomS3 Controller Pins
constexpr int ATOM_S3_STICK_H_PIN = 8;
constexpr int ATOM_S3_STICK_V_PIN = 7;
constexpr int ATOM_S3_STICK_SW_PIN = 6;
constexpr int ATOM_S3_TRG_SW_PIN = 5;
constexpr int ATOM_S3_OK_SW_PIN = 38;
constexpr int ATOM_S3_NG_SW_PIN = 39;

// M5Atom Controller Pins
constexpr int ATOM_STICK_H_PIN = 25;
constexpr int ATOM_STICK_V_PIN = 33;
constexpr int ATOM_STICK_SW_PIN = 19;
constexpr int ATOM_TRG_SW_PIN = 22;
constexpr int ATOM_OK_SW_PIN = 21;
constexpr int ATOM_NG_SW_PIN = 25; // Note: Same as STICK_H

// LPD32PCB Servo Pins
constexpr int LPD32_SERVO_PIN1 = 19;
constexpr int LPD32_SERVO_PIN2 = 17;
constexpr int LPD32_SERVO_PIN3 = 18;
constexpr int LPD32_SERVO_PIN4 = 13;

// M5Capsule Servo Pins
constexpr int CAPSULE_SERVO_PIN1 = 9;
constexpr int CAPSULE_SERVO_PIN2 = 7;
constexpr int CAPSULE_SERVO_PIN3 = 5;
constexpr int CAPSULE_SERVO_PIN4 = 3;
constexpr int CAPSULE_SERVO_PIN5 = 1;

// M5Atom Servo Pins
constexpr int ATOM_SERVO_PIN1 = 22;
constexpr int ATOM_SERVO_PIN2 = 19;
constexpr int ATOM_SERVO_PIN3 = 23;
constexpr int ATOM_SERVO_PIN4 = 33;

// M5AtomS3 Family Servo Pins
constexpr int ATOMS3_SERVO_PIN1 = 5;
constexpr int ATOMS3_SERVO_PIN2 = 6;
constexpr int ATOMS3_SERVO_PIN3 = 7;
constexpr int ATOMS3_SERVO_PIN4 = 8;

// M5StickC Servo Pins
constexpr int STICKC_SERVO_PIN1 = 0;
constexpr int STICKC_SERVO_PIN2 = 26;

// M5StickCPlus Servo Pins
constexpr int STICKCPLUS_SERVO_PIN1 = 0;
constexpr int STICKCPLUS_SERVO_PIN2 = 25;
constexpr int STICKCPLUS_SERVO_PIN3 = 26;

// -- Logic & Control --
constexpr float JOYSTICK_DEADBAND = 0.1f;
constexpr char JOYSTICK_LOG_SIZE = 10;
constexpr int MAX_SPEED = 60;
constexpr int MAX_ANGLE = 80;
constexpr int ARM_TRIM_SCALE = 120;
constexpr long BLYNK_TIMER_INTERVAL_MS = 1000L;
constexpr int RECENT_RECV_TIMEOUT_MS = 200;
constexpr int BUTTON_PRESS_DURATION_MS = 200;
constexpr int FLEX_CONTROLLER_CENTER_VAL = 128;
constexpr int BLYNK_DEADBAND = 5;
constexpr int RESTART_DELAY_MS = 100;
constexpr int SERVO_OFFSET = 90;

// Servo angles
constexpr int SERVO_ARM_ANGLE_TRIGGER = -70;
constexpr int SERVO_ARM_TRIM = 30;

// -- Display & UI --
constexpr int DISPLAY_CURSOR_Y_START = 24;
constexpr int FONT_ASCII_START = 27;
constexpr int FONT_ASCII_END = 127;
constexpr int FONT_BYTES_PER_CHAR = 5;
constexpr int MATRIX_SHIFT = 15;
constexpr int MATRIX_STEP = 25;
constexpr int MATRIX_RADIUS = 8;
constexpr uint32_t COLOR_GREEN = 0x008000;
constexpr uint32_t COLOR_MAGENTA = 0x800080;
constexpr uint32_t COLOR_BLUE = 0x0000FF;
constexpr uint32_t COLOR_WHITE = 0xFFFFFF;
constexpr int MATRIX_CHAR_DELAY_MS_NORMAL = 200;
constexpr int MATRIX_CHAR_DELAY_MS_FAST = 120;

bool lefty;

// -- Protocol --
uint8_t ESP_NOW_BROADCAST_ADDR[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

const uint8_t FONTDATA[500] = {0x0A, 0x0A, 0x00, 0x11, 0x0E,   0x00, 0x0A, 0x00, 0x11, 0x0E,   0x0A, 0x0A, 0x00, 0x11, 0x0E,   0x00, 0x0A, 0x00, 0x11, 0x0E,   0x0, 0x0, 0x0, 0x0, 0x5,  
  0x0, 0x0, 0x0, 0x0, 0x0, 0x8, 0x8, 0x8, 0x0, 0x8, 0xa, 0x4a, 0x40, 0x0, 0x0, 0xa, 0x5f, 0xea, 0x5f, 0xea, 0xe, 0xd9, 0x2e, 0xd3, 0x6e, 0x19, 0x32, 0x44, 0x89, 0x33, 0xc, 0x92, 0x4c, 0x92, 0x4d, 0x8, 0x8, 0x0, 0x0, 0x0, 0x4, 0x88, 0x8, 0x8, 0x4, 0x8, 0x4, 0x84, 0x84, 0x88, 0x0, 0xa, 0x44, 0x8a, 0x40, 0x0, 0x4, 0x8e, 0xc4, 0x80, 0x0, 0x0, 0x0, 0x4, 0x88, 0x0, 0x0, 0xe, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x8, 0x0, 0x1, 0x22, 0x44, 0x88, 0x10, 0xc, 0x92, 0x52, 0x52, 0x4c, 0x4, 0x8c, 0x84, 0x84, 0x8e, 0x1c, 0x82, 0x4c, 0x90, 0x1e, 0x1e, 0xc2, 0x44, 0x92, 0x4c, 0x6, 0xca, 0x52, 0x5f, 0xe2, 0x1f, 0xf0, 0x1e, 0xc1, 0x3e, 0x2, 0x44, 0x8e, 0xd1, 0x2e, 0x1f, 0xe2, 0x44, 0x88, 0x10, 0xe, 0xd1, 0x2e, 0xd1, 0x2e, 0xe, 0xd1, 0x2e, 0xc4, 0x88, 0x0, 0x8, 0x0, 0x8, 0x0, 0x0, 0x4, 0x80, 0x4, 0x88, 0x2, 0x44, 0x88, 0x4, 0x82, 0x0, 0xe, 0xc0, 0xe, 0xc0, 0x8, 0x4, 0x82, 0x44, 0x88, 0xe, 0xd1, 0x26, 0xc0, 0x4, 0xe, 0xd1, 0x35, 0xb3, 0x6c, 0xc, 0x92, 0x5e, 0xd2, 0x52, 0x1c, 0x92, 0x5c, 0x92, 0x5c, 0xe, 0xd0, 0x10, 0x10, 0xe, 0x1c, 0x92, 0x52, 0x52, 0x5c, 0x1e, 0xd0, 0x1c, 0x90, 0x1e, 0x1e, 0xd0, 0x1c, 0x90, 0x10, 0xe, 0xd0, 0x13, 0x71, 0x2e, 0x12, 0x52, 0x5e, 0xd2, 0x52, 0x1c, 0x88, 0x8, 0x8, 0x1c, 0x1f, 0xe2, 0x42, 0x52, 0x4c, 0x12, 0x54, 0x98, 0x14, 0x92, 0x10, 0x10, 0x10, 0x10, 0x1e, 0x11, 0x3b, 0x75, 0xb1, 0x31, 0x11, 0x39, 0x35, 0xb3, 0x71, 0xc, 0x92, 0x52, 0x52, 0x4c, 0x1c, 0x92, 0x5c, 0x90, 0x10, 0xc, 0x92, 0x52, 0x4c, 0x86, 0x1c, 0x92, 0x5c, 0x92, 0x51, 0xe, 0xd0, 0xc, 0x82, 0x5c, 0x1f, 0xe4, 0x84, 0x84, 0x84, 0x12, 0x52, 0x52, 0x52, 0x4c, 0x11, 0x31, 0x31, 0x2a, 0x44, 0x11, 0x31, 0x35, 0xbb, 0x71, 0x12, 0x52, 0x4c, 0x92, 0x52, 0x11, 0x2a, 0x44, 0x84, 0x84, 0x1e, 0xc4, 0x88, 0x10, 0x1e, 0xe, 0xc8, 0x8, 0x8, 0xe, 0x10, 0x8, 0x4, 0x82, 0x41, 0xe, 0xc2, 0x42, 0x42, 0x4e, 0x4, 0x8a, 0x40, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1f, 0x8, 0x4, 0x80, 0x0, 0x0, 0x0, 0xe, 0xd2, 0x52, 0x4f, 0x10, 0x10, 0x1c, 0x92, 0x5c, 0x0, 0xe, 0xd0, 0x10, 0xe, 0x2, 0x42, 0x4e, 0xd2, 0x4e, 0xc, 0x92, 0x5c, 0x90, 0xe, 0x6, 0xc8, 0x1c, 0x88, 0x8, 0xe, 0xd2, 0x4e, 0xc2, 0x4c, 0x10, 0x10, 0x1c, 0x92, 0x52, 0x8, 0x0, 0x8, 0x8, 0x8, 0x2, 0x40, 0x2, 0x42, 0x4c, 0x10, 0x14, 0x98, 0x14, 0x92, 0x8, 0x8, 0x8, 0x8, 0x6, 0x0, 0x1b, 0x75, 0xb1, 0x31, 0x0, 0x1c, 0x92, 0x52, 0x52, 0x0, 0xc, 0x92, 0x52, 0x4c, 0x0, 0x1c, 0x92, 0x5c, 0x90, 0x0, 0xe, 0xd2, 0x4e, 0xc2, 0x0, 0xe, 0xd0, 0x10, 0x10, 0x0, 0x6, 0xc8, 0x4, 0x98, 0x8, 0x8, 0xe, 0xc8, 0x7, 0x0, 0x12, 0x52, 0x52, 0x4f, 0x0, 0x11, 0x31, 0x2a, 0x44, 0x0, 0x11, 0x31, 0x35, 0xbb, 0x0, 0x12, 0x4c, 0x8c, 0x92, 0x0, 0x11, 0x2a, 0x44, 0x98, 0x0, 0x1e, 0xc4, 0x88, 0x1e, 0x6, 0xc4, 0x8c, 0x84, 0x86, 0x8, 0x8, 0x8, 0x8, 0x8, 0x18, 0x8, 0xc, 0x88, 0x18, 0x0, 0x0, 0xc, 0x83, 0x60};

bool matrix_inv = false;
bool gen2 = false;
bool fwd_inv = true;

void setRecvMode();


#include <Avatar.h>
m5avatar::Avatar avatar;

void setupAvatar()
{
  M5.Display.setRotation(2);
  avatar.setScale(0.4);
  avatar.setPosition(-56, -96);
  avatar.init();
}
//enum class Expression { Happy, Angry, Sad, Doubt, Sleepy, Neutral };
void setExpAvatar(int ex)
{
  if( ex ) avatar.setExpression((m5avatar::Expression)ex);
  else avatar.setExpression(m5avatar::Expression::Neutral);
}


void draw_matrix(const char str, uint32_t color, uint32_t bColor)
{
  if(str < FONT_ASCII_START || str > FONT_ASCII_END){
    Serial.println("Char range Error...");
    return;
  }
  uint8_t R  = color >> 16;
  uint8_t G  = (color >> 8) & 0xff;
  uint8_t B  = color & 0xff;
  uint8_t Rb  = bColor >> 16;
  uint8_t Gb  = (bColor >> 8) & 0xff;
  uint8_t Bb  = bColor & 0xff;    
  int start = ((int)str - FONT_ASCII_START) * FONT_BYTES_PER_CHAR;
  
  for (int row = 0 ; row < 5 ; row++){
    for (int i = 0; i < 5; i++){ 
      if(( FONTDATA[ start + row ] & (1 << abs(i-4))) != 0)
      {
        if(M5.getBoard() == m5::board_t::board_M5AtomS3) M5.Display.fillCircle(MATRIX_SHIFT+(matrix_inv?4-i:i)*MATRIX_STEP, MATRIX_SHIFT+(matrix_inv?4-row:row)*MATRIX_STEP, MATRIX_RADIUS, color);
        else strip.setPixelColor((matrix_inv?4-row:row) * 5 + (matrix_inv?4-i:i), strip.Color(R, G, B));
      }
      else
      {
        if(M5.getBoard() == m5::board_t::board_M5AtomS3) M5.Display.fillCircle(MATRIX_SHIFT+(matrix_inv?4-i:i)*MATRIX_STEP, MATRIX_SHIFT+(matrix_inv?4-row:row)*MATRIX_STEP, MATRIX_RADIUS, bColor); 
        else strip.setPixelColor((matrix_inv?4-row:row) * 5 + (matrix_inv?4-i:i), strip.Color(Rb, Gb, Bb));
      }
    }
  }
    if( M5.getBoard() == m5::board_t::board_M5Atom )  strip.show();
  //参考：https://pointofviewpoint.linclip.com/archives/3173
}

void draw_matrix_str(String str, uint delay_ms, uint32_t color, uint32_t bColor )
{
  for(int i=0;i<str.length();i++)
  {
    draw_matrix(str.charAt(i), color, bColor );
    if( i != str.length()-1 ) delay(delay_ms);
    M5.update();
    if(M5.BtnA.wasClicked()) setRecvMode();
  }
}

#include "UGOKU_Pad_Controller.hpp" 
UGOKU_Pad_Controller controller;

uint8_t lastPrintedCh  = 255;
uint8_t lastPrintedVal = 255;
bool isConnected = false; 

#define BLYNK_TEMPLATE_ID "TMPL6sDM_UUFi"
#define BLYNK_TEMPLATE_NAME "MiniKawa"
#define BLYNK_AUTH_TOKEN "hGE2Y1WV-wixA0lFD9nz-PJSbpyN58lG"

#define BLYNK_PRINT Serial
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
char auth[] = BLYNK_AUTH_TOKEN;

ulong old_rev_time;
File  macFile, iniFile;
const char *macFileName = "/mac.txt", *iniFileName = "/param.ini";

enum{MODE_LCONTROL, MODE_RCONTROL, MODE_ESPNOW, MODE_FLEX, MODE_LIM, MODE_BLYNC};  //
byte comm_mode;
esp_now_peer_info_t slave;

byte STICK_H, STICK_V, STICK_SW, TRG_SW, OK_SW, NG_SW;

float deadband = JOYSTICK_DEADBAND;
const char cnt_max = JOYSTICK_LOG_SIZE;
int max_speed = MAX_SPEED;
int max_angle = MAX_ANGLE;

float joy_h, joy_v, joy_w, joy_t;
float joy_h_sum, joy_v_sum, joy_w_sum, joy_t_sum;
char cnt=0;
float joy_h_log[cnt_max], joy_v_log[cnt_max], joy_t_log[cnt_max], joy_w_log[cnt_max];

int left, right, mp3, light, arm, trim, bat = 0;

float deadbanded( float dbd, float band )
{
  if( dbd < band && dbd > -band ) dbd = 0;
  else if( dbd > 0 ) dbd -= band;
  else dbd += band;

  return dbd;
}

// Reads joystick values, applies a moving average filter, and calculates motor/servo outputs.
void getStick()
{
  // Normalize joystick values from 0-4095 to -1.0 to 1.0
  joy_h_log[cnt] = 1.0f - 2.0f*analogRead(STICK_H)/ADC_MAX_VALUE, joy_v_log[cnt] =  1.0f - 2.0f*analogRead(STICK_V)/ADC_MAX_VALUE;
  if(gen2) {
    joy_t_log[cnt] = 1.0f - 2.0f*analogRead(TRG_SW)/ADC_MAX_VALUE;
    joy_w_log[cnt] = 1.0f - 2.0f*analogRead(STICK_SW)/ADC_MAX_VALUE;
  }

  // Use a circular buffer for the moving average
  if( ++cnt >= cnt_max ) cnt = 0;

  // Sum the values in the log for averaging
  for( int i=0; i<=cnt_max; i++ ){ joy_h_sum += joy_h_log[i]; joy_v_sum += joy_v_log[i]; joy_w_sum += joy_w_log[i]; joy_t_sum += joy_t_log[i]; }

  // Calculate the average and apply a deadband to prevent drift
  joy_h = (lefty?-1:1) * deadbanded( joy_h_sum / cnt_max, deadband );
  joy_v = (lefty?-1:1) * deadbanded( joy_v_sum / cnt_max, deadband );
  joy_w = (lefty?-1:1) * deadbanded( joy_w_sum / cnt_max, deadband/10 );
  joy_t = (lefty?-1:1) * deadbanded( joy_t_sum / cnt_max, deadband/10 );
  joy_h_sum = joy_v_sum = joy_w_sum = joy_t_sum = 0;
  
  // Calculate motor speeds for differential drive (tank-like) steering
  left = - max_speed * (joy_v - joy_h);
  right = max_speed * (joy_v + joy_h);
  // Scale other joystick values for arm and trim control
  if(gen2) {
    arm = ARM_TRIM_SCALE * joy_t;
    trim = ARM_TRIM_SCALE * joy_w;
  }
}

void dispStick()
{
  M5.Display.startWrite();
  M5.Display.setCursor(0, DISPLAY_CURSOR_Y_START);

  if( !M5.BtnA.isPressed() ) M5.Display.println("LCD_SW :  "); else M5.Display.println("LCD_SW : X");
  if( digitalRead(OK_SW) ) M5.Display.println(" OK_SW :  "); else M5.Display.println(" OK_SW : X");
  if( digitalRead(NG_SW) ) M5.Display.println(" NG_SW :  "); else M5.Display.println(" NG_SW : X");
  if( gen2 )
  {
    M5.Display.printf("\nJOY_W %4d   \nJOY_T %4d   \n", (int)(joy_w*1000), (int)(joy_t*1000) );
  }else{
    if( digitalRead(STICK_SW) ) M5.Display.println("JOY_SW :  "); else M5.Display.println("JOY_SW : X");
    if( digitalRead(TRG_SW) ) M5.Display.println("TRG_SW :  "); else M5.Display.println("TRG_SW : X");
  }
  M5.Display.printf("\nJOY_H %3d%%\nJOY_V %3d%%   Bat:%d", left, right, bat );

  M5.Display.endWrite();
}

void initController()
{
  if( M5.getBoard() == m5::board_t::board_M5AtomS3 )
  {
    STICK_H = ATOM_S3_STICK_H_PIN, STICK_V = ATOM_S3_STICK_V_PIN, STICK_SW = ATOM_S3_STICK_SW_PIN, TRG_SW = ATOM_S3_TRG_SW_PIN, OK_SW = ATOM_S3_OK_SW_PIN, NG_SW = ATOM_S3_NG_SW_PIN;
    pinMode(STICK_V, ANALOG);
    pinMode(STICK_H, ANALOG);
    pinMode(OK_SW, INPUT_PULLUP);
    pinMode(NG_SW, INPUT_PULLUP);
    pinMode(STICK_SW, gen2?ANALOG:INPUT_PULLUP);
    pinMode(TRG_SW, gen2?ANALOG:INPUT_PULLUP);
  }
  else if( M5.getBoard() == m5::board_t::board_M5Atom )
  {
    STICK_H = ATOM_STICK_H_PIN, STICK_V = ATOM_STICK_V_PIN, STICK_SW = ATOM_STICK_SW_PIN, TRG_SW = ATOM_TRG_SW_PIN, OK_SW = ATOM_OK_SW_PIN, NG_SW = ATOM_NG_SW_PIN;
    pinMode(STICK_V, ANALOG);
    pinMode(STICK_H, ANALOG);
    pinMode(OK_SW, INPUT_PULLUP);
    //pinMode(NG_SW, INPUT_PULLUP);
    pinMode(STICK_SW, gen2?ANALOG:INPUT_PULLUP);
    pinMode(TRG_SW, gen2?ANALOG:INPUT_PULLUP);
  }
}


enum { FLAG_BTN_A, FLAG_SW_OK, FLAG_SW_NG, FLAG_STICK, FLAG_TRGR, FLAG_SW_MAX };
enum { DATA_LEFT, DATA_RIGHT, DATA_BTN };
// const unsigned int BTN_A_FLAG = (1 << 0); // 0000 0000 0000 0001
// const unsigned int SW_OK_FLAG = (1 << 1); // 0000 0000 0000 0010
// const unsigned int SW_NG_FLAG = (1 << 2); // 0000 0000 0000 0100
// const unsigned int STICK_FLAG = (1 << 3); // 0000 0000 0000 1000
// const unsigned int TRGR_FLAG  = (1 << 4); // 0000 0000 0001 0000

void sendData()
{
  uint8_t data[5] = {(uint8_t)(left+SERVO_OFFSET), (uint8_t)(right+SERVO_OFFSET), 0, (uint8_t)(arm+SERVO_OFFSET), (uint8_t)(trim+SERVO_OFFSET)};
  data[DATA_BTN] = M5.BtnA.isPressed() + (!digitalRead(OK_SW) << FLAG_SW_OK) + (!digitalRead(NG_SW) << FLAG_SW_NG) + (!digitalRead(STICK_SW) << FLAG_STICK) + (!digitalRead(TRG_SW) << FLAG_TRGR);
  Serial.printf("data[%d]  [%d][%d][%d]\n", data[DATA_BTN], data[DATA_BTN]&FLAG_BTN_A, data[DATA_BTN]&(1<<FLAG_SW_OK) , data[DATA_BTN]&(1<<FLAG_SW_NG) );
  esp_now_send(slave.peer_addr, data, sizeof(data));
}

void sendBat()
{
  uint8_t data[3] = {'B', 'A', 'T'};
  esp_now_send(slave.peer_addr, data, sizeof(data));
}
void sendACK()
{
  uint8_t data[3] = {'A', 'C', 'K'};
  esp_now_send(slave.peer_addr, data, sizeof(data));
}

void sendENQ()
{
  uint8_t data[3] = {'E', 'N', 'Q'};
  esp_now_send(slave.peer_addr, data, sizeof(data));
}

void setRecvMode()
{
  iniFile = SPIFFS.open(iniFileName, FILE_WRITE);
  if( ++comm_mode >= MODE_LIM ) comm_mode = 0;
  iniFile.write(comm_mode);
  iniFile.close();
  SPIFFS.remove(macFileName);
  
  if( M5.getBoard() == m5::board_t::board_M5Atom || M5.getBoard() == m5::board_t::board_M5AtomS3 )
  for (int i=0; i < 5; i++)
  {
    for (int row = 0 ; row < 5 ; row++)   
    if( M5.getBoard() == m5::board_t::board_M5Atom ) strip.setPixelColor((row) * 5 + i, 0);
    else M5.Display.fillCircle(MATRIX_SHIFT+(matrix_inv?4-i:i)*MATRIX_STEP, MATRIX_SHIFT+(matrix_inv?4-row:row)*MATRIX_STEP, MATRIX_RADIUS, 0);
    
    if( M5.getBoard() == m5::board_t::board_M5Atom )   strip.show();
    delay(RESTART_DELAY_MS);
  }  
  esp_restart();
}

int servo_left, servo_right, servo_arm, trim_arm;
bool old_sw_ok, mode_arm = false;


enum { LEDC_CH_1=0, LEDC_CH_2, LEDC_CH_3, LEDC_CH_4, LEDC_CH_5};

byte SERVO_PIN1, SERVO_PIN2, SERVO_PIN3, SERVO_PIN4, SERVO_PIN5;
bool servo1_enable, servo2_enable, servo3_enable, servo4_enable, servo5_enable;

#define LEDC_TIMER_BIT 10   //LEDCのPWMタイマーの精度設定
#define LEDC_SERVO_FREQ 50   //サーボ信号の１サイクル　50Hz:20ms futaba71Hz:14ms

#define SERVO_MIN_WIDTH_MS 26//41//26
#define SERVO_MAX_WIDTH_MS 125//113//123

void onDeviceConnect() {
  Serial.println("Device connected!");
  isConnected = true;
}

void onDeviceDisconnect() {
  Serial.println("Device disconnected!");
  isConnected = false;
  
  servo_left = servo_right = servo_arm = 0;
}

void initServo()
{
  if( M5.getBoard() == m5::board_t::board_unknown ) //LPD32PCB = board_M5Atom? chech PowerIC?
  {
    servo1_enable = true, servo2_enable = true, servo3_enable = true, servo4_enable = true, servo5_enable = false;
    SERVO_PIN1 = LPD32_SERVO_PIN1, SERVO_PIN2 = LPD32_SERVO_PIN2, SERVO_PIN3 = LPD32_SERVO_PIN3, SERVO_PIN4 = LPD32_SERVO_PIN4;
  }
  else if( M5.getBoard() == m5::board_t::board_M5Capsule ) 
  {
    servo1_enable = true, servo2_enable = true, servo3_enable = true, servo4_enable = true, servo5_enable = true;
    SERVO_PIN1 = CAPSULE_SERVO_PIN1, SERVO_PIN2 = CAPSULE_SERVO_PIN2, SERVO_PIN3 = CAPSULE_SERVO_PIN3, SERVO_PIN4 = CAPSULE_SERVO_PIN4, SERVO_PIN5 = CAPSULE_SERVO_PIN5;
  }
  else if( M5.getBoard() == m5::board_t::board_M5Atom ) 
  {
    servo1_enable = true, servo2_enable = true, servo3_enable = true, servo4_enable = true, servo5_enable = false;
    SERVO_PIN1 = ATOM_SERVO_PIN1, SERVO_PIN2 = ATOM_SERVO_PIN2, SERVO_PIN3 = ATOM_SERVO_PIN3, SERVO_PIN4 = ATOM_SERVO_PIN4;
  }
  else if( M5.getBoard() == m5::board_t::board_M5AtomS3 || M5.getBoard() == m5::board_t::board_M5AtomS3Lite || M5.getBoard() == m5::board_t::board_M5AtomS3R ) 
  {
    servo1_enable = true, servo2_enable = true, servo3_enable = true, servo4_enable = true, servo5_enable = false;
    SERVO_PIN1 = ATOMS3_SERVO_PIN1, SERVO_PIN2 = ATOMS3_SERVO_PIN2, SERVO_PIN3 = ATOMS3_SERVO_PIN3, SERVO_PIN4 = ATOMS3_SERVO_PIN4;
  }
  else if( M5.getBoard() == m5::board_t::board_M5StickC  ) 
  {
    servo1_enable = false/*true*/, servo2_enable = false/*true*/, servo3_enable = false, servo4_enable = false, servo5_enable = false;
    SERVO_PIN1 = STICKC_SERVO_PIN1, SERVO_PIN2 = STICKC_SERVO_PIN2; //36 is input only
  }
  else if( M5.getBoard() == m5::board_t::board_M5StickCPlus || M5.getBoard() == m5::board_t::board_M5StickCPlus2 ) 
  {
    servo1_enable = true, servo2_enable = true, servo3_enable = true, servo4_enable = false, servo5_enable = false;
    SERVO_PIN1 = STICKCPLUS_SERVO_PIN1, SERVO_PIN2 = STICKCPLUS_SERVO_PIN2, SERVO_PIN3 = STICKCPLUS_SERVO_PIN3;
  }
  
  if( servo1_enable ){ pinMode(SERVO_PIN1, OUTPUT); ledcSetup(LEDC_CH_1, LEDC_SERVO_FREQ, LEDC_TIMER_BIT); ledcAttachPin(SERVO_PIN1, LEDC_CH_1); }
  if( servo2_enable ){ pinMode(SERVO_PIN2, OUTPUT); ledcSetup(LEDC_CH_2, LEDC_SERVO_FREQ, LEDC_TIMER_BIT); ledcAttachPin(SERVO_PIN2, LEDC_CH_2); }  
  if( servo3_enable ){ pinMode(SERVO_PIN3, OUTPUT); ledcSetup(LEDC_CH_3, LEDC_SERVO_FREQ, LEDC_TIMER_BIT); ledcAttachPin(SERVO_PIN3, LEDC_CH_3); }
  if( servo4_enable ){ pinMode(SERVO_PIN4, OUTPUT); ledcSetup(LEDC_CH_4, LEDC_SERVO_FREQ, LEDC_TIMER_BIT); ledcAttachPin(SERVO_PIN4, LEDC_CH_4); } 
  if( servo5_enable ){ pinMode(SERVO_PIN5, OUTPUT); ledcSetup(LEDC_CH_5, LEDC_SERVO_FREQ, LEDC_TIMER_BIT); ledcAttachPin(SERVO_PIN5, LEDC_CH_5); } 
}
void setServo(int degree1=0, int degree2=0, int degree3=0, int degree4=0, int degree5=0)
{
  if( servo1_enable ) ledcWrite( LEDC_CH_1, map(degree1, -90, 90, SERVO_MIN_WIDTH_MS, SERVO_MAX_WIDTH_MS ) );  
  if( servo2_enable ) ledcWrite( LEDC_CH_2, map(degree2, -90, 90, SERVO_MIN_WIDTH_MS, SERVO_MAX_WIDTH_MS ) );  
  if( servo3_enable ) ledcWrite( LEDC_CH_3, map(degree3, -90, 90, SERVO_MIN_WIDTH_MS, SERVO_MAX_WIDTH_MS ) );  
  if( servo4_enable ) ledcWrite( LEDC_CH_4, map(degree4, -90, 90, SERVO_MIN_WIDTH_MS, SERVO_MAX_WIDTH_MS ) );
  if( servo5_enable ) ledcWrite( LEDC_CH_5, map(degree5, -90, 90, SERVO_MIN_WIDTH_MS, SERVO_MAX_WIDTH_MS ) );
}

char my_addr[18];
byte my_MAC[6];

uint8_t peer_addr[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

// Callback function that will be executed when data is received via ESP-NOW.
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) 
{
  if( data_len < 3 ) return; // Ignore small packets
  
  // --- Protocol Handshake ---
  // Controller sends ENQ, Robot replies with ACK and its MAC address.
  if( data[0]=='E' && data[1]=='N' && data[2]=='Q') // For Robot: Received discovery request
  {
    sendACK();
  }
  else if( data[0]=='A' && data[1]=='C' && data[2]=='K') // For Controller: Received ACK, save MAC and restart
  {
    M5.Display.print(" -> ACK\n");

    macFile = SPIFFS.open(macFileName, FILE_WRITE);
    for( int i=0; i<6; i++ ) macFile.write(mac_addr[i]);
    macFile.close();
    
    esp_restart();
  }
  // --- Battery Level Exchange ---
  else if( data[0]=='B' && data[1]=='A' && data[2]=='T') // For Robot: Received battery level request
  {
    M5.Display.print(" -> BAT\n");
    uint8_t data[3] = {'B', 'T', (uint8_t)M5.Power.getBatteryLevel()};
    esp_now_send(slave.peer_addr, data, sizeof(data));
  }
  else if( data[0]=='B' && data[1]=='T') // For Controller: Received battery level
  {
    bat = data[2];
  }
  // --- Main Control Data ---
  else // For Robot: Received control data
  {
     servo_left = data[0]-SERVO_OFFSET;
     servo_right = data[1]-SERVO_OFFSET;

    // Unpack button flags from the 3rd byte
    bool sw[FLAG_SW_MAX];
    for(int i=0; i<FLAG_SW_MAX; i++) sw[i] = data[2] & (1<<i);

    if( gen2 )
    {
      servo_arm = data[3]-SERVO_OFFSET;
      trim_arm = data[4]-SERVO_OFFSET;
    }
    else
    {
      if( sw[FLAG_TRGR] ){ servo_arm = SERVO_ARM_ANGLE_TRIGGER; setExpAvatar(1); } 
      else
      {
        if( sw[FLAG_SW_OK] ) setExpAvatar(2); // && !old_sw_ok ) mode_arm = !mode_arm;
        else if( sw[FLAG_SW_NG] ) setExpAvatar(3); //trim_arm = SERVO_ARM_TRIM; }
        else{
          setExpAvatar(0); trim_arm = 0;
          //if( mode_arm ) trim_arm = SERVO_ARM_TRIM; else trim_arm = 0;
        }
        servo_arm = -40;
      }
    old_sw_ok = sw[FLAG_SW_OK];

    }
  }
  old_rev_time = millis(); // Update the time of the last received packet
}

BlynkTimer timer;
BLYNK_WRITE(V0)
{
  servo_left = param.asInt();
  servo_left = deadbanded( servo_left, BLYNK_DEADBAND );
}
BLYNK_WRITE(V1)
{
  servo_right = param.asInt();
  servo_right = deadbanded( servo_right, BLYNK_DEADBAND );
}
BLYNK_WRITE(V2)
{
  servo_arm = param.asInt();
}
void myTimerEvent(){}

void setupBlynk() {
    if( M5.getBoard() == m5::board_t::board_M5Atom || M5.getBoard() == m5::board_t::board_M5AtomS3 )  
      if( M5.Imu.isEnabled() ) draw_matrix_str("BLYNK B", MATRIX_CHAR_DELAY_MS_NORMAL, COLOR_GREEN, 0 );
      else{ strip.setPixelColor(0,COLOR_GREEN );  strip.show(); }
    else M5.Display.printf("Blynk IoT mode...");
    Blynk.begin(auth, "ssid", "pass");
    timer.setInterval(BLYNK_TIMER_INTERVAL_MS, myTimerEvent);
    Serial.printf("[%lu] Blynk\n", millis() );  
    M5.Display.printf("OK\n");
    initServo();
}

void setupFlex() {
    if( M5.getBoard() == m5::board_t::board_M5Atom || M5.getBoard() == m5::board_t::board_M5AtomS3 )  
      if( M5.Imu.isEnabled() ) draw_matrix_str("UGOKU U", MATRIX_CHAR_DELAY_MS_NORMAL, COLOR_MAGENTA, 0 );
      else{ strip.setPixelColor(0,COLOR_MAGENTA );  strip.show(); }
    else M5.Display.printf("UGOKU Pad mode\n");
    
    controller.setup("MiniKawa1");
    controller.setOnConnectCallback(onDeviceConnect);   // Function called on device connection
    controller.setOnDisconnectCallback(onDeviceDisconnect);  // Function called on device disconnection

    uint8_t macBT[6];
    esp_read_mac(macBT, ESP_MAC_BT);
    Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\r\n", macBT[0], macBT[1], macBT[2], macBT[3], macBT[4], macBT[5]);
    initServo();
      setupAvatar();
}

void setupEspNow() {
    WiFi.mode(WIFI_STA);
    WiFi.macAddress(my_MAC);
    snprintf(my_addr, sizeof(my_addr), "%02X:%02X:%02X:%02X:%02X:%02X",
            my_MAC[0], my_MAC[1], my_MAC[2], my_MAC[3], my_MAC[4], my_MAC[5]);
    WiFi.disconnect();

    if (esp_now_init() != ESP_OK) { M5.Display.print("ESPNow Init Failed\n"); ESP.restart(); }

    if( comm_mode == MODE_ESPNOW )
    {
      if( M5.getBoard() == m5::board_t::board_M5Atom || M5.getBoard() == m5::board_t::board_M5AtomS3 )  
        if( M5.Imu.isEnabled() ) draw_matrix_str("ESP-NOW E", MATRIX_CHAR_DELAY_MS_NORMAL, COLOR_BLUE, 0 );
        else{ strip.setPixelColor(0,COLOR_BLUE );  strip.show(); }
      else M5.Display.printf("ESP-NOW mode %s\n %s\n", version, my_addr);
      Serial.printf("[%lu] ESP-NOW mode\n", millis() );
      initServo();
      setupAvatar();
    }
    else if( comm_mode == MODE_LCONTROL || comm_mode == MODE_RCONTROL )
    {  
      if( M5.getBoard() == m5::board_t::board_M5Atom || M5.getBoard() == m5::board_t::board_M5AtomS3 ) 
      {
        if( M5.Imu.isEnabled() ) 
        {
          if( comm_mode == MODE_LCONTROL ) draw_matrix_str("L Controler ", MATRIX_CHAR_DELAY_MS_FAST, COLOR_WHITE, 0 );
          else                             draw_matrix_str("R Controler ", MATRIX_CHAR_DELAY_MS_FAST, COLOR_WHITE, 0 );
        }
        else{ strip.setPixelColor(0,COLOR_WHITE );  strip.show(); }        
      }
      else
      {
        if( comm_mode == MODE_LCONTROL ) M5.Display.printf("Left Controller mode %s\n %s\n", version, my_addr);
        else                             M5.Display.printf("Right Controller mode %s\n %s\n", version, my_addr);        
      }
      if( comm_mode == MODE_LCONTROL ) Serial.printf("[%lu] Left Controller mode\n", millis() );
      else                             Serial.printf("[%lu] Right Controller mode\n", millis() );       

      macFile = SPIFFS.open(macFileName, FILE_READ);
      if( SPIFFS.exists(macFileName) )
      {
        for( int i=0; i<6; i++ ) peer_addr[i] = macFile.read();
        Serial.printf("[%lu] macFile %x\n", millis(), peer_addr[5] );
      }
      else Serial.printf("[%lu] macFile failed! \n", millis());
      macFile.close();

      Serial.printf("M5.getBoard() = %d\n",M5.getBoard() );
      initController();
    }
    
    memset(&slave, 0, sizeof(slave));
    memcpy(slave.peer_addr, peer_addr, sizeof(peer_addr));
    
    esp_err_t addStatus = esp_now_add_peer(&slave);
    
    snprintf(my_addr, sizeof(my_addr), "%02X:%02X:%02X:%02X:%02X:%02X",
            peer_addr[0], peer_addr[1], peer_addr[2], peer_addr[3], peer_addr[4], peer_addr[5]);
    if( comm_mode == MODE_LCONTROL || comm_mode == MODE_RCONTROL ) M5.Display.printf("for %s", my_addr);

    esp_now_register_recv_cb(OnDataRecv);
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  Serial.begin(115200);
  SPIFFS.begin(true);

  iniFile = SPIFFS.open(iniFileName, FILE_READ);
  if( SPIFFS.exists(iniFileName) )
  {
    comm_mode = iniFile.read();
    Serial.printf("[%lu] iniFile %x\n", millis(), comm_mode );
  }
  else Serial.printf("[%lu] iniFile failed! \n", millis());
  iniFile.close();

  if( comm_mode == MODE_LCONTROL ) lefty = true;
  else if( comm_mode == MODE_RCONTROL ) lefty = false;

  if( M5.getBoard() == m5::board_t::board_M5Capsule ) comm_mode = MODE_ESPNOW; //Blyncも成立するが区別しにくいので専用化
  else if( M5.getBoard() == m5::board_t::board_M5AtomS3R ) comm_mode = MODE_ESPNOW; //Blyncも成立するが区別しにくいので専用化
  else if( (M5.getBoard() == m5::board_t::board_M5Atom || M5.getBoard() == m5::board_t::board_M5StickC ||
            M5.getBoard() == m5::board_t::board_M5StickCPlus || M5.getBoard() == m5::board_t::board_M5StickCPlus2)
            && (comm_mode == MODE_LCONTROL || comm_mode == MODE_RCONTROL) ) comm_mode++; //コントローラーモードスキップ＝ATOMS3系のみ

  if( M5.getBoard() == m5::board_t::board_M5StickC )
  {
    M5.Display.setRotation(1);
    M5.Display.setTextSize(1);
    M5.Display.setCursor(5, 5);
  }
  else if( M5.getBoard() == m5::board_t::board_M5StickCPlus || M5.getBoard() == m5::board_t::board_M5StickCPlus2 )
  {
    M5.Display.setRotation(1);
    M5.Display.setTextSize(2);
    M5.Display.setCursor(5, 5);
  }
  else if( M5.getBoard() == m5::board_t::board_M5AtomS3 || M5.getBoard() == m5::board_t::board_M5AtomS3R )
  {
    M5.Display.setRotation(lefty?2:0); // Controller
    //M5.Display.setRotation(2); // Robot
  }
  else if( M5.getBoard() == m5::board_t::board_M5Atom )  strip.begin();
  else  M5.Display.setRotation(2);

  switch(comm_mode) {
    case MODE_BLYNC:
      setupBlynk();
      break;
    case MODE_FLEX:
      setupFlex();
      break;
    case MODE_ESPNOW:
    case MODE_LCONTROL:
    case MODE_RCONTROL:
      setupEspNow();
      break;
  }

  old_rev_time = millis();
}

bool click_inv;

void loopBlynk() {
    Blynk.run();
    timer.run();
    setServo( (servo_right-servo_left)/2, -(servo_left+servo_right)/2, servo_arm, -servo_arm );
}

void loopFlex() {
  if (isConnected) {
    uint8_t err = controller.read_data();
    
    if (err == no_err) {
      uint8_t pairs = controller.getLastPairsCount();

      // If there is at least one pair, find out which channels changed
      if (pairs > 0) {

        uint8_t ch1Val = controller.getDataByChannel(1);
        if (ch1Val != 0xFF) {
          servo_left = FLEX_CONTROLLER_CENTER_VAL - ch1Val;
        }

        uint8_t ch2Val = controller.getDataByChannel(2);
        if (ch2Val != 0xFF) {
          servo_right = FLEX_CONTROLLER_CENTER_VAL - ch2Val;
        }

        uint8_t ch3Val = controller.getDataByChannel(3);
        if (ch3Val != 0xFF) {
          servo_arm = FLEX_CONTROLLER_CENTER_VAL - ch3Val;
        }
      }
    } else if (err == cs_err) {
      Serial.println("Checksum error on incoming packet");
    } else if (err == data_err) {
      Serial.println("Incoming packet length != 19");
    }
  }
  //servo_left = deadbanded( servo_left, 2 );
  //servo_right = deadbanded( servo_right, 2 );
  setServo( (servo_right+servo_left)/2, -(-servo_left+servo_right)/2, servo_arm, -servo_arm );
  //setServo( servo_left, -servo_right, servo_arm, -servo_arm );  
  // digitalWrite(SERVO_PIN4, servo_arm == SERVO_ARM_ANGLE_TRIGGER );
  // digitalWrite(SERVO_PIN3, servo_arm == SERVO_ARM_ANGLE_TRIGGER );

}

void loopEspNowRobot() {
    bool recent_recv = millis()-old_rev_time < RECENT_RECV_TIMEOUT_MS;

    if( M5.BtnA.pressedFor(BUTTON_PRESS_DURATION_MS) && click_inv ) setServo(max_angle, max_angle, max_angle, -max_angle, -max_angle);
    else if( M5.BtnA.pressedFor(BUTTON_PRESS_DURATION_MS) && !click_inv ) setServo(-max_angle, -max_angle, -max_angle, max_angle, max_angle);
    // The following line contains complex logic for servo control which might need tuning.
    else if( recent_recv ) setServo( servo_right, servo_left, trim_arm/2-180-servo_arm*3, -(trim_arm/2-180-servo_arm*3) );
    else setServo();

    if( M5.BtnA.wasPressed() ) click_inv = !click_inv;
}

void loopEspNowController() {
    if( M5.BtnA.wasClicked() ) sendBat();
    else if(M5.BtnA.wasHold()) sendENQ();
    getStick();
    sendData();
    dispStick();
}


void loop()
{
  M5.update();
  if(M5.BtnA.wasDoubleClicked()) setRecvMode();

  switch(comm_mode) {
    case MODE_BLYNC:
      loopBlynk();
      break;
    case MODE_FLEX:
      loopFlex();
      break;
    case MODE_ESPNOW:
      loopEspNowRobot();
      break;
    case MODE_LCONTROL:
    case MODE_RCONTROL:
      loopEspNowController();
      break;
  }

  delay(10);
}
