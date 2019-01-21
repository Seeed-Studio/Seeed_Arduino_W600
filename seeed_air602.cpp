/*
 * seeed_air602.cpp
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

#include "seeed_air602.h"


static String RSP_OK = "+OK\r\n";
static String RSP_OK_equal = "+OK=";
static const String AT_enter            = "\r\n";

#if defined(SAMD21)
void AtWifi::begin(HardwareSerial &uart,uint32_t baud)
{
    ATSerial::begin(uart,baud);
}
#else
void AtWifi::begin(SoftwareSerial &uart,uint32_t baud)
{
    ATSerial::begin(uart,baud);
}
#endif
bool AtWifi::scan(String &msg)
{
    bool ret = false;
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+WSCAN") + AT_enter;
    ret = ATSerial::sendCmdAndGetMsg(msg,cmd_,RSP_OK_equal,RSP_OK_equal.length());
    if(ret){
        msg.remove(msg.lastIndexOf('\n'),msg.length());
    }
    return ret;
}



bool AtWifi::wifiSetMode(WifiMode mode)
{
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+WPRT=!") + mode + AT_enter;
    return ATSerial::sendCmdAndCheckRsp(cmd_,RSP_OK,RSP_OK.length());
}

bool AtWifi::wifiGetMode(String &msg)
{
    bool ret = false;
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+WPRT") + AT_enter;
    ret = ATSerial::sendCmdAndGetMsg(msg,cmd_,RSP_OK_equal,RSP_OK_equal.length());
    if(ret){
        msg.remove(0,4);
    }
    return ret;
}


bool AtWifi::wifiStaSetTargetApSsid(String &ssid)
{
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += "AT+SSID=!" + ssid + AT_enter;
    return ATSerial::sendCmdAndCheckRsp(cmd_,RSP_OK,RSP_OK.length());
}


bool AtWifi::wifiStaGetTargetApSsid(String &msg)
{
    bool ret = false;
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+SSID") + AT_enter;
    ret = ATSerial::sendCmdAndGetMsg(msg,cmd_,RSP_OK_equal,RSP_OK_equal.length());
    if(ret){
        msg.remove(0,4);
    }
    return ret;
}


bool AtWifi::wifiStaSetTargetApPswd(String &Pswd)
{
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+KEY=!") + "1,0," + Pswd + AT_enter;
    return ATSerial::sendCmdAndCheckRsp(cmd_,RSP_OK,RSP_OK.length());
}

bool AtWifi::wifiStaGetTargetApPswd(String &msg)
{
    bool ret = false;
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+KEY") + AT_enter;
    ret = ATSerial::sendCmdAndGetMsg(msg,cmd_,RSP_OK_equal,RSP_OK_equal.length());
    if(ret){
        msg.remove(0,4);
    }
    return ret;
}



bool AtWifi::wifiApSetSsid(String &ssid)
{
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+APSSID=!") + ssid + AT_enter;
    return ATSerial::sendCmdAndCheckRsp(cmd_,RSP_OK,RSP_OK.length());
}

bool AtWifi::wifiApGetSsid(String &msg)
{
    bool ret = false;
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+APSSID") + AT_enter;
    ret = ATSerial::sendCmdAndGetMsg(msg,cmd_,RSP_OK_equal,RSP_OK_equal.length());
    if(ret){
        msg.remove(0,4);
    }
    return ret;
}




bool AtWifi::wifiApSetPswd(String &Pswd,Key_format format)
{
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+APKEY=!") + format + ',' + 4 + ',' + Pswd + AT_enter;
    return ATSerial::sendCmdAndCheckRsp(cmd_,RSP_OK,RSP_OK.length());
}

bool AtWifi::wifiApGetPswd(String &msg)
{
    bool ret = false;
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+APKEY") + AT_enter;
    ret = ATSerial::sendCmdAndGetMsg(msg,cmd_,RSP_OK_equal,RSP_OK_equal.length());
    if(ret){
        msg.remove(0,4);
    }
    return ret;
}


bool AtWifi::wifiApSetEncry()
{
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+APENCRY=!") + 5 + AT_enter;
    return ATSerial::sendCmdAndCheckRsp(cmd_,RSP_OK,RSP_OK.length());
}


bool AtWifi::wifiApGetEncry(String &msg)
{
    bool ret = false;
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+APENCRY") + AT_enter;
    ret = ATSerial::sendCmdAndGetMsg(msg,cmd_,RSP_OK_equal,RSP_OK_equal.length());
    if(ret){
        msg.remove(0,4);
    }
    return ret;
}

bool AtWifi::wifiApSetIpAndMask(Get_ip_approach appr,char* ip,char* gateway,char* mask,char* dns)
{
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+APNIP=!") + appr + "," + ip + "," + gateway+ "," + mask+ "," + dns + AT_enter;
    return ATSerial::sendCmdAndCheckRsp(cmd_,RSP_OK,RSP_OK.length());
}


bool AtWifi::wifiApGetIpAndMask(String &msg)
{
    bool ret = false;
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+APNIP") + AT_enter;
    ret = ATSerial::sendCmdAndGetMsg(msg,cmd_,RSP_OK_equal,RSP_OK_equal.length());
    if(ret){
        msg.remove(0,4);
    }
    return ret;
}

bool AtWifi::wifiReset()
{
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+Z") + AT_enter;
    ATSerial::ATWrite(cmd_.c_str());
}

bool AtWifi::enterTcMode()
{
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+ENTM") + AT_enter;
    return ATSerial::sendCmdAndCheckRsp(cmd_,RSP_OK,RSP_OK.length());
}

bool AtWifi::exit_Tc_mode()
{
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("+++");
    return ATSerial::sendCmdAndCheckRsp(cmd_,RSP_OK,RSP_OK.length());
}


bool AtWifi::resetTofactorySettings()
{
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+RSTF") + AT_enter;
    return ATSerial::sendCmdAndCheckRsp(cmd_,RSP_OK,RSP_OK.length());
}

bool AtWifi::saveSettingsToFlash()
{
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+PMTF") + AT_enter;
    return ATSerial::sendCmdAndCheckRsp(cmd_,RSP_OK,RSP_OK.length());
}


bool AtWifi::getStaMac(String &msg)
{
    bool ret = false;
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+QMAC") + AT_enter;
    ret = ATSerial::sendCmdAndGetMsg(msg,cmd_,RSP_OK_equal,RSP_OK_equal.length());
    if(ret){
        msg.remove(0,4);
    }
    return ret;
}

bool AtWifi::getApMac(String &msg)
{
    bool ret = false;
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+APMAC") + AT_enter;
    ret = ATSerial::sendCmdAndGetMsg(msg,cmd_,RSP_OK_equal,RSP_OK_equal.length());
    if(ret){
        msg.remove(0,4);
    }
    return ret;
}

bool AtWifi::wifiStaGetIp(String &msg)
{
    bool ret = false;
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+NIP") + AT_enter;
    ret = ATSerial::sendCmdAndGetMsg(msg,cmd_,RSP_OK_equal,RSP_OK_equal.length());
    if(ret){
        msg.remove(0,4);
    }
    return ret;
}




bool AtWifi::wifiCreateSocket(String &msg,NetProtocol pro,NetMode mode,const char* server,uint16_t remote_port,uint16_t local_port)
{
    bool ret = false;
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+SKCT=") + pro + ',' + mode + ',' + server + ',' + remote_port + ',' + local_port + AT_enter;
    ret = ATSerial::sendCmdAndGetMsg(msg,cmd_,RSP_OK_equal,RSP_OK_equal.length(),5000);
    if(ret){
        msg.remove(0,4);
    }
    return ret;
}

bool AtWifi::wifiCreateSocket(String &msg,NetProtocol pro,NetMode mode,uint32_t time_out,uint16_t remote_port,uint16_t local_port)
{
    bool ret = false;
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+SKCT=") + pro + ',' + mode + ',' + time_out + ',' + remote_port + ',' + local_port + AT_enter;
    ret = ATSerial::sendCmdAndGetMsg(msg,cmd_,RSP_OK_equal,RSP_OK_equal.length());
    if(ret){
        msg.remove(0,4);
    }
    return ret;
}


bool AtWifi::wifiSocketSend(String &msg,int32_t socket,String &send)
{
    bool ret = false;
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+SKSND=") + socket + ',' + send.length() + AT_enter;
    ret = ATSerial::sendCmdAndGetMsg(msg,cmd_,RSP_OK_equal,RSP_OK_equal.length());
    if(ret){
        msg.remove(0,4);
    }
    cmd_.remove(0,cmd_.length());
    cmd_+=send;
    ATSerial::ATWrite(cmd_.c_str());
    return ret;
}


bool AtWifi::wifiSocketRead(String &msg,int32_t socket,uint32_t read_len)
{
    bool ret = false;
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+SKRCV=") + socket + ',' + read_len + AT_enter;
    ret = ATSerial::sendCmdAndGetMsg(msg,cmd_,RSP_OK_equal,RSP_OK_equal.length());
    if(ret){
        msg.remove(0,4);
    }
    return ret;
}



/*Whether to enable ap ssid broadcast function!*/
bool AtWifi::broadcastApSsidSet(bool flag)
{
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+BRDSSID=!")+ flag + AT_enter;
    return ATSerial::sendCmdAndCheckRsp(cmd_,RSP_OK,RSP_OK.length());
}

