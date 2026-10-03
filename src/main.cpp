#include "NimBLE_Beacon.h"

extern "C" void app_main() {
    // Testing Hardware Port and a few Initializations
    NVS_Flash_Initialize();
    NimBLE_Host_Stack_Initialize();
    GAPService_TO_GATTServer();
}