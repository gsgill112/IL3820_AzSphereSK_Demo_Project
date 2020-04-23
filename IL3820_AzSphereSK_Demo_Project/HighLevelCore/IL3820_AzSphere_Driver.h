//This Driver is for IL3820 2.9" EPD Display by WaveShare electronics and is compatable for Good Display 2.9" EPD Pannels also
//Ver 0.1


#ifndef AzSphere_IL3820_DRV_H
#define AzSphere_IL3820_DRV_H

//Global Includes
#include <applibs/spi.h>
#include <applibs/gpio.h>
#include <applibs/log.h>

//Definitions for external Pins based on Click Slot Configured
#if CLICK_SLOT == 1 
#define DC_PIN AVNET_MT3620_SK_GPIO2
#define BUSY_PIN  AVNET_MT3620_SK_GPIO0
#define RST_PIN AVNET_MT3620_SK_GPIO16    //Connected to RST Pin on mikroBus Slot 1
//    Log_Debug("Configured for Mk Bus CLICK SLOT #1");
#elif CLICK_SLOT == 2
#define DC_PIN AVNET_MT3620_SK_GPIO2
#define BUSY_PIN  AVNET_MT3620_SK_GPIO1
#define RST_PIN AVNET_MT3620_SK_GPIO17    //Connected to RST Pin on mikroBus Slot 2
//    Log_Debug("Configured for Mk Bus CLICK SLOT #1");
#else 
#define CLICK_SLOT == 1  // Default configuration if NOT Defined bu user 
#endif

//Defined Variables 
extern int rstfp, dcfp, ledfp;
int max_x, max_y;