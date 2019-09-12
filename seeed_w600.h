/*
 * seeed_w600.h
 *
 * Copyright (c) 2018 seeed technology inc.
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

#ifndef _SERIAL_STA_H
#define _SERIAL_STA_H


#include <Arduino.h>
#include "ATSerial.h"
#include "SoftwareSerial.h"



#define DEBUG_EN 1


enum WifiMode{
        STA = 0,
        AP = 2,
        STA_AP = 3,
    };

typedef enum
{
    OPEN,
    WEP64,
    WEP128,
    WPA_PSK_TKIP,
}Encry_mode;

typedef enum{
    FORMAT_HEX,
    FORMAT_ASCII,
}Key_format;

typedef enum
{
    DHCP,
    STATIC
}Get_ip_approach;

typedef enum
{
    TCP,
    UDP,
}NetProtocol;

typedef enum
{
    Client,
    Server,
}NetMode;

typedef enum
{
    ONE_SHOT,
    SOFT_AP,
    SOFT_AP_WEB,
}WifiConfig;

typedef enum
{
    DISCONNECT = -1,           //Indicate that wifi module does not connect in sta mode or does not configure ok in AP mode. 
    AP_MODE_SET_OK = 1,            //Indicate that wifi module runs in AP mode and configure ok! 
    STA_MODE_CONNECTED,        //Indicate that wifi module runs in STA mode and had connected a ap already.
    AP_STA_MODE_CONNECT_OK,    //Indicate that wifi module runs in STA&AP mode,and,AP configure ok & STA had connect to specifed ap.
}WifiStatus;

class AtWifi:public ATSerial
{
    public:
        AtWifi(){wifi_status_ = DISCONNECT;};
        inline void setWifiStatus(WifiStatus status){
            wifi_status_ = status;
        }
        inline WifiStatus getWifiStatus(void){
            return wifi_status_;
        }

        #if defined(SAMD21)
        void begin(HardwareSerial &uart,uint32_t baud = DEFAULT_BAUD);
        #else
        void begin(SoftwareSerial &uart,uint32_t baud = DEFAULT_BAUD);
        #endif
        
        //Set wifi mode:1.station mode ,2.AP mode 3.station&AP mode. 
        bool wifiSetMode(WifiMode mode);
         
        //scan the nearby Aps' infomation,because of limitation of RAM memory,it only shows less than 512 bytes.
        bool scan(String &msg);

        //Set target Ap ssid,Module work in STA or STA&AP mode.
        bool wifiStaSetTargetApSsid(String &ssid);
        //Set target Ap password,Module work in STA or STA&AP mode.
        bool wifiStaSetTargetApPswd(String &Pswd);
        //Set Ap password,Module work in AP mode.
        bool wifiApSetPswd(String &Pswd,Key_format format);
        //Set Ap ssid,Module work in AP mode.
        bool wifiApSetSsid(String &ssid);
        /** Set encryption mode
         *  0 - OPEN
         *  1 - WEP64
         *  2 - WEP128
         *  3 - WPA-PSK(TKIP)
         *  4 - WPA-PSK(CCMP/AES)
         *  5 - WPA2-PSK(TKIP)
         *  6 - WPA2-PSK(CCMP/AES)
         *  We use the 5 - WPA2-PSK(TKIP) hear;
         * */
        bool wifiApSetEncry();
        /**Set network infomation for module when it works in AP mode.
         * appr:The way that client get ip addr:DHCP or static.
         * ip:ip addr
         * gateway:gateway ip addr
         * mask:network mask
         * dns:dns ip addr.
         * */
        bool wifiApSetIpAndMask(Get_ip_approach appr,char* ip,char* gateway,char* mask,char* dns);

        bool wifiApGetEncry(String &msg);
        
        bool wifiGetMode(String &msg);
        
        bool wifiStaGetTargetApSsid(String &msg);
        
        bool wifiApGetIpAndMask(String &msg);
        
        bool wifiApGetSsid(String &msg);
        
        bool wifiApGetPswd(String &msg);
        
        bool wifiStaGetTargetApPswd(String &msg);
        //Reset the module.
        bool wifiReset();
        //Enter transparent transmission mode.before that,The module must has connected a server,and set a default socket.
        bool enterTcMode();

        bool resetTofactorySettings();

        bool saveSettingsToFlash();

        bool getStaMac(String &msg);

        bool getApMac(String &msg);

        bool wifiStaGetIp(String &msg);
        /**Create a socket connection in STA mode.
         * msg - The socket number that module return.
         * pro - network protocol,TCP or UDP.
         * mode - client or server.
         * server - server ip addr.
         * remote_port
         * local_port
         * */
        bool wifiCreateSocket(String &msg,NetProtocol pro,NetMode mode,const char* server,uint16_t remote_port,uint16_t local_port);
        /**Create a socket binding in AP mode.
         * msg - The socket number that module return.
         * pro - network protocol,TCP or UDP.
         * mode - client or server.
         * time_out - time out for client,when there is not a communication between server and client for a long time,abort connection.
         * remote_port
         * local_port
         * */
        bool wifiCreateSocket(String &msg,NetProtocol pro,NetMode mode,uint32_t time_out,uint16_t remote_port,uint16_t local_port);
        /**Enable broadcast ssid or not,module works in AP mode.
         * 
         * */
        bool broadcastApSsidSet(bool flag);
        
        bool wifiGetOneshotInfo(String &msg);
        // Join to network,when module works in AP mode,it must has been set the ssid&pswd,encryption,ip,gateway,mask.when it works in sta mode,
        // it must has been set target ssid&pswd.
        bool joinNetwork();

        bool wifiStaDisconnect();

        bool wifiApDisconnect();

        bool getWifiConnectStatus(String &msg);

        bool wifiApGetStationsInfo(String &msg);
        /**Wifi send msg.
         * msg - the msg that module return.
         * socket - socket number.
         * data - data to send.
         * data_size
         * */
        bool wifiSendMsgWithSocket(String &msg,int32_t socket,uint8_t *data,int data_size);
        //Close socket.
        bool wifiCloseSpecSocket(int32_t socket);
        //Set default socket to communication.
        bool wifiSetDefaultSocket(int32_t socket);

        // bool wifiPing(const char *dst,int interval_time,int count,bool operate_flag);
        
        // bool wifiHttpService();
        /**Enable oneshot mode or not.
         * Oneshot mode is to set the module in sta mode,Use the phone app to configure the module to connect to ap.
         * */
        bool setWifiConfigEnable(bool flag);
        
        /**Set config mode.
         * 0 - oneshot
         * 1 soft ap configuration
         * 2 soft ap web configuration.
         * */
        bool setWifiConfigMode(WifiConfig mode);


        bool getBaudrate(String &msg);
        
        bool setBaudrate(uint32_t baud);
        // Exit transparen transmission mode.
        bool exit_Tc_mode();

        bool sendBinaryMsg(uint8_t *msg,uint32_t msg_len);

        bool wifiSetCmd(String &cmd);
        bool wifiGetCmdMsg(String &msg,String &cmd);

        bool recvData(uint8_t *recv_msg,uint32_t &len);
        /**Send msg from socket.
         * msg - the msg that module return by AT CMD.
         * socket - socket number.
         * send - the string msg to send.
         * */
        bool wifiSocketSend(String &msg,int32_t socket,String &send);
        /**Recv msg from socket.
         * msg - the msg recved from socket.
         * scoket - socket number
         * read_len - recv len.
         * */
        bool wifiSocketRead(String &msg,int32_t socket,uint32_t read_len);

        bool getSpecSocketInfo(String &msg,int32_t socket);


    private:


        WifiStatus wifi_status_;
        String cmd_;

};

#endif