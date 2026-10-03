#include "FreeRTOS_Objects.h"

const char *FreeRTOS_Objects_TAG = "FreeRTOS Objects";

static void Nimble_Host_Task(void *pvParameters) {
    ESP_LOGI(FreeRTOS_Objects_TAG, "Started NimBLE Host Task!");
    nimble_port_run();
    nimble_port_freertos_deinit();
}

void FreeRTOS_Task_Initialize(void) {
    nimble_port_freertos_init(Nimble_Host_Task);
}