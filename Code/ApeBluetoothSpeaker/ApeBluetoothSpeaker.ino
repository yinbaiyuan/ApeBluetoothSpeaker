#include "ApeBluetoothSpeaker.h"
#include "Apc_AreaMatrix.h"
#include "Fonts/ApcFont35.h"
#include <ArduinoJson.h>
#include <MSGEQ7.h>

#define   PIN_AUDIO         A0
#define   PIN_STROBE        10
#define   PIN_RESET         D0
#define   MSGEQ7_SMOOTH     true
#define   MSGEQ7_INTERVAL   ReadsPerSecond(50)

CMSGEQ7<MSGEQ7_SMOOTH, PIN_RESET, PIN_STROBE, PIN_AUDIO, PIN_AUDIO>  MSGEQ7;

ApeBluetoothSpeaker apc;

#define WIFI_SSID       "DLINK-0981"      //note:only support 2.4G wifi
#define WIFI_PASSWORD   "ape@2020"

void setup() {
  apc.systemInit(WIFI_SSID, WIFI_PASSWORD, 15);
  MSGEQ7.begin();

  ApcEffectDef* apcEffect_TimeShow = new ApcEffectDef;
  memset(apcEffect_TimeShow, 0, sizeof(ApcEffectDef));
  apcEffect_TimeShow->areaDef[0] = {0, 0, 16, 16, 1, 500};
  apcEffect_TimeShow->callbackFunc = apcEffect_TimeShow_callback;
  apcEffect_TimeShow->autoChangeTime = 5000;
  apc.addApcEffect(apcEffect_TimeShow);

  //  ApcEffectDef* apcEffect_youtubeSubscriberCount = new ApcEffectDef;
  //  memset(apcEffect_youtubeSubscriberCount, 0, sizeof(ApcEffectDef));
  //  apcEffect_youtubeSubscriberCount->areaDef[0] = {0, 0, 8, 8, 1, 99999999};
  //  apcEffect_youtubeSubscriberCount->areaDef[1] = {8, 0, 24, 8, 6, 1000};
  //  apcEffect_youtubeSubscriberCount->callbackFunc = apcEffect_youtubeSubscriberCount_callback;
  //  apcEffect_youtubeSubscriberCount->autoChangeTime = 5000;
  //  apc.addApcEffect(apcEffect_youtubeSubscriberCount);
  //
  //  ApcEffectDef* apcEffect_biliSubscriberCount = new ApcEffectDef;
  //  memset(apcEffect_biliSubscriberCount, 0, sizeof(ApcEffectDef));
  //  apcEffect_biliSubscriberCount->areaDef[0] = {0, 0, 8, 8, 1, 99999999};
  //  apcEffect_biliSubscriberCount->areaDef[1] = {8, 0, 24, 8, 6, 1000};
  //  apcEffect_biliSubscriberCount->callbackFunc = apcEffect_biliSubscriberCount_callback;
  //  apcEffect_biliSubscriberCount->autoChangeTime = 5000;
  //  apc.addApcEffect(apcEffect_biliSubscriberCount);
  //
  //  ApcEffectDef* apcEffect_temperature = new ApcEffectDef;
  //  memset(apcEffect_temperature, 0, sizeof(ApcEffectDef));
  //  apcEffect_temperature->areaDef[0] = {0, 0, 32, 8, 1, 99999999};
  //  apcEffect_temperature->callbackFunc = apcEffect_temperature_callback;
  //  apcEffect_temperature->autoChangeTime = 5000;
  //  apc.addApcEffect(apcEffect_temperature);
  //
  ApcEffectDef* apcEffect_freq = new ApcEffectDef;
  memset(apcEffect_freq, 0, sizeof(ApcEffectDef));
  apcEffect_freq->areaDef[0] = {0, 0, 16, 16, 1, 16};
  apcEffect_freq->callbackFunc = apcEffect_freq_callback;
  apcEffect_freq->autoChangeTime = 5000;
  apc.addApcEffect(apcEffect_freq);

  ApcEffectDef* apcEffect_pic = new ApcEffectDef;
  memset(apcEffect_pic, 0, sizeof(ApcEffectDef));
  apcEffect_pic->areaDef[0] = {0, 0, 16, 16, 1, 16};
  apcEffect_pic->callbackFunc = apcEffect_pic_callback;
  apcEffect_pic->autoChangeTime = 5000;
  apc.addApcEffect(apcEffect_pic);
  //
  //
  //
  //  apc.addApcScheduleCallback(60 * 60 * 1000, apcEffect_updateYouTubeSubscriberCount_callback);
  //  apcEffect_updateYouTubeSubscriberCount_callback();
  //
  //  apc.addApcScheduleCallback(60 * 60 * 1000, apcEffect_updatebiliSubscriberCount_callback);
  //  apcEffect_updatebiliSubscriberCount_callback();
  //
  //  apc.addApcScheduleCallback(15 * 60 * 1000, apcEffect_wether_callback);
  //  apcEffect_wether_callback();
}

