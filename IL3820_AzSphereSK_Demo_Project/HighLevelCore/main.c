#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "applibs_versions.h"

#include <applibs/log.h>
#include <applibs/spi.h>
#include "hw/avnet_mt3620_sk.h"
#include "../EPD/AzSphere_Interface.h"
#include "../Fonts/fonts.h"
#include "ImageData.h"

int spiFd;

/**
 * Image attributes
**/
typedef struct {
    uint8_t * Image;
    uint16_t  Width;
    uint16_t  Height;
    uint16_t  WidthMemory;
    uint16_t  HeightMemory;
    uint16_t  Color;
    uint16_t  Rotate;
    uint16_t  Mirror;
    uint16_t  WidthByte;
    uint16_t  HeightByte;
    uint16_t  Scale;
} PAINT;

extern PAINT Paint;

void Paint_SelectImage(uint8_t * image)
{
    Paint.Image = image;
}

void Paint_DrawBitMap(const unsigned char* image_buffer)
{
    
}

int main(void)
{
    // This minimal Azure Sphere app repeatedly prints "Tick" then "Tock" to the debug console.
    // Use this app to test that device and SDK installation succeeded that you can build,
    // deploy, and debug an app with Visual Studio, and that you can deploy an app over the air,
    // per the instructions here: https://docs.microsoft.com/azure-sphere/quickstarts/qs-overview
    //
    // It is NOT recommended to use this as a starting point for developing apps; instead use
    // the extensible samples here: https://github.com/Azure/azure-sphere-samples
    spiFd = -1;
    int Bsyfd = GPIO_OpenAsOutput(AVNET_MT3620_SK_GPIO1, GPIO_OutputMode_PushPull, GPIO_Value_High);
    spiFd = AzSphere_Spi_Init(spiFd);
    Display_Init_2in9(spiFd);
    //EPD_Busy();
    delay_s(1);
    //Create a new image cache
    uint8_t* BlackImage;
    /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
    uint16_t Imagesize = ((EPD_2in9_WIDTH % 8 == 0) ? (EPD_2in9_WIDTH / 8) : (EPD_2in9_WIDTH / 8 + 1)) * EPD_2in9_HEIGHT;
    if ((BlackImage = (uint8_t*)malloc(Imagesize)) == NULL) {
        //printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    //Paint_SelectImage(BlackImage);

    uint16_t  x, y;
    uint32_t Addr = 0;

//    for (y = 0; y < EPD_2in9_HEIGHT; y++) {
 //       for (x = 0; x < EPD_2in9_WIDTH; x++) {//8 pixel =  1 byte
 //           Addr = x + y * EPD_2in9_WIDTH;
 //           BlackImage[Addr] = (unsigned char)gImage_2in9[Addr];
 //       }
//    }

    //Paint_DrawBitMap(gImage_2in9);
    delay_ms(2000);
    EPD_Display_Image_2in9(spiFd, gImage_2in9);
    delay_ms(2000);

    //printf("Paint_NewImage\r\n");
    

    while (true) {
        Log_Debug("High Level Core tick\n");
        delay_s(1);
        Log_Debug("High Level Core tock\n");
        delay_s(1);
    }
}
