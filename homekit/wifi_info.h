/*
 * @Author: CuiYao
 * @Date: 2022-10-27 09:46:00
 * @LastEditors: CuiYao
 * @LastEditTime: 2022-10-27 15:56:07
 * @FilePath: /esp-outlet/blinker/wifi_info.h
 */

String Html = String("<!DOCTYPE html><html><head><title>配网页面</title><meta charset=\"UTF-8\"><meta name=\"viewport\"content=\"width=device-width, initial-scale=1\"><style>*{box-sizing:border-box}body{font-family:Arial;margin:0}.header{height:100vh;padding:80px;text-align:center;background:#1a83bc;color:white}label{display:inline-block;width:80px;text-align:justify;text-align-last:justify}.button{background-color:#1a83bc;border:2px solid#FFFFFF;height:50px;width:200px;justify-content:center;align-items:center;font-size:23px;border-radius:8px;color:#FFFFFF}.header h1{font-size:40px}</style></head><body><div class=\"header\"><h1>欢迎使用智能插座</h1><form name=\"my\"action=\"htmlresult\"method=\"POST\"><p><label>WiFi名称：</label><input type=\"text\"name=\"ssid\"placeholder=\"请输入您WiFi的名称\"id=\"aa\"><br><p><label>WiFi密码：</label><input type=\"text\"name=\"pwd\"placeholder=\"请输入您WiFi的密码\"id=\"bb\"><br><p><input class=\"button\"type=\"submit\"value=\"开始配网\"></form><a href=\"https://github.com/CuiYao631\">小崔搞科技出品</a></div></body></html>");

String Htmlret = String("<!DOCTYPE html><html><head><title>配网页面</title><meta charset=\"UTF-8\"><style>.header{height:100vh;")
                 + String("padding-top:50vh;padding-left:100px;padding-right:100px;text-align:center;background:#1a83bc;color:white}.header ")
                 + String("h1{font-size:40px}</style></head><body><div class=\"header\"><h1>配置成功，请重启，等待开发板连接WiFi。</h1><a ")
                 + String("href=\"https://github.com/CuiYao631\">小崔搞科技出品</a></div></body></html>");