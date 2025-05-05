#include "Wifi_ota.h"
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

void Setup_OTA(){
  //iot47_ota_set_debug(false);
  iot47_wifi_ota_begin(&server);
  server.begin();
}

