// ----- Bibliotecas -----
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// ----- Constantes -----
const int SENSOR_UMIDADE = 0;
const int RELE = 4;

const char* ssid = "ALUNOS_AP_46"; /* Add your router's SSID */
const char* password = "";         /*Add the password */

// ----- Variáveis -----
int umidade_solo = 0;

// ----- Configuração do Servidor -----
ESP8266WebServer server(8080);

void setup() {
  pinMode(SENSOR_UMIDADE, INPUT);
  pinMode(RELE, OUTPUT);

  Serial.begin(115200);
  Serial.println("Conectando ao: " + (String) ssid);

  WiFi.mode(WIFI_STA);        /* Configure ESP8266 in STA Mode */
  WiFi.begin(ssid, password); /* Connect to Wi-Fi based on above SSID and Password */

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("*");
    delay(1000);
  }

  Serial.print("\n");
  Serial.print("Conectado  a rede Wi-Fi: ");
  Serial.println(WiFi.SSID());
  delay(100);

  Serial.print("\n");
  
  // Configura as funções das páginas
  server.on("/", handle_pagina_inicial);
  server.on("/umidade", handle_umidade);
  server.on("/temperatura_ar", handle_umidade);
  server.on("/ligar_bomba", handle_ligar_bomba);
  server.on("/desligar_bomba", handle_desligar_bomba);

  server.onNotFound(handle_nao_existe);

  // Inicializa o servidor
  server.begin();
  
  Serial.println("Servidor ESP8266 inicializado com sucesso!");
  Serial.print("\n");
  Serial.print("A URL do servidor é: ");
  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.print("\n");
}

/*
 * Resposta da página inicial
 */
void handle_pagina_inicial()
{
  server.send(200, "text/html", "Projeto de irrigação");
}

/*
 * Resposta de uma página que não existe
 */
void handle_nao_existe()
{
  server.send(200, "text/html", "Endpoint não existe ou incorreto");
}

void handle_ligar_bomba(){
  
  Serial.println("Bomba acionada");
  digitalWrite(RELE, HIGH);
  server.send(200, "text/html", "bomba ligada");

}

void handle_desligar_bomba(){
  
  Serial.println("Bomba desligada");
  digitalWrite(RELE, LOW);
  server.send(200, "text/html", "bomba desligada");
}

void handle_umidade()
{

  umidade_solo = analogRead(SENSOR_UMIDADE);

  umidade_solo = map(umidade_solo, 300, 600, 100, 0);

  Serial.println("Umidade solo: " + (String) umidade_solo);
  server.send(200, "text/html", (String) umidade_solo);
}

void loop() {

  // Responde às requisições feitas
  server.handleClient();

}
