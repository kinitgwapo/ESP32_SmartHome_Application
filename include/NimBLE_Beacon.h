#ifndef NIMBLE_BEACON_H
#define NIMBLE_BEACON_H

#include <esp_err.h>
#include <nvs_flash.h>
#include <esp_log.h>
#include <nimble/nimble_port.h>

// Initializes Non-volatile Storage default partition
void NVS_Flash_Initialize(void);

// Initializes NimBLE Host Stack
void NimBLE_Host_Stack_Initialize(void);

#endif