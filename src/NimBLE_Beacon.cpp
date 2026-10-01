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