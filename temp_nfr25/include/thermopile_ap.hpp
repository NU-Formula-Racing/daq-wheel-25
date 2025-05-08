#ifndef __THERMOPILE_AP_H__
#define __THERMOPILE_AP_H__

#include <WebServer.h>
#include <WiFi.h>

#include "thermopile.hpp"

class ThermopileAP {
   public:
    ThermopileAP(const char* ssid,
                 const char* password,
                 Thermopile* sensors,
                 size_t sensorCount);

    /// start the AP + HTTP server; call once in setup()
    void begin();

    /// pump the HTTP server; call continuously in loop()
    void handleClient();

   private:
    const char* _ssid;
    const char* _password;
    Thermopile* _sensors;
    size_t _sensorCount;
    WebServer _server{80};

    void handleRoot();
    void handleData();
};

#endif  // __THERMOPILE_AP_H__