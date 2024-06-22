#include <Arduino.h>
#include <WiFiSSLClient.h>
#include <Arduino_LED_Matrix.h>

#include <Arduino_JSON.h>

#include "misakiUTF16.h"

#include "arduino_secrets.h"
#include "wifi_connection_manage.h"
#include "wifi_client_manage.h"

// URL
// Host for the Japan Meteorological Agency
#define JMA_HOST "www.jma.go.jp"
// Weather forecast summary for Tokyo
#define WFS_TOKYO "/bosai/forecast/data/overview_forecast/130000.json"

// WiFi connection management class
WifiConnectionManage wifiConManage;
// Client management class
WifiClientManage client;

ArduinoLEDMatrix matrix;          // LEDマトリックスドライバ
unsigned long frame[] = {0,0,0};  // 12x8ドット フレームバッファ

// フレームバッファの指定座標にドットをセット
void setAt(uint8_t x, uint8_t y, uint8_t color) {
  if (x>=12 || y >=8) {
    return;
  }  
  
  uint8_t bitno = y*12+x;      // 書込み位置を96ビットの連番に変換
  uint8_t index = bitno / 32;  // 連番からlongバイト配列位置を取得
  uint8_t pos   = bitno % 32;  // 配列内ビット位置を取得
  
  // 指定位置のビットにデータをセットする
  if (color) {
    frame[index]|=0b10000000000000000000000000000000>>pos;      // ドットのセット
  } else {
    frame[index]&=~(0b10000000000000000000000000000000>>pos);   // ドットのリセット
  }
}

// フレームバッファの1ドット左スクロール
void scroll() {
  uint8_t mlb[3]; // キャリーオーバービットの退避用
  
  // longデータの左ビットシフト
  for (uint8_t i=0; i<3; i++) {
    mlb[i] = frame[i] & 0b10000000000000000000000000000000 ?1:0;  // MSBのキャリーオーバービットの退避
    frame[i]=frame[i]<<1; // 左ビットシフト
  }

  // 0～7行の右端ビットクリアのためのマスク処理
  frame[0] &= 0b11111111111011111111111011111111;
  frame[1] &= 0b11101111111111101111111111101111;
  frame[2] &= 0b11111110111111111110111111111110;

  // キャリーオーバービットを元に戻す
  frame[0] = mlb[1] ? frame[0]|1:frame[0]&~1;
  frame[1] = mlb[2] ? frame[1]|1:frame[1]&~1;
}

// フレームバッファのクリア
void clear() {
   for (uint8_t i=0; i<3; i++) {
    frame[i] = 0;
   }
}

// 1文字左スクロール挿入
void scrollIn(uint8_t* font, uint8_t fw, uint8_t ypos, int wt) {
    int8_t wx,wy;

    // 1文字分 横幅fwドットフォントの挿入(縦位置オフセットyposを考慮)
    for (wx = 0; wx < fw; wx++) {
        scroll(); // 左に1ドットスクロール
        for (wy = 0; wy < 8; wy++) {
          setAt(11, wy+ypos, font[wy] & 0x80>>wx ? 1:0);
        }
        matrix.loadFrame(frame);
        delay(wt);
    }
}

// デモ4 漢字フォント利用
void demo(const char* dispString) {
  uint8_t font[8];
  uint8_t width;     // フォント幅

  // テスト用文字列
  char msg_str[] = "あいうえお123abcABCｱｲｳｴｵ、今日は3月18日です。かんたんなかんじの表示ができます。";

  size_t len = strlen(dispString);
  char *temp = new char[len]();
  strcpy(temp, dispString);

  char *str = temp;

  while(*str) {
    uint16_t ucode;  // utf16文字コード
    str += charUFT8toUTF16(&ucode, str);      // utf8 1文字分をutf16に変換
    width = isZenkaku(ucode) ? 8: 4;          // フォント幅の取得
    getFontDataByUTF16(font, ucode);          // フォントデータの取得
    scrollIn(font, width, 1, 80);             // 1文字分のスクロール表示
  }

  delete[] temp;
}

void setup()
{
  // Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  matrix.begin();

  // WiFi connection
  wifiConManage.init(SECRET_SSID, SECRET_PASS);

  // response data
  JSONVar response;

  // GET request
  client.get(JMA_ROOT_CA, JMA_HOST, WFS_TOKYO, response);

  demo((const char *)response["text"]);

}

void loop()
{
}
