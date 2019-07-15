#include <WiFi.h>
#include <PubSubClient.h>



const char* ssid = "InClean"; // Enter your WiFi name
const char* password =  "robojackets"; // Enter WiFi password
const char* mqttServer = "postman.cloudmqtt.com";
const int mqttPort = 11711;
const char* mqttUser = "uodfjdmd";
const char* mqttPassword = "PbpIBS3TNNHZ";


#define        COV_RATIO                       (0.2)            //ug/mmm / mv
#define        NO_DUST_VOLTAGE                 (400)            //mv
#define        SYS_VOLTAGE                     (5000)           



WiFiClient espClient;
PubSubClient client(espClient);

int measurePin = A5;
int ledPower = 12;

unsigned int samplingTime = 280;
unsigned int deltaTime = 40;
unsigned int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;


void setup() {
  pinMode(ledPower, OUTPUT);
  digitalWrite(ledPower, LOW); 
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");

  client.setServer(mqttServer, mqttPort);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP32Client", mqttUser, mqttPassword )) {

      Serial.println("connected");

    } else {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

  
void loop() {
  digitalWrite(ledPower,LOW);
  delayMicroseconds(samplingTime);

  voMeasured = analogRead(measurePin);

  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH);
  delayMicroseconds(sleepTime);

  calcVoltage = voMeasured*(5.0/1024);
  dustDensity = 0.17*calcVoltage-0.1;

  if ( dustDensity < 0)
  {
    dustDensity = 0.00;
  }
  char fValue[16];
  Serial.print(dustDensity);
  dtostrf(dustDensity, 3, 2, fValue);
  client.publish("esp32/test", fValue);
  Serial.print(fValue);
  delay(1000);
}

 
