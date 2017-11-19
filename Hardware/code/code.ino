
#include <ESP8266WiFi.h> //INCLUSÃO DA BIBLIOTECA NECESSÁRIA PARA FUNCIONAMENTO DO CÓDIGO
#include <Stepper.h>


const char* ssid = "****";
const char* password = "****";

const int stepsPerRevolution = 200;
Stepper myStepper(stepsPerRevolution, D1, D2, D5, D6);

WiFiServer server(80);

void setup() {
  myStepper.setSpeed(60);
  
  Serial.begin(115200);
  delay(10);

  Serial.println("");
  Serial.print("Conectando a ");
  Serial.print(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("."); //ESCREVE O CARACTER NA SERIAL
  }
  Serial.println("");
  Serial.print("Conectado a rede sem fio ");
  Serial.println(ssid);

  server.begin();
  Serial.println("Servidor iniciado");

  Serial.print("IP para se conectar ao NodeMCU: ");
  Serial.print("http://"); //ESCREVE O TEXTO NA SERIAL
  Serial.println(WiFi.localIP()); //ESCREVE NA SERIAL O IP RECEBIDO DENTRO DA REDE SEM FIO (O IP NESSA PRÁTICA É RECEBIDO DE FORMA AUTOMÁTICA)
}
void loop() {
  WiFiClient client = server.available(); //VERIFICA SE ALGUM CLIENTE ESTÁ CONECTADO NO SERVIDOR
  if (!client) {
    return;
  }
  Serial.print("Frequency = ");
  while (!client.available()) {
    delay(1);
  }

  float newFrequency = getnewFrequency(client);

  Serial.println(newFrequency);

}

float getnewFrequency(WiFiClient client) {
  String request = client.readString();
  request = request.substring(request.indexOf("$") + 1, request.lastIndexOf("$"));
  Serial.println(request);
  myStepper.step(stepsPerRevolution);
  client.flush();
  delay(2000);
  return (request.toFloat());
}

