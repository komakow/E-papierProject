/*
 * @file spi.c
 *
 * @Created on: 18.02.2018
 * @Author: Kamil
 * @brief Source code of abstract SPI controller work at stm32F072RB
 */

//*****************************************INCLUDE***********************************

#include "stm32f4xx.h"
#include "../inc/spiDriver.h"


//***************************************STATIC FUNCTION*****************************

/*
 * SPI1
 */
static int Init_SPI1_BaudRate(SPIcfg* config)       //Set Baud Rate
{
  int ret = E_OK;
  switch(config->baudRate)
  {
    case Fdiv2:
    {
      //BR=000
      break;
    }
    case Fdiv4:
    {
      SPI1->CR1 |= SPI_CR1_BR_0;  //BR=001
      break;
    }
    case Fdiv8:
    {
      SPI1->CR1 |= SPI_CR1_BR_1;  //BR=010
      break;
    }
    case Fdiv16:
    {
      SPI1->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1;    //BR=011
      break;
    }
    case Fdiv32:
    {
      SPI1->CR1 |= SPI_CR1_BR_2;   //BR=100
      break;
    }
    case Fdiv64:
    {
      SPI1->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_2;	   //BR=101
      break;
    }
    case Fdiv128:
    {
      SPI1->CR1 |= SPI_CR1_BR_1 | SPI_CR1_BR_2;	   //BR=110
      break;
    }
    case Fdiv256:
    {
      SPI1->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2;	 //BR=111
      break;
    }
    default:
    {
      ret = E_BaudRate;  //Baud Rate error
      break;
    }
  }
  return ret;
}

static int Init_SPI1_DataSize(SPIcfg* config)      //Data length for SPI transfers
{
  int ret = E_OK;

  switch(config->size)
  {
    case DataSize8:
    {
      SPI1->CR1 &=~ (SPI_CR1_DFF);   //clear dff place in register
      break;
    }
    case DataSize16:
    {
      SPI1->CR1 |= SPI_CR1_DFF;
      break;
    }
    default:
    {
      ret = E_DataSize;	   //data size error
      break;
    }
  }
  return ret;
}

static int Init_SPI1_SlaveSlect(SPIcfg* config)	   //Hw/Sw slave management
{
  int ret = E_OK;

  switch(config->SS)
  {
    case SwNSS:
    {
      SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;	  //software slave management
      break;
    }
    case HwNSSEN:
    {
      SPI1->CR2 |= SPI_CR2_SSOE;     //hardware management,SS output enable, SPI cannot work in multi-master

      break;
    }
    case HwNSSDS:
    {
      //SSM=0, SSOE=0, multi-master mode
      break;
    }
    default:
    {
      ret = E_SSType;     //Slave Select error
      break;
    }
  }
  return ret;
}

static int Init_SPI1_PoPha(SPIcfg* config)         //Polarity and phase management
{
  int ret = E_OK;

  switch(config->polarity)                    //set the polariti of clock
  {
    case CPolLow:
    {
      //CPol=0, CK to 0 when idle
      break;
    }
    case CPolHight:
    {
      SPI1->CR1 |= SPI_CR1_CPOL;    //CPol=1, CK to 1 when idle
      break;
    }
    default:
    {
      ret = E_CPolarity;    //error Clock polarity
      break;
    }
  }

  switch(config->phase)	    //set the phase of clock
  {
    case CPhaLow:
    {
      //CPha=0, first clock transition is the first data capture edge
      break;
    }
    case CPhaHight:
    {
      SPI1->CR1 |= SPI_CR1_CPHA;    //CPha=1, second clock transition is the first data capture edge
      break;
    }
    default:
    {
      ret = E_CPhase;	   //error Clock Phase
      break;
    }
  }
  return ret;
}

static int Init_SPI1_FrameFormat(SPIcfg* config)    //Data frame format
{
  int ret = E_OK;

  switch(config->frameFormat)
  {
    case MSBfirst:
    {
      //LSBfirst=0, MSB first
      break;
    }
    case LSBfirst:
    {
      SPI1->CR1 |= SPI_CR1_LSBFIRST;    //LSBfirst=1, LSB first
      break;
    }
    default:
    {
      ret = E_FrameFormat;     //error Frame Format
      break;
    }
  }

  return ret;
}

