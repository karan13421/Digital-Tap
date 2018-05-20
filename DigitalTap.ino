#include <Servo.h>
#include <SoftwareSerial.h>

Servo myservo; 

const int trig=5;
const int echo=6;

const int ir=A2;
int seek=0;

const int tx=10;
const int rx=11;

SoftwareSerial bluetooth(rx,tx);

int flow=180;

void setup() 
{
 myservo.attach(9);
 pinMode(trig,OUTPUT);
 pinMode(echo,INPUT);
 pinMode(ir,INPUT);
 bluetooth.begin(9600);
 Serial.begin(9600);
}


void loop() 
{
  int semaphore=0;
  int flow=0;
  int detect;
  
  if(bluetooth.available()>0)
  {
    seek=0;//bluetooth.read(); //to turn on seek=1; to turn off seek=0
  }
  
  if(seek>=1)
  {
    flow=seek;
    if(flow==180)
    servoTapOpen(180);
    else
    {
      if(seek==1)
      servoTapClose(0);
      else
      if((flow>=10)||(flow<=160))
      servoTapOpen(flow);
    }
  }
  else
  {
    detect=sonarDetector();
    Serial.print("?");
    Serial.print(detect);
    if(detect<50)
    {
     while(detect<50)
     {
      if(analogRead(ir)>600)
      {
         while(analogRead(ir)>600) 
         {
         Serial.print(":");
         Serial.print(analogRead(ir));
         servoTapOpen(160);
         semaphore=1;
         delay(10);
         }
        servoTapClose(1);
      }
      delay(100);
      detect=sonarDetector();
     }
     servoTapOpen(180);
     delay(1000);
    }
    servoTapClose(1);
  }

  
}

int sonarDetector()
{
  int distance;
  float duration;
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  duration=pulseIn(echo,HIGH);
  distance=duration*0.034/2;
  return distance;
}

void servoTapOpen(int flow)
{
  myservo.write(flow);
}

void servoTapClose(int flow)
{
  myservo.write(flow);              
}

