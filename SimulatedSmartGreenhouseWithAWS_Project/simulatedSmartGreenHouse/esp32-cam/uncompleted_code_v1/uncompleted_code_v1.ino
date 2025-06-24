#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoMqttClient.h>
#include "esp_camera.h"
#include "Base64.h" 
#include <ArduinoJson.h>
#include "time.h"
#define LED_RED 12
#define LED_GREEN 13
#define LED_BLUE 14

// كاميرا config
camera_config_t config;

// WiFi credentials
const char* ssid = "Rana";
const char* password = "focus12345";

// AWS endpoint
const char* mqtt_server = "a2baozekzk2ue8-ats.iot.us-east-1.amazonaws.com";
const int mqtt_port = 8883;

// Certificates
// AWS certs
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
MIIDWTCCAkGgAwIBAgIUW+IppbpoVG9WLIfIui0991ga4gowDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI1MDYxMzA4MDY1
NFoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAOlJAfgAulmomOPi/mE1
t8xj+lZo623Sz2rMzCuGEkK1Ic7v1RVo1Ft+5ptOsg9mKVISjJ/Q7OApUpZM9D/r
9TAuWm/61Db4fpJhwyVrE0lBRKxk62rwLCdqQs/9A6aAWNSBYyllWzKWXNH6mPhC
QTW/fNQvUS5y767NCHgCjb/i/kJlIvwhBkMyVz3IV/xygvEL4cW1Eie6pyzWFBkJ
h4G42I248c/ZkhLMQQk0YOesUmzbyzYdsUMFSMBYeURalrr5mpuk0mh4l5wxBiDp
yOww7e4sAn7jH+34HueTo8EarrtVdsqoPKRAOd+tmzoBwCmmfgiM11IDU2YNnFb7
+KMCAwEAAaNgMF4wHwYDVR0jBBgwFoAUidqHDtEWSU/JZXrQyTCxw1rO/a8wHQYD
VR0OBBYEFHd8cEjxzTts7DXR7ZBqPdqNZOmKMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQAvOVYXlloxPISifmwlsU3pB9y9
dGoYOJGFLznOQBjmnbdnWtEGs2uARohorYXQA/tkbOyp7UtPswuthJIyn/guBrGC
fAz7qkXzhMh/qdTalKSz00dfsspVLI30YwIeQxem7hBYX4zr9NshLMFKALACDJ5F
BWYmxhfUGDr6AzN7K1Qvt4DvOENVmv3DDc5JENIDa3ms152P8+eoNDIecdUQ5Rxw
mj5s8DU1MFN6OFXVnq1bT0SoQcPTKGBx2VVuGUqFNvalDk4AT/5Hkj4UJ1h4+iKZ
OyjVU0TbYha7BYi2X6EV2VyMoPsldmLfLwmBMNUwNncV6nm1u9DtzY7oktl/
-----END CERTIFICATE-----

)KEY";

const char* private_pem_key = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEA6UkB+AC6WaiY4+L+YTW3zGP6VmjrbdLPaszMK4YSQrUhzu/V
FWjUW37mm06yD2YpUhKMn9Ds4ClSlkz0P+v1MC5ab/rUNvh+kmHDJWsTSUFErGTr
avAsJ2pCz/0DpoBY1IFjKWVbMpZc0fqY+EJBNb981C9RLnLvrs0IeAKNv+L+QmUi
/CEGQzJXPchX/HKC8QvhxbUSJ7qnLNYUGQmHgbjYjbjxz9mSEsxBCTRg56xSbNvL
Nh2xQwVIwFh5RFqWuvmam6TSaHiXnDEGIOnI7DDt7iwCfuMf7fge55OjwRquu1V2
yqg8pEA5362bOgHAKaZ+CIzXUgNTZg2cVvv4owIDAQABAoIBAQDLX7h1qrzK2S6g
zQ2TVa9sYmSl3EYSskvdAbmk3/cfbKWSsIcFaSRot5L3Jn4Y6qlvQXiTAyLesWes
XF47M8vHO0Hgib1kRvKhTtU/0782h84EtN5/le4IVpz0kdES4KWQbVQzjIca7Pby
oBOnqqsK3mtG2BFf+xd1Po35gdBtzR5boD0lAt9hZLrHkxinK/yk0cY4hctQxuB6
KsVfXkegL/gbVKQEMeAbt3z3V/VecvYdUgOqcKAWaLO13Ys+0xGsU2pH4KP4ea0E
U+9xeaUKc6YpQMXyb/1Wzo8uhEnnpZe/RFxaLTzeUwGZ6yUjgEKywVm3uN1VTLNh
/rJzbiRBAoGBAPZ7J1qkEzHnz2GRTNylJ3E6SCgcUjfXc3L4j1ySFCt5P+fA7u8x
UCAgHLqDhxbjp2Iq58gfFWlIxyGUjIYAV7ZcVgbxnJSKDKruI7wSiAUpwfEs4Jbj
tjgAZIG3hkSsnGGN7emUj+7XePPPKEipX/8gVPPfK2i4qbBH0LYeFk/xAoGBAPJL
ZGuvwlGwlP2Fx1bEszFBIGPq8pXRspm9s9eAM/7CyBGA0e9d0XdoRRQ73eR7OOKZ
Zdgb7tQVjbYxOwokdzqYfSOBsV7i9ScN6qVEkQigmejdngxE/CzIpa4jSN061Rqt
E7FQeFs53a+7q8eyRo2X7goNxJgSVCihd9Yb7mXTAoGATk6ACT75TjCcZR35clwR
9NoUVJHy2QTdC9jGCx3pxIoH8IQOJPXgw+QP3H2u7Gm1Iu4Ipz6g2csiYJvGLUhs
lWYuepRGL7bQIXn45mZoymMi15FvZODBYndVdxSsVqMUJ7YksMHfHH1eUu8smDEf
a3mFI9lx/Bo25udZ+pICerECgYEAjj2oZHe4MlDiBrukvNGZCQ9cvovNghqrKQpv
K9I9tBAReAUSOd3v/JOgCjLjHyEf0heP7IDpgmRs30qjiF7ECAhBXrjisfu3RAgt
enx4UIJIGKcSA83z5S6d5U8dlZLWTGTTLMM7BQNBm8snG4MTXEiXHBvdEBGiin/Q
ZEfderUCgYBIMFl188WUU6iS1YHJkbmGFlTzWWutP94BD1cMbnO9YA0FkTbD9dba
bmDiav7jKfbu04tNMv2Ru2Im2SpTzYEvEsNwp76mQCgtv5T9i7tJLN1caXpTSNgd
+gn8jfc+nIVCcpVkatfrbwevdrkg8SK+kA4Ss7HKCe1OOa6Q9h98RQ==
-----END RSA PRIVATE KEY-----
)KEY";


