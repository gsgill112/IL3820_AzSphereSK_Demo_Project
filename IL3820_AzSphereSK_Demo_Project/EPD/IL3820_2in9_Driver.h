//This Driver is for EPD Display by WaveShare electronics and is compatable for Good Display EPD Pannels also
// Currently added support for 2.9" EPD by WaveShare/ Goos Display 
//Ver 0.1
#ifndef  EPD_2in9	
#define  EPD_2in9	

//Global Includes
#include <errno.h>

#include "../HighLevelCore/applibs_versions.h"

#include <applibs/spi.h>
#include <applibs/gpio.h>
#include <applibs/log.h>

#include "AzSphere_Interface.h"

//Defined Variables and Access Pin FD
int max_x, max_y;


// Display resolution
#define EPD_2in9_WIDTH       128
#define EPD_2in9_HEIGHT      296

#define EPD_2in9_FULL			0
#define EPD_2in9_PART			1

#define EPD_2in9_ORIENT_POTRAIT 0 // Sets Potrait Orient 
#define EPD_2in9_ORIENT_LANDSCAPE 1  // Sets Landscape Orient

#define BUFFER_SIZE EPD_2in9_WIDTH*EPD_2in9_HEIGHT  //Display Bufffer Size 

// Standard defines for ILI3820 I/F
#define DRIVER_OUTPUT_CONTROL                       0x01
#define BOOSTER_SOFT_START_CONTROL                  0x0C
#define GATE_SCAN_START_POSITION                    0x0F
#define DEEP_SLEEP_MODE                             0x10
#define DATA_ENTRY_MODE_SETTING                     0x11
#define SW_RESET                                    0x12
#define TEMPERATURE_SENSOR_CONTROL                  0x1A
#define MASTER_ACTIVATION                           0x20
#define DISPLAY_UPDATE_CONTROL_1                    0x21
#define DISPLAY_UPDATE_CONTROL_2                    0x22
#define WRITE_RAM                                   0x24
#define WRITE_VCOM_REGISTER                         0x2C
#define WRITE_LUT_REGISTER                          0x32
#define SET_DUMMY_LINE_PERIOD                       0x3A
#define SET_GATE_TIME                               0x3B
#define BORDER_WAVEFORM_CONTROL                     0x3C
#define SET_RAM_X_ADDRESS_START_END_POSITION        0x44
#define SET_RAM_Y_ADDRESS_START_END_POSITION        0x45
#define SET_RAM_X_ADDRESS_COUNTER                   0x4E
#define SET_RAM_Y_ADDRESS_COUNTER                   0x4F
#define TERMINATE_FRAME_READ_WRITE                  0xFF

int EPD_Init_2in9(int fd, uint8_t Mode);
int EPD_Clear_2in9(int fd);
int EPD_Display_2in9(int fd, uint8_t* Image);
int EPD_Sleep_2in9(void);

#endif