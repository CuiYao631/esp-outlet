/*
 * @Author: CuiYao
 * @Date: 2022-10-27 09:46:00
 * @LastEditors: CuiYao
 * @LastEditTime: 2022-10-27 15:56:07
 * @FilePath: /esp-outlet/blinker/wifi_info.h
 */

String Html = String("<!DOCTYPE html><html><head><title>配网页面</title><meta charset=\"UTF-8\"><meta name=\"viewport\"content=\"width=device-width, initial-scale=1\">")
              + String("<style>*{box-sizing:border-box}body{font-family:Arial;margin:0}.header{padding:80px;text-align:center;background:#1a83bc;color:white}label{display:inline-block;width:80px;text-align:justify;text-align-last:justify;margin-right:10px}.button{background-color:#3CA0D3;border:2px solid#FFFFFF;height:50px;width:200px;justify-content:center;align-items:center;font-size:23px;border-radius:8px;color:#FFFFFF}.header h1{font-size:40px}@media screen and(max-width:700px){.row{flex-direction:column}}@media screen and(max-width:400px){.navbar a{float:none;width:100%}}</style>")
              + String("</head><body><div class=\"header\"><h1>欢迎使用智能插座模块</h1>")
              + String("<form name=\"my\"action=\"htmlresult\"method=\"POST\">")
              + String("<p><label>WiFi名称：</label><input type=\"text\"name=\"ssid\"placeholder=\"请输入您WiFi的名称\"id=\"aa\"><br>")
              + String("<p><label>WiFi密码：</label><input type=\"text\"name=\"pwd\"placeholder=\"请输入您WiFi的密码\"id=\"bb\"><br>")
              + String("<p><label>auth:</label><input type=\"text\"name=\"umsg1\"placeholder=\"请输入您的auth\"id=\"bb\"><br>")
              + String("<p><input class=\"button\"type=\"submit\"value=\"保存\"></form>")
              + String("<p>小崔搞科技出品</p></div></body></html>");

String Htmlret = String("<!DOCTYPE html><html><head><title>配网页面</title><meta charset=\"UTF-8\">")
                 + String("<style>.header{padding:100px;text-align:center;background:#1a83bc;color:white}.header h1{font-size:40px}</style>")
                 + String("</head><body><div class=\"header\"><h1>配置成功，请重启，等待开发板连接WiFi。</h1><p>小崔搞科技出品</p></div></body></html>");