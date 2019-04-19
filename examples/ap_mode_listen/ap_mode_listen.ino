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
#include "seeed_air602.h"


/**Example desc:
 *      Create a AP which named ${AP_SSID},IP is "192.168.88.1",if create succeed,
 *      print sta information which connected .And then listening at port ${SERVER_PORT}.
 * 
 *      
 * */


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

String AP_SSID = "Seeedstudio";
String AP_PSWD = "1234567890";
int32_t socket = -1;
int32_t client_socket[5]={0};


static const uint16_t SERVER_PORT = 8888; 




void setup()
{
    debug.begin(115200);
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
    delay(500);

    wifi.setWifiStatus(AP_MODE_SET_OK);

    if(!wifi.wifiCreateSocket(msg,TCP,Server,60000,0,SERVER_PORT))
    {
        debug.println("Listen failed!!");
        return;
    }
    else
    {
        socket = msg[0] - 0x30;
    }

    debug.println("Wifi set ap mode ok!");
    debug.print("SSID = ");
    debug.println(AP_SSID);
    debug.print("PSWD = ");
    debug.println(AP_PSWD);

    debug.println("Server listen at addr :192.168.88.1,port :  8888");

}

String socket_read_msg;
void loop()
{
    if( wifi.getWifiStatus() > 0 )
    {
        if(wifi.wifiApGetStationsInfo(msg))
        {
            debug.println("Stations info :[sta num],[sta1 mac],[sta1_ip],[sta2_mac],[sta2_ip]...");
            debug.println(msg);
        }

        if(wifi.getSpecSocketInfo(msg,socket))
        {
            int i = 0;
            debug.println("Sockets info : [socket],[status],[host],[host port],[local port],[rxdata]");
            debug.println(msg);
            while((msg[0] > '0') && (msg[0] <= '9') )
            {
                client_socket[i] = msg[0] - 0x30;
                if(wifi.wifiSocketRead(socket_read_msg,client_socket[i],(uint32_t)10))
                {
                    if(socket_read_msg[0] != '0')
                    {
                        debug.println("************************************************************");
                        debug.print("Read from remote station= data len , data content = ");
                        debug.println(socket_read_msg);
                        debug.println("************************************************************");
                        debug.println("  ");
                        wifi.wifiSocketSend(msg,client_socket[i],socket_read_msg);
                    }
                }
                msg.remove(0,msg.indexOf('\n'));
                i++;
            }
        }
    }
    else
    {
        debug.println("AP mode init failed");
    }
    
    delay(1000);
}