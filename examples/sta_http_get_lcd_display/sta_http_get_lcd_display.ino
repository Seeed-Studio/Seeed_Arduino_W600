/*
 *  
 * Copyright (c) 2019 Seeed Technology Co., Ltd.
 * Website    : www.seeed.cc
 * Author     : downey
 * Create Time: Jan 2019
 * Change Log :
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <Wire.h>
#include "seeed_air602.h"
#include "rgb_lcd.h"


// Architecture specific include
#if defined(ARDUINO_ARCH_AVR)
#pragma message("Defined architecture for ARDUINO_ARCH_AVR.")
SoftwareSerial softSerial(2,3);
#define SERIAL Serial

#elif defined(ARDUINO_ARCH_SAM)
#pragma message("Defined architecture for ARDUINO_ARCH_SAM.")
#define SERIAL SerialUSB

#elif defined(ARDUINO_ARCH_SAMD)
#pragma message("Defined architecture for ARDUINO_ARCH_SAMD.")	
#define SERIAL SerialUSB

#elif defined(ARDUINO_ARCH_STM32F4)
#pragma message("Defined architecture for ARDUINO_ARCH_STM32F4.")
#define SERIAL SerialUSB
#else
SoftwareSerial softSerial(2,3);
#define SERIAL Serial
#endif

#define debug  SERIAL
String msg;
AtWifi wifi;

String SSID = "STU-EE";
String PSWD = "stu-ee-2022";

String date;
String city;
String temperature;
String humidity;
rgb_lcd lcd;

void getTargetString(String msg,String &target)
{
    for(int i=0;msg[i] != '\"';i++)
    {
        target += msg[i];
    }
}


int32_t parse_http_value(String msg)
{
    date.remove(0,date.length());
    city.remove(0,city.length());
    temperature.remove(0,temperature.length());
    humidity.remove(0,humidity.length());

    int32_t index = 0;
    index = msg.indexOf("\"days\"");
    //debug.print("index = ");
    //debug.println(index);
    if(index < 0){
        return -1;
    }
    //Get date
    msg.remove(0,index + 8);
    getTargetString(msg,date);
    //debug.println(date);

    index = msg.indexOf("\"cityno\"");
    //debug.println(index);
    if(index < 0){
        return -1;
    }
    msg.remove(0,index + 10);
    getTargetString(msg,city);
    //debug.println(city);

    index = msg.indexOf("\"temperature_curr\"");
    if(index < 0){
        return -1;
    }
    msg.remove(0,index + 20);
    getTargetString(msg,temperature);
    //debug.println(temperature);

    index = msg.indexOf("\"humidity\"");
    if(index < 0){
        return -1;
    }
    msg.remove(0,index + 12);
    getTargetString(msg,humidity);
    //debug.println(humidity);

    return 0;
    //debug.println(msg);
}


void setup()
{
    Wire.begin();
    lcd.begin(16, 2);
    

    debug.begin(115200);
     #if defined(SAMD21)
        wifi.begin(Serial,9600);
    #else
        wifi.begin(softSerial,9600);
    #endif
    
    wifi.wifiReset();
    delay(1000);

    if(!wifi.wifiSetMode(STA))
    {
        debug.println("Set config mode failed!");
        return ;
    }
    delay(100);
    if(!wifi.wifiStaSetTargetApSsid(SSID)){
        debug.println("Set target AP ssid failed!!");
        return;
    }
    delay(100);
    if(!wifi.wifiStaSetTargetApPswd(PSWD)){
        debug.println("Set target AP password failed!!");
        return;
    }
    delay(100);

    if(!wifi.joinNetwork())
    {
        debug.println("Join to AP network failed!!");
        return;
    }
    delay(2000);


    debug.println("Connect to target wifi : ");
    debug.print("Target AP SSID = ");
    debug.println(SSID);
    debug.print("Target AP PSWD = ");
    debug.println(PSWD);
    debug.println("OK!");
    

    if(!wifi.setSocketAutoReport(ENABLE))
    {
        debug.println("Set socket auto-report failed!!");
        return;
    }
    
    /*
    if(!wifi.wifiHttpService("http://api.k780.com/?app=weather.today&weaid=1&appkey=10003&sign=b59bc3ef6191eb9f747dd4e83c99f2a4&format=json",HTTP_GET,msg))
    {
        debug.println("Get http text failed!!");
    }*/
    


}

uint8_t cnt = 7;
uint8_t item = 0;

#define DATE        0
#define CITY        1
#define TEMPRATURE  2
#define HUMIDITY    3


void display_item(uint8_t item)
{
    switch(item)
    {
        case DATE:
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("date:");
            lcd.setCursor(0, 1);
            lcd.print(date.c_str());
        break;
        case CITY:
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("city:");
            lcd.setCursor(0, 1);
            lcd.print(city.c_str());
        break;
        case TEMPRATURE:
            lcd.clear();
            if(temperature.length() > 0){
                uint8_t t = (uint8_t)temperature.toInt();
                debug.println(t);   
                temperature.remove(0,temperature.length());
                temperature += t ;
                temperature += "C";
                debug.println(temperature);
            }
            
            lcd.setCursor(0, 0);
            lcd.print("temperature:");
            lcd.setCursor(0, 1);
            lcd.print(temperature.c_str());
        break;
        case HUMIDITY:
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("humidity:");
            lcd.setCursor(0, 1);
            lcd.print(humidity.c_str());
        break;
    }
}


void loop(void)
{
    
    cnt++;
    if(cnt > 6)
    {
        cnt = 0;
        if(!wifi.wifiHttpService("http://api.k780.com/?app=weather.today&weaid=1&appkey=10003&sign=b59bc3ef6191eb9f747dd4e83c99f2a4&format=json",HTTP_GET,msg))
        {
            debug.println("Get http text failed!!");
        }
        if(parse_http_value(msg) < 0 )
        {
            debug.println("Parse http text failed!!");
            lcd.setCursor(0, 0);
        }
    }
    
    {
        display_item(item);

    }
    item++;
    if(item > 3){
        item = 0;
    }
    delay(1500);
}