bool AtWifi::setBaudrate(uint32_t baud)
{
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+UART=!")+ baud + ',' + 0 + ',' + 0 + ',' + 0 + ',' + 0 + AT_enter;
    return ATSerial::sendCmdAndCheckRsp(cmd_,RSP_OK,RSP_OK.length());
}

bool AtWifi::getBaudrate(String &msg)
{
    bool ret = false;
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+UART") + AT_enter;
    ret = ATSerial::sendCmdAndGetMsg(msg,cmd_,RSP_OK_equal,RSP_OK_equal.length());
    if(ret){
        msg.remove(0,4);
    }
    return ret;
}


bool AtWifi::wifiGetOneshotInfo(String &msg)
{
    bool ret = false;
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+CUSTDATA") + AT_enter;
    ret = ATSerial::sendCmdAndGetMsg(msg,cmd_,RSP_OK_equal,RSP_OK_equal.length());
    if(ret){
        msg.remove(0,4);
    }
    return ret;
}

bool AtWifi::joinNetwork()
{
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+WJOIN") + AT_enter;
    return ATSerial::sendCmdAndCheckRsp(cmd_,RSP_OK_equal,RSP_OK_equal.length(),8000);
}

bool AtWifi::wifiStaDisconnect()
{
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+WLEAV") + AT_enter;
    return ATSerial::sendCmdAndCheckRsp(cmd_,RSP_OK,RSP_OK.length());
}

