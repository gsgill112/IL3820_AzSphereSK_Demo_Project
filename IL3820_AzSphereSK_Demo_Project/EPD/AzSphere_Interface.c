//This AzSphere Interface lib for EPD Display by WaveShare electronics and is compatable for Good Display EPD Pannels also
// Currently added support for 2.9" EPD by WaveShare/ Goos Display 
// To Use the Lib, Only add this File and Define the Display 
//Ver 0.1

//Global Includes
#include <time.h>
#include <errno.h>

#include "../HighLevelCore/applibs_versions.h"

#include <applibs/spi.h>
#include <applibs/gpio.h>
#include <applibs/log.h>

#include "AzSphere_Interface.h"
#include "hw/avnet_mt3620_sk.h"


//Global Variables
SPIMaster_Config config;
ssize_t spi_transferredBytes = -1;
uint8_t readGPIO = 500;

//Initialiazinf GPIO FD for EPd Display 
rstfd = -1;
dcfd = -1;
busyfd = -1;

//Constants Defined for Custom Delay Generation
const struct timespec delay_1s = { 1, 0 };
const struct timespec delay_1us = { 0, 1000 };
const struct timespec delay_1ms = { 0, 1000000 };

/// <summary>
/// Termination codes for this application. These are used for the
/// application exit code.  They they must all be between zero and 255,
/// where zero is reserved for successful termination.
/// </summary>
typedef enum {
    ExitCode_Success = 0,

    ExitCode_TermHandler_SigTerm = 1,

    ExitCode_Gpio_Open = 2,
    ExitCode_Gpio_Read = 3,

    ExitCode_Init_InitConfig = 4,
    ExitCode_Init_OpenSpiMaster = 5,
    ExitCode_Init_SetBusSpeed = 6,
    ExitCode_Init_SetMode = 7,

    ExitCode_Spi_Read = 8,
    ExitCode_SPi_Write = 9

}ExitCode;

// Global Universal Functions Definitations
 
// Milisec delay generator
int delay_ms(uint8_t delay) {
    while (delay != 0) {
        nanosleep(&delay_1ms, NULL);
        --delay;
    }
    return ExitCode_Success;
}

// Microsec delay generator
int delay_us(uint8_t delay) {
    while (delay != 0) {
        nanosleep(&delay_1us, NULL);
        --delay;
    }
    return ExitCode_Success;
}

// Sec delay generator
int delay_s(uint8_t delay) {
    while (delay != 0) {
        nanosleep(&delay_1s, NULL);
        --delay;
    }
    return ExitCode_Success;
}


// Initialization of GPIO Reqd for EPD Ref above for Pin Layouts
int EPD_gpio_init(void) {
    int ret;

    // initialiazing Extra Pins required for Display operation.
    rstfd = GPIO_OpenAsOutput(RST_PIN, GPIO_OutputMode_PushPull, GPIO_Value_High);
    dcfd = GPIO_OpenAsOutput(DC_PIN, GPIO_OutputMode_PushPull, GPIO_Value_High);
    busyfd = GPIO_OpenAsInput(BUSY_PIN);
    if (rstfd == -1 || dcfd == -1 || busyfd == -1) {
        Log_Debug("ERROR : EPD_gpio_init : Could not open gpio Fd : Rstfd - %d , dcfd - %d , busyfd = %d\n", rstfd, dcfd, busyfd);
        return ExitCode_Gpio_Open;
    }

    // setting d/c to d mode
    ret = GPIO_SetValue(dcfd, GPIO_Value_High);
    if (ret != 0) {
        Log_Debug("ERROR : EPD_gpio_init : Could not set dcfd gpio\n");
        return ExitCode_Gpio_Open;
    }

    // Setting RST Pin to RESET Pos. 
    ret = GPIO_SetValue(rstfd, GPIO_Value_High);
    if (ret != 0) {
        Log_Debug("ERROR : EPD_gpio_init : Could not set RstFd gpio\n");
        return ExitCode_Gpio_Open;
    }

    return ExitCode_Success;
}

