/*********
  Mohamed Al Howaidi
*********/

#include <esp_now.h>
#include <WiFi.h>

//Estructura del reciver
//Debe coincidir con la estructura del sender
typedef struct test_struct {
  int x;
  int y;
  int z;
  int a;
  int b;
} test_struct;

//Crea un mensaje en la variable Datos
test_struct Datos;

//Después de que se reciban existósamente los datos
void DatosRecibidos(const uint8_t * mac, const uint8_t *DatosLlegando, int len) {
  memcpy(&Datos, DatosLlegando, sizeof(Datos));
  Serial.print("Bytes recbidos: ");
  Serial.println(len);
  Serial.print("x: ");
  Serial.println(Datos.x);
  Serial.write(Datos.x);
  //Serial.print("y: ");
  //Serial.println(Datos.y);
  //Serial.write(Datos.y);
  //Serial.println("Z: ")
  //Serial.println(Datos.z);
  //Serial.write(Datos.z);
  //Serial.println("a: ");
  //Serial.println(Datos.a);
  //Serial.write(Datos.a);
  //Serial.println("b: ")
  //Serial.println(); 
}
 
void setup() {
  //Inizializa el serial
  Serial.begin(115200);
  
  //Configura el dispositivo como si fuera una "estación wifi", es decir como un reciver
  WiFi.mode(WIFI_STA);

  //Configuración del protocolo de ESP-NOW 
  if (esp_now_init() != ESP_OK) {
    Serial.println("Casi crack, no se ha inicializado bien ESP-NOW");
    return;
  }
  
  // Una vez que funciona el protcolo, se registran las notificaciones del reciver
  // para que el sender sepa la informacion del paquete en el reciver
  esp_now_register_recv_cb(DatosRecibidos);
}
 
void loop() {

}
