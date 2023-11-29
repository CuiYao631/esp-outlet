/*
   @Author: CuiYao
   @Date: 2022-10-27 09:45:56
 * @LastEditors: CuiYao
 * @LastEditTime: 2022-10-28 09:23:28
 * @FilePath: /esp-outlet/blinker/blinker.ino
*/

#define BLINKER_WIFI
#define BLINKER_MIOT_OUTLET  //支持小爱开关插座
#include <Blinker.h>
#include <EasyButton.h>
#include <BGWiFiConfig.h>
#include "wifi_info.h"
#define LED 2         //板子上的灯
#define BUTTON_PIN 0  //Button PIN

int duration = 2000;  //长按触发时间
int inputValue = 0;
bool wsState;         //小爱反馈的状态


BGWiFiConfig wifipw;  //声明BGWiFiConfig
EasyButton button(BUTTON_PIN);  //声明按钮
BlinkerButton Button1("btn-abc");  // 新建blinker组件对象、注意：要和APP组件’数据键名’一致

/*
  按下BlinkerAPP按键即会执行该函数
*/
void button1_callback(const String& state) {
  BLINKER_LOG("get button state: ", state);

  inputValue = digitalRead(LED);
  if (inputValue == LOW) {
    digitalWrite(LED, HIGH);
    updateStateForOFF();
  } else {
    digitalWrite(LED, LOW);
    updateStateForON();
  }
  Blinker.vibrate();
}
/*
 * 更新反馈状态为ON
 */
void updateStateForON() {
  Serial.println("on");          //串口打印状态
  Button1.print("on");           //反馈按键状态
  BlinkerMIOT.powerState("on");  //向小爱反馈电源状态
  BlinkerMIOT.print();           //将以上属性发送出去, 务必最后调用该函数
  wsState=true;
}
/*
 * 更新反馈状态为OFF
 */
void updateStateForOFF() {
  Serial.println("off");          //串口打印状态
  Button1.print("off");           //反馈按键状态
  BlinkerMIOT.powerState("off");  //向小爱反馈电源状态
  BlinkerMIOT.print();            //将以上属性发送出去, 务必最后调用该函数
  wsState=false;
}


/*
  小爱电源类操作的回调函数:
  当小爱同学向设备发起控制, 设备端需要有对应控制处理函数
*/
void miotPowerState(const String& state) {
  BLINKER_LOG("need set power state: ", state);

  if (state == BLINKER_CMD_ON) {
    digitalWrite(LED, LOW);
    updateStateForON();
  } else if (state == BLINKER_CMD_OFF) {
    digitalWrite(LED, HIGH);
    updateStateForOFF();
  }
}

/*
 *小爱同学的回调查询函数
 */
void miotQuery(int32_t queryCode)
{
    BLINKER_LOG("MIOT Query codes: ", queryCode);
    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("MIOT Query All");
            BlinkerMIOT.powerState(wsState ? "on" : "off");
            BlinkerMIOT.color(0);
            BlinkerMIOT.mode(0);
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
            BLINKER_LOG("MIOT Query Power State");
            BlinkerMIOT.powerState(wsState ? "on" : "off");
            BlinkerMIOT.print();
            break;   
        default :
            BlinkerMIOT.powerState(wsState ? "on" : "off");
            BlinkerMIOT.print();
            break;
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
  Serial.println("Button has been pressed!");
  inputValue = digitalRead(LED);
  if (inputValue == LOW) {
    digitalWrite(LED, HIGH);
    updateStateForOFF();
  } else {
    digitalWrite(LED, LOW);
    updateStateForON();
  }
  Blinker.vibrate();
}
// 长按事件函数
void onPressedForDuration() {
  Serial.println("ClearWiFi!");
  wifipw.clearWiFi();
}


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

/*
   初始化Blinker
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
    Blinker.begin("45d9bf644ae4", wifipw.readWiFi(0).c_str(), wifipw.readWiFi(1).c_str());
    //注册读取状态事件
    Blinker.attachData(dataRead);
    //注册按钮
    Button1.attach(button1_callback);
    //注册回调函数    
    //小爱同学务必在回调函数中反馈该控制状态
    BlinkerMIOT.attachPowerState(miotPowerState);
    //小爱同学查询状态   
    BlinkerMIOT.attachQuery(miotQuery);
  }
}

/*
  初始化函数
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
   Loop函数
*/
void loop() {
  wifipw.Loop();
  if (wifipw.OK())
    Blinker.run();
    // 持续更新按钮状态。
    button.read();
  

  
  
}