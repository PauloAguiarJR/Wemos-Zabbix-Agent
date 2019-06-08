
#include <ESP8266WiFi.h>
#include "settings.h"
#include "zabbixSender.h"

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>


DHT_Unified dht(DHTPIN, DHTTYPE);

float temp = 0;
float humid = 0;

// ****** Main ******

void setup() { 
    
    Serial.begin(115200);
    delay(100);    
    
    Serial.println("=== SETUP Start ===");
    
    pinMode(GREEN_LED, OUTPUT);

    Serial.println("Waiting for WiFi... ");   
    
    // Indicação para o Usuario
    digitalWrite(GREEN_LED, LOW);   
    WiFi.begin(ssid, wifiKey);
    WiFi.hostname(hostname);
    // current wifi ap connect cycles
    int wccycles = 0;
    while(WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        Serial.print((int)WiFi.status());
        wccycles++;
        
        // tentando maxWifiConnectcycles do que deepsleep
        if (wccycles > maxWifiConnectcycles) {
          Serial.printf("\n[ERROR] WLAN TIMEOUT - Abort\n");
          // indicate to user
          digitalWrite(GREEN_LED, HIGH);
          delay(1000);
         }
          digitalWrite(GREEN_LED, HIGH);
          delay(150);
          digitalWrite(GREEN_LED, LOW);
          delay(150);
    }
    
    // Apos conectar
    digitalWrite(GREEN_LED, HIGH); 
    Serial.printf("\nHostname: ");
    Serial.print(WiFi.hostname());  
    Serial.printf("\nWiFi connected. IP address:");
    Serial.println(WiFi.localIP());
    

    // Iniciar sensor DHT22
    beginDHT22();
    
    Serial.println("=== SETUP End ===");
   
} // setup





void loop() {
    Serial.println("=== START ==="); 
    // init sensor

    if(WiFi.status() == WL_CONNECTED){ Serial.println("Waiting for WiFi... ");  digitalWrite(GREEN_LED, LOW); }
      
     while(WiFi.status() != WL_CONNECTED) {
          
          Serial.print(".");
          Serial.print((int)WiFi.status());
          
          digitalWrite(GREEN_LED, HIGH);
          delay(250);
          digitalWrite(GREEN_LED, LOW);
          delay(750);
          
         }

        readDHT22(); // leitura dos dados do sensor
        
    Serial.printf("Valores Coletados. temp:%f, hum:%f\n", temp, humid);

    ZabbixSender zs;
    String jsonPayload;
    jsonPayload = zs.createPayload(hostname, temp, humid);
      
    // criar mensaagem no formato zabbix sender
    String msg = zs.createMessage(jsonPayload);
    
    // conectar e transmitir
    Serial.printf("==> Conectando para :%s:%d\n", server, port);
     digitalWrite(GREEN_LED, LOW);
    
    // conectar ao servidor
    WiFiClient client;
    
    if (!client.connect(server, port)) {
        Serial.printf("[ERROR] Conexão Falhou - Abortar\n"); 
        digitalWrite(GREEN_LED, HIGH);
        delay(1000);
            //Aguarda o proximo envio
        delay(sendDataIntervallSec * 1000);

    }
    
    Serial.println("Conectado. Enviado dados");
    client.print(msg);  
    unsigned long startTime = millis();
    while (client.available() == 0) {
        if (millis() - startTime > maxTransmissionTimeout) {
            Serial.printf("[ERROR] Transmissão TIMEOUT - Abortar \n");
            client.stop();
            // indicate to user
            digitalWrite(GREEN_LED, HIGH);
            delay(1000);            
                //Aguarda o proximo envio
            delay(sendDataIntervallSec * 1000);
        }
    }
    
  
    digitalWrite(GREEN_LED, HIGH);

    // ler replica do zabbix server
    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
    }    
    Serial.println();

    Serial.printf("Fechando Conexão - Aguarda por %d sec...\n", sendDataIntervallSec);
    client.stop(); 

    //Aguarda o proximo envio
    delay(sendDataIntervallSec * 1000);
   
}


void readDHT22() {
  
  sensors_event_t event; 
   
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println("Erro de Leitura de Temperatura!");
  }
  else temp = event.temperature;
  
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println("Erro de Leitura da Umidade!");
  }
  else humid = event.relative_humidity;

}

void beginDHT22(){

    dht.begin();

    Serial.println("");
    Serial.println("");
    Serial.println("DHTxx Unified Sensor Example");
    
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.println("Temperatura");
    Serial.print  ("Sensor:      "); Serial.println(sensor.name);
    Serial.print  ("Driver Ver:  "); Serial.println(sensor.version);
    Serial.print  ("Unique ID:   "); Serial.println(sensor.sensor_id);
    Serial.print  ("Max Valor:   "); Serial.print(sensor.max_value); Serial.println(" *C");
    Serial.print  ("Min Valor:   "); Serial.print(sensor.min_value); Serial.println(" *C");
    Serial.print  ("Resolução:   "); Serial.print(sensor.resolution); Serial.println(" *C");  
    Serial.println("------------------------------------");
    
    dht.humidity().getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.println("Humidity");
    Serial.print  ("Sensor:       "); Serial.println(sensor.name);
    Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
    Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
    Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println("%");
    Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println("%");
    Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println("%");  
    Serial.println("------------------------------------");
 
     
}
