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
    if(!wifi.setWifiConfigMode(ONE_SHOT))
    {
        debug.println("Set wifi config mode failed!");
        return ;
    }
    delay(100);
    if(!wifi.setWifiConfigEnable(true))
    {
        debug.println("Enter config mode failed!");
        return ;
    }
    
    delay(100);
    debug.println("Enter wifi mode OK!!!,Please use pyhon app to config it.");
}


void loop()
{
    delay(3000);
    if(wifi.getWifiConnectStatus(msg))
    {
        if('1' == msg[0])
        {
            delay(1000);
            wifi.getWifiConnectStatus(msg);
            debug.println("Wifi config successed!! info : [status],[ip],[netmask],[gateway],[dns]: ");
            debug.println(msg);
            while(1);
        }
    }
    
}