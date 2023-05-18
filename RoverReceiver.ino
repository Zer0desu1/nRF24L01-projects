#include<SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include<Servo.h>
#include "HCPCA9685.h"

#define  I2CAdd 0x40
HCPCA9685 HCPCA9685(I2CAdd);


//SOLAR SİSTEM TANIMLAMA
int l1=0,l2=0,l3=0,l4=0;
int ldr1=A0,ldr2=A1,ldr3=A2,ldr4=A3;
int m1=0,m2=0,m3=0;
int servo1=0,servo2=0;


//KABLOSUZ HABERLEŞME
RF24 radio (9,10); //ce csn
const byte kanal[6] = "00001";
float mesaj[5];

void setup() {
  digitalWrite(2,HIGH);
  Serial.begin(9600);//SERİ HABERLEŞME BAŞLATMA


  //KABLOSUZ HABERLEŞME BAŞLATMA
  radio.begin();
  delay(1000);
  radio.openReadingPipe(0,kanal);
   radio.setPALevel(RF24_PA_MIN);
   radio.setDataRate( RF24_250KBPS );
  radio.startListening();

  //SERVO DRİVER BAŞLATMA
  HCPCA9685.Init(SERVO_MODE);
  HCPCA9685.Sleep(false);  

  HCPCA9685.Servo(14, 0);//solar
  HCPCA9685.Servo(15, 20);//solar
  
  HCPCA9685.Servo(3, 210);//0,420
  HCPCA9685.Servo(7, 210);//100,300 
  HCPCA9685.Servo(5, 175);//50,95


  //DC MOTOR DRİVER PİNLERİ
  pinMode(2,OUTPUT);
  pinMode(5,OUTPUT);//1
  pinMode(6,OUTPUT);//2
  pinMode(7,OUTPUT);//3
  pinMode(8,OUTPUT);//4
  pinMode(3,OUTPUT);//enA ve enB
  
  pinMode(4,OUTPUT);//RÖLE

}

void ileri(int y){
  analogWrite(3,y);
  digitalWrite(5,HIGH);digitalWrite(6,LOW);
 
  digitalWrite(7,HIGH);digitalWrite(8,LOW);
  
}void geri(int y){
  analogWrite(3,y);
  digitalWrite(5,LOW);digitalWrite(6,HIGH);
  
  
  digitalWrite(7,LOW);digitalWrite(8,HIGH);
  
}

void sag(int x){

   analogWrite(3,x);
   digitalWrite(5,HIGH);digitalWrite(6,LOW);
   digitalWrite(7,LOW);digitalWrite(8,HIGH);

}

void sol(int x){
  analogWrite(3,x);
   digitalWrite(5,LOW);digitalWrite(6,HIGH);
   digitalWrite(7,HIGH);digitalWrite(8,LOW);
  
  }
void stopp(){
  digitalWrite(5,LOW);digitalWrite(6,LOW);
  digitalWrite(7,LOW);digitalWrite(8,LOW);
}


void solar(){
   l1=analogRead(ldr1)/10;
  l2=analogRead(ldr2)/10;
  l3=analogRead(ldr3)/10;
  l4=analogRead(ldr4)/10;


  m1=max(l1,l2);
  m2=max(l3,l4);
  m3=max(m1,m2);
  if(m3>l1&&m3>l2){
    if(servo1<=120)
      servo1+=1;
    HCPCA9685.Servo(15, servo1);    delayMicroseconds(10);

  }
  if(m3>l3&&m3>l4){
    if(servo1>0)
      servo1-=1;
    HCPCA9685.Servo(15, servo1);    delayMicroseconds(10);

  }
  if(m3>l3&&m3>l2){
    if(servo2>0)
      servo2-=1;
    HCPCA9685.Servo(14, servo2);    delayMicroseconds(10);

  }if(m3>l1&&m3>l4){
    if(servo2<=370)
      servo2+=1;
    HCPCA9685.Servo(14, servo2);
    delayMicroseconds(10);
  }
 HCPCA9685.Servo(14, 210);
}


void loop() {
  if(radio.available()){    //bağlantıyı kontrol eder
      radio.read(&mesaj,sizeof(mesaj));//mesaj alır
    
     Serial.print("x"); Serial.println(mesaj[0]);
     Serial.print("y");  Serial.println(mesaj[1]);
     float x=mesaj[0]; float y=mesaj[1];
     
     // datayı servo derecesine sınırlandıran işlemler
     x=x+1;x=x*(-90)+180;
     y=y+1; y=y*(-60)+150;
     
     Serial.print("x");Serial.println(x);
     Serial.print("y");Serial.println(y);


    //mod değiştiyse;
if(mesaj[2]){//TARET AKTİF
  digitalWrite(4,LOW);
    y=map(y,0,180,70,300);
    x=map(x,0,180,0,420);
    HCPCA9685.Servo(3, x);//TARET HAREKET
    HCPCA9685.Servo(7,y);
    if(mesaj[3]){//TARET ATEŞLEME
  HCPCA9685.Servo(5, 0);
  delay(100);
  HCPCA9685.Servo(5,175);
    }
}
else{//ROVER AKTİF
  digitalWrite(4,HIGH);
    if(y<=45){//DATAYA GÖRE FONKSİYON ÇAĞIRMA
      int temp=(-2.3)*y+255;
      ileri(temp);
      }
    else if(y>=120){
      int temp=(1.75)*y-60;
      geri(temp);
    }
    else if(x<=25){
      int temp=(-4.2)*x+255;
      sag(temp);
    }
    else if(x>=145){
      int temp=(0.95)*x+13;
      sol(temp);
    }
    else{
      stopp();
    }
}
}

//solar();//SOLAR DAİMA ÇALISMALI
}
