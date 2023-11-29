/*
 * @Author: CuiYao 
 * @Date: 2022-11-02 10:35:48 
 * @Last Modified by: CuiYao
 * @Last Modified time: 2022-11-02 10:37:27
 * 
 * 智能语言控制控制，支持同时天猫、小爱、小度、google HomeAssistent控制
 * 也同时支持web控制、小程序控制、app控制，定时控制等
 * 项目示例：通过发送on或off控制开关
 * 官网：bemfa.com
 */

#include <EasyButton.h> 
#include <ESP8266WiFi.h>   //默认，加载WIFI头文件
#include <Ticker.h>
#include <ESP8266httpUpdate.h>
#include "PubSubClient.h"  //默认，加载MQTT库文件
#include "wifi_info.h"     //默认, 加载WIFI文件

//********************需要修改的部分*******************//
#define ID_MQTT "9c11b5ce8d9925cf93d4a2e6023039d9"    // 用户私钥，控制台获取
const char* topic = "LED002";                    // 主题名字，可在巴法云控制台自行创建，名称随意
const int B_led = 2;                                  // 单片机LED引脚值，D系列是NodeMcu引脚命名方式，其他esp8266型号将D2改为自己的引脚
String upUrl = "http://bin.bemfa.com/b/1BcOWMxMWI1Y2U4ZDk5MjVjZjkzZDRhMmU2MDIzMDM5ZDk=outlet01001.bin";
//**************************************************//

const char* mqtt_server = "bemfa.com";  //默认，MQTT服务器
const int mqtt_server_port = 9501;      //默认，MQTT服务器

#define LED 2         // 板子上的灯
#define BUTTON_PIN 0  // Button PIN

int duration = 2000;  // 长按触发时间
int inputValue = 0;   // LED电平状态

Ticker ticker;                  // 声明定时器
WiFiClient espClient;           // 声明wifi
PubSubClient client(espClient); // 声明mMQTT库
EasyButton button(BUTTON_PIN);  // 声明按钮


// 单按事件函数
void onPressed() {
  Serial.println("Button has been pressed!");
  inputValue = digitalRead(LED);
  if (inputValue == LOW) {
    turnOffLed();
    sendtoTCPServer("off");
  } else {
    turnOnLed();
    sendtoTCPServer("on");
  }
}
// 长按事件函数
void onPressedForDuration() {

  //OTA
  // Serial.println("start update");    
  // updateBin(); 
  Serial.println("ClearWiFi!");
  WiFi.begin("000000", "000000");
}

/*
  *发送数据到TCP服务器
 */
void sendtoTCPServer(String p) {

  // 建立发布信息。信息内容以Hello World为起始，后面添加发布次数。
  String messageString = p;
  char publishMsg[messageString.length() + 1];
  strcpy(publishMsg, messageString.c_str());

  // 实现ESP8266向主题发布信息
  if (client.publish(topic, publishMsg)) {
    Serial.println("Publish Topic:");
    Serial.println(topic);
    Serial.println("Publish message:");
    Serial.println(publishMsg);
  } else {
    Serial.println("Message Publish Failed.");
  }
}


//打开灯泡
void turnOnLed() {
  digitalWrite(B_led, LOW);
}
//关闭灯泡
void turnOffLed() {
  digitalWrite(B_led, HIGH);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Topic:");
  Serial.println(topic);
  String msg = "";
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  Serial.print("Msg:");
  Serial.println(msg);
  if (msg == "on") {          //如果接收字符on，开
    turnOnLed();              //开灯函数
  } else if (msg == "off") {  //如果接收字符off，关
    turnOffLed();             //关灯函数
  }
  msg = "";
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(ID_MQTT)) {
      Serial.println("connected");
      Serial.print("subscribe:");
      Serial.println(topic);
      //订阅主题，如果需要订阅多个主题，可发送多条订阅指令client.subscribe(topic2);client.subscribe(topic3);
      client.subscribe(topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


//当升级开始时，打印日志
void update_started() {
  Serial.println("CALLBACK:  HTTP update process started");
}

//当升级结束时，打印日志
void update_finished() {
  Serial.println("CALLBACK:  HTTP update process finished");
}

//当升级中，打印日志
void update_progress(int cur, int total) {
  Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total);
}

//当升级失败时，打印日志
void update_error(int err) {
  Serial.printf("CALLBACK:  HTTP update fatal error code %d\n", err);
}


/**
 * 固件升级函数
 * 在需要升级的地方，加上这个函数即可，例如setup中加的updateBin(); 
 * 原理：通过http请求获取远程固件，实现升级
 */
void updateBin(){
  WiFiClient UpdateClient;
  ESPhttpUpdate.onStart(update_started);//当升级开始时
  ESPhttpUpdate.onEnd(update_finished); //当升级结束时
  ESPhttpUpdate.onProgress(update_progress); //当升级中
  ESPhttpUpdate.onError(update_error); //当升级失败时
  
  t_httpUpdate_return ret = ESPhttpUpdate.update(UpdateClient, upUrl);
  switch(ret) {
    case HTTP_UPDATE_FAILED:      //当升级失败
        Serial.println("[update] Update failed.");
        break;
    case HTTP_UPDATE_NO_UPDATES:  //当无升级
        Serial.println("[update] Update no Update.");
        break;
    case HTTP_UPDATE_OK:         //当升级成功
        Serial.println("[update] Update ok.");
        break;
  }
}

void setup() {
  pinMode(B_led, OUTPUT);                               // 设置引脚为输出模式
  digitalWrite(B_led, HIGH);                            // 默认引脚上电高电平
  Serial.begin(115200);                                 // 初始化串口日志
  ticker.attach(0.1, buttonRead);                       // 定时器
  wifi_connect();                                       // 初始化wifi
  client.setServer(mqtt_server, mqtt_server_port);      // 设置mqtt服务器
  client.setCallback(callback);                         // mqtt消息处理
  button.onPressed(onPressed);                          // 定义按键单按事件回调
  button.onPressedFor(duration, onPressedForDuration);  // 定义按键长按事件回调
}
// 检测按键状态
void buttonRead()
{
  button.read();
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}