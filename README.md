# ESP32_SmartHome_Application

## ESP-IDF BLE Configuration Progress

### DONE:

#### BLE Beacon Example

- NVS Initialization
- NimBLE Host Stack Initialization
- Expose some GAP Service to GATT Server
- Configured callback functions for NimBLE Host Stack
- FreeRTOS NimBLE Host Stack Task Initialization
- Unconnectable ESP32 Device Advertising with URI Message

#### BLE Connection Example

- Connectable ESp32 Device Advertising
- Callback function for BLE GAP Connect/Disconnect/Update (Update not used)
- Added LED for Connection Status

<img width="1509" height="854" alt="LED Wiring Diagram" src="https://github.com/user-attachments/assets/d3f55785-29f0-4d3a-98ed-af33fe030044" />

```
{
  "version": 1,
  "author": "Kenneth O. Restauro",
  "editor": "wokwi",
  "parts": [
    { "type": "board-esp32-devkit-c-v4", "id": "esp", "top": 0, "left": 0, "attrs": {} },
    {
      "type": "wokwi-led",
      "id": "led1",
      "top": -3.6,
      "left": 196.2,
      "attrs": { "color": "red", "flip": "1" }
    },
    {
      "type": "wokwi-resistor",
      "id": "r1",
      "top": 32.75,
      "left": 124.8,
      "attrs": { "value": "220" }
    },
    {
      "type": "wokwi-text",
      "id": "text1",
      "top": 9.6,
      "left": 240,
      "attrs": { "text": "Connection Status:\n          On = Connected\n          Off = Disconnected" }
    }
  ],
  "connections": [
    [ "esp:TX", "$serialMonitor:RX", "", [] ],
    [ "esp:RX", "$serialMonitor:TX", "", [] ],
    [ "r1:2", "led1:A", "red", [ "v0" ] ],
    [ "r1:1", "esp:23", "red", [ "v0" ] ],
    [ "led1:C", "esp:GND.3", "black", [ "v0" ] ]
  ],
  "dependencies": {}
}
```

## References:

BLE Guide: https://github.com/espressif/esp-idf/tree/master/examples/bluetooth/ble_get_started/nimble

BLE Beacon Guide: https://github.com/espressif/esp-idf/tree/master/examples/bluetooth/ble_get_started/nimble/NimBLE_Beacon

BLE Connection Guide: https://github.com/espressif/esp-idf/tree/master/examples/bluetooth/ble_get_started/nimble/NimBLE_Connection
