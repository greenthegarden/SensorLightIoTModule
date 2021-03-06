#ifndef SENSORLIGHTIOTMODULE_MQTTCONFIG_H_
#define SENSORLIGHTIOTMODULE_MQTTCONFIG_H_


#include <PubSubClient.h>


// MQTT parameters
IPAddress mqttServerAddr(192, 168, 1, 55);        // Pi eth0 interface                      
char* mqttClientId                                = "sensorlight";
int mqttPort                                      = 1883;
//#define MQTT_MAX_PACKET_SIZE                      168
//#define MQTT_KEEPALIVE                            300

unsigned long lastReconnectAttempt                = 0UL;
const unsigned long RECONNECTION_ATTEMPT_INTERVAL = 30UL * 1000UL;  // attempt to reconnect every 30 seconds

//const char COMMAND_SEPARATOR        = ':';

char message[BUFFER_SIZE];


// Status topics

const char CONNECTED_STATUS[]   PROGMEM = "sensorlight/status/connected";
const char IP_ADDR_STATUS[]     PROGMEM = "sensorlight/status/ip_addr";
const char UPTIME_STATUS[]      PROGMEM = "sensorlight/status/uptime";
const char MEMORY_STATUS[]      PROGMEM = "sensorlight/status/memory";
const char TIME_STATUS[]        PROGMEM = "sensorlight/status/time";
const char LED_STATUS[]         PROGMEM = "sensorlight/status/led";
const char COLOUR_STATUS[]      PROGMEM = "sensorlight/status/colour";
const char PIR_STATUS[]         PROGMEM = "sensorlight/status/pir";
const char DHT22_STATUS[]       PROGMEM = "sensorlight/status/dht22";
const char TEMPERATURE_STATUS[] PROGMEM = "sensorlight/status/temperature";
const char HUMIDITY_STATUS[]    PROGMEM = "sensorlight/status/humidity";

PGM_P const STATUS_TOPICS[]     PROGMEM = { CONNECTED_STATUS,    // idx = 0
                                            IP_ADDR_STATUS,      // idx = 1
                                            UPTIME_STATUS,       // idx = 2
                                            MEMORY_STATUS,       // idx = 3
                                            TIME_STATUS,         // idx = 4
                                            LED_STATUS,          // idx = 5
                                            COLOUR_STATUS,       // idx = 6
                                            PIR_STATUS,          // idx = 7
                                            DHT22_STATUS,        // idx = 8
                                            TEMPERATURE_STATUS,  // idx = 9
                                            HUMIDITY_STATUS,     // idx = 10
                                           };


// Control topics

const char LED_CONTROL[]        PROGMEM = "sensorlight/control/led";

PGM_P const CONTROL_TOPICS[]    PROGMEM = { LED_CONTROL,         // idx = 0
                                          };

#if USE_SDCARD
boolean readMqttConfiguration() {
  /*
   * Length of the longest line expected in the config file.
   * The larger this number, the more memory is used
   * to read the file.
   * You probably won't need to change this number.
   */
  const uint8_t CONFIG_LINE_LENGTH = 32;
  
  // The open configuration file.
  SDConfigFile cfg;
  
  // Open the configuration file.
  if (!cfg.begin(CONFIG_FILE, CONFIG_LINE_LENGTH)) {
    DEBUG_LOG(1, "Failed to open configuration file: ");
    DEBUG_LOG(1, CONFIG_FILE);
    return false;
  }
  
  // Read each setting from the file.
  while (cfg.readNextSetting()) {
    
    // Put a nameIs() block here for each setting you have.
    
    if (cfg.nameIs("mqttBrokerIP")) {
      // Dynamically allocate a copy of the string.
      char* str = cfg.copyValue();
      mqttServerAddr.fromString(str);
      DEBUG_LOG(1, "Read mqttBrokerIP: ");
      DEBUG_LOG(1, str);
    } else if (cfg.nameIs("mqttClientId")) { // mqttClientId string (char *)
      // Dynamically allocate a copy of the string.
      mqttClientId = cfg.copyValue();
      DEBUG_LOG(1, "Read mqttClientId: ");
      DEBUG_LOG(1, mqttClientId);
    } else if (cfg.nameIs("mqttPort")) { // mqttPort integer
      mqttPort = cfg.getIntValue();
      DEBUG_LOG(1, "Read mqttPort: ");
      DEBUG_LOG(1, mqttPort);
    } else {
      // report unrecognized names.
      DEBUG_LOG(1, "Unknown name in config: ");
      DEBUG_LOG(1, cfg.getName());
    }
  }
  
  // clean up
  cfg.end();
}
#endif

// callback function definition
void callback(char* topic, uint8_t* payload, unsigned int length);

PubSubClient   mqttClient(mqttServerAddr, mqttPort, callback, ethernetClient);

void publish_connected()
{
  progBuffer[0] = '\0';
  strcpy_P(progBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[0])));
  mqttClient.publish(progBuffer, "");
}

void publish_ip_address()
{
  progBuffer[0] = '\0';
  strcpy_P(progBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[1])));
  mqttClient.publish(progBuffer, "192.168.1.90");
}

void publish_uptime()
{
  progBuffer[0] = '\0';
  strcpy_P(progBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[2])));
  charBuffer[0] = '\0';
  ltoa(millis(), charBuffer, 10);
  mqttClient.publish(progBuffer, charBuffer);
}


#endif   /* SENSORLIGHTIOTMODULE_MQTTCONFIG_H_ */
