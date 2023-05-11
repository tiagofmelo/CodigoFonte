//Inclusão de bibliotecas
#include <ESP8266WiFi.h>       // Biblioteca para funcionamento do WiFi do ESP
#include <ESP8266WebServer.h>  // Biblioteca para o ESP funcionar como servidor

//Declaração de constantes
const char* ssid = "Melo";            // Rede WiFi
const char* password = "124@124@me";  // Senha da Rede WiFi
ESP8266WebServer server(80);          // Server na porta 80
const int pinoRELE = 16;              // Pino digital (realiza acionamento do relé)
const int pinoLDR = A0;               // Pino analógico (realiza a leitura do sensor LDR)

// Declaração de variáveis
int valorSensor;

void setup() {

  Serial.begin(9600);    // Inicializa a comunicação serial com a taxa de 9600 bps
  delay(50);             // Intervalo para aguardar a estabilização do sistema

  Serial.println("Conectando a Rede: ");  // Imprime na serial a mensagem
  Serial.println(ssid);                   // Imprime na serial o nome da Rede Wi-Fi

  WiFi.begin(ssid, password);  // Inicialização da comunicação Wi-Fi

  // Verificação da conexão
  while (WiFi.status() != WL_CONNECTED) {  // Enquanto estiver aguardando status da conexão
    delay(1000);
    Serial.print(".");  // Imprime pontos
  }
  Serial.println("");
  Serial.println("WiFi Conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());  //Função para exibir o IP da ESP

  server.on("/", handle_OnConnect);    // Servidor recebe uma solicitação HTTP - chama a função handle_OnConnect
  server.onNotFound(handle_NotFound);  // Servidor recebe uma solicitação HTTP não especificada - chama a função handle_NotFound

  server.begin();  // Inicializa o servidor
  Serial.println("Servidor HTTP inicializado");

  pinMode(pinoRELE, OUTPUT);  // Define o pino de acionamento do relé como saída (output)
  pinMode(pinoLDR, INPUT);    // Define o pino de leitura do sensor LDR como entrada (input)

}

void loop() {

  server.handleClient(); //Chama o método handleClient()

}

void handle_OnConnect() {

  AcionamentoRele();

  Serial.print("LDR: "); 
  Serial.println(valorSensor); // Imprime no monitor serial o valor da sendor lido no LDR

  server.send(200, "text/html", EnvioHTML(valorSensor)); //Envia as informações usando o código 200, especifica o conteúdo como "text/html" e chama a função EnvioHTML

}

void handle_NotFound() { //Função para lidar com o erro 404

  server.send(404, "text/plain", "Não encontrado"); //Envia o código 404, especifica o conteúdo como "text/pain" e envia a mensagem "Não encontrado"

}

void AcionamentoRele() {

  valorSensor = analogRead(pinoLDR);  // Faz a leitura do sensor LDR e armazena o valor na variável valorSensor
  
  if (valorSensor < 1000) {         // Verifica se o valor está abaixo de 500
    digitalWrite(pinoRELE, HIGH);  // Se a condição for verdadeira, aciona o relé
  } else {
    digitalWrite(pinoRELE, LOW);  // Se a condição for falsa, desaciona o relé
  }

}

String EnvioHTML(int valorSensor) { // Exibindo a página da web em HTML
  String ptr = "<!DOCTYPE html> <html>\n"; // Indica o envio do código HTML
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n"; //Torna a página da Web responsiva em qualquer navegador Web
  ptr += "<meta http-equiv='refresh' content='2'>"; // Atualizar browser a cada 2 segundos
  ptr += "<link href=\"https://fonts.googleapis.com/css?family=Open+Sans:300,400,600\" rel=\"stylesheet\">\n";
  ptr += "<title>Monitor de Temperatura e Umidade</title>\n"; // Define o título da página

  // Configurações de fonte do título e do corpo do texto da página web
  ptr += "<style>html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #000000;}\n";
  ptr += "body{margin-top: 50px;}\n";
  ptr += "h1 {margin: 50px auto 30px;}\n";
  ptr += "h2 {margin: 40px auto 20px;}\n";
  ptr += "p {font-size: 24px;color: #000000;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"webpage\">\n";
  ptr += "<h1>Controle da Iluminacao</h1>\n";

  //Exibe as informações na página web
  ptr += "<p><b>A luz esta: </b>";
  ptr += (int)valorSensor;

  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;

}