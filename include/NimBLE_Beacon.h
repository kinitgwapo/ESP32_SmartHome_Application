#ifndef NIMBLE_BEACON_H
#define NIMBLE_BEACON_H

#define ESP32_DEVICE_NAME "ESP32_Advertiser"
#define BLE_GAP_APPEARANCE_GENERIC_TAG 0x0200

#include <esp_err.h>
#include <nvs_flash.h>
#include <esp_log.h>

#include <nimble/nimble_port.h>

#include <services/gap/ble_svc_gap.h>

#include <host/ble_hs.h>

// Initializes Non-volatile Storage default partition
void NVS_Flash_Initialize(void);

// Initializes NimBLE Host Stack
void NimBLE_Host_Stack_Initialize(void);

// Expose some GAP Services to GATT Server
void GAPService_TO_GATTServer(void);

// Configuration for the NimBle Host Stack
void NimBLE_Host_Config_Init(void);

extern "C" {
// Library Function Declaration (Due to ESP-IDF Omitting the ble_store_config header file)
void ble_store_config_init(void);
}

#endif