void loop() {
  apc.apcLoop();
}

void apcEffect_TimeShow_callback(unsigned int areaCount, unsigned int frameCount, Apc_AreaMatrix& matrix)
{
  if (areaCount == 0)
  {
    matrix.setFont(&ApcFont35);
    matrix.setTextColor(matrix.Color(255, 255, 255));
    DateTime now = apc.now();
    matrix.setCursor(4, 7);
    matrix.print(dateFormat(now.hour()));
    matrix.setCursor(4, 14);
    matrix.print(dateFormat(now.minute()));

    int sec = now.second();
    for (int i = 0; i <= sec; i++)
    {
      if (i >= 1 && i <= 7) matrix.drawPixel(7 + i, 0, matrix.Color(255, 255, 255));
      else if (i >= 8 && i < 23) matrix.drawPixel(15, i - 8, matrix.Color(255, 255, 255));
      else if (i >= 23 && i < 38) matrix.drawPixel(38 - i, 15, matrix.Color(255, 255, 255));
      else if (i >= 38 && i < 53) matrix.drawPixel(0, 53 - i, matrix.Color(255, 255, 255));
      else if (i >= 53 && i < 60) matrix.drawPixel(i - 53, 0, matrix.Color(255, 255, 255));
    }
  }
}
String dateFormat(int n)
{
  if (n < 10) {
    return "0" + String(n);
  }
  else {
    return String(n);
  }
}
int youTubeSubscriberCount = 0;
void apcEffect_updateYouTubeSubscriberCount_callback()
{
  if (apc.isWifiConnected())
  {
    const String CHANNEL = "UCVP3cwbysoohuvQbSWN8RgA";
    const String APIKEY = "AIzaSyDVR3QM5-5Z3JihJOSB9";
    const String API = "https://www.googleapis.com/youtube/v3/channels?part=statistics";
    String url = API + "&id=" + CHANNEL + "&key=" + APIKEY;
    int errCode = 0;
    String res = apc.httpsRequest(url, &errCode);
    if (errCode == 0)
    {
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, res);
      JsonObject obj = doc.as<JsonObject>();
      youTubeSubscriberCount = obj["items"][0]["statistics"]["subscriberCount"].as<int>();
    }
  }
}

int biliSubscriberCount = 0;
void apcEffect_updatebiliSubscriberCount_callback()
{
  if (apc.isWifiConnected())
  {

    const String UID = "298146460";
    const String API = "https://api.bilibili.com/x/relation/stat?vmid=";
    String url = API + UID ;
    int errCode = 0;
    String res = apc.httpsRequest(url, &errCode);
    if (errCode == 0)
    {
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, res);
      JsonObject obj = doc.as<JsonObject>();
      biliSubscriberCount = obj["data"]["follower"].as<int>();
    }
  }
}
int temperature = 0;
int humidity = 0;
void apcEffect_wether_callback()
{
  if (apc.isWifiConnected())
  {

    const String API = "https://free-api.heweather.net/s6/weather/now?location=beijing&key=";
    const String apiKey = "057f8559ca8d4399b5910";
    String url = API + apiKey;
    int errCode = 0;
    String res = apc.httpsRequest(url, &errCode);
    Serial.println(res);
    if (errCode == 0)
    {
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, res);
      JsonObject obj = doc.as<JsonObject>();
      JsonObject nowObj = obj["HeWeather6"][0]["now"].as<JsonObject>();
      temperature = nowObj["tmp"].as<int>();
      humidity = nowObj["hum"].as<int>();
    }
    Serial.printf("temperature:%d\n", temperature);
    Serial.printf("humidity:%d\n", humidity);
  }

}

unsigned long colorArr[3] = {0x000000, 0xFF0000, 0xFFFFFF};
unsigned char pixels[64] =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,
  0x01, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x02, 0x02, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x02, 0x02, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01, 0x01,
  0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void apcEffect_youtubeSubscriberCount_callback(unsigned int areaCount, unsigned int frameCount, Apc_AreaMatrix& matrix)
{
  if (areaCount == 0)
  {
    matrix.drawColorIndexFrame(colorArr, 8, 8, pixels);
  } else if (areaCount == 1)
  {
    String countStr = String(youTubeSubscriberCount);
    unsigned int strLength = countStr.length();
    matrix.setFont(&ApcFont35);
    matrix.setTextColor(matrix.Color(255, 255, 255));
    matrix.setCursor(apc.textCenterX(strLength, 4, 6), 7);
    matrix.print(countStr);
  }
}


