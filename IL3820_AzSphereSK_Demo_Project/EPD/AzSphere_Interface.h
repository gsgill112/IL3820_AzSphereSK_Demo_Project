//This AzSphere Interface lib for EPD Display by WaveShare electronics and is compatable for Good Display EPD Pannels also
// Currently added support for 2.9" EPD by WaveShare/ Goos Display 
// To Use the Lib, Only add this File and Define the Display 
//Ver 0.1
#ifndef AzSphere_EPD_INTERFACE_H
#define AzSphere_EPD_INTERFACE_H

//Global Includes
#include <errno.h>
//#include <stdafx.h>

#include "../HighLevelCore/applibs_versions.h"

#include <applibs/spi.h>
#include <applibs/gpio.h>
#include <applibs/log.h>

#include "hw/avnet_mt3620_sk.h"



//Definitions for external Pins based on Click Slot Configured
#if CLICK_SLOT == 2 
#define DC_PIN AVNET_MT3620_SK_GPIO2
#define BUSY_PIN  AVNET_MT3620_SK_GPIO1
#define RST_PIN AVNET_MT3620_SK_GPIO17    //Connected to RST Pin on mikroBus Slot 2
//    Log_Debug("Configured for Mk Bus CLICK SLOT #1");
#else
#define DC_PIN AVNET_MT3620_SK_GPIO2
#define BUSY_PIN  AVNET_MT3620_SK_GPIO0
#define RST_PIN AVNET_MT3620_SK_GPIO16    //Connected to RST Pin on mikroBus Slot 1
//Log_Debug("Configured for Mk Bus CLICK SLOT #1");
#endif

#define EPD_COMMAND 1 //Sending if command /Data 
#define EPD_DATA 0 //Sending if command /Data 

//Defined Variables and Access Pin FD accross EPD Display 
extern int rstfd, dcfd, busyfd;
extern uint8_t *spiReadData;
extern uint8_t readGPIO;
extern ssize_t spi_transferredBytes;

#ifdef EPD_2IN9	//Including Files and Parameterss coorosponding to EPD 2.9" Display 
#include "IL3820_2in9_Driver.h"
#endif


// Some Global  Universal Functions
int delay_ms(uint8_t delay);  // Milisec delay generator
int delay_us(uint8_t delay);  // Microsec delay generator
int delay_s(uint8_t delay);	  // Sec delay generator
int EPD_gpio_init(void);   // Initialization of GPIO Reqd for EPD Ref above for Pin Layouts 
int EPD_reset(void);   // Initialization of GPIO Reqd for EPD Ref above for Pin Layouts
int AzSphere_Spi_Init(int fd);  // returns the SPI FD, for further use 
int EPD_Busy(void);   // Checks weather EPD IS Busy or Not (You can use Fixed Delay if Not using the pin
int Spi_Write(int fd, uint8_t dc, uint8_t* buf, uint32_t len); //Writes Byte to Spi Bus 
int Spi_Read(int fd, uint8_t reg, uint8_t* buf, uint32_t len); // Reads the Value from SPI Bus : Vestigial function as MOSI is NC in the Display Module. 

#endif