import paho.mqtt.client as mqtt
import json
import time


# =========================
# MQTT CONFIG
# =========================

BROKER = "YOUR_MQTT_BROKER"
PORT = 1883
USERNAME = "YOUR_MQTT_USERNAME"
PASSWORD = "YOUR_MQTT_PASSWORD"

BASE_TOPIC = "homeassistant"


# ======================
# ACTUATOR DEVICE
# ======================
ACTUATOR_ID = "virtual_actuator_oulha001"
ACTUATOR_NAME = "Virtual Actuator oulha001"

# LED
LED_COMMAND = f"{BASE_TOPIC}/light/{ACTUATOR_ID}/led/set"
LED_STATE   = f"{BASE_TOPIC}/light/{ACTUATOR_ID}/led/state"
LED_CONFIG  = f"{BASE_TOPIC}/light/{ACTUATOR_ID}/led/config"

# Display
DISPLAY_COMMAND = f"{BASE_TOPIC}/light/{ACTUATOR_ID}/display/set"
DISPLAY_STATE   = f"{BASE_TOPIC}/light/{ACTUATOR_ID}/display/state"
DISPLAY_CONFIG  = f"{BASE_TOPIC}/light/{ACTUATOR_ID}/display/config"

# Brightness
BRIGHTNESS_COMMAND = f"{DISPLAY_COMMAND}/brightness"
BRIGHTNESS_STATE   = f"{DISPLAY_STATE}/brightness"

# Color
COLOR_COMMAND = f"{BASE_TOPIC}/light/{ACTUATOR_ID}/display/color/set"
COLOR_STATE   = f"{BASE_TOPIC}/light/{ACTUATOR_ID}/display/color/state"

# ======================
# SENSOR DEVICE
# ======================
SENSOR_ID = "virtual_multisensor_oulha001"
SENSOR_NAME = "Multisensor oulha001"

TVOC_STATE  = f"{BASE_TOPIC}/sensor/{SENSOR_ID}_tvoc/state"
TVOC_CONFIG = f"{BASE_TOPIC}/sensor/{SENSOR_ID}_tvoc/config"

ECO2_STATE  = f"{BASE_TOPIC}/sensor/{SENSOR_ID}_eco2/state"
ECO2_CONFIG = f"{BASE_TOPIC}/sensor/{SENSOR_ID}_eco2/config"

# ======================
# MQTT CLIENT
# ======================
client = mqtt.Client()
client.username_pw_set(USERNAME, PASSWORD)
client.connect(BROKER, PORT, 60)

def publish_discovery():

    # ------------------
    # LED
    # ------------------
    led_cfg = {
        "name": "LED oulha001",
        "unique_id": f"{ACTUATOR_ID}_led",
        "command_topic": LED_COMMAND,
        "state_topic": LED_STATE,
        "payload_on": "ON",
        "payload_off": "OFF",
        "device": {
            "identifiers": [ACTUATOR_ID],
            "name": ACTUATOR_NAME,
            "model": "M5StickC",
            "manufacturer": "M5Stack"
        }
    }

    # ------------------
    # DISPLAY (RGB + BRIGHTNESS)
    # ------------------
    display_cfg = {
        "name": "Display oulha001",
        "unique_id": f"{ACTUATOR_ID}_display",
        "command_topic": DISPLAY_COMMAND,
        "state_topic": DISPLAY_STATE,
        "brightness": True,
        "brightness_scale": 255,
        "brightness_command_topic": BRIGHTNESS_COMMAND,
        "brightness_state_topic": BRIGHTNESS_STATE,
        "rgb": True,
        "rgb_command_topic": COLOR_COMMAND,
        "rgb_state_topic": COLOR_STATE,
        "payload_on": "ON",
        "payload_off": "OFF",
        "device": {
            "identifiers": [ACTUATOR_ID],
            "name": ACTUATOR_NAME,
            "model": "M5StickC",
            "manufacturer": "M5Stack"
        }
    }

    # ------------------
    # TVOC SENSOR
    # ------------------
    tvoc_cfg = {
        "name": "TVOC oulha001",
        "state_topic": TVOC_STATE,
        "unit_of_measurement": "ppb",
        "device_class": "volatile_organic_compounds",
        "unique_id": f"{SENSOR_ID}_tvoc",
        "device": {
            "identifiers": [SENSOR_ID],
            "name": SENSOR_NAME,
            "model": "M5StickC + SGP30",
            "manufacturer": "M5Stack"
        }
    }

    # ------------------
    # eCO2 SENSOR
    # ------------------
    eco2_cfg = {
        "name": "eCO2 oulha001",
        "state_topic": ECO2_STATE,
        "unit_of_measurement": "ppm",
        "device_class": "carbon_dioxide",
        "unique_id": f"{SENSOR_ID}_eco2",
        "device": {
            "identifiers": [SENSOR_ID],
            "name": SENSOR_NAME,
            "model": "M5StickC + SGP30",
            "manufacturer": "M5Stack"
        }
    }

    # ======================
    # PUBLISH DISCOVERY
    # ======================
    client.publish(LED_CONFIG, json.dumps(led_cfg), retain=True)
    client.publish(DISPLAY_CONFIG, json.dumps(display_cfg), retain=True)
    client.publish(TVOC_CONFIG, json.dumps(tvoc_cfg), retain=True)
    client.publish(ECO2_CONFIG, json.dumps(eco2_cfg), retain=True)

    print("✅ MQTT Discovery published (LED, Display, TVOC, eCO2)")

# Run once
publish_discovery()
time.sleep(1)
client.disconnect()
