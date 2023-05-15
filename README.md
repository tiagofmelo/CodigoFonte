## Projeto IoT

O projeto de controle de iluminação utiliza um sensor de luminosidade para detectar a quantidade de luz ambiente, um microcontrolador para processar os dados do sensor e um atuador para controlar a iluminação. O sensor captura a quantidade de luz presente no ambiente e envia essa informação para o microcontrolador, que processa os dados e determina a intensidade ideal da iluminação. O atuador é responsável por controlar as lâmpadas, aumentando ou reduzindo a intensidade de acordo com as informações do sensor e do microprocessador. O objetivo desse projeto é garantir uma iluminação adequada e eficiente, proporcionando economia de energia e maior conforto aos usuários do ambiente.

### Lista de Materiais:
- NodeMCU (ESP-8266)
- Fonte para protoboard
- Resistor 10k ohms
- LDR 5mm
- Relé
- Protoboard
- Jumpers
- Lâmpada
- Fonte 12V

### Visão geral do circuito
![image](https://github.com/tiagofmelo/Projeto-IoT/assets/78428504/5e731ccd-6054-48b5-8754-9ce5f7509996)

Para poder alcançar esse objetivo é necessário realizar a montagem do sistema de forma que o microcontrolador tenha condições de realizar a leitura do sensor e através desse processamento tome a decisão de como irá controlar a saída, de forma que controle o acionamento do relé e  consequentemente acionando a lâmpada. No ponto da ligação onde temos os bornes do resistor e do LDR, será onde iremos inserir um jumper (representado pela linha de cor verde), o qual será responsável por levar o sinal da variação do LDR que ocorre pela mudança da luminosidade, para o pino de entrada do microcontrolador.

![image](https://github.com/tiagofmelo/Projeto-IoT/assets/78428504/a4bcf978-e09b-46dd-8319-7f1724d6fd11)

O pino VIN é o pino de alimentação positivo, onde iremos alimentar com 5V que vem do barramento positivo da protoboard.
O pino GND é o pino de alimentação negativo, onde iremos conectar diretamente no barramento negativo da protoboard.
O pino ADC0 é o pino de entrada analógica do microcontrolador, nele será conectado o jumper (representado pela linha de cor verde) com o sinal do LDR, que é o sinal que informa a variação da luminosidade do ambiente.
O pino D1 é o pino de saída digital do microcontrolador, o qual será responsável por enviar o sinal para o relé, acionando e desacionando, será conectado um jumper (representado pela linha de cor amarela), ligado diretamente no pino de trigger do relé.

![image](https://github.com/tiagofmelo/Projeto-IoT/assets/78428504/1bcd20f1-4cff-4755-ab36-a66b1120a810)

Realizaremos a ligação de alimentação do relé, através de jumpers que virão diretamente dos barramentos da protoboard, ou seja, a alimentação que vem da fonte de alimentação de 5V.
O pino DC+ do relé, será alimentado com o jumper (representado pela cor vermelha) com os 5V que sai do barramento positivo da protoboard.
O pino DC- do relé, será alimentado com o jumper (representado pela cor preta) com o GND que sai do barramento negativo da protoboard.
O trigger é o pino de controle do relé e será alimentado com o jumper (representado pela cor amarela), alimentado com o sinal que vem da saída D1 do microcontrolador. O sinal irá determinar se o relé ficará acionado ou desacionado de forma que os contatos de alimentação que levam energia para a lâmpada fechem ou abram.

![image](https://github.com/tiagofmelo/Projeto-IoT/assets/78428504/b907563e-8ac1-46b8-b907-2b07efbf1adc)

O circuito de potência é o que realiza a alimentação da lâmpada, será utilizado como fonte de energia a tensão da rede 127V, para que haja energia suficiente para que a lâmpada acenda.

### MQTT

Para realizar o teste da comunicação MQTT, será utilizado um aplicativo via smartphone chamado “MyMQTT”, disponibilizado na loja de aplicativos PlayStore no sistema operacional Android.
 
![image](https://github.com/tiagofmelo/Projeto-IoT/assets/78428504/d5abde15-cc30-4b69-8a63-611342d6d116)

Após o download do aplicativo, será necessário acessar o broker com as informações que foram definidas no código fonte do programa, inserindo no campo “Host” e “Port” que estão disponíveis na tela inicial. Também é possível inserir as credenciais de acesso, caso tenha sido configuradas nas definições do código fonte.
 
![image](https://github.com/tiagofmelo/Projeto-IoT/assets/78428504/4ed77bc0-74f0-4f82-93f3-1d356c00abd7)

Após inserir as informações e clicar em “Connect”, o aplicativo irá redirecionar para a próxima tela, onde deverá ser inserido os campos de Publish e Subscribe.
 
![image](https://github.com/tiagofmelo/Projeto-IoT/assets/78428504/62681b8a-1fab-4a94-8c88-6bd1e7df9e90)

Em seguida, após o cadastro dos dados, será possível realizar o envio das menssagens somente ao clicar nos itens da lista. Essas menssagens serão recebidas pelo microcontrolador que realizará o controle da iluminação, conforme a mensagem que tiver sido enviada.
 
![image](https://github.com/tiagofmelo/Projeto-IoT/assets/78428504/86304fca-ddb2-4084-b010-e9196a965bda)

Após o envio das menssagens, é possível visualizar no dashboard o histórico das menssagens que foram enviadas, assim como para quais destinos as menssagens foram enviadas e a ordem de envio.

### Exemplo do circuito montado

![image](https://github.com/tiagofmelo/Projeto-IoT/assets/78428504/678a3124-8ceb-4c49-b869-3bcfb8e9c747)