static int Init_SPI1(SPIcfg* config)                //Init SPI_1
{
  int ret;

  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;          //clock for SPI1

  if(config->mode == 0)                        //if MCU is master
  {
    SPI1->CR1  = 0;	                           //clear register CR1
    SPI1->CR2  = 0x0700;                       //clear register CR2 but DS must not be cleared

    SPI1->CR1 |= SPI_CR1_MSTR;                 //master

    ret = Init_SPI1_BaudRate(config);          //set Baud Rate
    ret = Init_SPI1_DataSize(config);          //set Data length for SPI transfers
    ret = Init_SPI1_SlaveSlect(config);	       //set slave select mode
    ret = Init_SPI1_PoPha(config);             //set clock polarity and phase
    ret = Init_SPI1_FrameFormat(config);       //set Frame Format

    SPI1->CR2 |= SPI_CR2_RXNEIE;               //RXNE event generated if not empty
    SPI1->CR1 |= SPI_CR1_SPE;                  //start SPI
  }

  if(config->mode == 1)                        //if MCU is slave
  {
	SPI1->CR1  = 0;                           //clear register CR1
	SPI1->CR2  = 0x0700;                      //clear register CR2 but DS must not be cleared

	SPI1->CR1 &= ~SPI_CR1_MSTR;	               //slave

	ret = Init_SPI1_DataSize(config);         //set Data length for SPI transfers
	ret = Init_SPI1_SlaveSlect(config);       //set slave select mode
	ret = Init_SPI1_PoPha(config);	          //set clock polarity and phase
	ret = Init_SPI1_FrameFormat(config);      //set Frame Format

    SPI1->CR2 |= SPI_CR2_RXNEIE;              //RXNE event generated if not empty
    SPI1->CR1 |= SPI_CR1_SPE;                 //start SPI
  }

  if((config->mode != 1) && (config->mode != 0))  //if MODE is invalid
  {
    ret = E_INVALID_PARAM;
  }

  return ret;
}

static uint16_t SPI1_RW(SPIcfg* config, uint16_t send)     //send/read data from SPI1
{
  uint16_t receive;
  if ((SPI1->SR & SPI_SR_TXE) == SPI_SR_TXE)               //TX empty
  {
    if((config->size == DataSize4) || (config->size == DataSize8))
    {
      *(uint8_t *)&(SPI1->DR) = (uint8_t)send;              //send 8bit data
    }
    else
    {
      *(uint16_t *)&(SPI1->DR) = (uint16_t)send;                //send 16bit data
    }
  }

  if (((SPI1->SR & SPI_SR_RXNE) == SPI_SR_RXNE))               //RX not empty
  {
    if((config->size == DataSize4) || (config->size == DataSize8))
    {
      *(uint8_t*)&receive = *(uint8_t *)&(SPI1->DR);            //read 8bit data
    }
    else
    {
      *(uint8_t*)&receive= *(uint16_t *)&(SPI1->DR);            //read 16bit data
    }
  }
  return receive;
}

/*
 * SPI2
 */

static int Init_SPI2_BaudRate(SPIcfg* config)     //Set Baud Rate
{
  int ret = E_OK;
  switch(config->baudRate)
  {
    case Fdiv2:
    {
      //BR=000
      break;
    }
    case Fdiv4:
    {
      SPI2->CR1 |= SPI_CR1_BR_0;                      //BR=001
      break;
    }
    case Fdiv8:
    {
      SPI2->CR1 |= SPI_CR1_BR_1;                     //BR=010
      break;
    }
    case Fdiv16:
    {
      SPI2->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1;       //BR=011
      break;
    }
    case Fdiv32:
    {
      SPI2->CR1 |= SPI_CR1_BR_2;                      //BR=100
      break;
    }
    case Fdiv64:
    {
      SPI2->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_2;       //BR=101
      break;
    }
    case Fdiv128:
    {
      SPI2->CR1 |= SPI_CR1_BR_1 | SPI_CR1_BR_2;       //BR=110
      break;
    }
    case Fdiv256:
    {
      SPI2->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2;    //BR=111
      break;
    }
    default:
    {
      ret = E_BaudRate;                         //Baud Rate error
      break;
    }
  }
  return ret;
}

static int Init_SPI2_DataSize(SPIcfg* config)     //Data length for SPI transfers
{
  int ret = E_OK;

  switch(config->size)
  {
    case DataSize8:
    {
      SPI2->CR1 &=~ (SPI_CR1_DFF);                //clear DS place in register

      break;
    }
    case DataSize16:
    {
      SPI2->CR1 |=SPI_CR1_DFF;  //DS=1111
      break;
    }
    default:
    {
      ret = E_DataSize;                           //data size error
      break;
    }
  }
  return ret;
}

static int Init_SPI2_SlaveSlect(SPIcfg* config)   //Hw/Sw slave management
{
  int ret = E_OK;

  switch(config->SS)
  {
    case SwNSS:
    {
      SPI2->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;     //software slave management
      break;
    }
    case HwNSSEN:
    {
      SPI2->CR2 |= SPI_CR2_SSOE;                  //hardware management,SS output enable, SPI cannot work in multi-master

      break;
    }
    case HwNSSDS:
    {
      //SSM=0, SSOE=0, multi-master mode
      break;
    }
    default:
    {
      ret = E_SSType;                           //Slave Select error
      break;
    }
  }
  return ret;
}

