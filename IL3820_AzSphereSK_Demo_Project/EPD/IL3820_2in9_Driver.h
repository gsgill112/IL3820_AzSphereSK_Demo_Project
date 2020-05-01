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
extern uint16_t max_h, max_w;


// Display resolution
///extern uint16_t EPD_2in9_WIDTH = 128;
///extern uint16_t EPD_2in9_HEIGHT = 296;

#define EPD_2in9_FULL			0
#define EPD_2in9_PART			1


#define EPD_2in9_ORIENT_POTRAIT 0 // Sets Potrait Orient 
#define EPD_2in9_ORIENT_LANDSCAPE 1  // Sets Landscape Orient

//#define BUFFER_SIZE EPD_2in9_WIDTH*EPD_2in9_HEIGHT  //Display Bufffer Size 

// Standard register set for ILI3820 I/F
static const uint8_t DRIVER_OUTPUT_CONTROL = 0x01;
static const uint8_t BOOSTER_SOFT_START_CONTROL = 0x0C;
static const uint8_t GATE_SCAN_START_POSITION = 0x0F;
static const uint8_t DEEP_SLEEP_MODE = 0x10;
static const uint8_t DATA_ENTRY_MODE_SETTING = 0x11;
static const uint8_t SW_RESET = 0x12;
static const uint8_t TEMPERATURE_SENSOR_CONTROL = 0x1A;
static const uint8_t MASTER_ACTIVATION = 0x20;
static const uint8_t DISPLAY_UPDATE_CONTROL_1 = 0x21;
static const uint8_t DISPLAY_UPDATE_CONTROL_2 = 0x22;
static const uint8_t WRITE_RAM = 0x24;
static const uint8_t WRITE_VCOM_REGISTER = 0x2C;
static const uint8_t WRITE_LUT_REGISTER = 0x32;
static const uint8_t SET_DUMMY_LINE_PERIOD = 0x3A;
static const uint8_t SET_GATE_TIME = 0x3B;
static const uint8_t BORDER_WAVEFORM_CONTROL = 0x3C;
static const uint8_t SET_RAM_X_ADDRESS_START_END_POSITION = 0x44;
static const uint8_t SET_RAM_Y_ADDRESS_START_END_POSITION = 0x45;
static const uint8_t SET_RAM_X_ADDRESS_COUNTER = 0x4E;
static const uint8_t SET_RAM_Y_ADDRESS_COUNTER = 0x4F;
static const uint8_t TERMINATE_FRAME_READ_WRITE = 0xFF;

int EPD_Init_2in9(int fd, uint8_t Mode);
int EPD_Clear_2in9(int fd);
int EPD_Refresh_2in9(int fd);
int EPD_Display_Image_2in9(int fd, uint8_t* Image);
int EPD_Display_Image_2in9_test(int fd, uint8_t* Image);
int EPD_Sleep_2in9(int fd);
int EPD_Turn_On_Display_2in9(int fd);
int EPD_Set_Orientation_2in9(int orient);
int EPD_Set_Cursor_2in9(int fd, uint8_t Xs, uint8_t Ys);
int EPD_Set_Display_Area_2in9(int fd, uint16_t Xs, uint16_t Ys, uint16_t Xe, uint16_t Ye);
int Display_Init_2in9(int fd);

#endif