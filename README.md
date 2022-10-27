# 基于esp8266 开发的智能插座


### 文件夹结构
* bemfa     【巴法云】
* blinker   【点灯科技】
* homekit   【HomeKit 原生】
* otherInfo 【其它文件素材】

## 关于 HomeAssistant

【巴法云】 可以通过 MQTT 的方式接入 HomeAssistant、具体接入方法，参考 [官方文档](https://cloud.bemfa.com/docs/#/)

【HomeKit】 HomeAssistant安装homekit插件、当局域网中有homekit 设备、HomeAssistant会检测到，并提示

## 已存在的问题
homekit直接连接手机，会存在未响应的情况、原因可能出现在路由器网络的 2.4G和5G wifi的问题上、具体还在研究

## 项目使用的第三方库 

[EasyButton](https://github.com/evert-arias/EasyButton) 

用于消除瞬时接触开关的抖动，检测按下、释放、长按和带有事件定义和回调的序列

[BGWiFiConfig](https://github.com/fish-five/BGWiFiConfig)

一个为esp8266和esp32设计的简单易用Arduino配网库，可通过api配置WiFi网络，支持GET和POST请求配置；可通过微信小程序配网(小程序名：配网库)；可通过默认网页配置WiFi网络；可自定义配网页面；可通过OTA网页升级程序固件

[blinker](https://github.com/blinker-iot/blinker-library) (点灯科技)
