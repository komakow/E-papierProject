/*
 *@defGPIO.h
 *
 *@Created on: 12.04.2018
 *@Author: KamilM
 *
 *@brief Header file with define all of used GPIO pins
 */

#ifndef DEFGPIO_H_
#define DEFGPIO_H_

#include <GPIODriver.h>


int GPIO_InitializeAll();

//miso pin
#define Miso_port   PORTA
#define Miso_pin    PIN6
GPIOcfgType miso;

//mosi pin
#define Mosi_port   PORTA
#define Mosi_pin    PIN7
GPIOcfgType mosi;

//clk pin
#define Clk_port   PORTA
#define Clk_pin    PIN5
GPIOcfgType clk;

//nss pin
#define Nss_port   PORTA
#define Nss_pin    PIN4
GPIOcfgType nss;

//DC pin
#define DC_port   PORTE
#define DC_pin    PIN11
GPIOcfgType dc;

//RST pin
#define RST_port   PORTE
#define RST_pin    PIN12
GPIOcfgType RST;

//BUSY pin
#define BUSY_port   PORTE
#define BUSY_pin    PIN13
GPIOcfgType BUSY;

//Red diode
#define DiodeRed_port       PORTD
#define DiodeRed_pin        PIN14
GPIOcfgType DiodeRed;

//Orange diode
#define DiodeOrange_port    PORTD
#define DiodeOrange_pin     PIN13
GPIOcfgType DiodeOrange;
#endif /* DEFGPIO_H_ */
