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

String SSID = "SEEED-MKT";
String PSWD = "depot0510";

AtWifi wifi;

/**Notice:Because of limitation of RAM,The scan function will not display all the result when there are too many APs.
 *        The max len is UART_MAX_LEN.
 * */

//uint8_t msg[256];
String msg;
void setup()
{
    debug.begin(115200);
    // Serial.begin(115200);
    #if defined(SAMD21)
        wifi.begin(Serial,9600);
    #else
        wifi.begin(softSerial,9600);
    #endif
    // debug.println("hello world!!!");

    if(!wifi.wifiSetMode(STA))
    {
        debug.println("Set config mode failed!");
        return ;
    }
    delay(100);
    if(!wifi.scan(msg))
    {
        debug.println("Set config mode failed!");
        return ;
    }

    debug.print("scan = \r\n");
    debug.println(msg);
}


void loop()
{
    
}