bool AtWifi::wifiApDisconnect()
{
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+WLEAV=2") + AT_enter;
    return ATSerial::sendCmdAndCheckRsp(cmd_,RSP_OK,RSP_OK.length());
}

bool AtWifi::getWifiConnectStatus(String &msg)
{
    bool ret = false;
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+LKSTT") + AT_enter;
    ret = ATSerial::sendCmdAndGetMsg(msg,cmd_,RSP_OK_equal,RSP_OK_equal.length());
    if(ret){
        msg.remove(0,4);
    }
    return ret; 
}


bool AtWifi::wifiApGetStationsInfo(String &msg)
{
    bool ret = false;
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+SLIST") + AT_enter;
    ret = ATSerial::sendCmdAndGetMsg(msg,cmd_,RSP_OK_equal,RSP_OK_equal.length());
    if(ret){
        msg.remove(0,4);
    }
    return ret; 
}

bool AtWifi::wifiSendMsgWithSocket(String &msg,int32_t socket,uint8_t *data,int data_size)
{
    bool ret = false;
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+SKSND=") + socket + ',' + data_size + AT_enter;
    ret = ATSerial::sendCmdAndGetMsg(msg,cmd_,RSP_OK_equal,RSP_OK_equal.length());
    if(ret){
        msg.remove(0,4);
    }
    for(int i=0;i<data_size;i++)
    {
        ATWrite(data[i]);
    }
    return ret; 
}



bool AtWifi::getSpecSocketInfo(String &msg,int32_t socket)
{
    bool ret = false;
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+SKSTT=") + socket + AT_enter;
    ret = ATSerial::sendCmdAndGetMsg(msg,cmd_,RSP_OK_equal,RSP_OK_equal.length());
    if(ret){
        msg.remove(0,msg.indexOf('\n')+1);
    }
    return ret; 
}


bool AtWifi::wifiCloseSpecSocket(int32_t socket)
{
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+SKCLS=") + socket + AT_enter;
    return ATSerial::sendCmdAndCheckRsp(cmd_,RSP_OK,RSP_OK.length());
}

bool AtWifi::wifiSetDefaultSocket(int32_t socket)
{
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+SKSDF=") + socket + AT_enter;
    return ATSerial::sendCmdAndCheckRsp(cmd_,RSP_OK,RSP_OK.length());
}


bool AtWifi::setWifiConfigMode(WifiConfig mode)
{
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+ONEMODE=!") + mode + AT_enter;
    return ATSerial::sendCmdAndCheckRsp(cmd_,RSP_OK,RSP_OK.length());
}

bool AtWifi::setWifiConfigEnable(bool flag)
{
    ATSerial::flush();
    cmd_.remove(0,cmd_.length());
    cmd_ += String("AT+ONESHOT=!") + flag + AT_enter;
    return ATSerial::sendCmdAndCheckRsp(cmd_,RSP_OK,RSP_OK.length());
}

// bool AtWifi::wifiPing(const char *dst,int interval_time,int count,bool operate_flag)
// {
//     ATSerial::flush();
//     cmd_.remove(0,cmd_.length());
//     cmd_ += String("AT+PING=") + dst + ',' + interval_time+ ',' + count + ',' + operate_flag + AT_enter;
//     return ATSerial::sendCmdAndCheckRsp(cmd_,RSP_OK,RSP_OK.length());
// }

// bool AtWifi::wifiHttpService()
// {

// }


bool AtWifi::sendBinaryMsg(uint8_t *msg,uint32_t msg_len)
{
    ATSerial::flush();
    ATSerial::binWrite(msg,msg_len);
}


bool AtWifi::recvData(uint8_t *recv_msg,uint32_t &len)
{
    return ATSerial::waitForData(recv_msg,len,5000);
}


// bool AtWifi::wifiSetCmd(String &cmd)
// {
//     cmd_.remove(0,cmd_.length());
//     return ATSerial::sendCmdAndCheckRsp(cmd_,RSP_OK,RSP_OK.length());
// }


// bool AtWifi::wifiGetCmdMsg(String &msg,String &cmd)
// {
//     bool ret = false;
//     cmd_.remove(0,cmd_.length());
//     ret = ATSerial::sendCmdAndGetMsg(msg,cmd,RSP_OK_equal,RSP_OK_equal.length());
//     if(ret){
//         msg.remove(0,4);
//     }
//     return ret;
// }