// Resets EPD Display 
int EPD_reset(void) {
    int ret; 

    ret = GPIO_SetValue(rstfd, GPIO_Value_High);
    if (ret != 0) {
        Log_Debug("ERROR : EPD_reset : Could not set RstFd gpio\n");
        return ExitCode_Gpio_Open;
    }
    delay_ms(200);
    ret = GPIO_SetValue(rstfd, GPIO_Value_Low);
    if (ret != 0) {
        Log_Debug("ERROR : EPD_reset : Could not set RstFd gpio\n");
        return ExitCode_Gpio_Open;
    }
    delay_ms(200);
    ret = GPIO_SetValue(rstfd, GPIO_Value_High);
    if (ret != 0) {
        Log_Debug("ERROR : EPD_reset : Could not set RstFd gpio\n");
        return ExitCode_Gpio_Open;
    }
    delay_ms(200);
    
    return ExitCode_Success;
}

// returns the SPI FD, for further use 
int AzSphere_Spi_Init(int fd) {
    int ret = SPIMaster_InitConfig(&config);
    if (ret != 0) {
        Log_Debug("ERROR : AzSphere_Spi_Init : SPIMaster_InitConfig = %d\n", ret);
        return ExitCode_Init_InitConfig;
    }
    config.csPolarity = SPI_ChipSelectPolarity_ActiveLow;
    fd = SPIMaster_Open(ILI9341_SK_ISU1_SPI, ILI9341_SPI_CS, &config);
    if (fd < 0) {
        Log_Debug("ERROR: SPIMaster_Open : %d\n", fd);
        return ExitCode_Init_OpenSpiMaster;
    }

    ret =  SPIMaster_SetBitOrder(fd, SPI_BitOrder_MsbFirst);

    int result = SPIMaster_SetBusSpeed(fd, 400000);
    //int result = SPIMaster_SetBusSpeed(fd, 10000000);
    if (result != 0) {
        Log_Debug("ERROR: SPIMaster_SetBusSpeed: %d\n", result);
        return ExitCode_Init_SetBusSpeed;
    }

    //result = SPIMaster_SetMode(fd, SPI_Mode_3);
    result = SPIMaster_SetMode(fd, SPI_Mode_0);
    if (result != 0) {
        Log_Debug("ERROR: SPIMaster_SetMode: %d\n", result);
        return ExitCode_Init_SetMode;
    }
    return fd;
}

//  Checks if EPD is Busy
int EPD_Busy(void) {
    int ret;
    Log_Debug("INFO : EPD_Busy : e-Paper busy \n");
    ret = GPIO_GetValue(busyfd, &readGPIO);
    if (ret != 0) {
        Log_Debug("ERROR : EPD_Busy : Failed to read GPIO Value : %d\n", ret);
        return ExitCode_Gpio_Read;
    }
    while (readGPIO == 1) {      //LOW: idle, HIGH: busy
        delay_us(100);
    }
    Log_Debug("INFO : EPD_Busy : e-Paper busy released \n");
    
    return ExitCode_Success;
}

// Reads the Value from SPI Bus : Vestigial function as MOSI is NC in the Display Module. 
// Havent Tested this Function !!
int Spi_Read(int fd, uint8_t reg, uint8_t *buf, uint32_t len) {

    if (((uint8_t)SPIMaster_WriteThenRead(fd, &reg, 1, buf, len)) < 0) {
        Log_Debug("ERROR: SPI Read : unable to WriteThenRead\n");
        return ExitCode_Spi_Read;
    }

    return ExitCode_Success;
}

//Writes Byte to Spi Bus 
int Spi_Write(int fd, uint8_t dc, uint8_t *buf, uint32_t len) {
    const size_t       transferCount = 1;
    SPIMaster_Transfer transfers;

    int result = SPIMaster_InitTransfers(&transfers, transferCount);
    if (result != 0) {
        Log_Debug("ERROR : Spi_Write : Error Initialiazing SPI Transfers%d \n", result);
        return -1;
    }

    transfers.flags = SPI_TransferFlags_Write;
    transfers.writeData = buf;
    transfers.length = len; //sizeof(buf);

    if (dc == 1) //if sending a Command
        GPIO_SetValue(dcfd, GPIO_Value_Low);

    spi_transferredBytes = SPIMaster_TransferSequential(fd, &transfers, transferCount);
    Log_Debug("INFO : Spi_Write : No of Bytes Transfered over SPIFD=%d : %d \n",fd, spi_transferredBytes);
    if (dc == 1) // changing to Data 
        GPIO_SetValue(dcfd, GPIO_Value_High);

    if (spi_transferredBytes == transfers.length) {
        //delay_ms(500);
        Log_Debug("INFO : Spi_Write : Leaving SPIFD=%d : %d \n", fd, spi_transferredBytes);
        return spi_transferredBytes;
    }

    return ExitCode_SPi_Write;
}