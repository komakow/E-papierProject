#include "stm32f4xx.h"
#include "spiDriver.h"
#include "GPIODriver.h"
#include "defGPIO.h"

static void spiConfig();


int main(void)
{
  int i = 0;



  while (1)
  {

  }
}



static void spiConfig()
{
	gpiocfg();
	spi.SS = SwNSS;
	spi.frameFormat = MSBfirst;
	spi.mode = Master;
	spi.phase = CPhaLow;
	spi.polarity = CPolLow;
	spi.size = DataSize8;
	spi.spi = SPI_1;
	SPI_Cfg(&spi);

	return;
}
