#include "ESP8266WiFi.h"
#include <PubSubClient.h>

//Parametros de conexão
const char *ssid = "Melo";            // REDE
const char *password = "124@124@me";  // SENHA

// MQTT Broker
const char *mqtt_broker = "test.mosquitto.org";  //Host do broket
const char *topic = "IoT/Iluminacao";            //Topico a ser subscrito e publicado
const char *mqtt_username = "";                  //Usuario
const char *mqtt_password = "";                  //Senha
const int mqtt_port = 1883;                      //Porta

//Variáveis
bool mqttStatus = 0;
int valorSensor;

//Objetos
WiFiClient espClient;
PubSubClient client(espClient);

//Constantes
const int pinoRELE = 16;              // Pino digital (realiza acionamento do relé)
const int pinoLDR = A0;               // Pino analógico (realiza a leitura do sensor LDR)

//Prototipos
bool connectMQTT();
void callback(char *topic, byte *payload, unsigned int length);

void setup(void) {
  Serial.begin(9600);

  // Conectar
  WiFi.begin(ssid, password);

  //Aguardando conexão
  Serial.println();
  Serial.print("Conectando");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  //Envia IP através da UART
  Serial.println(WiFi.localIP());

  mqttStatus = connectMQTT();

  pinMode(pinoRELE, OUTPUT);  // Define o pino de acionamento do relé como saída (output)
  pinMode(pinoLDR, INPUT);    // Define o pino de leitura do sensor LDR como entrada (input)
}

void loop() {
  static long long pooling = 0;
  if (mqttStatus) {

    client.loop();

    if (millis() > pooling + 1000) {
      pooling = millis();

      AcionamentoRele();
      //client.publish(topic, "{teste123,113007042022}");
    }
  }
}

bool connectMQTT() {
  byte tentativa = 0;
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);

  do {
    String client_id = "BOBSIEN-";
    client_id += String(WiFi.macAddress());

    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Exito na conexão:");
      Serial.printf("Cliente %s conectado ao broker\n", client_id.c_str());
    } else {
      Serial.print("Falha ao conectar: ");
      Serial.print(client.state());
      Serial.println();
      Serial.print("Tentativa: ");
      Serial.println(tentativa);
      delay(2000);
    }
    tentativa++;
  } while (!client.connected() && tentativa < 5);

  if (tentativa < 5) {
    // publish and subscribe
    // client.publish(topic, "{teste123,113007042022}");
    client.subscribe(topic);
    return 1;
  } else {
    Serial.println("Não conectado");
    return 0;
  }
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}

void AcionamentoRele() {
  valorSensor = analogRead(pinoLDR);  // Faz a leitura do sensor LDR e armazena o valor na variável valorSensor

  if (valorSensor > 1000) {       // Verifica se o valor está abaixo de 500
    digitalWrite(pinoRELE, LOW);  // Se a condição for verdadeira, aciona o relé
  } else {
    digitalWrite(pinoRELE, HIGH);  // Se a condição for falsa, desaciona o relé
  }
}