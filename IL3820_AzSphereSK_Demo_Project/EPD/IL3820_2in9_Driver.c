//This Driver is for IL3820 2.9" EPD Display by WaveShare electronics and is compatable for Good Display 2.9" EPD Pannels also
//Ver 0.1

// Global Includes 
#include <errno.h>

#include "../HighLevelCore/applibs_versions.h"

#include <applibs/spi.h>
#include <applibs/gpio.h>
#include <applibs/log.h>

#include "AzSphere_Interface.h"
#include "IL3820_2in9_Driver.h"

/// <summary>
/// Termination codes for this application. These are used for the
/// application exit code.  They they must all be between zero and 255,
/// where zero is reserved for successful termination.
/// </summary>
typedef enum {
    ExitCode_Success = 0,

    ExitCode_TermHandler_SigTerm = 1,

    ExitCode_SPI_WriteData = 2,
    ExitCode_SPI_ReadData = 3,
   
    ExitCode_GPIO_ReadValue = 4,
    ExitCode_GPIO_WriteValue = 5,

    ExitCode_RST = 6
}ExitCode;

// LUT Values for 2.9" EPd Display 
const unsigned char EPD_2IN9_lut_full_update[] = {
    0x50, 0xAA, 0x55, 0xAA, 0x11, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xFF, 0xFF, 0x1F, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static unsigned char LUTDefault_full[] = { 
    0x32,0x02,0x02,0x01,0x11,0x12,0x12,
    0x22,0x22,0x66,0x69,0x69,0x59,0x58,
    0x99,0x99,0x88,0x00,0x00,0x00,0x00,
    0xF8,0xB4,0x13,0x51,0x35,0x51,0x51,
    0x19,0x01,0x00 
};

static unsigned char LUTDefault_part[] = { 
    0x32,0x10,0x18,0x18,0x08,0x18,0x18,
    0x08,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x13,0x14,0x44,0x12,0x00,0x00,0x00,
    0x00,0x00,0x00 
};

const unsigned char EPD_2IN9_lut_partial_update[] = {
    0x10, 0x18, 0x18, 0x08, 0x18, 0x18,
    0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x13, 0x14, 0x44, 0x12,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


// User Functions 

//Initialises the Display
int EPD_Init_2in9(int fd, uint8_t Mode) {
    int ret;
    //Resetting the Display 
    ret = EPD_reset();
    delay_ms(100);

    //Disable deep sleep Mode 
    ret = Spi_Write(fd, EPD_COMMAND, DEEP_SLEEP_MODE);
    ret = Spi_Write(fd, EPD_DATA, 0x00);

    // Setting DRIVER_OUTPUT_CONTROL
    ret = Spi_Write(fd, EPD_COMMAND, DRIVER_OUTPUT_CONTROL);
    Log_Debug("INFO : EPD_Init_2in9 : DRIVER_OUTPUT_CONTROL_CMD Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : DRIVER_OUTPUT_CONTROL CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }

    ret = Spi_Write(fd, EPD_DATA, (296 - 1) & 0xFF);
    ret += Spi_Write(fd, EPD_DATA, ((296 - 1) >> 8) & 0xFF);
    ret += Spi_Write(fd, EPD_DATA, 0x00); // GD = 0; SM = 0; TB = 0;
    //EPD_Busy();
    Log_Debug("INFO : EPD_Init_2in9 : DRIVER_OUTPUT_CONTROL Data Bytes Tfr %d \n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : DRIVER_OUTPUT_CONTROL Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }

    /// gate driving violtage 
   // ret = Spi_Write(fd, EPD_COMMAND, 0x03);
  //  ret = Spi_Write(fd, EPD_DATA, 0x00);

    /// source driving violtage 
    //ret = Spi_Write(fd, EPD_COMMAND, 0x04);
    //ret = Spi_Write(fd, EPD_DATA, 0x0A);
    
    ret = -1;
    // BOOSTER_SOFT_START_CONTROL
    ret = Spi_Write(fd, EPD_COMMAND, BOOSTER_SOFT_START_CONTROL);
    Log_Debug("INFO : EPD_Init_2in9 : BOOSTER_SOFT_START_CONTROL_CMD Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : BOOSTER_SOFT_START_CONTROL CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }

    ret = Spi_Write(fd, EPD_DATA, 0xD7);
    ret += Spi_Write(fd, EPD_DATA, 0xD6);
    ret += Spi_Write(fd, EPD_DATA, 0x9D);
    Log_Debug("INFO : EPD_Init_2in9 : BOOSTER_SOFT_START_CONTROL Data Bytes Tfr %d \n", ret);
    if (ret <0) {
        Log_Debug("ERROR : EPD_Init_2in9 : BOOSTER_SOFT_START_CONTROL Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    
    ret = -1;
    // WRITE_VCOM_REGISTER
    ret = Spi_Write(fd, EPD_COMMAND, WRITE_VCOM_REGISTER);
    Log_Debug("INFO : EPD_Init_2in9 : WRITE_VCOM_REGISTER Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : WRITE_VCOM_REGISTER CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    ret = Spi_Write(fd, EPD_DATA, 0xA8); //VCOM 7C
    Log_Debug("INFO : EPD_Init_2in9 : WRITE_VCOM_REGISTER Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : WRITE_VCOM_REGISTER Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }

    ret = -1;
    // SET_DUMMY_LINE_PERIOD
    ret = Spi_Write(fd, EPD_COMMAND, SET_DUMMY_LINE_PERIOD);
    Log_Debug("INFO : EPD_Init_2in9 : SET_DUMMY_LINE_PERIOD Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : SET_DUMMY_LINE_PERIOD CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    ret = Spi_Write(fd, EPD_DATA, 0x1A); //4 dummy lines per gate
    //ret = Spi_Write(fd, EPD_DATA, 0x16);
    Log_Debug("INFO : EPD_Init_2in9 : SET_DUMMY_LINE_PERIOD Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : SET_DUMMY_LINE_PERIOD Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }

    ret = -1;
    // SET_GATE_TIME
    ret = Spi_Write(fd, EPD_COMMAND, SET_GATE_TIME);
    Log_Debug("INFO : EPD_Init_2in9 : SET_GATE_TIME Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : SET_GATE_TIME CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    ret = Spi_Write(fd, EPD_DATA, 0x08); //2us per line
    Log_Debug("INFO : EPD_Init_2in9 : SET_GATE_TIME Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : SET_GATE_TIME Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }

    ret = -1;
    // BORDER_WAVEFORM_CONTROL
    ret = Spi_Write(fd, EPD_COMMAND, BORDER_WAVEFORM_CONTROL);
    Log_Debug("INFO : EPD_Init_2in9 : BORDER_WAVEFORM_CONTROL Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : BORDER_WAVEFORM_CONTROL CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    ret = Spi_Write(fd, EPD_DATA, 0x03); //2us per line
    //ret = Spi_Write(fd, EPD_DATA, 0x33);
    Log_Debug("INFO : EPD_Init_2in9 : BORDER_WAVEFORM_CONTROL Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : BORDER_WAVEFORM_CONTROL Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
 
    ret = -1;
    // DATA_ENTRY_MODE_SETTING
    ret = Spi_Write(fd, EPD_COMMAND, DATA_ENTRY_MODE_SETTING);
    Log_Debug("INFO : EPD_Init_2in9 : DATA_ENTRY_MODE_SETTING Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : DATA_ENTRY_MODE_SETTING CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    ret = Spi_Write(fd, EPD_DATA, 0x03); //2us per line
    //ret = Spi_Write(fd, EPD_DATA, 0x07);
    Log_Debug("INFO : EPD_Init_2in9 : DATA_ENTRY_MODE_SETTING Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : DATA_ENTRY_MODE_SETTING Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    
    //set the look-up table register
    ret = -1;
    // DATA_ENTRY_MODE_SETTING
    ret = Spi_Write(fd, EPD_COMMAND, WRITE_LUT_REGISTER); // WRITE_LUT_REGISTER
    Log_Debug("INFO : EPD_Init_2in9 : WRITE_LUT_REGISTER Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : WRITE_LUT_REGISTER CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    if (Mode == EPD_2in9_FULL) {
        for (uint16_t i = 0; i < 30; i++) {
            ret += Spi_Write(fd, EPD_DATA, EPD_2IN9_lut_full_update[i]);  
            //ret += Spi_Write(fd, EPD_DATA, LUTDefault_full[i]);
        }
        Log_Debug("INFO : EPD_Init_2in9 : WRITE_LUT_REGISTER EPD_2IN9_lut_full_update Data Bytes Tfr %d\n", ret);
    }
    else if (Mode == EPD_2in9_PART) {
        for (uint16_t i = 0; i < 30; i++) {
            ret = Spi_Write(fd, EPD_DATA, EPD_2IN9_lut_partial_update[i]);
        }
        Log_Debug("INFO : EPD_Init_2in9 : WRITE_LUT_REGISTER EPD_2IN9_lut_partial_update Data Bytes Tfr %d \n", ret);
    }
    else {
        Log_Debug("ERROR : EPD_Init_2in9 : ONLY Mode is EPD_2IN9_FULL or EPD_2IN9_PART");
    }

    //Enable the Display 
    //ret = Spi_Write(fd, EPD_COMMAND, DISPLAY_UPDATE_CONTROL_2); // WRITE_LUT_REGISTER
    //ret = Spi_Write(fd, EPD_DATA, 0x04);
    //ret = Spi_Write(fd, EPD_COMMAND, MASTER_ACTIVATION);
    //delay_ms(1800);

    return ExitCode_Success;
}

//Clears the Display Orientation NOT Impl as of Now 
int EPD_Clear_2in9(int fd) {
    uint16_t W, H;
    uint16_t k = 0;
    uint16_t l = 0;
    int ret = -1;

    W = (128 % 8 == 0) ? (128 / 8) : (128 / 8 + 1);
    H = 296;
    //W++;
    EPD_Set_Display_Area_2in9(fd, 0, 0, 128, 296);

    for (uint16_t j = 0; j < H; j++) {
        EPD_Set_Cursor_2in9(fd, 0, j);
        k++;
        ret += Spi_Write(fd, EPD_COMMAND, WRITE_RAM);
        //ret += Spi_Write(fd, EPD_DATA, 0x5A);
        for (uint16_t i = 0; i < W; i++) {
            ret += Spi_Write(fd, EPD_DATA, 0xFF);
            l++;
        }
    }
    Log_Debug("INFO : EPD_Clear_2in9 : Bytes Tfr %d : 0-Height -- %d \n : 0-Width/8 -- %d", ret, k, l);

    EPD_Turn_On_Display_2in9(fd);
    //delay_ms(1800);
    return ExitCode_Success;
}

// Refreshes the epd Display  
int EPD_Refresh_2in9(int fd) {
    uint16_t W, H;
    int ret;
    int b_tfr = 0;
    uint8_t tmp;

    W = (128 % 8 == 0) ? (128 / 8) : (128 / 8 + 1);
    H = 296;

    EPD_Set_Display_Area_2in9(fd, 0, 0, 128, 296);

    for (uint16_t j = 0; j < H; j++) {
        EPD_Set_Cursor_2in9(fd, 0, j);
        ret += Spi_Write(fd, EPD_COMMAND, WRITE_RAM);
        for (uint16_t i = 0; i < W; i++) {
            ret += Spi_Write(fd, EPD_DATA, 0x00);
        }
    }
    EPD_Turn_On_Display_2in9(fd);
    delay_ms(1800);
    for (uint16_t j = 0; j < H; j++) {
        EPD_Set_Cursor_2in9(fd, 0, j);
        ret += Spi_Write(fd, EPD_COMMAND, &WRITE_RAM);
        for (uint16_t i = 0; i < W; i++) {
            ret += Spi_Write(fd, EPD_DATA, &tmp);
        }
    }
    Log_Debug("INFO : EPD_Clear_2in9 :Bytes Tfr %d  \n", ret);

    EPD_Turn_On_Display_2in9(fd);
    delay_ms(1800);
    return ExitCode_Success;
}


int EPD_Display_Image_2in9(int fd, uint8_t *Image) {
    uint16_t W, H;
    int ret;
    int b_tfr = 0;

    W = (128 % 8 == 0) ? (128 / 8) : (128 / 8 + 1);
    H = 296;
    uint32_t Addr = 0;

    // UDOUBLE Offset = ImageName;
    EPD_Set_Display_Area_2in9(fd, 0, 0, 128, 296);

    for (uint16_t j = 0; j < H; j++) {
        EPD_Set_Cursor_2in9(fd, 0, j);
        ret += Spi_Write(fd, EPD_COMMAND, WRITE_RAM);
        for (uint16_t i = 0; i < W; i++) {
            Addr = i + j * W;
            b_tfr += Spi_Write(fd, EPD_DATA, Image[Addr]);
        }
    }

    Log_Debug("INFO : EPD_Display_Image_2in9 : WRITE_RAM Command Bytes Tfr %d ; Data Tfr Bytes %d \n", ret, b_tfr);
    delay_ms(100);
    EPD_Turn_On_Display_2in9(fd);
    return ExitCode_Success;
}

int EPD_Display_Image_2in9_test(int fd, uint8_t* Image) {
    uint8_t W, H;
    int ret;
    int b_tfr = 0;

    W = 128;
    H = 296;

    uint32_t Addr = 0;

    // UDOUBLE Offset = ImageName;
    EPD_Set_Display_Area_2in9(fd, 0, 0, 128, 296);
    //EPD_Busy();

    for (uint8_t j = 0; j < H; j++) {
        EPD_Set_Cursor_2in9(fd, 0, j);
        ret += Spi_Write(fd, EPD_COMMAND, &WRITE_RAM);
        //Log_Debug("INFO : EPD_Display_Image_2in9 : WRITE_RAM Bytes Tfr %d ; RetValue %d\n", spi_transferredBytes, ret);
        if (ret < 0) {
            Log_Debug("ERROR : EPD_Display_Image_2in9 : WRITE_RAM CMD Bytes Tfr ERROR\n");
            return ExitCode_SPI_WriteData;
        }
        for (uint8_t i = 0; i < W; i++) {
            Addr = i + j * W;
            b_tfr += Spi_Write(fd, EPD_DATA, &Image[Addr]);
            //Log_Debug("INFO : EPD_Display_Image_2in9 : WRITE_RAM Data Bytes Tfr %d ; RetValue %d\n", spi_transferredBytes, ret);
            if (b_tfr < 0) {
                Log_Debug("ERROR : EPD_Display_Image_2in9 : WRITE_RAM Data Bytes Tfr ERROR\n");
                return ExitCode_SPI_WriteData;
            }
        }
    }
    //EPD_Busy();

    Log_Debug("INFO : EPD_Display_Image_2in9 : WRITE_RAM Command Bytes Tfr %d ; Data Tfr Bytes %d \n", ret, b_tfr);
    EPD_Turn_On_Display_2in9(fd);
    //EPD_Busy();
    return ExitCode_Success;
}

// Puts the Display in Sleep Mode 
int EPD_Sleep_2in9(int fd) {
    int ret;
    ret = Spi_Write(fd, EPD_COMMAND, DEEP_SLEEP_MODE);
    Log_Debug("INFO : EPD_Sleep_2in9 : DEEP_SLEEP_MODE_CMD Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Sleep_2in9 : DEEP_SLEEP_MODE CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    ret = Spi_Write(fd, EPD_DATA, 0x01);
    Log_Debug("INFO : EPD_Sleep_2in9 : DEEP_SLEEP_MODE Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Sleep_2in9 : DEEP_SLEEP_MODE Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    return ExitCode_Success;
}

// Turns on the display 
int EPD_Turn_On_Display_2in9(int fd) {
    int ret;

    // DISPLAY_UPDATE_CONTROL_2
    ret = Spi_Write(fd, EPD_COMMAND, DISPLAY_UPDATE_CONTROL_2);
    Log_Debug("INFO : EPD_Turn_On_Display_2in9 : DISPLAY_UPDATE_CONTROL_2 Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Turn_On_Display_2in9 : DISPLAY_UPDATE_CONTROL_2 CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    ret = Spi_Write(fd, EPD_DATA, 0xC4);
    Log_Debug("INFO : EPD_Turn_On_Display_2in9 : DISPLAY_UPDATE_CONTROL_2 Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Turn_On_Display_2in9 : DISPLAY_UPDATE_CONTROL_2 Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }

    ret = Spi_Write(fd, EPD_COMMAND, MASTER_ACTIVATION);
    Log_Debug("INFO : EPD_Turn_On_Display_2in9 : MASTER_ACTIVATION Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Turn_On_Display_2in9 : MASTER_ACTIVATION CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }

    delay_ms(1700);

    ret = Spi_Write(fd, EPD_COMMAND, TERMINATE_FRAME_READ_WRITE);
    Log_Debug("INFO : EPD_Turn_On_Display_2in9 : TERMINATE_FRAME_READ_WRITE Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Turn_On_Display_2in9 : TERMINATE_FRAME_READ_WRITE CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }

    ret = EPD_Busy();
    return ExitCode_Success;
}

// Change sthe Orientation of the display 
int EPD_Set_Orientation_2in9( int orient) {
    if (orient == EPD_2in9_ORIENT_POTRAIT) {
        max_w = 128;
        max_h = 296;
    }
    else if (orient == EPD_2in9_ORIENT_LANDSCAPE) {
        max_h = 128;
        max_w = 296;
    }
    else Log_Debug("WARNING : ILLIGAL ORIENTATION TYPE USING DEFAULT ORIENTATION LANDSCAPE");
    max_w = 296;
    max_h = 128;

    return ExitCode_Success;
}

// Set the Cursor Loc on the Display 
int EPD_Set_Cursor_2in9(int fd, uint8_t Xs, uint8_t Ys) {
    int ret;
    // SET_RAM_X_ADDRESS_COUNTER
    ret = Spi_Write(fd, EPD_COMMAND, SET_RAM_X_ADDRESS_COUNTER);
    Log_Debug("INFO : EPD_Set_Cursor_2in9 : SET_RAM_X_ADDRESS_COUNTER Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Set_Cursor_2in9 : SET_RAM_X_ADDRESS_COUNTER CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    ret = Spi_Write(fd, EPD_DATA, (Xs >> 3) & 0xFF);
    //ret = Spi_Write(fd, EPD_DATA, Xs & 0xFF);
    Log_Debug("INFO : EPD_Set_Cursor_2in9 : SET_RAM_X_ADDRESS_COUNTER Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Set_Cursor_2in9 : SET_RAM_X_ADDRESS_COUNTER Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }

    // SET_RAM_Y_ADDRESS_COUNTER
    ret = Spi_Write(fd, EPD_COMMAND, SET_RAM_Y_ADDRESS_COUNTER);
    Log_Debug("INFO : EPD_Set_Cursor_2in9 : SET_RAM_Y_ADDRESS_COUNTER Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Set_Cursor_2in9 : SET_RAM_Y_ADDRESS_COUNTER CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }

    ret = Spi_Write(fd, EPD_DATA, Ys & 0xFF);
    Log_Debug("INFO : EPD_Set_Cursor_2in9 : SET_RAM_Y_ADDRESS_COUNTER Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Set_Cursor_2in9 : SET_RAM_Y_ADDRESS_COUNTER Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    ret = Spi_Write(fd, EPD_DATA, (Ys >> 8) & 0xFF);
    Log_Debug("INFO : EPD_Set_Cursor_2in9 : SET_RAM_Y_ADDRESS_COUNTER Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Set_Cursor_2in9 : SET_RAM_Y_ADDRESS_COUNTER Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    return ExitCode_Success;
}

// Sets the Display Window For Display 
int EPD_Set_Display_Area_2in9(int fd, uint16_t Xs, uint16_t Ys, uint16_t Xe, uint16_t Ye) {
    int ret;
    // SET_RAM_X_ADDRESS_START_END_POSITION
    ret = Spi_Write(fd, EPD_COMMAND, SET_RAM_X_ADDRESS_START_END_POSITION);
    Log_Debug("INFO : EPD_Set_Display_Area_2in9 : SET_RAM_X_ADDRESS_START_END_POSITION Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Set_Display_Area_2in9 : SET_RAM_X_ADDRESS_START_END_POSITION CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    // Send Start Data
    ret = Spi_Write(fd, EPD_DATA, (Xs >> 3) & 0xFF);
    //ret = Spi_Write(fd, EPD_DATA, Xs & 0xFF);
    Log_Debug("INFO : EPD_Set_Display_Area_2in9 : SET_RAM_X_ADDRESS_START_END_POSITION Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Set_Display_Area_2in9 : SET_RAM_X_ADDRESS_START_END_POSITION Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    // Send End Data
    ret = Spi_Write(fd, EPD_DATA, (Xe >> 3) & 0xFF);
    //ret = Spi_Write(fd, EPD_DATA, Xe & 0xFF);
    Log_Debug("INFO : EPD_Set_Display_Area_2in9 : SET_RAM_X_ADDRESS_START_END_POSITION Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Set_Display_Area_2in9 : SET_RAM_X_ADDRESS_START_END_POSITION Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }

    // SET_RAM_Y_ADDRESS_START_END_POSITION
    ret = Spi_Write(fd, EPD_COMMAND, SET_RAM_Y_ADDRESS_START_END_POSITION);
    Log_Debug("INFO : EPD_Set_Display_Area_2in9 : SET_RAM_Y_ADDRESS_START_END_POSITION Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Set_Display_Area_2in9 : SET_RAM_Y_ADDRESS_START_END_POSITION CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    // Send Start Data
    ret = Spi_Write(fd, EPD_DATA, Ys & 0xFF);
    Log_Debug("INFO : EPD_Set_Display_Area_2in9 : SET_RAM_Y_ADDRESS_START_END_POSITION Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Set_Display_Area_2in9 : SET_RAM_Y_ADDRESS_START_END_POSITION Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    // Send Start Data (H)
    ret = Spi_Write(fd, EPD_DATA, (Ys >> 8) & 0xFF);
    Log_Debug("INFO : EPD_Set_Display_Area_2in9 : SET_RAM_Y_ADDRESS_START_END_POSITION Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Set_Display_Area_2in9 : SET_RAM_Y_ADDRESS_START_END_POSITION Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }

    // Send end Data
    ret = Spi_Write(fd, EPD_DATA, Ye & 0xFF);
    Log_Debug("INFO : EPD_Set_Display_Area_2in9 : SET_RAM_Y_ADDRESS_START_END_POSITION Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Set_Display_Area_2in9 : SET_RAM_Y_ADDRESS_START_END_POSITION Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    // Send end Data (H)
    ret = Spi_Write(fd, EPD_DATA, (Ye >> 8) & 0xFF);
    Log_Debug("INFO : EPD_Set_Display_Area_2in9 : SET_RAM_Y_ADDRESS_START_END_POSITION Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Set_Display_Area_2in9 : SET_RAM_Y_ADDRESS_START_END_POSITION Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    //EPD_Busy();
    return ExitCode_Success;
}

// Does everything 
int Display_Init_2in9(int fd) {
    int ret;
    // Init GPIO
    ret = EPD_gpio_init();

    // Init SPI
    //ret = AzSphere_Spi_Init(fd);   
    
    // Reset 
    ret = EPD_reset();

    // Init Display 
    ret = EPD_Init_2in9(fd, EPD_2in9_FULL);
    //EPD_Set_Cursor_2in9(fd,0,0);
    /*
    ret = Spi_Write(fd, EPD_COMMAND, &SET_RAM_X_ADDRESS_START_END_POSITION);
    ret = Spi_Write(fd, EPD_DATA, 0);
    ret = Spi_Write(fd, EPD_DATA5);
    ret = Spi_Write(fd, EPD_COMMAND, &SET_RAM_Y_ADDRESS_START_END_POSITION);
    ret = Spi_Write(fd, EPD_DATA, 0);
    ret = Spi_Write(fd, EPD_DATA, 0);
    ret = Spi_Write(fd, EPD_DATA, 295&255);
    ret = Spi_Write(fd, EPD_DATA, 295>>8);

    //EPD_Set_Display_Area_2in9(fd, 0, 29, 295&255 , 295>>8);
    //EPD_Busy();
    */
    // Toggle B/W
    //ret = EPD_Clear_2in9(fd);

    return ExitCode_Success;
}