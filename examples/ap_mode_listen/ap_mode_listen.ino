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
    if(wifi.wifiApGetStationsInfo(msg))
    {
        debug.print("Stations info : ");
        debug.println(msg);
    }

    if(wifi.getSpecSocketInfo(msg,socket))
    {
        int i = 0;
        debug.print("Sockets info : ");
        debug.println(msg);
        while((msg[0] > '0') && (msg[0] <= '9') )
        {
            client_socket[i] = msg[0] - 0x30;
            if(wifi.wifiSocketRead(socket_read_msg,client_socket[i],(uint32_t)10))
            {
                if(socket_read_msg[0] != '0')
                {
                    debug.print("Read from remote server========================================= data = ");
                    debug.println(socket_read_msg);
                    debug.println(" ");
                    debug.println(" ");
                }
            }
            msg.remove(0,msg.indexOf('\n'));
            i++;
        }
    }

    delay(2000);
}