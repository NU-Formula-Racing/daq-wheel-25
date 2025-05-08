// thermopile_ap.cpp

#include "thermopile_ap.hpp"

ThermopileAP::ThermopileAP(const char* ssid,
                           const char* password,
                           Thermopile* sensors,
                           size_t sensorCount)
    : _ssid(ssid), _password(password), _sensors(sensors), _sensorCount(sensorCount) {}

void ThermopileAP::begin() {
    // 1) start the soft-AP
    WiFi.softAP(_ssid, _password);
    Serial.print("AP running, IP = ");
    Serial.println(WiFi.softAPIP());

    // 2) define our routes
    _server.on("/", [&]() { handleRoot(); });
    _server.on("/data", [&]() { handleData(); });
    _server.begin();
}

void ThermopileAP::handleClient() {
    _server.handleClient();
}

void ThermopileAP::handleRoot() {
    // inline HTML+JS: fetch /data → JSON[nums] → CSS gradient
    static const char* page = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <title>Thermopile Gradient</title>
  <style>
    body { font-family: sans-serif; text-align: center; margin: 20px; }
    #gradient { width: 100%; height: 50px; border: 1px solid #333; }
  </style>
  <script>
    function getColorForTemperature(t) {
      // map t from [min,max] to blue→red
      const min = -10, max = 100;
      let ratio = (t - min) / (max - min);
      ratio = Math.min(Math.max(ratio, 0), 1);
      const r = Math.round(ratio * 255);
      const b = 255 - r;
      return 'rgb(' + r + ',0,' + b + ')';
    }

    function fetchData() {
      fetch('/data')
        .then(resp => resp.json())
        .then(data => {
          const container = document.getElementById('gradient');
          // build CSS gradient stops
          const stops = data.map((t,i) => {
            const pos = (i/(data.length-1))*100;
            return getColorForTemperature(t) + ' ' + pos + '%';
          }).join(',');
          container.style.background = 'linear-gradient(to right,' + stops + ')';
        });
    }

    setInterval(fetchData, 1000);
    window.onload = fetchData;
  </script>
</head>
<body>
  <h1>Thermopile Sensor Gradient</h1>
  <div id="gradient"></div>
</body>
</html>
)rawliteral";

    _server.send(200, "text/html", page);
}

void ThermopileAP::handleData() {
    String json = "[";
    for (size_t i = 0; i < _sensorCount; i++) {
        float t = _sensors[i].getObjectTemperature();
        json += String(t, 2);
        if (i + 1 < _sensorCount) json += ',';
    }
    json += "]";
    _server.send(200, "application/json", json);
}
