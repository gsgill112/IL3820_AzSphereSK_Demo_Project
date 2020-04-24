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
    int b_tfrd = 0;
    uint8_t tmp;
    //Resetting the Display 
    ret = EPD_reset();
    delay_ms(100);

    if (ret != 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : ERROR while Reseting The Display\n");
        return ExitCode_RST;
    }

    /// //Disable deep sleep Mode 
    //ret = Spi_Write(fd, EPD_COMMAND, DEEP_SLEEP_MODE, 1);
    //ret = Spi_Write(fd, EPD_DATA, 0x00, 1);
    ///

    // DRIVER_OUTPUT_CONTROL
    ret = Spi_Write(fd, EPD_COMMAND, &DRIVER_OUTPUT_CONTROL, 1);
    Log_Debug("INFO : EPD_Init_2in9 : DRIVER_OUTPUT_CONTROL_CMD Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : DRIVER_OUTPUT_CONTROL CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    delay_ms(100);
    //tmp = (EPD_2in9_HEIGHT - 1) & 0xFF;
    tmp = 295 % 256;
    ret = Spi_Write(fd, EPD_DATA, &tmp, 1);
    //ret = Spi_Write(fd, EPD_DATA, 295%256, 1);
    b_tfrd = ret;
    //tmp = ((EPD_2in9_HEIGHT - 1) >> 8) & 0xFF;
    tmp = 295 / 256;
    ret = Spi_Write(fd, EPD_DATA, &tmp, 1);
    //ret = Spi_Write(fd, EPD_DATA, 295/256, 1);
    b_tfrd += ret;
    tmp = 0x00;
    ret = Spi_Write(fd, EPD_DATA, &tmp, 1); // GD = 0; SM = 0; TB = 0;
    //EPD_Busy();
    b_tfrd += ret;
    Log_Debug("INFO : EPD_Init_2in9 : DRIVER_OUTPUT_CONTROL Data Bytes Tfr %d ; RetValue %d\n", b_tfrd, ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : DRIVER_OUTPUT_CONTROL Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }

    /// gate driving violtage 
    //ret = Spi_Write(fd, EPD_COMMAND, 0x03, 1);
    //ret = Spi_Write(fd, EPD_DATA, 0x00, 1);
    ///

     /// source driving violtage 
    //ret = Spi_Write(fd, EPD_COMMAND, 0x04, 1);
    //ret = Spi_Write(fd, EPD_DATA, 0x0A, 1);
    ///
    
    b_tfrd = 0; ret = -1;
    // BOOSTER_SOFT_START_CONTROL
    ret = Spi_Write(fd, EPD_COMMAND, &BOOSTER_SOFT_START_CONTROL, 1);
    Log_Debug("INFO : EPD_Init_2in9 : BOOSTER_SOFT_START_CONTROL_CMD Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : BOOSTER_SOFT_START_CONTROL CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }

    tmp = 0xD7;
    ret = Spi_Write(fd, EPD_DATA, &tmp, 1);
    b_tfrd = ret;
    tmp = 0xD6;
    ret = Spi_Write(fd, EPD_DATA, &tmp, 1);
    b_tfrd += ret;
    tmp = 0x9D;
    ret = Spi_Write(fd, EPD_DATA, &tmp, 1);
    //EPD_Busy();
    b_tfrd += ret;
    Log_Debug("INFO : EPD_Init_2in9 : BOOSTER_SOFT_START_CONTROL Data Bytes Tfr %d ; RetValue %d\n", b_tfrd, ret);
    if (ret <0) {
        Log_Debug("ERROR : EPD_Init_2in9 : BOOSTER_SOFT_START_CONTROL Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    
    b_tfrd = 0; ret = -1;
    // WRITE_VCOM_REGISTER
    ret = Spi_Write(fd, EPD_COMMAND, &WRITE_VCOM_REGISTER, 1);
    Log_Debug("INFO : EPD_Init_2in9 : WRITE_VCOM_REGISTER Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : WRITE_VCOM_REGISTER CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    tmp = 0xA8;
    ret = Spi_Write(fd, EPD_DATA, &tmp, 1); //VCOM 7C
    Log_Debug("INFO : EPD_Init_2in9 : WRITE_VCOM_REGISTER Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : WRITE_VCOM_REGISTER Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    //EPD_Busy();
    b_tfrd = 0; ret = -1;
    // SET_DUMMY_LINE_PERIOD
    ret = Spi_Write(fd, EPD_COMMAND, &SET_DUMMY_LINE_PERIOD, 1);
    Log_Debug("INFO : EPD_Init_2in9 : SET_DUMMY_LINE_PERIOD Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : SET_DUMMY_LINE_PERIOD CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    tmp = 0x1A;
    ret = Spi_Write(fd, EPD_DATA, &tmp, 1); //4 dummy lines per gate
    //ret = Spi_Write(fd, EPD_DATA, 0x16, 1);
    Log_Debug("INFO : EPD_Init_2in9 : SET_DUMMY_LINE_PERIOD Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : SET_DUMMY_LINE_PERIOD Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    EPD_Busy();
    b_tfrd = 0; ret = -1;
    // SET_GATE_TIME
    ret = Spi_Write(fd, EPD_COMMAND, &SET_GATE_TIME, 1);
    Log_Debug("INFO : EPD_Init_2in9 : SET_GATE_TIME Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : SET_GATE_TIME CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    tmp = 0x08;
    ret = Spi_Write(fd, EPD_DATA, &tmp, 1); //2us per line
    Log_Debug("INFO : EPD_Init_2in9 : SET_GATE_TIME Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : SET_GATE_TIME Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    EPD_Busy();
    b_tfrd = 0; ret = -1;

    // BORDER_WAVEFORM_CONTROL
    ret = Spi_Write(fd, EPD_COMMAND, &BORDER_WAVEFORM_CONTROL, 1);
    Log_Debug("INFO : EPD_Init_2in9 : BORDER_WAVEFORM_CONTROL Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : BORDER_WAVEFORM_CONTROL CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    tmp = 0x03;
    ret = Spi_Write(fd, EPD_DATA, &tmp, 1); //2us per line
    //ret = Spi_Write(fd, EPD_DATA, 0x33, 1);
    Log_Debug("INFO : EPD_Init_2in9 : BORDER_WAVEFORM_CONTROL Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : BORDER_WAVEFORM_CONTROL Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    //EPD_Busy();
    b_tfrd = 0; ret = -1;
    // DATA_ENTRY_MODE_SETTING
    ret = Spi_Write(fd, EPD_COMMAND, &DATA_ENTRY_MODE_SETTING, 1);
    Log_Debug("INFO : EPD_Init_2in9 : DATA_ENTRY_MODE_SETTING Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : DATA_ENTRY_MODE_SETTING CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    tmp = 0x03;
    ret = Spi_Write(fd, EPD_DATA, &tmp, 1); //2us per line
    //ret = Spi_Write(fd, EPD_DATA, 0x07, 1);
    Log_Debug("INFO : EPD_Init_2in9 : DATA_ENTRY_MODE_SETTING Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : DATA_ENTRY_MODE_SETTING Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    //------
    //EPD_Busy();
    //set the look-up table register
    b_tfrd = 0; ret = -1;
    // DATA_ENTRY_MODE_SETTING
    ret = Spi_Write(fd, EPD_COMMAND, &WRITE_LUT_REGISTER, 1); // WRITE_LUT_REGISTER
    Log_Debug("INFO : EPD_Init_2in9 : WRITE_LUT_REGISTER Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Init_2in9 : WRITE_LUT_REGISTER CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    //EPD_Busy();
    if (Mode == EPD_2in9_FULL) {
        for (uint8_t i = 0; i < 30; i++) {
            ret = Spi_Write(fd, EPD_DATA, &EPD_2IN9_lut_full_update[i], 1);
            b_tfrd++;
        }
        Log_Debug("INFO : EPD_Init_2in9 : WRITE_LUT_REGISTER EPD_2IN9_lut_full_update Data Bytes Tfr %d ; RetValue %d\n", b_tfrd, ret);
    }
    else if (Mode == EPD_2in9_PART) {
        for (uint8_t i = 0; i < 30; i++) {
            ret = Spi_Write(fd, EPD_DATA, &EPD_2IN9_lut_partial_update[i], 1);
            b_tfrd++;
        }
        Log_Debug("INFO : EPD_Init_2in9 : WRITE_LUT_REGISTER EPD_2IN9_lut_partial_update Data Bytes Tfr %d ; RetValue %d\n", b_tfrd, ret);
    }
    else {
        Log_Debug("ERROR : EPD_Init_2in9 : ONLY Mode is EPD_2IN9_FULL or EPD_2IN9_PART");
    }
    //EPD_Busy();

    //ret = Spi_Write(fd, EPD_COMMAND, DISPLAY_UPDATE_CONTROL_2, 1); // WRITE_LUT_REGISTER
    //ret = Spi_Write(fd, EPD_DATA, 0x04, 1);
    //ret = Spi_Write(fd, EPD_COMMAND, MASTER_ACTIVATION, 1);
    //delay_ms(1700);
    return ExitCode_Success;
}

//Clears the Display Orientation NOT Impl as of Now 
int EPD_Clear_2in9(int fd) {
    uint8_t W, H;
    int ret;
    int b_tfr = 0;
    uint8_t tmp = 0x55;

    W = (EPD_2in9_WIDTH % 8 == 0) ? (EPD_2in9_WIDTH / 8) : (EPD_2in9_WIDTH / 8 + 1);
    H = EPD_2in9_HEIGHT;

    EPD_Set_Display_Area_2in9(fd, 0, 0, EPD_2in9_WIDTH, EPD_2in9_HEIGHT);
    EPD_Busy();

    for (uint8_t j = 0; j < H; j++) {
        EPD_Set_Cursor_2in9(fd, 0, j);
        ret += Spi_Write(fd, EPD_COMMAND, &WRITE_RAM, 1);
        //Log_Debug("INFO : EPD_Clear_2in9 : WRITE_RAM Bytes Tfr %d ; RetValue %d\n", spi_transferredBytes, ret);
        if (ret < 0) {
            Log_Debug("ERROR : EPD_Clear_2in9 : WRITE_RAM CMD Bytes Tfr ERROR\n");
            return ExitCode_SPI_WriteData;
        }
        for (uint8_t i = 0; i < W; i++) {
            b_tfr += Spi_Write(fd, EPD_DATA, &tmp, 1);
            //Log_Debug("INFO : EPD_Clear_2in9 : WRITE_RAM Data Bytes Tfr %d ; RetValue %d\n", spi_transferredBytes, ret);
            if (b_tfr < 0) {
                Log_Debug("ERROR : EPD_Clear_2in9 : WRITE_RAM Data Bytes Tfr ERROR\n");
                return ExitCode_SPI_WriteData;
            }
        }
    }
    Log_Debug("INFO : EPD_Clear_2in9 : WRITE_RAM Command Bytes Tfr %d ; Data Tfr Bytes %d \n", ret, b_tfr);
    //EPD_Busy();

    EPD_Turn_On_Display_2in9(fd);
    delay_ms(1800);
    //EPD_Busy();
    return ExitCode_Success;
}

// Refreshes the epd Display  
int EPD_Refresh_2in9(int fd) {
    uint8_t W, H;
    int ret;
    int b_tfr = 0;
    uint8_t tmp;

    W = (EPD_2in9_WIDTH % 8 == 0) ? (EPD_2in9_WIDTH / 8) : (EPD_2in9_WIDTH / 8 + 1);
    H = EPD_2in9_HEIGHT;

    EPD_Set_Display_Area_2in9(fd, 0, 0, EPD_2in9_WIDTH, EPD_2in9_HEIGHT);
    EPD_Busy();
    tmp = 0x00;
    for (uint8_t j = 0; j < H; j++) {
        EPD_Set_Cursor_2in9(fd, 0, j);
        ret += Spi_Write(fd, EPD_COMMAND, &WRITE_RAM, 1);
        //Log_Debug("INFO : EPD_Clear_2in9 : WRITE_RAM Bytes Tfr %d ; RetValue %d\n", spi_transferredBytes, ret);
        if (ret < 0) {
            Log_Debug("ERROR : EPD_Clear_2in9 : WRITE_RAM CMD Bytes Tfr ERROR\n");
            return ExitCode_SPI_WriteData;
        }
        for (uint8_t i = 0; i < W; i++) {
            b_tfr += Spi_Write(fd, EPD_DATA, &tmp, 1);
            //Log_Debug("INFO : EPD_Clear_2in9 : WRITE_RAM Data Bytes Tfr %d ; RetValue %d\n", spi_transferredBytes, ret);
            if (b_tfr < 0) {
                Log_Debug("ERROR : EPD_Clear_2in9 : WRITE_RAM Data Bytes Tfr ERROR\n");
                return ExitCode_SPI_WriteData;
            }
        }
    }
    EPD_Busy();
    tmp = 0xFF;
    for (uint8_t j = 0; j < H; j++) {
        EPD_Set_Cursor_2in9(fd, 0, j);
        ret += Spi_Write(fd, EPD_COMMAND, &WRITE_RAM, 1);
        //Log_Debug("INFO : EPD_Clear_2in9 : WRITE_RAM Bytes Tfr %d ; RetValue %d\n", spi_transferredBytes, ret);
        if (ret < 0) {
            Log_Debug("ERROR : EPD_Clear_2in9 : WRITE_RAM CMD Bytes Tfr ERROR\n");
            return ExitCode_SPI_WriteData;
        }
        for (uint8_t i = 0; i < W; i++) {
            b_tfr += Spi_Write(fd, EPD_DATA, &tmp, 1);
            //Log_Debug("INFO : EPD_Clear_2in9 : WRITE_RAM Data Bytes Tfr %d ; RetValue %d\n", spi_transferredBytes, ret);
            if (b_tfr < 0) {
                Log_Debug("ERROR : EPD_Clear_2in9 : WRITE_RAM Data Bytes Tfr ERROR\n");
                return ExitCode_SPI_WriteData;
            }
        }
    }
    Log_Debug("INFO : EPD_Clear_2in9 : WRITE_RAM Command Bytes Tfr %d ; Data Tfr Bytes %d \n", ret, b_tfr);
    EPD_Busy();
    EPD_Turn_On_Display_2in9(fd);
    EPD_Busy();
    return ExitCode_Success;
}


int EPD_Display_Image_2in9(int fd, uint8_t *Image) {
    uint8_t W, H;
    int ret;
    int b_tfr = 0;

    W = (EPD_2in9_WIDTH % 8 == 0) ? (EPD_2in9_WIDTH / 8) : (EPD_2in9_WIDTH / 8 + 1);
    H = EPD_2in9_HEIGHT;

    uint32_t Addr = 0;

    // UDOUBLE Offset = ImageName;
    EPD_Set_Display_Area_2in9(fd, 0, 0, EPD_2in9_WIDTH, EPD_2in9_HEIGHT);
    //EPD_Busy();

    for (uint8_t j = 0; j < H; j++) {
        EPD_Set_Cursor_2in9(fd, 0, j);
        ret += Spi_Write(fd, EPD_COMMAND, &WRITE_RAM, 1);
        //Log_Debug("INFO : EPD_Display_Image_2in9 : WRITE_RAM Bytes Tfr %d ; RetValue %d\n", spi_transferredBytes, ret);
        if (ret < 0) {
            Log_Debug("ERROR : EPD_Display_Image_2in9 : WRITE_RAM CMD Bytes Tfr ERROR\n");
            return ExitCode_SPI_WriteData;
        }
        for (uint8_t i = 0; i < W; i++) {
            Addr = i + j * W;
            b_tfr += Spi_Write(fd, EPD_DATA, &Image[Addr], 1);
            //Log_Debug("INFO : EPD_Display_Image_2in9 : WRITE_RAM Data Bytes Tfr %d ; RetValue %d\n", spi_transferredBytes, ret);
            if (b_tfr < 0) {
                Log_Debug("ERROR : EPD_Display_Image_2in9 : WRITE_RAM Data Bytes Tfr ERROR\n");
                return ExitCode_SPI_WriteData;
            }
        }
    }
    EPD_Busy();

    Log_Debug("INFO : EPD_Display_Image_2in9 : WRITE_RAM Command Bytes Tfr %d ; Data Tfr Bytes %d \n", ret, b_tfr);
    EPD_Turn_On_Display_2in9(fd);
    EPD_Busy();
    return ExitCode_Success;
}

// Puts the Display in Sleep Mode 
int EPD_Sleep_2in9(int fd) {
    int ret;
    uint8_t tmp;
    ret = Spi_Write(fd, EPD_COMMAND, &DEEP_SLEEP_MODE, 1);
    Log_Debug("INFO : EPD_Sleep_2in9 : DEEP_SLEEP_MODE_CMD Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Sleep_2in9 : DEEP_SLEEP_MODE CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    tmp = 0x01;
    ret = Spi_Write(fd, EPD_DATA, &tmp, 1);
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
    uint8_t tmp;
    // DISPLAY_UPDATE_CONTROL_2
    ret = Spi_Write(fd, EPD_COMMAND, &DISPLAY_UPDATE_CONTROL_2, 1);
    Log_Debug("INFO : EPD_Turn_On_Display_2in9 : DISPLAY_UPDATE_CONTROL_2 Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Turn_On_Display_2in9 : DISPLAY_UPDATE_CONTROL_2 CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    tmp = 0xC4;
    ret = Spi_Write(fd, EPD_DATA, &tmp, 1);
    Log_Debug("INFO : EPD_Turn_On_Display_2in9 : DISPLAY_UPDATE_CONTROL_2 Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Turn_On_Display_2in9 : DISPLAY_UPDATE_CONTROL_2 Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }

    ret = Spi_Write(fd, EPD_COMMAND, &MASTER_ACTIVATION, 1);
    Log_Debug("INFO : EPD_Turn_On_Display_2in9 : MASTER_ACTIVATION Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Turn_On_Display_2in9 : MASTER_ACTIVATION CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }

    delay_ms(2000);

    ret = Spi_Write(fd, EPD_COMMAND, &TERMINATE_FRAME_READ_WRITE, 1);
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
        max_w = EPD_2in9_WIDTH;
        max_h = EPD_2in9_HEIGHT;
    }
    else if (orient == EPD_2in9_ORIENT_LANDSCAPE) {
        max_h = EPD_2in9_WIDTH;
        max_w = EPD_2in9_HEIGHT;
    }
    else Log_Debug("WARNING : ILLIGAL ORIENTATION TYPE USING DEFAULT ORIENTATION LANDSCAPE");
    max_w = EPD_2in9_HEIGHT;
    max_h = EPD_2in9_WIDTH;

    return ExitCode_Success;
}

// Set the Cursor Loc on the Display 
int EPD_Set_Cursor_2in9(int fd, uint8_t Xs, uint8_t Ys) {
    int ret;
    uint8_t tmp;
    // SET_RAM_X_ADDRESS_COUNTER
    ret = Spi_Write(fd, EPD_COMMAND, &SET_RAM_X_ADDRESS_COUNTER, 1);
    Log_Debug("INFO : EPD_Set_Cursor_2in9 : SET_RAM_X_ADDRESS_COUNTER Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Set_Cursor_2in9 : SET_RAM_X_ADDRESS_COUNTER CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    tmp = (Xs >> 3) & 0xFF;
    ret = Spi_Write(fd, EPD_DATA, &tmp, 1);
    Log_Debug("INFO : EPD_Set_Cursor_2in9 : SET_RAM_X_ADDRESS_COUNTER Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Set_Cursor_2in9 : SET_RAM_X_ADDRESS_COUNTER Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }

    // SET_RAM_Y_ADDRESS_COUNTER
    ret = Spi_Write(fd, EPD_COMMAND, &SET_RAM_Y_ADDRESS_COUNTER, 1);
    Log_Debug("INFO : EPD_Set_Cursor_2in9 : SET_RAM_Y_ADDRESS_COUNTER Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Set_Cursor_2in9 : SET_RAM_Y_ADDRESS_COUNTER CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }

    tmp = Ys & 0xFF;
    ret = Spi_Write(fd, EPD_DATA, &tmp, 1);
    Log_Debug("INFO : EPD_Set_Cursor_2in9 : SET_RAM_Y_ADDRESS_COUNTER Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Set_Cursor_2in9 : SET_RAM_Y_ADDRESS_COUNTER Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    tmp = (Ys >> 8) & 0xFF;
    ret = Spi_Write(fd, EPD_DATA, &tmp, 1);
    Log_Debug("INFO : EPD_Set_Cursor_2in9 : SET_RAM_Y_ADDRESS_COUNTER Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Set_Cursor_2in9 : SET_RAM_Y_ADDRESS_COUNTER Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    EPD_Busy();
    return ExitCode_Success;
}

// Sets the Display Window For Display 
int EPD_Set_Display_Area_2in9(int fd, uint8_t Xs, uint8_t Ys, uint8_t Xe, uint8_t Ye) {
    int ret;
    uint8_t tmp;
    // SET_RAM_X_ADDRESS_START_END_POSITION
    ret = Spi_Write(fd, EPD_COMMAND, &SET_RAM_X_ADDRESS_START_END_POSITION, 1);
    Log_Debug("INFO : EPD_Set_Display_Area_2in9 : SET_RAM_X_ADDRESS_START_END_POSITION Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Set_Display_Area_2in9 : SET_RAM_X_ADDRESS_START_END_POSITION CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    // Send Start Data
    tmp = (Xs >> 3) & 0xFF;
    ret = Spi_Write(fd, EPD_DATA, &tmp, 1);
    Log_Debug("INFO : EPD_Set_Display_Area_2in9 : SET_RAM_X_ADDRESS_START_END_POSITION Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Set_Display_Area_2in9 : SET_RAM_X_ADDRESS_START_END_POSITION Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    // Send End Data
    tmp = (Xe >> 3) & 0xFF;
    ret = Spi_Write(fd, EPD_DATA, &tmp, 1);
    Log_Debug("INFO : EPD_Set_Display_Area_2in9 : SET_RAM_X_ADDRESS_START_END_POSITION Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Set_Display_Area_2in9 : SET_RAM_X_ADDRESS_START_END_POSITION Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }

    // SET_RAM_Y_ADDRESS_START_END_POSITION
    ret = Spi_Write(fd, EPD_COMMAND, &SET_RAM_Y_ADDRESS_START_END_POSITION, 1);
    Log_Debug("INFO : EPD_Set_Display_Area_2in9 : SET_RAM_Y_ADDRESS_START_END_POSITION Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Set_Display_Area_2in9 : SET_RAM_Y_ADDRESS_START_END_POSITION CMD Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    // Send Start Data
    tmp = Ys & 0xFF;
    ret = Spi_Write(fd, EPD_DATA, &tmp, 1);
    Log_Debug("INFO : EPD_Set_Display_Area_2in9 : SET_RAM_Y_ADDRESS_START_END_POSITION Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Set_Display_Area_2in9 : SET_RAM_Y_ADDRESS_START_END_POSITION Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    // Send Start Data (H)
    tmp = (Ys >> 8) & 0xFF;
    ret = Spi_Write(fd, EPD_DATA, &tmp, 1);
    Log_Debug("INFO : EPD_Set_Display_Area_2in9 : SET_RAM_Y_ADDRESS_START_END_POSITION Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Set_Display_Area_2in9 : SET_RAM_Y_ADDRESS_START_END_POSITION Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }

    // Send end Data
    tmp = Ye & 0xFF;
    ret = Spi_Write(fd, EPD_DATA, &tmp, 1);
    Log_Debug("INFO : EPD_Set_Display_Area_2in9 : SET_RAM_Y_ADDRESS_START_END_POSITION Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Set_Display_Area_2in9 : SET_RAM_Y_ADDRESS_START_END_POSITION Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    // Send end Data (H)
    tmp = (Ye >> 8) & 0xFF;
    ret = Spi_Write(fd, EPD_DATA, &tmp, 1);
    Log_Debug("INFO : EPD_Set_Display_Area_2in9 : SET_RAM_Y_ADDRESS_START_END_POSITION Data Bytes Tfr %d\n", ret);
    if (ret < 0) {
        Log_Debug("ERROR : EPD_Set_Display_Area_2in9 : SET_RAM_Y_ADDRESS_START_END_POSITION Data Bytes Tfr ERROR\n");
        return ExitCode_SPI_WriteData;
    }
    EPD_Busy();
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
    ret = Spi_Write(fd, EPD_COMMAND, &SET_RAM_X_ADDRESS_START_END_POSITION, 1);
    ret = Spi_Write(fd, EPD_DATA, 0, 1);
    ret = Spi_Write(fd, EPD_DATA, 15, 1);
    ret = Spi_Write(fd, EPD_COMMAND, &SET_RAM_Y_ADDRESS_START_END_POSITION, 1);
    ret = Spi_Write(fd, EPD_DATA, 0, 1);
    ret = Spi_Write(fd, EPD_DATA, 0, 1);
    ret = Spi_Write(fd, EPD_DATA, 295&255, 1);
    ret = Spi_Write(fd, EPD_DATA, 295>>8, 1);

    //EPD_Set_Display_Area_2in9(fd, 0, 29, 295&255 , 295>>8);
    //EPD_Busy();
    */
    // Toggle B/W
    ret = EPD_Clear_2in9(fd);

    return ExitCode_Success;
}