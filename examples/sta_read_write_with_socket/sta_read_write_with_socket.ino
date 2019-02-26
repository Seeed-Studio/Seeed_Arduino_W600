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
#pragma message("Not found any architecture.")
#endif

#define debug  SERIAL


String msg;
AtWifi wifi;

char *TARGET_IP   = "\"192.243.112.176\"";
uint16_t TARGET_PORT = 8888;
uint16_t LOCAL_PORT  = 1234;

String SSID = "STU-EE";
String PSWD = "stu-ee-2018";

int32_t socket = -1;


void print_recv_buf(uint8_t *buf,uint8_t buf_len)
{
    debug.print("Recv data = ");
    for(int i=0;i<buf_len;i++)
    {
        debug.print(buf[i],HEX);
        debug.print(".");
    }
    debug.println(" ");
    debug.println(" ");
}


String send_msg("Seeedstudio");

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
    delay(1500);
    if(!wifi.wifiCreateSocket(msg,TCP,Client,TARGET_IP,TARGET_PORT,LOCAL_PORT))
    {
        debug.println("Connect to remote server failed!!");
        return;
    }
    delay(100);
    socket = msg[0] - 0x30;
    debug.print("socket = ");
    debug.println(socket);

    /* Test a echo server.*/
    
    if(!wifi.wifiSocketSend(msg,socket,send_msg))
    {
        debug.println("Socket send failed!!!");
        return ;
    }

    debug.println("Wifi connect target server OK,send msg . . .");
}


void loop()
{
    static uint32_t count = 0;
    delay(1000);
    if(socket >= 0 ){
        if(wifi.wifiSocketRead(msg,socket,send_msg.length()))
        {
            count++;
            if(msg[0] != '0')
            {
                debug.print("Read from remote server========================================= data = ");
                debug.println(msg);
                debug.println(" ");
                debug.println(" ");
                if(wifi.wifiCloseSpecSocket(socket)){
                    debug.println("Close socket!");
                    while(1);
                }  
            }
            if(count >= 5){
                debug.print("read data failed! ");
                wifi.wifiCloseSpecSocket(socket);
                debug.println("Close socket!");
                while(1);
            
            }
        }
    }
    
}