/*
 * wifi_info.h
 *
 *  Created on: 2020-05-15
 *      Author: Mixiaoxiao (Wang Bin)
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include <Ticker.h>


//配置相关
#define HOST_NAME  "HomeKit-Switch"

char config_flag = 0;
extern "C" {
#include "user_interface.h"
#include "smartconfig.h"
}
struct config_type
{
  char stassid[32];
  char stapsw[64];

  uint8_t reboot;
  uint8_t magic;
};
config_type wifi_config;


//相关函数初始化
void doWiFiTick();       //检查WIFI状态
void doTCPClientTick();  //进行tcp连接相关
void startTCPClient();   //开始连接tcp
void sendtoTCPServer(String p); //tcp发送数据
void doSmartconfig();  //微信智能配网
void saveConfig();     //保存配网信息
void initWiFi();       //wifi初始化
void loadConfig();     //加载wifi信息
void restoreFactory(); //恢复出厂设置
void waitKey();        //检查是否重置
void delayRestart(float t);

/*
 * 开启smartconfig，等待配置完成，若超时，延时重启
*/
void doSmartconfig()
{
  Serial.print(" Smartconfig begin,Waiting for WeChat Config.....");
  WiFi.mode(WIFI_STA);
  WiFi.stopSmartConfig();
  WiFi.beginSmartConfig();
  int cnt = 0;
  bool flag_ok = false;
  while (true) {
    ESP.wdtFeed();
    delay(300);

    if (flag_ok == true) continue;

    if (WiFi.smartConfigDone()) {

      strcpy(wifi_config.stassid, WiFi.SSID().c_str());
      strcpy(wifi_config.stapsw, WiFi.psk().c_str());
      wifi_config.magic = 0xAA;
      saveConfig();

      delayRestart(10);
      flag_ok = true;
    }
    cnt++;
    if (cnt >= 600) {

      delayRestart(0);
    }
  }
}


void saveConfig()
{  
  int rand_key;
  uint8_t mac[6];
  WiFi.macAddress(mac);
  wifi_config.reboot = 0;
  
  EEPROM.begin(2018);
  uint8_t *p = (uint8_t*)(&wifi_config);
  for (int i = 0; i < sizeof(wifi_config); i++)
  {
    EEPROM.write(i, *(p + i));
  }
  EEPROM.commit();
}
Ticker delayTimer;
void delayRestart(float t) {
    delayTimer.attach(t, []() {
    ESP.restart();
  });
}
/*
*初始化WiFi
*/
void initWiFi()
{
  char temp[32];
  uint8_t mac[6];
  WiFi.macAddress(mac);
  sprintf(temp, "%s_%02X%02X%02X", HOST_NAME, mac[3], mac[4], mac[5]);
 
  WiFi.hostname(temp);

  if (strcmp(wifi_config.stassid, "") != 0)
  {
    WiFi.mode(WIFI_AP_STA);
    WiFi.disconnect();
    delay(100);
    WiFi.begin(wifi_config.stassid, wifi_config.stapsw);
  }
  else {

    WiFi.disconnect();
    delay(100);
    WiFi.mode(WIFI_AP);
  }
  WiFi.softAP(temp);
}

/**************************************************************************
                                 WIFI
***************************************************************************/
/*
  WiFiTick
  检查是否需要初始化WiFi
  检查WiFi是否连接上，若连接成功启动TCP Client
  控制指示灯
*/
void doWiFiTick(){
  static bool startSTAFlag = false;
  static bool taskStarted = false;
  static uint32_t lastWiFiCheckTick = 0;

  if (!startSTAFlag) {
    startSTAFlag = true;
    initWiFi();
  }

  //未连接1s重连
  if ( WiFi.status() != WL_CONNECTED ) {
    if (millis() - lastWiFiCheckTick > 1000) {
      lastWiFiCheckTick = millis();
    }
  }
  //连接成功建立
  else {
    if (taskStarted == false) {
      taskStarted = true;
      Serial.print("\r\nGet IP Address: ");
      Serial.println(WiFi.localIP());
    }
  }
}

/*
 * 从EEPROM加载参数
*/
uint8_t *p = (uint8_t*)(&wifi_config);
void loadConfig()
{

  uint8_t mac[6];
  Serial.println(" LoadConfig.......");
  WiFi.macAddress(mac);
  EEPROM.begin(2018);
  for (int i = 0; i < sizeof(wifi_config); i++)
  {
    *(p + i) = EEPROM.read(i);
  }
  wifi_config.reboot = wifi_config.reboot + 1;
  if(wifi_config.reboot>=4){
    restoreFactory();
  }
    if(wifi_config.magic != 0xAA){
      config_flag = 1;
    }
  EEPROM.begin(2018);
  for (int i = 0; i < sizeof(wifi_config); i++){
    EEPROM.write(i, *(p + i));
  }
  EEPROM.commit();
  delay(2000);

  
  Serial.println("loadConfig Over");
  EEPROM.begin(2018);
  wifi_config.reboot = 0;
  for (int i = 0; i < sizeof(wifi_config); i++){
    EEPROM.write(i, *(p + i));
  }
  EEPROM.commit();
  delay(2000);
}

/* 
 * 恢复出厂设置
*/
void restoreFactory()
{
  Serial.println("\r\n Restore Factory....... ");
  wifi_config.magic = 0x00;
  strcpy(wifi_config.stassid, "");
  strcpy(wifi_config.stapsw, "");
  wifi_config.magic = 0x00;
  saveConfig();
  delayRestart(1);
  while (1) {
    ESP.wdtFeed();
    delay(100);
  }
}


/*
 * 启动前5秒检查用户是否按键，如果有按键判断是短按或者长按，分别进入smartconfig模式或者恢复出厂
*/
void waitKey()
{  
  if(config_flag == 1){
    doSmartconfig(); 
    }
}


void wifi_connect() {
  ESP.wdtEnable(5000);//超时重启
  loadConfig();//加载wifi信息
  waitKey();  //判断是否恢复出厂
  Serial.println("wifi config ok");
}
