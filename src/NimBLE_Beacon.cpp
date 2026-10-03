#include "NimBLE_Beacon.h"

const char *NimBLE_Beacon_TAG = "NimBLE Beacon";

void NVS_Flash_Initialize(void) {
    esp_err_t nvs_result = nvs_flash_init();

    if(nvs_result == ESP_ERR_NVS_NO_FREE_PAGES || nvs_result == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_result = nvs_flash_init();
    }
    if(nvs_result != ESP_OK) {
        ESP_LOGE(NimBLE_Beacon_TAG, "Failed to Initialize default nvs partition. Error Code: %d", nvs_result);
        return;
    }
}

void NimBLE_Host_Stack_Initialize(void) {
    esp_err_t nimble_result = nimble_port_init();
    if(nimble_result != ESP_OK) {
        ESP_LOGE(NimBLE_Beacon_TAG, "Failed to Initialize Nimble Host Stack. Error Code: %d", nimble_result);
        return;
    }
}

void GAPService_TO_GATTServer(void) {
    ble_svc_gap_init();

    int gapsvc_result = ble_svc_gap_device_name_set(ESP32_DEVICE_NAME);
    if(gapsvc_result != 0) {
        ESP_LOGE(NimBLE_Beacon_TAG, "Failed to set Device Name. Error Code: %d", gapsvc_result);
        return;
    }

    gapsvc_result = ble_svc_gap_device_appearance_set(BLE_GAP_APPEARANCE_GENERIC_TAG);
    if(gapsvc_result != 0) {
        ESP_LOGE(NimBLE_Beacon_TAG, "Failed to set Device Apperance. Error Code: %d", gapsvc_result);
        return;
    }

    return;
}

static void NimBLE_Reset_Callback(int reason) {
    ESP_LOGI(NimBLE_Beacon_TAG, "NimBLE Stack Reset. Reason: %d", reason);
}

static void NimBLE_Sync_Callback(void) {

}

void NimBLE_Host_Config_Init(void) {
    // Callback functions to be used by NimBLE Host Stack
    ble_hs_cfg.reset_cb = NimBLE_Reset_Callback;
    ble_hs_cfg.sync_cb = NimBLE_Sync_Callback;
    ble_hs_cfg.store_status_cb = ble_store_util_status_rr;

    ble_store_config_init();
}