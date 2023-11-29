/*
 * @Author: CuiYao 
 * @Date: 2022-11-02 10:42:53 
 * @Last Modified by: CuiYao
 * @Last Modified time: 2022-11-02 10:47:56
 * 
 * HomeKit 原生模块直接通过扫码添加
 * 问   题：由于sep8266 只能连接2.4G 网络，会出现手机端无响应情况
 * 解决方法：配合 HomeAssistent 将模块添加到HomeAssistent中
 */

#include <Arduino.h>
#include <arduino_homekit_server.h>
#include <EasyButton.h>
#include <BGWiFiConfig.h>
#include "wifi_info.h"



#define PIN_SWITCH 2  //Switch PIN
#define PIN_BUTTON 0  //Button PIN
int duration = 2000;  //长按触发时间 ms
int inputValue = 0;


BGWiFiConfig wifipw;            //声明BGWiFiConfi
EasyButton button(PIN_BUTTON);  //声明按钮


#define LOG_D(fmt, ...) printf_P(PSTR(fmt "\n"), ##__VA_ARGS__);





/*
   初始化WiFI
*/
void initWifi() {
  wifipw.setWiFiTimeOut(40);//配网超时时间
  wifipw.autoStart(true);//配网信息写入后自动重启配置
  wifipw.setZDYhtml(Html);//自定配网页面
  wifipw.setZDYhtmlret(Htmlret);//自定义配网完成页面
  wifipw.begin();
}


void setup() {
  // 初始化串口，并开启调试信息
  Serial.begin(115200);
  //初始化按键
  button.begin();

  //定义按键单按事件回调
  button.onPressed(onPressed);
  //定义按键长按事件回调
  button.onPressedFor(duration, onPressedForDuration);
  //初始化WIFI
  initWifi();
  // 在首次运行此新的HomeKit示例时删除以前的HomeKit配对存储
  //homekit_storage_reset();
  my_homekit_setup();
}

void loop() {
  wifipw.Loop();
  if (wifipw.OK()) 
    // 持续更新homekit状态
    my_homekit_loop();
    // 持续更新按钮状态。
    button.read();
  delay(10);
}




//==============================
// HomeKit setup and loop
//==============================

// access your HomeKit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_outlet_on;

static uint32_t next_heap_millis = 0;



//Called when the switch value is changed by iOS Home APP
void cha_switch_on_setter(const homekit_value_t value) {
  bool on = value.bool_value;
  cha_outlet_on.value.bool_value = on;  //sync the value
  LOG_D("Switch: %s", on ? "ON" : "OFF");
  digitalWrite(PIN_SWITCH, on ? LOW : HIGH);
}

// 单按事件函数
void onPressed() {
  Serial.println("Button has been pressed!");
  inputValue = digitalRead(PIN_SWITCH);

  if (inputValue == LOW) {
    Serial.println("OFF");
    cha_outlet_on.value.bool_value = false;
    homekit_characteristic_notify(&cha_outlet_on, cha_outlet_on.value);
    digitalWrite(PIN_SWITCH, HIGH);
  } else {
    Serial.println("ON");
    cha_outlet_on.value.bool_value = true;
    homekit_characteristic_notify(&cha_outlet_on, cha_outlet_on.value);
    digitalWrite(PIN_SWITCH, LOW);
  }
}
// 长按事件函数
void onPressedForDuration() {
  wifipw.clearWiFi();
  Serial.println("wifi_reset");
  homekit_storage_reset();
  Serial.println("homekit_storage_reset");
  system_restart();
  Serial.println(" system_restart");
}

void my_homekit_setup() {
  pinMode(PIN_SWITCH, OUTPUT);
  digitalWrite(PIN_SWITCH, HIGH);

  cha_outlet_on.setter = cha_switch_on_setter;

  arduino_homekit_setup(&config);
}

void my_homekit_loop() {
  arduino_homekit_loop();
  const uint32_t t = millis();
  if (t > next_heap_millis) {
    // show heap info every 5 seconds
    next_heap_millis = t + 5 * 1000;
    LOG_D("Free heap: %d, HomeKit clients: %d",
          ESP.getFreeHeap(), arduino_homekit_connected_clients_count());
  }
}