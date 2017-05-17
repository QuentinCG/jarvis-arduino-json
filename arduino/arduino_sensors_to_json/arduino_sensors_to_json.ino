/*
 * \brief Arduino to JSON temperature/hygrometer/light/PIR sensors
 *
 * \author Quentin Comte-Gaz <quentin@comte-gaz.com>
 * \date 25 March 2017
 * \license MIT License (contact me if too restrictive)
 * \copyright Copyright (c) 2017 Quentin Comte-Gaz
 * \version 1.1
 *
 * \history
 *  - v1.0 (21 March 2017): temperature/hygrometer/light sensors
 *  - v1.1: Adding asynchronous and synchronous PIR sensor state
 */
// ---------------- Activate or not debug ---------------

// Comment to deactivate serial port debugging
#define VERBOSE

#ifdef VERBOSE
#define INIT_SERIAL(baudrate) Serial.begin(baudrate)
#define PRINT(str) Serial.print(str)
#define PRINTLN(str) Serial.println(str)
#define INFO_PRINT(str) Serial.print("[INFO] "); Serial.print(str)
#define INFO_PRINTLN(str) Serial.print("[INFO] "); Serial.println(str)
#define WARNING_PRINT(str) Serial.print("[WARNING] "); Serial.print(str)
#define WARNING_PRINTLN(str) Serial.print("[WARNING] "); Serial.println(str)
#define ERROR_PRINT(str) Serial.print("[ERROR] "); Serial.print(str)
#define ERROR_PRINTLN(str) Serial.print("[ERROR] "); Serial.println(str)
#else
#define PRINT(str)
#define PRINTLN(str)
#define INIT_SERIAL(baudrate)
#define INFO_PRINT(str)
#define INFO_PRINTLN(str)
#define WARNING_PRINT(str)
#define WARNING_PRINTLN(str)
#define ERROR_PRINT(str)
#define ERROR_PRINTLN(str)
#endif

// ---------------- Define ---------------
// Base information
#define POSITION_IN_HOUSE "Salon"

// JSON
#define MAX_JSON_BUFFER_SIZE 500

// Hygrometer sensor
#define HYGRO_TYPE HygrometerSensor::ANALOG
#define HYGRO_PIN A1
#define ANALOG_HYGRO_MIN 210
#define ANALOG_HYGRO_MAX 1024
#define ANALOG_HYGRO_MEDIUM 800

// Light sensor
#define LDR_OTHER_RESISTOR 3300 //ohms
#define LDR_PIN A0
#define LDR_PHOTOCELL LightDependentResistor::GL5528
#define LDR_POSITION_ON_GROUND true

// PIR sensor
#define PIR_DIGITAL_PIN 2

// Web server
#define SERVER_PORT 80

#define IP_PART_1 192
#define IP_PART_2 168
#define IP_PART_3 1
#define IP_PART_4 45

#define MAC_PART_1 0xDE
#define MAC_PART_2 0xAD
#define MAC_PART_3 0xBE
#define MAC_PART_4 0xEF
#define MAC_PART_5 0xFE
#define MAC_PART_6 0xED

// Jarvis
#define JARVIS_IP_PART_1 192
#define JARVIS_IP_PART_2 168
#define JARVIS_IP_PART_3 1
#define JARVIS_IP_PART_4 19

#define JARVIS_API_PORT 8080

#define JARVIS_REQUEST "?order=arduinoSayingSomeoneIsDetected"
#define JARVIS_REQUEST_SUFFIX "" //"&key=12345"

// --------------- Includes ---------------
// LM75A Temperature sensor
#include <LM75A.h>

// Hygrometer sensor
#include <HygrometerSensor.h>

// Light sensor
#include <LightDependentResistor.h>

// PIR sensor (digital sensor)
#include <DigitalSensor.h>

// Web server
#include <SPI.h>
#include <Ethernet.h>

// JSON
#include <ArduinoJson.h>

// --------------- Instances ---------------
// LM75A Temperature sensor
LM75A lm75a_sensor(false,  //A0 LM75A pin state
                   false,  //A1 LM75A pin state
                   false); //A2 LM75A pin state

// Hygrometer sensor
HygrometerSensor analog_hygrometer(HYGRO_TYPE, HYGRO_PIN);

// Light sensor
LightDependentResistor photocell(LDR_PIN, LDR_OTHER_RESISTOR, LDR_PHOTOCELL);

// PIR sensor
DigitalSensor pir(PIR_DIGITAL_PIN);

// Web server
byte mac[] = {MAC_PART_1, MAC_PART_2, MAC_PART_3, MAC_PART_4, MAC_PART_5, MAC_PART_6};
IPAddress ip(IP_PART_1, IP_PART_2, IP_PART_3, IP_PART_4);
EthernetServer server(SERVER_PORT);

// Jarvis client
IPAddress jarvis_ip(JARVIS_IP_PART_1, JARVIS_IP_PART_2, JARVIS_IP_PART_3, JARVIS_IP_PART_4);


// --------------- Asynchronous PIR sensor state change ---------------

/*!
 * Inform Jarvis that someone is detected
 */

