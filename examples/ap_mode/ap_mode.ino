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


#include "seeed_w600.h"



/**Example desc:
 *      Create a AP which named ${AP_SSID},if create succeed,
 *      print sta information which connected .
 * */

String msg;
AtWifi wifi;


String AP_SSID = "Seeedstudio";
String AP_PSWD = "1234567890";



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

void setup()
{
    debug.begin(115200);

    // wifi.begin(Serial);
    #if defined(SAMD21)
        wifi.begin(Serial,9600);
    #else
        wifi.begin(softSerial,9600);
    #endif
    
    wifi.wifiReset();
    delay(1000);
    
    if(!wifi.wifiSetMode(AP))
    {
        debug.println("Set config mode failed!");
        return ;
    }
    delay(100);
    if(!wifi.wifiApSetSsid(AP_SSID)){
        debug.println("Set target AP ssid failed!!");
        return;
    }
    delay(100);
    if(!wifi.wifiApSetPswd(AP_PSWD,FORMAT_ASCII)){
        debug.println("Set target AP password failed!!");
        return;
    }
    delay(100);
    if(!wifi.wifiApSetEncry()){
        debug.println("Set target AP password failed!!");
        return;
    }
    delay(100);

    if(!wifi.wifiApSetIpAndMask(STATIC,"\"192.168.88.1\"","\"255.255.255.0\"","\"192.168.88.1\"","\"114.114.114.114\"")){
        debug.println("Set target AP password failed!!");
        return;
    }
    delay(100);

    if(!wifi.joinNetwork())
    {
        debug.println("Join to AP network failed!!");
        return;
    }
    wifi.setWifiStatus(AP_MODE_SET_OK);

    /*There is a huge difference between samd21-board(arduino zero、this board or other board based on samd21) 
    and mega328-board(arduino uno or arduino avr board),is that : When you use a avr board,you can simplely 
    reset the board by reopen serial monitor.
    But for samd board,it can't! In addition，The samd board uses different serial ports for bootloader and 
    application,This characteristic leads that :If user open the board's application serial monitor,
    when user reset the module,it runs bootloader at first,causing in OS-system can't find the serial port which user opened,
    So that the port will be closed.
    and then jump to application,the serial port changes too,user shoult reopen the serial monitor manually. 
    */
    debug.println("Wifi set ap mode ok!");
    debug.print("SSID = ");
    debug.println(AP_SSID);
    debug.print("PSWD = ");
    debug.println(AP_PSWD);
}


void loop()
{
    if(wifi.getWifiStatus() > 0 )
    {
        //Scanning the stations which connect to this ap periodly and print their info.
        if(wifi.wifiApGetStationsInfo(msg))
        {
            debug.println("Stations info :[sta num],[sta1 mac],[sta1_ip],[sta2_mac],[sta2_ip]...");
            debug.println(msg);
        }
    }
    
    delay(2000);
}