static int Init_SPI2_PoPha(SPIcfg* config)        //Polarity and phase management
{
  int ret = E_OK;

  switch(config->polarity)                       //set the polarity  of clock
  {
    case CPolLow:
    {
      //CPol=0, CK to 0 when idle
      break;
    }
    case CPolHight:
    {
      SPI2->CR1 |= SPI_CR1_CPOL;                 //CPol=1, CK to 1 when idle
      break;
    }
    default:
    {
      ret = E_CPolarity;                         //error Clock polarity
      break;
    }
  }

  switch(config->phase)                           //set the phase of clock
  {
    case CPhaLow:
    {
      //CPha=0, first clock transition is the first data capture edge
      break;
    }
    case CPhaHight:
    {
      SPI1->CR1 |= SPI_CR1_CPHA;                  //CPha=1, second clock transition is the first data capture edge
      break;
    }
    default:
    {
      ret = E_CPhase;                             //error Clock Phase
      break;
    }
  }
  return ret;
}

static int Init_SPI2_FrameFormat(SPIcfg* config)  //Data frame format
{
  int ret = E_OK;

  switch(config->frameFormat)
  {
    case MSBfirst:
    {
      //LSBfirst=0, MSB first
      break;
    }
    case LSBfirst:
    {
      SPI2->CR1 |= SPI_CR1_LSBFIRST;              //LSBfirst=1, LSB first
      break;
    }
    default:
    {
      ret = E_FrameFormat;                       //error Frame Format
      break;
    }
  }

  return ret;
}

static int Init_SPI2(SPIcfg* config)              //Init SPI_2
{
  int ret;

  RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;                       //clock for SPI2

  if(config->mode == 0)                                     //if MCU is master
  {
    SPI2->CR1  = 0;                                         //clear register CR1
    SPI2->CR2  = 0x0700;                                    //clear register CR2 but DS must not be cleared

    SPI2->CR1 |= SPI_CR1_MSTR;                              //master

    ret = Init_SPI2_BaudRate(config);                       //set Baud Rate
    ret = Init_SPI2_DataSize(config);                       //set Data length for SPI transfers
    ret = Init_SPI2_SlaveSlect(config);                     //set slave select mode
    ret = Init_SPI2_PoPha(config);                          //set clock polarity and phase
    ret = Init_SPI2_FrameFormat(config);                    //set Frame Format

    SPI2->CR2 |= SPI_CR2_RXNEIE;                             //RXNE event generated if FIFO >=8bit
    SPI2->CR1 |= SPI_CR1_SPE;                               //start SPI
  }

  if(config->mode == 1)                                     //if MCU is slave
  {
	SPI2->CR1  = 0;                                         //clear register CR1
	SPI2->CR2  = 0x0700;                                    //clear register CR2 but DS must not be cleared

	ret = Init_SPI2_DataSize(config);                       //set Data length for SPI transfers
	ret = Init_SPI2_SlaveSlect(config);                     //set slave select mode
	ret = Init_SPI2_PoPha(config);                          //set clock polarity and phase
	ret = Init_SPI2_FrameFormat(config);                    //set Frame Format

    SPI2->CR2 |= SPI_CR2_RXNEIE;                            //RXNE event generated if FIFO >=8bit
    SPI2->CR1 |= SPI_CR1_SPE;                               //start SPI
  }

  if((config->mode != 1) && (config->mode != 0))            //if MODE is invalid
  {
    ret = E_INVALID_PARAM;
  }

  return ret;
}

static uint8_t SPI2_RW(SPIcfg* config, uint8_t send)
{
  volatile uint8_t receive;
  if ((SPI2->SR & SPI_SR_TXE) == SPI_SR_TXE)                    //TX empty
  {
    if((config->size == DataSize4) || (config->size == DataSize8))
    {

      *(volatile uint8_t *)&(SPI2->DR) = send;                  //send 8bit data
    }
    else
    {
      *(uint16_t *)&(SPI2->DR) = (uint16_t)send;                //send 16bit data
    }
  }


  if (!((SPI2->SR & SPI_SR_RXNE) == SPI_SR_RXNE))               //RX empty
  {
    while( !(SPI2->SR & SPI_SR_RXNE));
    if((config->size == DataSize4) || (config->size == DataSize8))
    {

     receive = *(volatile uint8_t *)&(SPI2->DR);            //read 8bit data
    }
    else
    {
      *(uint8_t*)&receive= *(uint16_t *)&(SPI2->DR);            //read 16bit data
    }
 }
  return receive;
}

//**************************************INTERFACE FUNCTION***************************

int SPI_Cfg(SPIcfg* config)
{
  int ret;

  switch(config->spi)
  {
    case SPI_1:
    {
      ret = Init_SPI1(config);
      break;
    }
    case SPI_2:
    {
      ret = Init_SPI2(config);
      break;
    }
    case SPI_3:
    case SPI_4:
    {
      ret = E_NOT_IMPLEMENTED;
      break;
    }
    default:
    {
      ret = E_INVALID_PARAM;
      break;
    }
  }

  return ret;
}

uint8_t SPI_RW(SPIcfg* config, uint8_t send)
{
  uint8_t receive;
  if(config->spi == SPI_1)
  {
    receive = SPI1_RW(config,send);
  }

  else if(config->spi == SPI_2)
  {
    receive = SPI2_RW(config,send);
  }
  else
  {
    receive = 121;
  }

  return receive;
}


