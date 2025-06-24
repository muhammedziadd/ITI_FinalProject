#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoMqttClient.h>
#include "base64.h"
#include "esp_camera.h"
#include "time.h"

// WiFi creds
const char* ssid = "Rana";
const char* password = "focus12345";

// AWS IoT Core Endpoint
const char* mqtt_server = "a2baozekzk2ue8-ats.iot.us-east-1.amazonaws.com";

// Certificates
const char* root_ca = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

const char* certificate_pem_crt = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUXQn5bEJRrLG6KfMb77fskGULqmIwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI1MDYxMTIzMDA1
NloXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMKtiFe4j0gRjpA+VFB5
p8vtNCtAlQqybTjPs45qURIxhY+hUpREjDMrpID0W3ClIno+61uFZYmbfoRBb3a/
St3Xn9tlpO1R0K0htZyVz5ecdHM9v5unh2uUxqQmYXfkq4o7c6UiV89V9ruRjVXM
F8IJBMS7TpuH1L5sTjPSTpuZ67xK8yGKbjUFusXcctlfN8doKOUAC078KA1iHx/Q
06mAhtf0Kf7F6G9eb/HsOKlPq+pkMkjEw2b8Nw8W3uWDI32JFYx5FUnlQ5kxZ2Yl
AdxNvGwuVJxp0C5CVUhFYnGKPjq2y7ZYYGTRRSw2FvJLIlnEIlEpHgzSbsqkMPL3
edUCAwEAAaNgMF4wHwYDVR0jBBgwFoAUbtnqOQO+Lp2JFiImSmiDlSOD9lUwHQYD
VR0OBBYEFCj2zsllzJCkd/kp0o+P/jJiSlPuMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQBkkLbkxaQb3+ZPLh28q3naMlzb
MEueKEIMIs1Kjj5lg9QVuQZXXk97lytYhnRaWn6LN6CNg3aTOq4gIh+uZCXH0XaZ
311Smr1rts7vakmGGavHbRPhhzVdd4lnFY5T160nja0KuJe2kWHbLayAc0PAgjU6
jvME8eFJ7Fv+41W2hxrykcQSDbr2VXMa0+Hq53m92gqqJucWyoiInUQbf4cJfxVy
mSZVIXzPgFeSFEf0MCGKRej3XcIxp89asZrUyxry5GRCrskFntdXjOmrOF2Jub5P
/JZ2SaOazd8KnOyXmfzans6f/tu1IGYvo0cw6t0ZIovawLoIrrf8qkSv9Tnz
-----END CERTIFICATE-----
)KEY";

const char* private_pem_key = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEogIBAAKCAQEAwq2IV7iPSBGOkD5UUHmny+00K0CVCrJtOM+zjmpREjGFj6FS
lESMMyukgPRbcKUiej7rW4VliZt+hEFvdr9K3def22Wk7VHQrSG1nJXPl5x0cz2/
m6eHa5TGpCZhd+SrijtzpSJXz1X2u5GNVcwXwgkExLtOm4fUvmxOM9JOm5nrvErz
IYpuNQW6xdxy2V83x2go5QALTvwoDWIfH9DTqYCG1/Qp/sXob15v8ew4qU+r6mQy
SMTDZvw3Dxbe5YMjfYkVjHkVSeVDmTFnZiUB3E28bC5UnGnQLkJVSEVicYo+OrbL
tlhgZNFFLDYW8ksiWcQiUSkeDNJuyqQw8vd51QIDAQABAoIBABod3aSnBdHNSSrg
sb76EyCwecczqsr8lNqCQrxgVu/x5NEX+1QYvhY/TJpgLcQ24yEzP6/PfYewbJCh
lqp/jzOjsbZlNHMB/tS7KPoQvwC/5H9xUTs4hlpQYzWyUHEdJEYvI5yLtuwBxE/1
fth7wNQvEYyy9CC5YDkLnpIlLJaDG85YAqFQq/gFOnGu+taAOtbjmrB14P1yaq2/
thTdeYHdO3wJ+mhViharIH2esTvfgR3tAT/3e5Y6IWIWqEkNPV2tjvG6e/kui3WG
yEuYmM95eddsmMPjONAqTva+0Tcjsx1WsQyj7QzZegoaFNky9oCt625GzsS+JtCY
b1ESGCECgYEA4S2eKKem9rptdpi9JRj1y1X4b2JbbFTD9Fjolv0C3i5CBh9xehje
nloeonJD+So+h4AZY7J40xdQ9sM+IzHuUqcOw6IhtNQtdfVvPLtjyR6dfwtj1W4T
GsUXNQyVNjYO+s5SxWBypGbHJcJ7FmrAeopmtsOGOEQmK5lzzAp7EUcCgYEA3VMq
FXGjuHLfeVFrV9diKdkjvZW6PfpacLrtbIQRyqPP7xjJx1uKCxEjLPtfI3gOquVW
tfvWM3beglP/kJ3B0a9CR6/U/3NQtYdGTnhp+WElIwee7oMDLfl/8Op1AeWd59ke
44fVnMGErlVYRDgdanDlVjvPPfbibwVrGO0aigMCgYBwC2ThUdqwdj81ffbP2MWl
Mdf2hHh9RQhkM0E4sBiXOWk85rRuTqWnIayQpE7SUoTeo4z+oLWiQ73Tq37lwFiX
4+xix/ABrHoacc+x5oRWdNdleeNpNeZnsZ5ElUhBu1Bk1/kzoTQqPcHmNglNND0m
PGuzkULeCHr9wtNtueFWzQKBgDojAVf2rNnjmt4kS/0b/5Yciq3fKbfkDmO0Rm+N
1ZqUIyecLEyot/8mNkY3lYscm0W6zmcpSt/j5pj3dZdISURKNC/eeSXM5bN5YDd2
m+e84+Nqug3O7kPiWQAwpiGkbjEbsGh2It91WePtWS06TEfMdgGL3mlZc6tUEgjP
J3x/AoGADGRfMbTexvVs0zzocaXU80QwprnuZgPfF6Zbv49i+6PiogV53RiZjQTS
C2CDm+WAR7xXYdi7AIeZqYPlgT0XdFwoRBzrR3KSAVtUO2QR69nutmtytjKessID
jwf7lBnJdcPY0rdVVFj6kBvJj8NcIXn/18bY89KrcbX9/w/Qegs=
-----END RSA PRIVATE KEY-----
)KEY";

