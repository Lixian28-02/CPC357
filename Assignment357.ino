#include <WiFi.h>
#include <PubSubClient.h>
#include <NewPing.h>

const char *WIFI_SSID = "Petas1087"; //WiFi SSID
const char *WIFI_PASSWORD = "DU888petas"; //password
const char *MQTT_SERVER = "34.171.185.194"; //VM instance public IP address
const int MQTT_PORT = 1883;
const char *MQTT_TOPIC = "iot"; // MQTT topic

//pin 
const int signalpin = 40; // Arduino pin tied to trigger pin on the ultrasonic sensor.
const int echopin = 39;// Arduino pin tied to echo pin on the ultrasonic sensor.
const int rainpin = A4;//Arduino pin tied to input pin of the rain sensor.
const int pirpin = A2;// Arduino pin tied to input pin on the PIR sensor.
const int maxdist = 200;// Maximum distance we want to ping for (in centimeters). //Maximum sensor distance is rated at 400-500cm.

WiFiClient espClient;
PubSubClient client(espClient);

NewPing sonar(signal, echopin, maxdist); 
// NewPing setup of pins and maximum distance.


void setup_wifi()
{
  delay(10);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup(){
  pinMode(signalpin, OUTPUT);
  pinMode(echopin, INPUT);
  pinMode(rainpin, INPUT);
  pinMode(pirpin, INPUT);

  Serial.begin(115200);
  setup_wifi();
  client.setServer(MQTT_SERVER, MQTT_PORT);
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.println("Attempting MQTT connection...");
    if (client.connect("ESP32Client"))
    {
      Serial.println("Connected to MQTT server");
    }
    else
    {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  delay(500);

  long distance = sonar.ping_cm();
  int raining = !digitalRead(rainpin);
  int pirValue = digitalRead(pirpin);

  char payload[40];
  sprintf(payload, "%.2f|%d|%d", distance, raining, pirValue);
  if (client.publish(MQTT_TOPIC, payload)) {
    Serial.println("Message published successfully");
    Serial.println("distance: ");
    Serial.print(distance);
    Serial.print(" cm");
    Serial.println(raining);
    Serial.println(pirValue);

  } else {
    Serial.println("Failed to publish message");
  }
}