const int pinoRELE = 16;  //PINO DIGITAL UTILIZADO PARA ENVIAR SINAL DE ACIONAMENTO DO RELÉ
const int pinoLDR = A0;   //PINO ANALÓGICO UTILIZADO PARA LEITURA DO LDR
int valorsensor;

void setup() {

  pinMode(pinoRELE, OUTPUT);  //DEFINE O PINO DE ACIONAMENTO DO RELÉ COMO SAÍDA
  pinMode(pinoLDR, INPUT);    //DEFINE O PINO DE LEITURA DO SENSOR LDR COMO ENTRADA
  Serial.begin(9600);         // inicializa a comunicação serial com a taxa de 9600 bps
}

void loop() {

  valorsensor = analogRead(pinoLDR);  // Faz a leitura do pino analógico LDR e armazena o valor na variavel valorsensor
  Serial.print("LDR : ");         // Mostra o valor no monitor serial
  Serial.println(valorsensor);

  if (valorsensor < 500) {  //VERIFICA SE O VALOR MEDIDO NO SENSOR É MENOR QUE 500
    digitalWrite(pinoRELE, HIGH);   //SE A CONDIÇÃO FOR VERDADEIRA, ACIONA O RELÉ
  } else {
    digitalWrite(pinoRELE, LOW);  //SE A CONDIÇÃO FOR FALSA, DESACIONA O RELÉ
  }

  delay(100);  // Aguarda 100 milisegundos
}

void AcionamentoRele() {
}