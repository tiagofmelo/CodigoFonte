//Inclusão de bibliotecas
#include "ESP8266WiFi.h"
#include <PubSubClient.h>
#include <string>

//Parametros de conexão
const char *ssid = "Melo";            // Rede
const char *password = "124@124@me";  // Senha

//MQTT Broker
const char *mqtt_broker = "test.mosquitto.org";  // Host do broket
const char *topic = "IoT/Iluminacao";            // Topico a ser subscrito e publicado
const char *mqtt_username = "";                  // Usuario
const char *mqtt_password = "";                  // Senha
const int mqtt_port = 1883;                      // Porta

//Variáveis
bool mqttStatus = 0;                // Variável que armazena o status da conexão MQTT
int valorSensor;                    // Variável que armazena o valor do sensor LDR
String menssagemRecebida = "AUTO";  // Variável que armazena a menssagem que foi recebida via MQTT

//Objetos
WiFiClient espClient;           // Criação do objeto WifiCliente
PubSubClient client(espClient); // Criação do objeto PubSubClient

//Constantes
const int pinoRELE = 16;  // Pino digital (realiza acionamento do relé)
const int pinoLDR = A0;   // Pino analógico (realiza a leitura do sensor LDR)

//Prototipos
bool connectMQTT();                                             // Protótipo da função connectMQTT
void callback(char *topic, byte *payload, unsigned int length); // Protótipo da função callback

void setup(void) {
  Serial.begin(9600); // Define a taxa de comunicação serial

  WiFi.begin(ssid, password); // Função para conectar na rede wifi

  Serial.println();
  Serial.print("Conectando");
  while (WiFi.status() != WL_CONNECTED) { // Executa enquanto a conexão WiFi não é estabelecida
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.println(WiFi.localIP()); // Imprime o IP da rede que a conexão foi realizada

  mqttStatus = connectMQTT(); // Executa a função connectMQTT e armazena o retorno na variável mqttStatus

  pinMode(pinoRELE, OUTPUT);  // Define o pino de acionamento do relé como saída (output)
  pinMode(pinoLDR, INPUT);    // Define o pino de leitura do sensor LDR como entrada (input)
}

void loop() {
  static long long pooling = 0;
  if (mqttStatus) {

    client.loop();

    if (millis() > pooling + 1000) { // Executa entre intervalos de 1 segundo
      pooling = millis();

      AcionamentoRele(); // Executa a função responsável pelo acionamento do relé
    }
  }
}

bool connectMQTT() {
  byte tentativa = 0;
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);

  do {
    String client_id = "ESP8266-";
    client_id += String(WiFi.macAddress());

    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) { // Realiza a tentativa de conexão no broker MQTT
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
    client.subscribe(topic);
    return 1;
  } else {
    Serial.println("Não conectado");
    return 0;
  }
}

void callback(char *topic, byte *payload, unsigned int length) {

  Serial.print("Menssagem recebida no topic: ");
  Serial.println(topic);
  Serial.print("Menssagem:");

  char buff_p[length];
  for (int i = 0; i < length; i++) {
    buff_p[i] = (char)payload[i]; // Armazena os bytes recebidos na menssagem na variável buff_p realizando a conversão para char
  }

  buff_p[length] = '\0';
  menssagemRecebida = String(buff_p); // Armazena buff_p na variável menssagemRecebida

  Serial.println(menssagemRecebida); // Imprime a menssagem recebida no display serial
}

void AcionamentoRele() {

  if (menssagemRecebida == "LIGA") {
    digitalWrite(pinoRELE, LOW);  // Se a condição for verdadeira, aciona o relé
  }

  if (menssagemRecebida == "DESLIGA") {
    digitalWrite(pinoRELE, HIGH);  // Se a condição for verdadeira, desaciona o relé
  } 

  if (menssagemRecebida == "AUTO") {
    valorSensor = analogRead(pinoLDR);  // Faz a leitura do sensor LDR e armazena o valor na variável valorSensor

    if (valorSensor > 1000) {       // Verifica se o valor está acima de 1000
      digitalWrite(pinoRELE, LOW);  // Se a condição for verdadeira, aciona o relé
    } else {
      digitalWrite(pinoRELE, HIGH);  // Se a condição for falsa, desaciona o relé
    }
  }
}