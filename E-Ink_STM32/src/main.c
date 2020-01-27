#include <stdio.h>
#include <stdlib.h> // malloc() free()
#include "stm32f4xx.h"
#include "spiDriver.h"
#include "GPIODriver.h"
#include "defGPIO.h"
#include "EPD_2in9.h"
#include "GUI_Paint.h"

SPIcfg spi;



static void spiConfig();


int main(void)
{

  spiConfig();           //config spi
  GPIO_InitializeAll();  //config gpio

  DEV_Module_Init();

  EPD_2IN9_Init(EPD_2IN9_FULL);
  EPD_2IN9_Clear();
  DEV_Delay_ms(500);

  //Create a new image cache
  UBYTE *BlackImage;
  /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
  UWORD Imagesize = ((EPD_2IN9_WIDTH % 8 == 0)? (EPD_2IN9_WIDTH / 8 ): (EPD_2IN9_WIDTH / 8 + 1)) * EPD_2IN9_HEIGHT;
  if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
      return -1;
  }
  Paint_NewImage(BlackImage, EPD_2IN9_WIDTH, EPD_2IN9_HEIGHT, 270, WHITE);

  //1.Select Image
  Paint_SelectImage(BlackImage);
  Paint_Clear(WHITE);

  // 2.Drawing on the image
  Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
  Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
  Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);

  Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
  Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);

  Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
  Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);

  Paint_DrawCircle(45, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
  Paint_DrawCircle(105, 95, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);



  EPD_2IN9_Display(BlackImage);
  DEV_Delay_ms(2000);


  while (1)
  {

  }
}



static void spiConfig()
{
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
