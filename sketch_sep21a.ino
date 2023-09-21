//header files
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>

//Wifi credentials
char WIFI_NAME[] = "OnePlus Nord CE 2 Lite 5G";    
char WIFI_PASWORD[] = "12345678";  

//defining the pin number
int LIGHT_PIN_NO = 2;

WiFiClient wifi_Client;
MqttClient mqttClient(wifi_Client);

//MQTT credentials
const char BROKER[] = "mqtt-dashboard.com";
int       PORT_NO    = 1883;
const char TOPIC] = "ekam-wave";

//setup procedure which will run once to set things
void setup() {
  //baud rate
  Serial.begin(9600);
  //setting the led pin number as output
  pinMode(LIGHT_PIN_NO, OUTPUT);
  while (!Serial) {
    ; 
      }

  //connecting with wifi
  Serial.print("TRYING TO CONNECT WIFI_NAME ");
  Serial.println(WIFI_NAME);
  while (WiFi.begin(WIFI_NAME, WIFI_PASWORD) != WL_CONNECTED) {
   
    Serial.print(".");
    delay(5000);
  }
  Serial.println("cONNECTED TO WIFI SUCCESS FULLY");
  Serial.println();

  
  //connecting with mqtt broker
  Serial.print("TRYING TO CONNECT MQTT BROKER: ");
  Serial.println(BROKER);

  if (!mqttClient.connect(BROKER, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT BROKER!");
  Serial.println();

  //subscribing to a topic 
  Serial.print("TOPIC SUBSCRIBING: ");
  Serial.println(topic);
  Serial.println();

  mqttClient.subscribe(topic);


  Serial.print("TOPIC:");
  Serial.println(topic);
  Serial.println();
}


void loop() {
  int messageSize = mqttClient.parseMessage();
  //check if any message is received
  if (messageSize) {
    Serial.print("Received a message with topic '");
    Serial.print(mqttClient.messageTopic());
    Serial.print("', length ");
    Serial.print(messageSize);
    Serial.println(" bytes:");

    //printing a message
    while (mqttClient.available()) {
      Serial.print((char)mqttClient.read());
    }
    Serial.println();

    //blinking the led in response to the message received
    for (int i = 0; i < 3; i++) {
      digitalWrite(LIGHT_PIN_NO, HIGH);
      delay(200);
      digitalWrite(LIGHT_PIN_NO, LOW);
      delay(200);
    }

    Serial.println();
  }
}