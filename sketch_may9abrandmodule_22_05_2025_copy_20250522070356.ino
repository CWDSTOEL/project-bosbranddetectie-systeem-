#include <WiFiS3.h>
#include <PubSubClient.h>
#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT11
const int fire_detector = 3;
const int smoke_detector = 7;

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "iotroam";
const char* password = "Og6SyxFOFM";



const char* mqtt_server = "145.93.236.59";  // 
const int mqtt_port = 1883;
const char* mqtt_topic = "rook/melding";


// ==== MQTT & WIFI CLIENTS ====
WiFiClient wifiClient;
PubSubClient client(wifiClient);

// ==== WIFI VERBINDING ====
void setup_wifi() {
  Serial.print("Verbinden met WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi verbonden!");
}

// ==== MQTT VERBINDING ====
void reconnect() {
  while (!client.connected()) {
    Serial.print("Verbinding maken met MQTT...");
    if (client.connect("UNO_R4_Client","arduino","abc123")) {
      Serial.println("MQTT verbonden!");
    } else {
      Serial.print("Fout, rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(fire_detector, INPUT);
  pinMode(smoke_detector, INPUT);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // === Meetgegevens verzamelen ===
  int fire = digitalRead(fire_detector);
  int smoke = digitalRead(smoke_detector);
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  // === JSON bericht maken ===
  String payload = "{";
  payload += "\"fire\":" + String(fire) + ",";
  payload += "\"smoke\":" + String(smoke) + ",";
  payload += "\"temperature\":" + String(temp) + ",";
  payload += "\"humidity\":" + String(hum);
  payload += "}";

  // === Verzenden via MQTT ===
  client.publish(mqtt_topic, payload.c_str());

  // === Debug output ===
  Serial.println("Verzonden MQTT data:");
  Serial.println(payload);

  delay(5000);  // Wacht 5 seconden voor volgende meting
}
