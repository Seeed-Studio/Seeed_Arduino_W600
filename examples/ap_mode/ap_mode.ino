#include "seeed_air602.h"

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
#pragma message("Not found any architecture.")
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

    debug.println("Wifi set ap mode ok!");
    debug.print("SSID = ");
    debug.println(AP_SSID);
    debug.print("PSWD = ");
    debug.println(AP_PSWD);
}


void loop()
{
    //Scanning the stations which connect to this ap periodly and print their info.
    if(wifi.wifiApGetStationsInfo(msg))
    {
        debug.println("Stations info :[sta num],[sta1 mac],[sta1_ip],[sta2_mac],[sta2_ip]...");
        debug.println(msg);
    }
    delay(2000);
}