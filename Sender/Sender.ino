/*********
  Mohamed Al Howaidi
*********/
#include <esp_now.h>
#include <WiFi.h>

int incomingByte = 0;

// La mac a la que hará broadcast
uint8_t broadcast[] = {0x24, 0x6F, 0x28, 0xA9, 0xFA, 0x70};

typedef struct test_struct {
  int* x;
  int y;
  int z;
  int a;
  int b;
} test_struct;

test_struct test;

esp_now_peer_info_t Info;

// cuando se envian los datos (sale del sender)
void OnDataSent(const uint8_t *mac, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Paquete para: ");
  // Copia la mac del sender y la convierte en String
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Envio exitoso" : "Envio Fallido");
}

void LeerUART(){
  Serial.read();
}
 
void setup() {
  Serial.begin(115200);
 
  WiFi.mode(WIFI_STA);
 
  if (esp_now_init() != ESP_OK) {
    Serial.println("Casi crack, no se ha inicializado bien ESP-NOW");
    return;
  }
  
  esp_now_register_send_cb(OnDataSent);
   
  // Conecta las ESP32 C
  Info.channel = 0;  
  Info.encrypt = false;
    
  memcpy(Info.peer_addr, broadcast, 6);
  if (esp_now_add_peer(&Info) != ESP_OK){
    Serial.println("Fallo en intentar conectar");
    return;
  }
}
 
void loop() {
  // send data only when you receive data:
  int n = Serial.available();
  int circularBuffer[n];
  int circularBufferIndex = 0;

  if (n > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    for (int thisPin = 0; thisPin < n; thisPin++) {
      circularBuffer[circularBufferIndex] = incomingByte;
      circularBufferIndex++;
      if (circularBufferIndex >= n) circularBufferIndex = 0;
    }
    
    // say what you got:
    // Serial.println(incomingByte, DEC)
    test.x = circularBuffer;

    esp_err_t result = esp_now_send(0, (uint8_t *) &test, sizeof(test_struct));
   
   if (result == ESP_OK) {
    Serial.println("Datos enviados exitosamente");
   }
   else {
    Serial.println("Error en el envío de datos");
   }
   delay(2000);
   } 
}
