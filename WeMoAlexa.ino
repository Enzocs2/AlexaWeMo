#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"

/* Credenciais de rede Wi-Fi */
#define WIFI_SSID "Repour 2.4"
#define WIFI_PASS "repour2011"

#define SERIAL_BAUDRATE 115200

/* Emulação de dispositivos Belkin WeMo */
fauxmoESP fauxmo;

/* Define os pinos dos relés */
#define RELAY_1 D5
#define RELAY_2 D6
#define RELAY_3 D7
#define RELAY_4 D8

void setup() 
{
   Serial.begin(SERIAL_BAUDRATE);
   // Configuração e conexão Wi-Fi
   wifiSetup();
 
   // Define os pinos dos relés como saída
   pinMode(RELAY_1, OUTPUT);
   pinMode(RELAY_2, OUTPUT);
   pinMode(RELAY_3, OUTPUT);
   pinMode(RELAY_4, OUTPUT);

   // Define cada pino do relé como HIGH (NOTA: os relés usam lógica inversa)
   digitalWrite(RELAY_1, HIGH);   
   delay(500);
   digitalWrite(RELAY_2, HIGH);  
   delay(500);
   digitalWrite(RELAY_3, HIGH);  
   delay(500);
   digitalWrite(RELAY_4, HIGH);   
   delay(500);
    
   // Nomes dos dispositivos para simular interruptores WeMo
   fauxmo.addDevice("Light One");
   fauxmo.addDevice("Light Two");
   fauxmo.addDevice("Outlet One");
   fauxmo.addDevice("Outlet Two");
   fauxmo.addDevice("Bed Room");
   fauxmo.addDevice("Living Room");
   fauxmo.addDevice("All Devices");
   fauxmo.onMessage(callback); 
}

void loop() 
{
  fauxmo.handle();
}

/* -----------------------------------------------------------------
 Função de retorno para ação do dispositivo
 -----------------------------------------------------------------*/
void callback(uint8_t device_id, const char * device_name, bool state) 
{
  Serial.print("Device "); Serial.print(device_name); 
  Serial.print(" state: ");
  if (state) 
  {
    Serial.println("ON");
  } 
  else 
  {
    Serial.println("OFF");
  }
  
  // Ação de comutação ao detectar o nome do dispositivo
  
  if ( (strcmp(device_name, "Light One") == 0) ) 
  {
    if (!state) 
    {
      digitalWrite(RELAY_1, HIGH); // Desliga o relé (lógica inversa)
    } 
    else 
    {
      digitalWrite(RELAY_1, LOW); // Liga o relé (lógica inversa)
    }
  }

  // Repita o processo para outros dispositivos...

}

/* -----------------------------------------------------------------
 Configuração de conexão Wi-Fi
------------------------------------------------------------------*/
void wifiSetup() 
{
   // Configura o módulo Wi-Fi para o modo STA (Estação)
   WiFi.mode(WIFI_STA);

   // Conecta-se à rede Wi-Fi
   Serial.println ();
   Serial.printf("[WIFI] Conectando a %s ", WIFI_SSID);
   Serial.println();
   WiFi.begin(WIFI_SSID, WIFI_PASS);

   // Aguarda a conexão
   while (WiFi.status() != WL_CONNECTED) 
   {
      Serial.print(".");
      delay(100);
   }
   Serial.print(" ==> CONECTADO!" );
   Serial.println();

   // Conectado!
   Serial.printf("[WIFI] Modo ESTAÇÃO, SSID: %s, Endereço IP: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
   Serial.println();
}
