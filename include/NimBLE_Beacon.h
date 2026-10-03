#ifndef NIMBLE_BEACON_H
#define NIMBLE_BEACON_H

#define ESP32_DEVICE_NAME "ESP32_Advertiser"
#define BLE_GAP_APPEARANCE_GENERIC_TAG 0x0200
#define BLE_GAP_LE_ROLE_PERIPHERAL 0x00

#include <esp_err.h>
#include <nvs_flash.h>
#include <esp_log.h>

#include <nimble/nimble_port.h>

#include <services/gap/ble_svc_gap.h>

#include <host/ble_hs.h>

#include "host/util/util.h"

#include "FreeRTOS_Objects.h"

// Initializes Non-volatile Storage default partition
void NVS_Flash_Initialize(void);

// Initializes NimBLE Host Stack
void NimBLE_Host_Stack_Initialize(void);

// Expose some GAP Services to GATT Server
void GAPService_TO_GATTServer(void);

// Configuration for the NimBle Host Stack
void NimBLE_Host_Config_Init(void);

#endif