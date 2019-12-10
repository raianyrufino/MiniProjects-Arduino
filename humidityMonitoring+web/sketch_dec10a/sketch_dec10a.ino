// Porta que receberá o sinal digital do sensor
#define sensor 13
// Porta que acenderá o led vermelho quando a areia estiver seca
#define pino_led_vermelho 16
// Porta que acenderá o led amarelo quando a areia estiver molhada
#define pino_led_amarelo 14
//Porta que acionará o buzzer
#define buzzer 12

#include <ESP8266WiFi.h>

/*
 * ssid é o nome do wifi
 */
char ssid [] = "....";
char senha[] = "...";

WiFiServer server(80);

void setup() {
    
  Serial.begin(9600);
  delay(10);

  conectaWiFi(ssid,senha);
  
  server.begin();
  Serial.println("Servidor inicializado");

  pinMode(pino_led_vermelho, OUTPUT);
  pinMode(pino_led_amarelo, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(sensor, INPUT_PULLUP);
}

void loop() {
  // Iniciando o circuito
  readSensor();

  WiFiClient client = server.available();
  if(!client){
    return;
  }

  while(!client.available()){
    delay(1);
  }

  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();  

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<meta http-equiv='refresh' content='2'>");
  client.println("<h1>Controle o xixi do seu gatinho (:</h1>");
  client.println("<h2>Staus da areia:</h2>");

  if(digitalRead(sensor)){
  client.println("<h1>Sua areia esta sequinha, parabens \\o </h1>");
  } else
  client.println("<h1>HORA DE TROCAR A AREIA D: </h1>");

  client.println("</html>");
   
}


void conectaWiFi(char SSID[],char SENHA[]){

  Serial.print("Conectando a rede ");
  Serial.println(SSID);

  WiFi.begin(SSID,SENHA);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println(" ");
  Serial.println("WiFi Conectado");
  Serial.println("Endereço de IP: ");
  Serial.println(WiFi.localIP());
}

void readSensor(){
  if(digitalRead(sensor)){
    digitalWrite(buzzer, LOW);
    digitalWrite(pino_led_vermelho, LOW);
    digitalWrite(pino_led_amarelo, HIGH);  
  } else {
    digitalWrite(pino_led_amarelo, LOW);
    digitalWrite(pino_led_vermelho, HIGH);
    digitalWrite(buzzer, HIGH);
  }
}
