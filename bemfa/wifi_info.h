/*
 * @Author: CuiYao 
 * @Date: 2022-11-02 10:37:44 
 * @Last Modified by:   CuiYao 
 * @Last Modified time: 2022-11-02 10:37:44 
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "TEST"
#define STAPSK  "1234567890"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);
bool LED_Flag = false;
String str = 
"<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><meta name=\"viewport\"content=\"width=device-width, initial-scale=1.0\"><meta http-equiv=\"X-UA-Compatible\"content=\"ie=edge\"><title>配网页面</title><style>*{box-sizing:border-box}body{font-family:Arial;margin:0}.header{height:100vh;padding:80px;text-align:center;background:#1a83bc;color:white}label{display:inline-block;width:80px;text-align:justify;text-align-last:justify}.button{background-color:#1a83bc;border:2px solid#FFFFFF;height:50px;width:200px;justify-content:center;align-items:center;font-size:23px;border-radius:8px;color:#FFFFFF}.header h1{font-size:40px}</style></head><body><div class=\"header\"><h1>欢迎使用智能插座</h1><form name=\"my\">WiFi名称：<p><input type=\"text\"name=\"s\"placeholder=\"请输入您WiFi的名称\"id=\"aa\"><br>WiFi密码：<p></p><input type=\"text\"name=\"p\"placeholder=\"请输入您WiFi的密码\"id=\"bb\"><br><p></p><input class=\"button\"type=\"button\"value=\"连接\"onclick=\"wifi()\"></form><p></p><a href=\"https://github.com/CuiYao631\">小崔搞科技出品</a></div><script language=\"javascript\">function wifi(){var ssid=my.s.value;var password=bb.value;var xmlhttp=new XMLHttpRequest();xmlhttp.open(\"GET\",\"/HandleVal?ssid=\"+ssid+\"&password=\"+password,true);xmlhttp.send()}</script></body></html>";

/*****************************************************
 * 函数名称：handleRoot()
 * 函数说明：客户端请求回调函数
 * 参数说明：无
******************************************************/
void handleRoot() {
  server.send(200, "text/html", str);
}
/*****************************************************
 * 函数名称：HandleVal()
 * 函数说明：对客户端请求返回值处理
 * 参数说明：无
******************************************************/
void HandleVal()
{
    String wifis = server.arg("ssid"); //从JavaScript发送的数据中找ssid的值
    String wifip = server.arg("password"); //从JavaScript发送的数据中找password的值
    Serial.println(wifis); Serial.println(wifip);
    WiFi.begin(wifis,wifip);
}
/*****************************************************
 * 函数名称：handleNotFound()
 * 函数说明：响应失败函数
 * 参数说明：无
******************************************************/
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
/*****************************************************
 * 函数名称：autoConfig()
 * 函数说明：自动连接WiFi函数
 * 参数说明：无
 * 返回值说明:true：连接成功 false：连接失败
******************************************************/
bool autoConfig()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin();
  Serial.print("AutoConfig Waiting......");
  for (int i = 0; i < 20; i++)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("AutoConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      WiFi.printDiag(Serial);
      return true;
      //break;
    }
    else
    {
      Serial.print(".");
//      LED_Flag = !LED_Flag;
//      if(LED_Flag)
//      else
      delay(500);
    }
  }
  Serial.println("AutoConfig Faild!" );
  return false;
  //WiFi.printDiag(Serial);
}
/*****************************************************
 * 函数名称：htmlConfig()
 * 函数说明：web配置WiFi函数
 * 参数说明：无
******************************************************/
void htmlConfig()
{
    WiFi.mode(WIFI_AP_STA);//设置模式为AP+STA
    WiFi.softAP(ssid, password);
    Serial.println("AP设置完成");
    
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
  
    if (MDNS.begin("esp8266")) {
      Serial.println("MDNS responder started");
    }
  
    server.on("/", handleRoot);
    server.on("/HandleVal", HTTP_GET, HandleVal);
    server.onNotFound(handleNotFound);//请求失败回调函数
  
    server.begin();//开启服务器
    Serial.println("HTTP server started");
    while(1)
    {
        server.handleClient();
        MDNS.update();  
        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("HtmlConfig Success");
            Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
            Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
            Serial.println("HTML连接成功");
            break;
        }
    }  
}

void wifi_connect() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
    
    bool wifiConfig = autoConfig();
    
    if(wifiConfig == false)
        htmlConfig();//HTML配网
}
