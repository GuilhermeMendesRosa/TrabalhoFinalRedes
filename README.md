# Monitoramento de Nível de Água com ESP32 e MQTT

Este projeto implementa um sistema de monitoramento de nível de água em tempo real utilizando o microcontrolador ESP32, um sensor de boia e o protocolo MQTT. Os dados coletados são enviados para um broker MQTT na nuvem e, em caso de anormalidades, alertas são enviados via Telegram.

---

## 📋 **Pré-requisitos**
Antes de iniciar, verifique se você possui os seguintes itens:
- **Hardware**:  
  - ESP32  
  - Sensor de boia  
  - Garrafa PET ou recipiente similar para testes  

- **Software**:  
  - Arduino IDE (instalado e configurado para o ESP32)

---

## 🚀 **Instalação e Configuração**

### Configuração do ESP32
1. Siga o passo a passo deste vídeo para configurar o ESP32 na Arduino IDE:  
   [Como configurar o ESP32 na Arduino IDE](https://www.youtube.com/watch?v=ROkhP5oWRUU&t=189s)

2. Caso ainda não consiga se conectar ao ESP32, siga as orientações deste vídeo:  
   [Resolvendo problemas de conexão com o ESP32](https://www.youtube.com/watch?v=t-gMQDGB-9g&t=1s)

### Configurando o Wi-Fi no código
- No arquivo do projeto, localize as variáveis `ssid` e `password`.
- Substitua os valores delas pelo nome e senha da sua rede Wi-Fi:  
  ```cpp
  const char* ssid = "Seu_SSID";      // Nome do Wi-Fi
  const char* password = "Sua_Senha"; // Senha do Wi-Fi
  ```

---

## 🧪 **Execução do Teste**
Para validar o sistema de monitoramento, siga os passos abaixo:

1. **Prepare o ambiente de teste:**
   - Corte uma garrafa PET ao meio.
   - Faça um pequeno furo na lateral da garrafa e cubra-o com fita adesiva.
   - Encha a garrafa com água até o nível desejado.

2. **Configure o sensor:**
   - Coloque o sensor de boia dentro da garrafa.
   - Conecte o ESP32 ao computador e carregue o programa para o dispositivo.

3. **Execute o teste:**
   - Abra o console da Arduino IDE para acompanhar as leituras do sensor.
   - Retire a fita adesiva da garrafa para permitir o esvaziamento gradual.
   - Observe os dados enviados ao broker MQTT e os alertas gerados no Telegram.

Para mais detalhes, assista ao vídeo do teste completo: [Vídeo do Teste](#).

---

## 📡 **Arquitetura do Sistema**
- **ESP32**: Responsável por coletar os dados do sensor de boia.
- **Broker MQTT**: Centraliza os dados recebidos do ESP32.
- **Telegram**: Notifica o usuário em caso de anormalidades no nível da água.
