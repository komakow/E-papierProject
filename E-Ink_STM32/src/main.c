#include <stdio.h>
#include <stdlib.h> // malloc() free()
#include "stm32f4xx.h"
#include "spiDriver.h"
#include "GPIODriver.h"
#include "defGPIO.h"
#include "EPD_2in9.h"
#include "GUI_Paint.h"

SPIcfg spi;

//Create a new image cache
UBYTE *BlackImage;


static void spiConfig();
static void screenConfig();
static void screenTestDraw();
static void screenTestWriteTime();


int main(void)
{

  spiConfig();           //config spi
  GPIO_InitializeAll();  //config gpio

  screenConfig();
  screenTestDraw();
  screenTestWriteTime();


  while (1)
  {

  }
}


static void screenTestDraw()
{
  //1.Select Image
  Paint_SelectImage(BlackImage);
  Paint_Clear(WHITE);

  DEV_Delay_ms(200);

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
  Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);

  EPD_2IN9_Display(BlackImage);
  DEV_Delay_ms(2000);

  return;
}

static void screenTestWriteTime()
{
  EPD_2IN9_Init(EPD_2IN9_PART);
  Paint_SelectImage(BlackImage);
  PAINT_TIME sPaint_time;
  sPaint_time.Hour = 12;
  sPaint_time.Min = 34;
  sPaint_time.Sec = 56;
  UBYTE num = 20;
  for (;;) {
      sPaint_time.Sec = sPaint_time.Sec + 1;
      if (sPaint_time.Sec == 60) {
          sPaint_time.Min = sPaint_time.Min + 1;
          sPaint_time.Sec = 0;
          if (sPaint_time.Min == 60) {
              sPaint_time.Hour =  sPaint_time.Hour + 1;
              sPaint_time.Min = 0;
              if (sPaint_time.Hour == 24) {
                  sPaint_time.Hour = 0;
                  sPaint_time.Min = 0;
                  sPaint_time.Sec = 0;
              }
          }
      }
      Paint_ClearWindows(150, 80, 150 + Font20.Width * 7, 80 + Font20.Height, WHITE);
      Paint_DrawTime(150, 80, &sPaint_time, &Font20, WHITE, BLACK);

      num = num - 1;
      if(num == 0) {
          break;
      }
      EPD_2IN9_Display(BlackImage);
      DEV_Delay_ms(500);//Analog clock 1s
  }
}

static void screenConfig()
{
  DEV_Module_Init();

  EPD_2IN9_Init(EPD_2IN9_FULL);
  EPD_2IN9_Clear();
  DEV_Delay_ms(500);

  /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
  UWORD Imagesize = ((EPD_2IN9_WIDTH % 8 == 0)? (EPD_2IN9_WIDTH / 8 ): (EPD_2IN9_WIDTH / 8 + 1)) * EPD_2IN9_HEIGHT;
  if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
      return;
  }
  Paint_NewImage(BlackImage, EPD_2IN9_WIDTH, EPD_2IN9_HEIGHT, 270, WHITE);
  return;
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
