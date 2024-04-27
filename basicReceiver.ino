#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);  // CE, CSN
const byte kanal[6] = "00001";
float mesaj[5];

void setup() {
  Serial.begin(9600);
  radio.begin();
  delay(1000);
  radio.openReadingPipe(0, kanal);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();
}

void loop() {
  if (radio.available()) {    
    radio.read(&mesaj, sizeof(mesaj));
    Serial.println("Receiver 1:");
    Serial.print("x: ");
    Serial.println(mesaj[0]);
    Serial.print("y: ");
    Serial.println(mesaj[1]);
    delay(100); // Optional delay
  }
}
