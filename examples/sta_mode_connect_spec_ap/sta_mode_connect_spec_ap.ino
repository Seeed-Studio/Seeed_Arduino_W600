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

String SSID = "STU-EE";
String PSWD = "stu-ee-2018";

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

    debug.println("Connect to target wifi : ");
    debug.print("Target AP SSID = ");
    debug.println(SSID);
    debug.print("Target AP PSWD = ");
    debug.println(PSWD);
    debug.println("OK!");
}


void loop()
{
    delay(1000);
    if(wifi.getWifiConnectStatus(msg))
    {
        debug.println("My IP = [status],[ip],[netmask],[gateway],[dns] : ");
        debug.println(msg);
        while(1);
    }
    
}