// MQTT Topics
const char* publish_topic = "esp32-image";
const char* led_topic = "led-color-topic";

WiFiClientSecure net;
MqttClient client(net);

// LED Pins
const int RED_LED_PIN = 12;
const int GREEN_LED_PIN = 13;
const int BLUE_LED_PIN = 15;

String currentColor = "GREEN";

void setLEDColor(const String& color) {
  digitalWrite(RED_LED_PIN, (color == "RED") ? HIGH : LOW);
  digitalWrite(GREEN_LED_PIN, (color == "GREEN") ? HIGH : LOW);
  digitalWrite(BLUE_LED_PIN, (color == "BLUE") ? HIGH : LOW);
  currentColor = color;
}

void onMqttMessage(int messageSize) {
  String topic = client.messageTopic();
  String payload;
  while (client.available()) {
    payload += (char)client.read();
  }
  Serial.print("Received on topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  Serial.println(payload);
  payload.replace("\"", "");
  if (topic == led_topic) {
    if (payload == "RED" || payload == "GREEN" || payload == "BLUE") {
      setLEDColor(payload);
    }
  }
}

void syncTime() {
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println(" done.");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.println(asctime(&timeinfo));
}

void connectAWS() {
  net.setCACert(root_ca);
  net.setCertificate(certificate_pem_crt);
  net.setPrivateKey(private_pem_key);

  Serial.print("Connecting to AWS IoT...");
  int retryCount = 0;
  while (!client.connect("esp32cam") && retryCount < 5) {
    Serial.print(".");
    delay(1000);
    retryCount++;
  }

  if (!client.connected()) {
    Serial.println("\nFailed to connect to AWS IoT after retries");
    while (1) delay(5000);
  }

  Serial.println("\nConnected to AWS IoT");

  client.onMessage(onMqttMessage);
  client.subscribe(led_topic);
}

void setupCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 5;
  config.pin_d1 = 18;
  config.pin_d2 = 19;
  config.pin_d3 = 21;
  config.pin_d4 = 36;
  config.pin_d5 = 39;
  config.pin_d6 = 34;
  config.pin_d7 = 35;
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 25;
  config.pin_href = 23;
  config.pin_sscb_sda = 26;
  config.pin_sscb_scl = 27;
  config.pin_pwdn = 32;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera init failed");
    while (1) delay(1000);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);

  setLEDColor("GREEN");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");

  setupCamera();
  syncTime();
  connectAWS();
}

void loop() {
  client.poll();
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    delay(1000);
    return;
  }

  String encodedImage = base64::encode(fb->buf, fb->len);
  esp_camera_fb_return(fb);

  String payload = "{\"image\":\"" + encodedImage + "\"}";

  client.beginMessage(publish_topic);
  client.print(payload);
  client.endMessage();

  Serial.println("Published image payload");

  unsigned long waitUntil = millis() + 300000;
  while (millis() < waitUntil) {
    client.poll();
    delay(100);
  }
}
