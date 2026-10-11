#include "NimBLE_Beacon.h"

extern "C" {
// Library Function Declaration (Due to ESP-IDF Omitting the ble_store_config header file)
void ble_store_config_init(void);
}

static void Start_Advertise(void);

inline static void address_format(char *btaddress_string, uint8_t address_value[]);
static void print_connection_descriptor(struct ble_gap_conn_desc *descriptor);

static int GAP_Event_Handler(struct ble_gap_event *event, void *arg);

static void NimBLE_Reset_Callback(int reason);
static void NimBLE_Sync_Callback(void);

static uint8_t btaddress_type;
static uint8_t btaddress_value[6] = {0};

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

void NimBLE_Host_Config_Init(void) {
    // Callback functions to be used by NimBLE Host Stack
    ble_hs_cfg.reset_cb = NimBLE_Reset_Callback;
    ble_hs_cfg.sync_cb = NimBLE_Sync_Callback;
    ble_hs_cfg.store_status_cb = ble_store_util_status_rr;

    ble_store_config_init();
}










static void Start_Advertise(void) {
    const char *name;
    struct ble_hs_adv_fields advertise_data = {};
    struct ble_hs_adv_fields response_data = {};
    struct ble_gap_adv_params advertise_parameters = {};
    uint8_t uri[] = {0x00,'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!'};

    advertise_data.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;

    name = ble_svc_gap_device_name();
    if(name == NULL) {
        name = ESP32_DEVICE_NAME;
    }
    advertise_data.name = (uint8_t *)name;
    advertise_data.name_len = strlen(name);
    advertise_data.name_is_complete = 1;

    advertise_data.tx_pwr_lvl = BLE_HS_ADV_TX_PWR_LVL_AUTO;
    advertise_data.tx_pwr_lvl_is_present = 1;

    advertise_data.appearance = BLE_GAP_APPEARANCE_GENERIC_TAG;
    advertise_data.appearance_is_present = 1;

    advertise_data.le_role = BLE_GAP_LE_ROLE_PERIPHERAL;
    advertise_data.le_role_is_present = 1;

    int result = ble_gap_adv_set_fields(&advertise_data);
    if(result != 0) {
        ESP_LOGE(NimBLE_Beacon_TAG, "Failed to set Advertising Data. Error Code: %d", result);
        return;
    }

    response_data.device_addr = btaddress_value;
    response_data.device_addr_type = btaddress_type;
    response_data.device_addr_is_present = 1;

    response_data.uri = uri;
    response_data.uri_len = sizeof(uri);

    response_data.adv_itvl = BLE_GAP_ADV_ITVL_MS(500);

    result = ble_gap_adv_rsp_set_fields(&response_data);
    if(result != 0) {
        ESP_LOGE(NimBLE_Beacon_TAG, "Failed to set scan response data. Error Code: %d", result);
    }

    advertise_parameters.conn_mode = BLE_GAP_CONN_MODE_UND;
    advertise_parameters.disc_mode = BLE_GAP_DISC_MODE_GEN;

    advertise_parameters.itvl_min = BLE_GAP_ADV_ITVL_MS(500);
    advertise_parameters.itvl_max = BLE_GAP_ADV_ITVL_MS(510);

    result = ble_gap_adv_start(btaddress_type, NULL, BLE_HS_FOREVER, &advertise_parameters, GAP_Event_Handler, NULL);
    if(result != 0) {
        ESP_LOGE(NimBLE_Beacon_TAG, "Failed to Start Advertising. Error Code: %d", result);
        return;
    }

    ESP_LOGI(NimBLE_Beacon_TAG, "Advertising Started!");
}

inline static void address_format(char *btaddress_string, uint8_t address_value[]) {
    sprintf(btaddress_string, "%02X:%02X:%02X:%02X:%02X:%02X", btaddress_value[0], btaddress_value[1], btaddress_value[2], btaddress_value[3], btaddress_value[4], btaddress_value[5]);
    ESP_LOGI(NimBLE_Beacon_TAG, "Device Address: %s", btaddress_string);
}

static void print_connection_descriptor(struct ble_gap_conn_desc *descriptor) {
    char address_string[18] = {0};

    ESP_LOGI(NimBLE_Beacon_TAG, "Connection Handle: %d", descriptor->conn_handle);

    address_format(address_string, descriptor->our_id_addr.val);
}






static int GAP_Event_Handler(struct ble_gap_event *event, void *arg) {
    struct ble_gap_conn_desc descriptor;

    switch(event->type) {
        case BLE_GAP_EVENT_CONNECT:
            if(event->connect.status == 0) {
                int result = ble_gap_conn_find(event->connect.conn_handle, &descriptor);

                if(result != 0) {
                    ESP_LOGE(NimBLE_Beacon_TAG, "Failed to find connection handle. Error Code: %d", result);
                    return;
                }

                struct ble_gap_upd_params update_params = {
                    .itvl_min = descriptor.conn_itvl,
                    .itvl_max = descriptor.conn_itvl,
                    .latency = 3,
                    .supervision_timeout = descriptor.supervision_timeout
                };

                result = ble_gap_update_params(event->connect.conn_handle, &update_params);
                if(result != 0) {
                    ESP_LOGE(NimBLE_Beacon_TAG, "Failed to update connection parameters. Error Code: %d", result);
                    return;
                }

            } else {
                ESP_LOGI(NimBLE_Beacon_TAG, "Connection Status: %d", event->connect.status);
                Start_Advertise();
            }
            break;
    }
}










static void NimBLE_Reset_Callback(int reason) {
    ESP_LOGI(NimBLE_Beacon_TAG, "NimBLE Stack Reset. Reason: %d", reason);
}

static void NimBLE_Sync_Callback(void) {
    char btaddress_string[18] = {0};

    int btaddress_result = ble_hs_util_ensure_addr(0);
    if(btaddress_result != 0) {
        ESP_LOGE(NimBLE_Beacon_TAG, "Device does not have any available Bluetooth Address!");
        return;
    }

    btaddress_result = ble_hs_id_infer_auto(0, &btaddress_type);
    if(btaddress_result != 0) {
        ESP_LOGE(NimBLE_Beacon_TAG, "Failed to Infer Address Type. Error Code: %d", btaddress_result);
        return;
    }

    btaddress_result = ble_hs_id_copy_addr(btaddress_type, btaddress_value, NULL);
    if(btaddress_result != 0) {
        ESP_LOGE(NimBLE_Beacon_TAG, "Failed to Copy Device address. Error Code: %d", btaddress_result);
        return;
    }

    // Arrange Address Format
    address_format(btaddress_string, btaddress_value);

    Start_Advertise();
}