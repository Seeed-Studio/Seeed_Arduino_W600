/*
 * ATSerial.cpp
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

#include "ATSerial.h"

/**
 * Uses default parameters.
 */
ATSerial::ATSerial() {
  
}
#if defined(SAMD21)
void ATSerial::begin(HardwareSerial &uart,uint32_t baud)
{
  _uart = &uart;
  _uart->begin(baud);
}
#else
void ATSerial::begin(SoftwareSerial &uart,uint32_t baud)
{
  _uart = &uart;
  _uart->begin(baud);
}
#endif


// ATSerial::ATSerial(uint16_t rx, uint16_t tx, long baud) {
//   _uart = new SoftwareSerial(rx, tx);
//   _uart->begin(baud);
// }

/**
 * Clears the software serial buffer.
 */
void ATSerial::flush(void) {
  while (0 < _uart->read()){}
}


/*
bool ATSerial::readResp(String &rsp,uint32_t time_out)
{
  int i = 0;
  int ch = 0;
  uint32_t milli = millis();
  while(!_uart->available()){
     if((millis() - milli) > time_out){
		 return false;
	 }
  }
  delayMicroseconds(5000);
  while(_uart->available())
  {
    rsp += (char)_uart->read();
    if(rsp.length() >= UART_MAX_LEN)
      break;
    delayMicroseconds(2000);
  }
  flush();
  return true;
}
*/

bool ATSerial::readResp(String &rsp,uint32_t time_out)
{
  int i = 0;
  int ch = 0;
  int cnt = 0;
  uint32_t milli = millis();
  while(!_uart->available()){
     if((millis() - milli) > time_out){
		 return false;
	 }
  }
  delayMicroseconds(5000);
  while(1)
  {
    cnt = 0;
    rsp += (char)_uart->read();
    if(rsp.length() >= UART_MAX_LEN)
      break;
    while(!_uart->available())
    {
      delayMicroseconds(200);
      cnt++;
      if(cnt >= 10)
        break;
    }
    if(!_uart->available())
      break;
  }
  flush();
  return true;
}



/**
 * Checks for the substring in the response received from the device.
 * @param s the substring to be checked
 * @param match_len The substring's length to be check.
 * @param time_out If the wifi module had not rsp within time_out(ms),return false
 * @return true if the provided substring is found in the response, else false
 */
bool ATSerial::checkResponse(String& s,uint16_t match_len,uint32_t time_out) 
{
  bool ret = false;
  String rsp;

  ret = readResp(rsp,time_out);
  if(!ret){
	  return false;
  }

  #if DEBUG_EN
  debug.print("Check Read <===: ");
  debug.println(rsp);
  #endif

  for(int i=0;i<match_len;i++)
  {
    if((i != match_len) && (rsp[i] != s[i]))
    {
      return false;
    }
  }
  return true;
}

bool ATSerial::getMsg(String& msg,uint32_t time_out)
{
  bool ret = false;
  //String rsp;

  ret = readResp(msg,time_out);
  if(!ret){
	  return false;
  }

  #if DEBUG_EN
  debug.print("get Read <===: ");
  debug.println(msg);
  #endif

  return true;
}


/** Check the resp of the AT-CMD,and get the msg returned from wifi module.
 *  @param s The substring to be checked.
 *  @param match_len The substring's length to be check.
 *  @param time_out If the wifi module had not rsp within time_out(ms),return false
 *  @param msg The msg reveived from wifi module.
 *  @return  true if the provided substring is found in the response, else false
 * */
bool ATSerial::checkResponseAndGetMessage(String& s,uint16_t match_len,uint32_t time_out,String& msg) 
{
  bool ret = false;
  String rsp;

  ret = readResp(rsp,time_out);
  if(!ret){
	  return false;
  }

  #if DEBUG_EN
  debug.print("Read <===: ");
  debug.println(rsp);
  #endif

  for(int i=0;i<match_len;i++)
  {
    if((i != match_len) && (rsp[i] != s[i]))
    {
      return false;
    }
  }
  msg = rsp;
  return true;
}





int ATSerial::read(void) {
  return _uart->read();
}

/**
 * Writes a string to the device.
 *
 * @param s the string to be written
 * @return the number of bytes written
 */
int ATSerial::ATWrite(const char* s) {
  
  #if DEBUG_EN
  debug.print("Write ===>: ");
  debug.println(s);
  #endif

  return _uart->write(s);
}

int ATSerial::ATWrite(const uint8_t uc) {
  
  return _uart->write(uc);
}


void ATSerial::binWrite(uint8_t *msg,uint32_t len)
{
  for(int i=0;i<len;i++)
  {
    _uart->write(msg[i]);
  }
}




bool ATSerial::sendCmdAndCheckRsp(String &cmd,String &match,uint16_t match_len,uint32_t time_out,uint8_t max_resend_cnt)
{
	bool ret = false;
	uint8_t resend_cnt = 0;
	ATWrite(cmd.c_str());
	ret = checkResponse(match,match_len,time_out);
	while(!ret){
		resend_cnt++;
		if(resend_cnt > max_resend_cnt){
			return false;
		}
		ATWrite(cmd.c_str());
		ret = checkResponse(match,match_len,time_out);
	}
	return true;
}


bool ATSerial::sendCmdAndGetMsg(String &msg,String &cmd,String &match,uint16_t match_len,uint32_t time_out,uint8_t max_resend_cnt)
{
  bool ret = false;
	uint8_t resend_cnt = 0;
  ATWrite(cmd.c_str());
  ret = checkResponseAndGetMessage(match,match_len,time_out,msg);

  //debug.print("msg  =  ");
  //debug.println(msg);

  while(!ret){
		resend_cnt++;
		if(resend_cnt > max_resend_cnt){
			return false;
		}
		ATWrite(cmd.c_str());
		ret = checkResponseAndGetMessage(match,match_len,time_out,msg);
	}
  return true;
}

bool ATSerial::waitForData(uint8_t *recv_msg,uint32_t &recv_len,uint32_t time_out,uint8_t max_resend_cnt)
{
  int i = 0;
  int ch = 0;
  uint32_t milli = millis();
  while(!_uart->available()){
     if((millis() - milli) > time_out){

		 return false;
	 }
  }
  delayMicroseconds(1000);
  while(_uart->available())
  {
    recv_msg[recv_len] = (uint8_t)_uart->read();
    recv_len++;
    if(recv_len >= UART_MAX_LEN)
      break;
    delayMicroseconds(1000);
  }
  flush();
  return true;
}



