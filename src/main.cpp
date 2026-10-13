//DO NOT EDIT (START)/
#include <M5Unified.h>
#include <M5ModuleDisplay.h>
//DO NOT EDIT (END)/

// ✅ مكاتب ضرورية
#include <WiFi.h>
#include <PubSubClient.h>
#include "M5_TVOC.h"  // مكتبة sensor

// 🟢 WiFi & MQTT

const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

const char* MQTT_SERVER = "YOUR_MQTT_SERVER";
const int MQTT_PORT = 1883;
const char* MQTT_USER = "YOUR_MQTT_USER";
const char* MQTT_PASSWORD_MQTT = "YOUR_MQTT_PASSWORD";


WiFiClient espClient;
PubSubClient client(espClient);

// ✅ Sensor TVOC
M5_TVOC tvocSensor;

// 🟢 MQTT Topics ديال display و LED
const char* LED_CMD_TOPIC      = "homeassistant/light/virtual_actuator_oulha001/led/set";
const char* LED_STATE_TOPIC    = "homeassistant/light/virtual_actuator_oulha001/led/state";

const char* DISP_CMD_TOPIC     = "homeassistant/light/virtual_actuator_oulha001/display/set";
const char* DISP_STATE_TOPIC   = "homeassistant/light/virtual_actuator_oulha001/display/state";

const char* BRIGHTNESS_CMD_TOPIC = "homeassistant/light/virtual_actuator_oulha001/display/set/brightness";
const char* BRIGHTNESS_STATE_TOPIC = "homeassistant/light/virtual_actuator_oulha001/display/state/brightness";

const char* COLOR_CMD_TOPIC    = "homeassistant/light/virtual_actuator_oulha001/display/color/set";
const char* COLOR_STATE_TOPIC  = "homeassistant/light/virtual_actuator_oulha001/display/color/state";

// ✅ MQTT topics ديال sensor
const char* TVOC_TOPIC  = "homeassistant/sensor/virtual_multisensor_oulha001_tvoc/state";
const char* ECO2_TOPIC  = "homeassistant/sensor/virtual_multisensor_oulha001_eco2/state";

//DO NOT EDIT (START)/
void callback(char *topic, byte *message, unsigned int length);
//DO NOT EDIT (END)/

void setup()
{
    //DO NOT EDIT (START)/
    Serial.begin(115200);
    auto cfg = M5.config();
    cfg.external_display.module_display = true;
    M5.begin(cfg);
    M5.Display.fillScreen(ORANGE);
    pinMode(10, OUTPUT);
    digitalWrite(10, LOW);
    //DO NOT EDIT (END)/

    // ✅ Connect WiFi
    Serial.println("Connecting to WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\nWiFi connected!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    // ✅ Connect MQTT
    client.setServer(MQTT_SERVER, MQTT_PORT);
    client.setCallback(callback);

    Serial.println("Connecting to MQTT...");
    while (!client.connected()) {
        if (client.connect("oulha001_client", MQTT_USER, MQTT_PASSWORD_MQTT)) {
            Serial.println("MQTT connected!");
        } else {
            Serial.print("Failed, rc=");
            Serial.println(client.state());
            delay(1500);
        }
    }

    delay(200);

    // Subscriptions
    client.subscribe(LED_CMD_TOPIC);
    client.subscribe(DISP_CMD_TOPIC);
    client.subscribe(BRIGHTNESS_CMD_TOPIC);
    client.subscribe(COLOR_CMD_TOPIC);
    Serial.println("Subscribed to actuator topics!");

    // ✅ Init TVOC sensor
    if (!tvocSensor.begin()) {
        Serial.println("TVOC sensor not found!");
    } else {
        Serial.println("TVOC sensor initialized.");
    }
}

void loop()
{
    //DO NOT EDIT (START)/
    M5.update();
    //DO NOT EDIT (END)/
    client.loop();

    static unsigned long lastSensorTime = 0;

    if (millis() - lastSensorTime > 5000) {
        lastSensorTime = millis();

        // ✅ قراءة TVOC و eCO₂
        tvocSensor.update();
        int tvoc = tvocSensor.getTVOC();
        int eco2 = tvocSensor.getCO2();

        Serial.printf("TVOC: %d ppb, eCO2: %d ppm\n", tvoc, eco2);

        // ✅ إرسال MQTT
        client.publish(TVOC_TOPIC, String(tvoc).c_str());
        client.publish(ECO2_TOPIC, String(eco2).c_str());

        // ✅ عرض جودة الهواء على الشاشة
        String msg;
        uint16_t color;

        if (eco2 > 1000 || tvoc > 300) {
            msg = "Luft schlecht!\nBitte lueften!";
            color = M5.Display.color565(255, 0, 0);  // أحمر
        } else {
            msg = "Luft gut\nFenster zu.";
            color = M5.Display.color565(0, 200, 0);  // أخضر
        }

        M5.Display.fillScreen(color);
        M5.Display.setTextColor(WHITE);
        M5.Display.setTextSize(2);
        M5.Display.setCursor(10, 20);
        M5.Display.println(msg);
    }
}

void callback(char *topic, byte *message, unsigned int length)
{
    //DO NOT EDIT (START)/
    Serial.print("Message arrived on topic: ");
    Serial.println(topic);
    Serial.print(". Message: ");
    String messagePayload;
    for (int i = 0; i < length; i++) {
        Serial.print((char)message[i]);
        messagePayload += (char)message[i];
    }
    Serial.println();
    //DO NOT EDIT (END)/

    // ✅ نفس الكود القديم ديالك بلا تغيير
    if (String(topic) == LED_CMD_TOPIC) {
        if (messagePayload == "ON") {
            digitalWrite(10, LOW);
            client.publish(LED_STATE_TOPIC, "ON");
        } else {
            digitalWrite(10, HIGH);
            client.publish(LED_STATE_TOPIC, "OFF");
        }
    }

    if (String(topic) == DISP_CMD_TOPIC) {
        static int lastBrightness = 200;
        if (messagePayload == "ON") {
            M5.Display.setBrightness(lastBrightness);
            client.publish(DISP_STATE_TOPIC, "ON");
            client.publish(BRIGHTNESS_STATE_TOPIC, String(lastBrightness).c_str());
        } else {
            M5.Display.setBrightness(0);
            client.publish(DISP_STATE_TOPIC, "OFF");
            client.publish(BRIGHTNESS_STATE_TOPIC, "0");
        }
    }

    if (String(topic) == BRIGHTNESS_CMD_TOPIC) {
        int bright = messagePayload.toInt();
        bright = constrain(bright, 0, 255);
        M5.Display.setBrightness(bright);
        if (bright > 0) {
            static int lastBrightness = 200;
            lastBrightness = bright;
        }
        client.publish(BRIGHTNESS_STATE_TOPIC, String(bright).c_str());
    }

    if (String(topic) == COLOR_CMD_TOPIC) {
        int r, g, b;
        sscanf(messagePayload.c_str(), "%d,%d,%d", &r, &g, &b);
        uint16_t color565 = M5.Display.color565(r, g, b);
        M5.Display.fillScreen(color565);
        client.publish(COLOR_STATE_TOPIC, messagePayload.c_str());
    }
}