# Seeed grove air602 UART wifi

## Introduction
The Air602 is a 2.4G Wi-Fi module based on the embedded Wi-Fi SoC chip (W600) with a  12-pin Stamp Hole Expansion Interface. 

The air602 UART wifi module receive AT command,So arduino board can control it via UART port.  

## Notice
* Since atmega328(like arduino UNO) has only one serial port,The library use hardware serial port to output log info,and control the Air602 via softserial(2,3)，So the grove cable should attach to (D2,D3) port.  

* When using the samd board(like arduino zero,seeeduino cortex M0+),the SerialUSB for printing log info.The Serial(defined in library)for send AT command.  

* By default, The arduino board outputs the instruction info while sending AT command to air602 wifi module,user can close it in file:ATSerial.h,change the DEBUG_EN from 1 to 0.

***
This software is written by downey  for seeed studio<br>
Email:dao.huang@seeed.cc
and is licensed under [The MIT License](http://opensource.org/licenses/mit-license.php). Check License.txt for more information.<br>

Contributing to this software is warmly welcomed. You can do this basically by<br>
[forking](https://help.github.com/articles/fork-a-repo), committing modifications and then [pulling requests](https://help.github.com/articles/using-pull-requests) (follow the links above<br>
for operating guide). Adding change log and your contact into file header is encouraged.<br>
Thanks for your contribution.

Seeed Studio is an open hardware facilitation company based in Shenzhen, China. <br>
Benefiting from local manufacture power and convenient global logistic system, <br>
we integrate resources to serve new era of innovation. Seeed also works with <br>
global distributors and partners to push open hardware movement.<br>



