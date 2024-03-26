#include <WiFi.h>
#include <PubSubClient.h>

#define SEND_SCHEDULE 4

#define wifi_ssid "Wagner"
#define wifi_password "123456789"

#define MQTT_HOST "172.20.10.8"
#define MQTT_PORT 1883

#define CAF_SCHEDULE "caf_schedule"

WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
  Serial.begin(115200);

  // Configure Wifi
  setup_wifi();

  // Configure Mosquitto
  connect_mqtt();

  // Set the button as a Input Pin
  pinMode(SEND_SCHEDULE, INPUT);
}

void connect_mqtt() {
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(callback);

  while (!client.connected()) {
    //Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32Client")) { // Connect without username and password
      //Serial.println("Connected to MQTT");
    } else {
      //Serial.print("Failed to connect to MQTT, rc=");
      //Serial.println(client.state());
      delay(2000);
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
      Serial.print((char) payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}

String macToStr(const uint8_t *mac)
{
  String result;
  for (int i = 0; i < 6; ++i)
  {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}

void setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void loop()
{

  if (!client.connected())
  {
    connect_mqtt();
  }

  if (digitalRead(SEND_SCHEDULE) == HIGH) {
      client.publish(CAF_SCHEDULE, "Schedule");
      delay(1000);
  }

  client.loop();
}