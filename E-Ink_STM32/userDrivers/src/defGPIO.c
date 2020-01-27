/*
 *@defGPIO.c
 *
 *@Created on: 12.04.2018
 *@Author: KamilM
 *
 *@brief Source code for config all of GPIO ports and pins.
 */

#include "GPIODriver.h"
#include "defGPIO.h"
#include "stm32f4xx.h"

int GPIO_InitializeAll()
{

	int ret=0;

	miso.mode = alternate;
	miso.pin = Miso_pin;
	miso.port = Miso_port;
	miso.pull = pullUp;
	miso.typ = pushPull;
	miso.speed = high;
	miso.alter = AF5;
	if (gpioCfg(&miso) != 0) 			//if status of init GPIO is 'error'
	{
		ret = -1;
	}

	mosi.mode = alternate;
	mosi.pin = Mosi_pin;
	mosi.port = Mosi_port;
	mosi.pull = pullUp;
	mosi.typ = pushPull;
	mosi.speed = high;
	mosi.alter = AF5;
	if (gpioCfg(&mosi) != 0) 			//if status of init GPIO is 'error'
	{
		ret = -2;
	}

	clk.mode = alternate;
	clk.pin = Clk_pin;
	clk.port = Clk_port;
	clk.pull = pullUp;
	clk.typ = pushPull;
	clk.speed = high;
	clk.alter = AF5;
	if (gpioCfg(&clk) != 0) 			//if status of init GPIO is 'error'
	{
		ret = -3;
	}

	nss.mode = output;
	nss.pin = Nss_pin;
	nss.port = Nss_port;
	nss.pull = pullUp;
	nss.typ = pushPull;
	nss.speed = high;
	if (gpioCfg(&nss) != 0) 			//if status of init GPIO is 'error'
	{
		ret = -4;
	}

	dc.mode = output;
  dc.pin = DC_pin;
  dc.port = DC_port;
  dc.pull = pullUp;
  dc.typ = pushPull;
  dc.speed = high;
  if (gpioCfg(&dc) != 0)       //if status of init GPIO is 'error'
  {
    ret = -5;
  }

  RST.mode = output;
  RST.pin = RST_pin;
  RST.port = RST_port;
  RST.pull = pullUp;
  RST.typ = pushPull;
  RST.speed = high;
  if (gpioCfg(&RST) != 0)       //if status of init GPIO is 'error'
  {
    ret = -6;
  }

  BUSY.mode = input;
  BUSY.pin = BUSY_pin;
  BUSY.port = BUSY_port;
  BUSY.pull = pullUp;
  BUSY.typ = pushPull;
  RST.speed = high;
  if (gpioCfg(&BUSY) != 0)       //if status of init GPIO is 'error'
  {
    ret = -6;
  }

	DiodeRed.mode     = output;
	DiodeRed.pin      = DiodeRed_pin;
	DiodeRed.port     = DiodeRed_port;
	DiodeRed.pull     = pullDown;
	DiodeRed.typ      = pushPull;
	DiodeRed.speed    = medium;
  if(gpioCfg(&DiodeRed) != 0)       //if status of init GPIO is 'error'
  {
    ret = -7;
  }


  DiodeOrange.mode     = output;
  DiodeOrange.pin      = DiodeOrange_pin;
  DiodeOrange.port     = DiodeOrange_port;
  DiodeOrange.pull     = pullDown;
  DiodeOrange.typ      = pushPull;
  DiodeOrange.speed    = medium;
  if(gpioCfg(&DiodeOrange) != 0)       //if status of init GPIO is 'error'
  {
    ret = -8;
  }
return ret;
}
