#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);  // CE, CSN
const byte kanal[6] = "00001";
float mesaj[5];

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(kanal);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();
}

void loop() {
  mesaj[0] = 42;
  mesaj[1] = 23;
  
  // Send data to receiver 1
  radio.write(&mesaj, sizeof(mesaj));
  delay(100); // Optional delay
  
  // Send data to receiver 2
  radio.write(&mesaj, sizeof(mesaj));
  delay(100); // Optional delay
}
