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
ATSerial::ATSerial(uint32_t timeout,uint8_t max_resend_cnt) {
  _timeout = timeout;
  _max_resend_cnt = max_resend_cnt;
}
#if defined(SAMD21) || defined(HAVE_HWSERIAL1)
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

/**
 * Clears the software serial buffer.
 */
void ATSerial::flush(void) {
  while (0 < _uart->read()){}
}


bool ATSerial::readResp(char* rsp)
{
  int cnt = 0;
  int len = 0;
  uint32_t milli = millis();
  while(!_uart->available()){
     if((millis() - milli) > _timeout){
		 return false;
	 }
  }
  //delayMicroseconds(5000);
  while(1) 
  {
    cnt = 0;
    *rsp = (char)_uart->read();
    rsp ++;
    len ++;

    if(len >= UART_MAX_LEN)
      break;
    
    // wait upto 2000us for another character to become available
    while(!_uart->available())
    {
      delayMicroseconds(200);
      cnt++;
      if(cnt >= 10)
        break;
    }
    if(!_uart->available())
      break;
  } // this loop exits when UART_MAX_LEN chars have been read or _uart is unavailable for more than 2000us.
  flush();
  *rsp=0; // null terminate string
  return true;
}

/**
 * @param prefix
 * @param resp_buffer char array of length UART_MAX_LEN+1 (513) to which response from device is written
 * @return whether the response begins with prefix
 */
bool ATSerial::checkResponse(const char* prefix,char* resp_buffer) 
{
  if(!readResp(resp_buffer)){
	  return false;
  }

  #if DEBUG_EN
  debug.print(F("Check Read <===: "));
  debug.println(resp_buffer);
  #endif

  return (strncmp(prefix,resp_buffer,strlen(prefix)) == 0);
}

/**
 * Send an AT command. Read the response into resp_buffer and return if the responce begins with the specified prefix
 * @param prefix, the desired reponse prefix, eg AT_OK
 * @
 */
bool ATSerial::sendCmdAndCheckRsp(const char* cmd, const char* prefix, char* resp_buffer)
{
	bool ret = false;
	uint8_t resend_cnt = 0;
	ATWrite(cmd);
	while(!checkResponse(prefix, resp_buffer)){
		resend_cnt++;
		if(resend_cnt > _max_resend_cnt){
			return false;
		}
		ATWrite(cmd);
	}
  #if DEBUG_EN
    debug.print(F("AT cmd ===>: "));
    debug.println(cmd);
    debug.print(F("RESP <===: "));
    debug.println(resp_buffer);
  #endif
	return true;
}

bool ATSerial::waitForData(uint8_t *recv_msg,uint32_t *chars_read)
{
  int recv_len = 0;
  int i = 0;
  int ch = 0;
  uint32_t milli = millis();
  while(!_uart->available()){
     if((millis() - milli) > _timeout){
     *chars_read = 0;
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
  *chars_read = recv_len;
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
  debug.print(F("Write ===>: "));
  debug.println(s);
  #endif

  return _uart->write(s);
}

/***
 * fmt : A format string stored in Flash.
 * ... : arguaments to pass to sprintf_P(fmt).
 ***/
int ATSerial::ATWritef_P(const __FlashStringHelper* fmt, ...) {
  va_list va;
  va_start(va, fmt);
  size_t len = vsnprintf_P(nullptr, 0, reinterpret_cast<const char*>(fmt), va);
  va_end(va);

  char buf[len];

  va_start(va, fmt);
  vsprintf_P(buf, reinterpret_cast<const char*>(fmt), va);
  va_end(va);

  ATWrite(buf);
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
