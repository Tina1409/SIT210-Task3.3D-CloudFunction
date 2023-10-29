//header files 
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>

// WiFi credentials 
char WIFI_NAME[] = "OnePlus Nord CE 2 Lite 5G";
char WIFI_PASS[] = "12345678";

// Sensor pins of ultrasonic sensor
const int trigger_pin = 2;
const int echo_pin = 3;
float time_taken, distance;

WiFiClient wifi_client;
MqttClient mqttClient(wifi_client);

// MQTT credentials
const char BROKER[] = "BROKER.mqttdashboard.com";
int PORT_NO = 1883;
const char TOPIC[] = "SIT210-wave";
const long INTERVAL = 1000;
unsigned long previous_milli = 0;
int count = 0;

//A procedcure that will only run once for the setup in our code
void setup() {
  Serial.begin(9600); //baud rate
  while (!Serial) { //waiting for the serial connection to be established
    ;
  }
  
  Serial.print("Trying to connect to the WiFi ");
  Serial.println(WIFI_NAME);
  //if the wifi is not connected
  while (WiFi.begin(WIFI_NAME, WIFI_PASS) != WL_CONNECTED) {
    Serial.print(".");
    delay(5000); 
  }
  //successfully connected the WiFi
  Serial.println("Connected to WiFi successsfully");
  Serial.println();

  Serial.print("Trying to connect to MQTT broker ");
  Serial.println(BROKER);

  //if the MQTT Broker connection is not established
  if (!mqttClient.connect(BROKER, PORT_NO)) {
    Serial.print("MQTT connection failed!");
    Serial.println(mqttClient.connectError());
    while (1); 
  }
  
  //Successfully connected to the MQTT broker
  Serial.println("Connected to MQTT broker successfully");
  Serial.println();
}

void loop() {
  // Polling for MQTT for receiving as well as sending the messages
  mqttClient.poll();

  // current time
  unsigned long milli = millis();

  //storing the message sent time in a variable
  if (milli - previous_milli >= INTERVAL) {
    previous_milli = milli;

    // measuring the distance with ultrasonic sensor
    digitalWrite(trigger_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigger_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger_pin, LOW);

    // time taken by ultrasonic pulse
    time_taken = pulseIn(echo_pin, HIGH);
    distance = (time_taken * 0.0343) / 2; //calculating distance

    Serial.print("distance: ");
    Serial.println(distance);

  //sending a notification whenever the distance is less than 12 units.
    if (distance < 12) {
      mqttClient.beginMessage(TOPIC);
      mqttClient.print("Tina and Sanchit waves");
      mqttClient.print("distance: ");
      mqttClient.print(distance);
      mqttClient.endMessage();
      delay(1000); 
    }
    
    //incrementing the count by one when a message is sent
    Serial.println();
    count++;
  }
}
