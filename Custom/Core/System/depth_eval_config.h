/**
 * @file depth_eval_config.h
 * @brief Hard-coded Wi-Fi / MQTT settings for the FastDepth evaluation build.
 *
 * Edit the macros below, then rebuild the application. On boot these values
 * replace the stored network and MQTT settings for the running session.
 */
#ifndef DEPTH_EVAL_CONFIG_H
#define DEPTH_EVAL_CONFIG_H

#define DEPTH_EVAL_ENABLE 0

#define DEPTH_EVAL_WIFI_SSID       "CHANGE_ME_SSID"
#define DEPTH_EVAL_WIFI_PASSWORD   "CHANGE_ME_PASSWORD"
#define DEPTH_EVAL_MQTT_HOST       "192.168.1.100"
#define DEPTH_EVAL_MQTT_PORT       1883
#define DEPTH_EVAL_MQTT_USERNAME   ""
#define DEPTH_EVAL_MQTT_PASSWORD   ""
#define DEPTH_EVAL_MQTT_TOPIC      "ne302/depth"
#define DEPTH_EVAL_INTERVAL_MS     30000

#endif
