#include <WiFi.h> 
int measurePin = A5;
int ledPower = 12;

unsigned int samplingTime = 280;
unsigned int deltaTime = 40;
unsigned int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

const char* ssid = "InClean";

const char* pass = "abcdef0123";

const uint16_t port = 8090;

const char * host = "192.168.0.10";


void setup(){
  Serial.begin(9600);
  pinMode(ledPower,OUTPUT);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting...");
  }
  
  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());
}

void loop(){
  WiFiClient client;
 
  
  if (!client.connect(host, port)) {
    Serial.println("Connection to host failed");
    delay(1000);
    return;
  }
  Serial.println("Connected to Raspberry Pi!");
  
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

  //Serial.println("Raw Signal Value (0-1023):");
  //Serial.println(voMeasured);

  //Serial.println("Voltage:");
  //Serial.println(calcVoltage);

  Serial.println("Dust Density:");
  Serial.println(dustDensity);

  
  delay(1000);
  client.println(dustDensity);
  client.stop();
}
