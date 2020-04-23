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

//#Defines required for proper Driver Configration
// Defines EPD Display connection Slot 
#define CLICK_SLOT 1

// Defines Which EPD are you using
#define EPD_2IN9  // using 2.9" EPd Display with IL3820 Driver 



int main(void)
{
    // This minimal Azure Sphere app repeatedly prints "Tick" then "Tock" to the debug console.
    // Use this app to test that device and SDK installation succeeded that you can build,
    // deploy, and debug an app with Visual Studio, and that you can deploy an app over the air,
    // per the instructions here: https://docs.microsoft.com/azure-sphere/quickstarts/qs-overview
    //
    // It is NOT recommended to use this as a starting point for developing apps; instead use
    // the extensible samples here: https://github.com/Azure/azure-sphere-samples

    while (true) {
        Log_Debug("High Level Core tick\n");
        delay_s(1);
        Log_Debug("High Level Core tock\n");
        delay_s(1);
    }
}