unsigned long biliColorArr[2] = {0x000000, 0x00A1F1};
unsigned char biliPixels[64] =
{
  0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
  0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00,
  0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x01,
  0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x01,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00
};


void apcEffect_biliSubscriberCount_callback(unsigned int areaCount, unsigned int frameCount, Apc_AreaMatrix& matrix)
{
  if (areaCount == 0)
  {
    matrix.drawColorIndexFrame(biliColorArr, 8, 8, biliPixels);
  } else if (areaCount == 1)
  {
    String countStr = String(biliSubscriberCount);
    unsigned int strLength = countStr.length();
    matrix.setFont(&ApcFont35);
    matrix.setTextColor(matrix.Color(255, 255, 255));
    matrix.setCursor(apc.textCenterX(strLength, 4, 6), 7);
    matrix.print(countStr);
  }
}

unsigned long picColorArr[4] = {0x000000, 0xf83400, 0xaf7f00, 0xffa344};
unsigned char picPixels[256] =
{

  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x03, 0x03, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x02, 0x03, 0x02, 0x03, 0x03, 0x03, 0x02, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x02, 0x03, 0x02, 0x02, 0x03, 0x03, 0x03, 0x02, 0x03, 0x03, 0x03, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x02, 0x02, 0x03, 0x03, 0x03, 0x03, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x01, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x01, 0x02, 0x02, 0x01, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00,
  0x00, 0x00, 0x03, 0x03, 0x02, 0x01, 0x03, 0x01, 0x01, 0x03, 0x01, 0x02, 0x03, 0x03, 0x00, 0x00,
  0x00, 0x00, 0x03, 0x03, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x03, 0x03, 0x00, 0x00,
  0x00, 0x00, 0x03, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x03, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00,
};
void apcEffect_pic_callback(unsigned int areaCount, unsigned int frameCount, Apc_AreaMatrix& matrix)
{
  matrix.drawColorIndexFrame(picColorArr, 16, 16, picPixels);
}

unsigned long eyesColorArr[2] = {0x000000, 0x00A1F1};
unsigned char eyesPixels[64] =
{

  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01,
  0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void apcEffect_temperature_callback(unsigned int areaCount, unsigned int frameCount, Apc_AreaMatrix& matrix)
{
  if (areaCount == 0)
  {
    char wInfo[20];
    sprintf(wInfo, "%dC  %d%%", temperature, humidity);
    String countStr = String(wInfo);
    unsigned int strLength = countStr.length();
    matrix.setFont(&ApcFont35);
    matrix.setTextColor(matrix.Color(255, 255, 255));
    matrix.setCursor(apc.textCenterX(strLength, 4, 10), 7);
    matrix.print(countStr);
  } else if (areaCount == 1)
  {
    matrix.drawColorIndexFrame(eyesColorArr, 8, 8, eyesPixels);
  }
}

long color[7] = {0xde101e, 0xfe6014, 0xe2d202, 0x6ccf49, 0x002fb6, 0x029fe9, 0x611fb0};
float heightestFreqVal[7];
void apcEffect_freq_callback(unsigned int areaCount, unsigned int frameCount, Apc_AreaMatrix& matrix)
{
  MSGEQ7.read(MSGEQ7_INTERVAL);
  for (byte i = 0; i < 7; i++) {  // cycle through the seven channels
    int averageValue = MSGEQ7.get(i) ;
    int logLed = max(averageValue / 16, 1); //logPeak(averageValue);  //log-scale into range for the display
    if (heightestFreqVal[i] < logLed) heightestFreqVal[i] = logLed;
    for (int y = 0; y < logLed; y++)
    {
      for (int offset_x = 1; offset_x <= 2; offset_x++)
      {
        matrix.drawPixel(offset_x + 2 * i, 15 - y, matrix.Color(color[i] >> 16 & 0xFF, color[i] >> 8 & 0xFF, color[i] >> 0 & 0xFF));
      }
    }
    for (int offset_x = 1; offset_x <= 2; offset_x++)
    {
      matrix.drawPixel(offset_x + 2 * i, 15 - heightestFreqVal[i], matrix.Color(255, 255, 255));
    }
    heightestFreqVal[i] -= 0.05;
  }
}