void pirSomeoneDetected()
{
  INFO_PRINTLN("Someone detected");

  EthernetClient jarvis_client;
  if (jarvis_client.connect(jarvis_ip, JARVIS_API_PORT))
  {
    INFO_PRINT("Sending detection info to Jarvis...");
    jarvis_client.print("GET /");
    jarvis_client.print(JARVIS_REQUEST);
    jarvis_client.print(JARVIS_REQUEST_SUFFIX);
    jarvis_client.println(" HTTP/1.1");
    jarvis_client.print("Host: ");
    jarvis_client.print(JARVIS_IP_PART_1);
    jarvis_client.print(".");
    jarvis_client.print(JARVIS_IP_PART_2);
    jarvis_client.print(".");
    jarvis_client.print(JARVIS_IP_PART_3);
    jarvis_client.print(".");
    jarvis_client.println(JARVIS_IP_PART_4);
    jarvis_client.println("Connection: close");
    jarvis_client.println();
    PRINTLN(" done");
  }
  else
  {
    WARNING_PRINTLN("Connection with Jarvis failed");
  }

  delay(1);
  jarvis_client.stop();
}

// --------------- Web server and JSON functions ---------------

/*!
 * Check if request from client is valid
 */
bool readRequest(EthernetClient& client)
{
  bool currentLineIsBlank = true;

  while (client.connected())
  {
    if (client.available())
    {
      char c = client.read();
      // INFO_PRINTLN(c);

      if (c == '\n' && currentLineIsBlank)
      {
        return true;
      }
      else if (c == '\n')
      {
        currentLineIsBlank = true;
      }
      else if (c != '\r')
      {
        currentLineIsBlank = false;
      }
    }
  }

  return false;
}

/*!
 * Create a JSON object containing all sensor data
 */
JsonObject& prepareResponse(JsonBuffer& jsonBuffer)
{
  JsonObject& root = jsonBuffer.createObject();

  JsonObject& moduleInfo = root.createNestedObject("moduleInfo");
  moduleInfo["position"] = POSITION_IN_HOUSE;

  JsonArray& sensorsInfo = root.createNestedArray("sensors");

  JsonObject& humidityInfo = sensorsInfo.createNestedObject();
  humidityInfo["name"] = "Humidite plante";
  humidityInfo["type"] = "humidity";
  humidityInfo["unit"] = "percent";
  humidityInfo["value"] = analog_hygrometer.readPercentageHumidity();

  JsonObject& temperatureInfo = sensorsInfo.createNestedObject();
  temperatureInfo["name"] = "Temperature";
  temperatureInfo["type"] = "temperature";
  temperatureInfo["unit"] = "degrees";
  temperatureInfo["value"] = lm75a_sensor.getTemperatureInDegrees();

  JsonObject& luminosityInfo = sensorsInfo.createNestedObject();
  luminosityInfo["name"] = "Luminosite";
  luminosityInfo["type"] = "luminosity";
  luminosityInfo["unit"] = "lux";
  luminosityInfo["value"] = photocell.getCurrentLux();

  JsonObject& pirInfo = sensorsInfo.createNestedObject();
  pirInfo["name"] = "Presence";
  pirInfo["type"] = "motion";
  pirInfo["unit"] = "bool";
  pirInfo["value"] = pir.status();

  return root;
}

/*!
 * Send a JSON response to the client
 */
void writeResponse(EthernetClient& client, JsonObject& json)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  //client.println("Server: Arduino");
  client.println("Connection: close");
  client.println();

  json.prettyPrintTo(client);
}


// --------------- Main program ---------------
void setup(void)
{
  // Initialize debuging
  INIT_SERIAL(9600);
  INFO_PRINTLN("Serial initialized");

  // Temperature sensor initialized during instantiation
  INFO_PRINTLN("Temperature sensor initialized");

  // Initialize hygrometer sensor
  if (!analog_hygrometer.setAnalogParameters(ANALOG_HYGRO_MIN, ANALOG_HYGRO_MAX, ANALOG_HYGRO_MEDIUM))
  {
    ERROR_PRINTLN("Error while setting Analog parameters");
  }
  else
  {
    INFO_PRINTLN("Hygrometer initialized");
  }

  // Initialize light sensor
  photocell.setPhotocellPositionOnGround(LDR_POSITION_ON_GROUND);
  INFO_PRINTLN("LDR initialized");

  // Start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  INFO_PRINT("Server initialized with IP ");
  PRINTLN(Ethernet.localIP());

  // Initialize pir sensor (with asynchronous function call when state changes)
  pir.attachStateChangeToFunction(pirSomeoneDetected, RISING);
  INFO_PRINTLN("PIR initialized");
}

void loop()
{
  // Check if there is a client
  EthernetClient client = server.available();
  if (client)
  {
    // Check if request from client is valid
    if (readRequest(client))
    {
      INFO_PRINTLN("Request received");

      // Get all sensors data, put them in JSON format and send it to the client
      StaticJsonBuffer<MAX_JSON_BUFFER_SIZE> jsonBuffer;
      JsonObject& json = prepareResponse(jsonBuffer);
      writeResponse(client, json);

      INFO_PRINTLN("Response sent");
    }

    // Give time to client to receive the response
    delay(1);

    // Stop connection with the client
    client.stop();
  }
}

