/**
 * @file spi.h
 *
 * @Created on: 18.02.2018
 * @Author: Kamil Malski
 *
 * @brief File included the interface of abstract SPI controller working at all hardware.
 * ie. Controller used to initial, send and read data from SPI.
 * ie. Application MUST TO config GPIO.MISO, GPIO.MOSI, GPIO.CLK and GPIO.NSS. Controller is NOT config GPIO
 */

#ifndef SPI_H_
#define SPI_H_

/**
 * @brief Numeric representation of error
 * TODO This might be multiple if necessary
 */
#define E_OK                0     //Operation success
#define E_NOT_IMPLEMENTED  -1     //Functionality not implemented on target hardware
#define E_NOT_INITIALIZED  -2     //Functionality not initialized
#define	E_INVALID_PARAM	   -3     //Invalid input parameter
#define E_SPI              -4     //SPIType is not initialize
#define E_Mode             -5     //ModeType is not initialize
#define E_DataSize         -6     //DataSizeType is not initialize
#define E_SSType           -7     //SlaveSelectType is not initialize
#define E_CPolarity        -8     //CPolarityType is not initialize
#define E_CPhase           -9     //CPhaseType is not initialize
#define E_BaudRate         -10    //BaudRateType is not initialize
#define E_FrameFormat      -11    //FrameFormatType is not initialize
#define E_DMAType          -12    //DMAType is not initialize


/**
 * @brief Abstract numeric representation of hardware SPI
 * TODO This might be multiple if necessary
 */
typedef enum
{
	SPI_1 = 1,
	SPI_2 = 2,
	SPI_3 = 3,
	SPI_4 = 4
}SPIType;

/**
 * @brief Numeric representation of role
 */
typedef enum
{
	Master = 0,	   //SPI work like master
	Slave  = 1	   //SPI work like slave

}SPIModeType;

/**
 * @brief Numeric representation of size of read and send data by abstract SPI
 */
typedef enum
{
	DataSize4   = 0,    //data frame is 4bits
	DataSize8   = 1,    //data frame is 8 bits
	DataSize16  = 2	    //data frame is 16 bits
}SPIDataSizeType;

/**
 * @brief Numeric representation of functionality "Slave select NSS pin management"
 * ie.
 */
typedef enum
{
	SwNSS    = 0,     //external pin NSS is free, recommended if SPI master
	HwNSSEN  = 1,     //Hardware NSS management, SSOE=1, used only if MCU is master
	HwNSSDS	 = 2      //Hardware NSS management, SSOE=0, allows multimaster capability
}SPISlaveSelectType;

/**
 * @brief Numeric representation of clock polarity
 */
typedef enum
{
	CPolLow    = 0,	  //CK to 0 when idle
	CPolHight  = 1	  //CK to 1 when idle
}SPICPolarityType;

/**
 * @brief Numeric representation of clock phase
 */
typedef enum
{
	CPhaLow	  = 0,	  //The first clock transmission is the first data capture edge
	CPhaHight = 1	  //The second clock transmission is the first data capture edge
}SPICPhaseType;

/**
 * @brief Numeric representation of Baud Rate control
 */
typedef enum
{
	Fdiv2    = 0,	  //Fpclk/2
	Fdiv4  	 = 1,     //Fpclk/4
	Fdiv8 	 = 2,     //Fpclk/8
	Fdiv16   = 3,     //Fpclk/16
	Fdiv32   = 4,     //Fpclk/32
	Fdiv64   = 5,     //Fpclk/64
	Fdiv128  = 6,     //Fpclk/128
	Fdiv256  = 7,     //Fpclk/256
}SPIBaudRateType;

/*
 * @brief Numeric representation of Frame format
 */
typedef enum
{
	MSBfirst  = 0,   //data is transmitted/received with MSB first
	LSBfirst  = 1    //data is transmitted/received with LSB first
}SPIFrameFormatType;

/*
 * @brief Numeric representation of DMA functionality. DMA is used or DMA is not used
 */


/*
 * @brief Initial struct of abstract SPI. All fields should be filled out
 */
typedef struct
{
    /*
     *  SPI_1 = 1
     *  SPI_2 = 2
     *  ...
     */
    SPIType  spi;   //number of specific SPI

    /*
     *  Master = 0
     *  Slave  = 1
     */
    SPIModeType   mode;   //Master or slave mode

    /*
     *  DataSize4   = 0
     *  DataSize8   = 1
     *  DataSize16  = 2
     */
    SPIDataSizeType  size;   //4bit,8bit or 16bit data frame

    /*
     *  SwNSS    = 0
     *  HwNSSEN  = 1
     *  HwNSSDS  = 2
     */
    SPISlaveSelectType  SS;	   //CS pin external used or not

    /*
     *  CPolLow    = 0
     *  CPolHight  = 1
     *
     */
    SPICPolarityType   polarity;	   //CK is 0 in idle or CK is 1 in idle

    /*
     *  CPhaLow   = 0
     *  CPhaHight = 1
     */
    SPICPhaseType    phase;   //first clock is first data or second clock is first data

    /*
     *  Fdiv2    = 0
     *  Fdiv4    = 1
     *  Fdiv8    = 2
     *  ...
     */
    SPIBaudRateType   baudRate;	  //baudRate prescaler

    /*
     *  MSBfirst  = 0
     *  LSBfirst  = 1
     */
    SPIFrameFormatType  frameFormat;  //MSBfirst or LSBfirst

}SPIcfg;

/**
 * @brief Function used to initialize specific SPI
 *
 * @param config - Struct with initial parameters
 *
 * @return Status of initial
 */
int SPI_Cfg(SPIcfg* config);

/**
 * @brief Function used to send data with specific SPI
 *
 * @param config   - Struct with initial parameters
 */
uint8_t SPI_RW(SPIcfg* config, uint8_t send);



#endif /* SPI_H_ */
