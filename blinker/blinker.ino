/*
 * @Author: CuiYao
 * @Date: 2022-10-27 09:45:56
 * @LastEditors: CuiYao
 * @LastEditTime: 2022-10-27 15:56:09
 * @FilePath: /esp-outlet/blinker/blinker.ino
 */

#define BLINKER_WIFI
#define BLINKER_MIOT_LIGHT  //支持小爱开关LED
#include <Blinker.h>
#include <EasyButton.h>
#include <BGWiFiConfig.h>
#include "wifi_info.h"
#define LED 2         //板子上的灯
#define BUTTON_PIN 0  //Button PIN

int duration = 2000;  //长按触发时间
int counter = 0;

BGWiFiConfig wifipw;  //声明BGWiFiConfig

EasyButton button(BUTTON_PIN);  //声明按钮

BlinkerButton Button1("btn-abc");  // 新建blinker组件对象、注意：要和APP组件’数据键名’一致

/*
 *按下BlinkerAPP按键即会执行该函数
 */
void button1_callback(const String& state) {
  BLINKER_LOG("get button state: ", state);
  digitalWrite(LED, !digitalRead(LED));
  //Button1.vibrate();
}

/*
 *小爱电源类操作的回调函数:
 *当小爱同学向设备发起控制, 设备端需要有对应控制处理函数
 */
void miotPowerState(const String& state) {
  BLINKER_LOG("need set power state: ", state);

  if (state == BLINKER_CMD_ON) {
    digitalWrite(LED_BUILTIN, LOW);

    BlinkerMIOT.powerState("on");
    BlinkerMIOT.print();
  } else if (state == BLINKER_CMD_OFF) {
    digitalWrite(LED_BUILTIN, HIGH);

    BlinkerMIOT.powerState("off");
    BlinkerMIOT.print();
  }
}

void dataRead(const String& data) {
  BLINKER_LOG("Blinker readString: ", data);
  
  Blinker.vibrate();

  uint32_t BlinkerTime = millis();

  Blinker.print("millis", BlinkerTime);
}

// 单按事件函数
void onPressed() {
}
// 长按事件函数
void onPressedForDuration() {
}
/*
 * 初始化WiFI
 */
void initWifi() {
  wifipw.offConnectWiFi(true);
  wifipw.setZDYhtml(Html);
  wifipw.setZDYhtmlret(Htmlret);
  wifipw.setNumUMSG(1);
  wifipw.begin();
}

/*
 * 初始化Blinker
 */
void initBlinker() {
  if (wifipw.OK()) {
    Serial.println(">>> blinker start debug <<<");
    Serial.println(wifipw.readUMSG(1));
    Serial.println(wifipw.readWiFi(0));
    Serial.println(wifipw.readWiFi(1));
    Serial.println(">>> end <<<");
    //DeBug
    BLINKER_DEBUG.stream(Serial);
    //初始化IO
    pinMode(LED, OUTPUT);
    //连接wifi
    Blinker.begin(wifipw.readUMSG(1).c_str(), wifipw.readWiFi(0).c_str(), wifipw.readWiFi(1).c_str());
    //注册读取状态事件
    Blinker.attachData(dataRead);
    //注册按钮
    Button1.attach(button1_callback);
    //小爱同学务必在回调函数中反馈该控制状态
    //注册回调函数
    BlinkerMIOT.attachPowerState(miotPowerState);
  }
}
/*
 *初始化函数
 */
void setup() {
  // 初始化串口，并开启调试信息，调试用可以删除
  Serial.begin(115200);

  //初始化WIFI
  initWifi();
  //初始化Blinker
  initBlinker();
  //定义按键单按事件回调
  button.onPressed(onPressed);
  //定义按键长按事件回调
  button.onPressedFor(duration, onPressedForDuration);
}
/*
 * Loop函数
 */
void loop() {
  wifipw.Loop();
  if (wifipw.OK())
    Blinker.run();
}