// This sketch provide the functionality of OTA Firmware Upgrade
#include "WiFi.h"
#include "HttpsOTAUpdate.h"
#include "cert.h"
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
// This sketch shows how to implement HTTPS firmware update Over The Air.
// Please provide your WiFi credentials, https URL to the firmware image and the server certificate.

String FirmwareVer = "1.1";

static const char *ssid = "Tenda_6BED60";      // your network SSID (name of wifi network)
static const char *password = "sickqueen144";  // your network password

static const char *url = "https://github.com/alogusAdam/OTATest/raw/main/OTA_Test/ESP32_Blink/build/esp32.esp32.esp32/ESP32_Blink.ino.bin";  //state url of your firmware image
                                                                                                                                             //https://github.com/alogusAdam/OTATest/raw/main/OTA_Test/ESP32_Blink/build/esp32.esp32.esp32/ESP32_Blink.ino.bin

static HttpsOTAStatus_t otastatus;

void HttpEvent(HttpEvent_t *event) {
  switch (event->event_id) {
    case HTTP_EVENT_ERROR:
      Serial.println("Http Event Error");
      break;
    case HTTP_EVENT_ON_CONNECTED:
      Serial.println("Http Event On Connected");
      break;
    case HTTP_EVENT_HEADER_SENT:
      Serial.println("Http Event Header Sent");
      break;
    case HTTP_EVENT_ON_HEADER:
      Serial.printf("Http Event On Header, key=%s, value=%s\n", event->header_key, event->header_value);
      break;
    case HTTP_EVENT_ON_DATA:
      break;
    case HTTP_EVENT_ON_FINISH:
      Serial.println("Http Event On Finish");
      break;
    case HTTP_EVENT_DISCONNECTED:
      Serial.println("Http Event Disconnected");
      break;
  }
}

void setup() {

  Serial.begin(115200);
  Serial.print("Attempting to connect to SSID: ");
  WiFi.begin(ssid, password);

  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("Connected to ");
  Serial.println(ssid);
  /*
    HttpsOTA.onHttpEvent(HttpEvent);
    Serial.println("Starting OTA");
    HttpsOTA.begin(url, rootCACertificate); 

    Serial.println("Please Wait it takes some time ...");
    */
}

void loop() {

  if (FirmwareVersionCheck()) {
    HttpsOTA.onHttpEvent(HttpEvent);
    Serial.println("Starting OTA");
    HttpsOTA.begin(url, rootCACertificate);
    while (1) {
      otastatus = HttpsOTA.status();
      if (otastatus == HTTPS_OTA_SUCCESS) {
        Serial.println("Firmware written successfully. Restarting Device");
        ESP.restart();
      } else if (otastatus == HTTPS_OTA_FAIL) {
        Serial.println("Firmware Upgrade Fail");
        ESP.restart();
      }
    }
  }
  delay(5000);
}