// WiFi and MQTT clients
WiFiClientSecure net;
MqttClient client(net);


// Topics
const char* imageTopic = "esp32-image";
const char* ledTopic = "led-color-topic";

// NTP Time server config
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 0;

void captureAndSendImage() {
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  Serial.println("captured");

  String encoded = base64::encode(fb->buf, fb->len);
  String jsonMessage = "{\"image\":\"" + encoded + "\"}";

  if (client.connected()) {
    client.beginMessage("esp32-image");
    client.print(jsonMessage);
    client.endMessage();
    Serial.println("Image sent.");
  } else {
    Serial.println("MQTT disconnected before sending image.");
  }

  esp_camera_fb_return(fb);
}


void messageReceived(int messageSize) {
  // استقبل الرسالة
  String message = "";
  while (client.available()) {
    message += (char)client.read();
  }

  Serial.print("Received message: ");
  Serial.println(message);

  // نفّذ بناءً على محتوى الرسالة
  setLED(message);
}

void setLED(String color) {
  // اطفي كل الليدات الأول
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, LOW);

  // شغّل اللي جالك
  if (color == "red") {
    digitalWrite(LED_RED, HIGH);
  }
  else if (color == "green") {
    digitalWrite(LED_GREEN, HIGH);
  }
  else if (color == "blue") {
    digitalWrite(LED_BLUE, HIGH);
  }
  else {
    Serial.println("Unknown color received!");
  }
}
void reconnect() {
  while (!client.connected()) {
    Serial.print("Reconnecting to AWS IoT Core...");
    if (client.connect(mqtt_server, mqtt_port)) {
      Serial.println("Reconnected.");
      // لازم نعمل subscribe تاني بعد reconnect
      client.onMessage(messageReceived);
      client.subscribe("led-color-topic");
    } else {
      Serial.print("Failed, error: ");
      Serial.println(client.connectError());
      delay(5000); // استنى 5 ثواني وجرّب تاني
    }
  }
}



void setup() {
  Serial.begin(115200);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  // Camera config
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
  
  if(psramFound()){
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_QQVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");

  // Time setup for SSL
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.print("Waiting for time sync");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("\nTime synchronized.");

  // Certificates setup
  net.setCACert(root_ca);
  net.setCertificate(certificate_pem_crt);
  net.setPrivateKey(private_pem_key);

  // Connect to AWS IoT MQTT broker
  Serial.print("Connecting to AWS IoT Core...");
  if (!client.connect(mqtt_server, mqtt_port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(client.connectError());
    while (1);
  }
  Serial.println("\nConnected to AWS IoT Core.");
  client.onMessage(messageReceived);
  client.subscribe("led-color-topic");
  captureAndSendImage();

}

unsigned long lastSendTime = 0;
const unsigned long interval = 1 * 60 * 1000; 
void loop() {
  
   // Ensure connection alive
  if (!client.connected()) {
  reconnect();
  }
  client.poll();
  unsigned long now = millis();
  if (now - lastSendTime > interval) {
    captureAndSendImage();
    lastSendTime = now;
  }
}
