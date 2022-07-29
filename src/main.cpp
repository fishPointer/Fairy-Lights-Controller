//Basic - Libraries:
  #include <Arduino.h>
  #include <WiFi.h>
  #include <PubSubClient.h>

//Specific - Libraries:


//Basic - Declarations
  const char *SSID = "Fish Tank";
  const char *PWD = "jellyfish64";
  const char *HUTAO = "10.0.0.143";
  int mqttPort = 1616;
  WiFiClient wifiClient;
  PubSubClient mqttClient(wifiClient); 
  void connectToWiFi();
  void callback(char* topic, byte* payload, unsigned int length);
  void setupMQTT();
  void reconnect();

//Specific - Declarataions
  ////Misc
    #define PIN1 4
    int flc_payload = 0;
  ////Topics
    const char *TOPIC_FLC = "FLC";



////////////////////////

void setup() {
  //Basic - setup()
    Serial.begin(115200);
    connectToWiFi();
    setupMQTT();
  //Specific - setup()
    pinMode(PIN1, OUTPUT);


}

void loop() {
  
  
  //Basic - loop()
    if (!mqttClient.connected())
      reconnect();
    mqttClient.loop();
  

  //Specific - loop()
  if (flc_payload == 1)
  {
    digitalWrite(PIN1, HIGH);
    delay(50);
    digitalWrite(PIN1, LOW);
    flc_payload = 0;
  }

  
  

}

////////////////////////

void callback(char* topic, byte* payload, unsigned int length) {

  //Basic - Callback Payload Pre-Processing
  ////Converts Payload from byte* to String
    int callbackmode = 0;
    int num_payload = -1;
    String str_payload;
    char buffer_payload[length];
    //char out_msg[40];
    for (int i = 0; i < length; i++) 
    {
      buffer_payload[i] = char(payload[i]);
    }
    str_payload = String(buffer_payload);
  //Specific - Callback Topic Switching
  ////Not sure how to turn this into a switch statement
    if(strcmp(topic, TOPIC_FLC) == 0)
    {
      callbackmode = 1;
    }

  //Specific - Callback Payload Processing
    switch(callbackmode)
    {
      case 1:
        num_payload = str_payload.toInt();
        flc_payload = num_payload;
        break;


      //Default Functionality, read off message and topic
      default:
        Serial.print("Callback - ");
        Serial.println(topic);
        Serial.print("Message:");
        for (int i = 0; i < length; i++) 
        {
          Serial.print(char(payload[i]));
        }
        Serial.println();
    }
  
}

////////////////////////

//Basic - Connection Functions
void connectToWiFi() {
  Serial.print("Connecting to ");
 
  WiFi.begin(SSID, PWD);
  Serial.println(SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("Connected.");
}

void setupMQTT() {
  mqttClient.setServer(HUTAO, mqttPort);
  mqttClient.setCallback(callback);
}

void reconnect() {
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected()) {
      Serial.println("Reconnecting to MQTT Broker..");
      String clientId = "Crumble-";
      clientId += String(random(0xff), HEX);
      
      if (mqttClient.connect(clientId.c_str())) {
        Serial.println("Connected.");
//Important:
        //Specific - Topic Subscriptions
        mqttClient.subscribe("FLC");
      }
      
  }
}


//Specific - Function Declarations
