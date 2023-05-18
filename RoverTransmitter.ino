#include<SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Arduino.h>
#include <TinyMPU6050.h>

MPU6050 mpu (Wire);

//BAGLANTI PİNLERİ
RF24 radio (9,10); //ce csn
const byte kanal[6] = "00001";
float mesaj[5];
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(kanal);
   radio.setPALevel(RF24_PA_MIN);
   radio.setDataRate( RF24_250KBPS );
  radio.stopListening();
  mpu.Initialize();
  //SENSOR BAŞLATMA
  // KALİBRASYON
  Serial.println("=====================================");
  Serial.println("Starting calibration...");
  mpu.Calibrate();
  Serial.println("Calibration complete!");
  pinMode(4,INPUT_PULLUP);//Rover
  pinMode(5,INPUT_PULLUP);//Turret
  pinMode(7,INPUT_PULLUP);//Shoot
}

void loop() {
  mpu.Execute();
  
int num=mpu.GetAccX()*100;
float temp=num/100;
num=(num/10)*10;
float XF=(float)num/100;
  float a=(mpu.GetAccX());
  float b=(mpu.GetAccY());
  //MESAJ YOLLAMA---- X,Y,MOD,ATEŞ
   mesaj[0]=a;
    mesaj[1]=b;
    mesaj[2]=digitalRead(5);
    mesaj[3]=digitalRead(7);
  radio.write(&mesaj,sizeof(mesaj));
//  Serial.println(x);
 // Serial.println("y: ");Serial.println(y);
 
delay(100);


 
}
