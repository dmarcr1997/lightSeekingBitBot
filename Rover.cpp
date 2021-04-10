#include "DHT.h"
#define DHTPIN 10
#define DHTTYPE DHT11
#define tempLEDPin 8
#define ML_Ctrl 13  //define the direction control pin of left motor
#define ML_PWM 11   //define the PWM control pin of left motor
#define MR_Ctrl 12  //define direction control pin of right motor
#define MR_PWM 3   // define the PWM control pin of right motor
 
int lightInPin1 = A2;
int lightInPin2 = A4;  
int maxValue = 0;
int lightSensorValue1 = 0;
int lightSensorValue2 = 0;
int frontVal = 0;
int tolerance = 100;
int sittingValue = 600;
float tempVal;
DHT dht(DHTPIN, DHTTYPE);
int trigPin = 5;
int echoPin = 4;
long duration, cm;
long hitDistance;
void setup() {
  // put your setup code here, to run once:
  pinMode(ML_Ctrl, OUTPUT);
  pinMode(ML_PWM, OUTPUT);
  pinMode(MR_Ctrl, OUTPUT);
  pinMode(MR_PWM, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  dht.begin();
  Serial.begin(9600);
}

void loop() {
  lightValues();
  checkTemp();
  moveRover();
}
void moveRover(){
  lightSensorValue1 = analogRead(lightInPin1);
  lightSensorValue2 = analogRead(lightInPin2);
  frontVal = (lightSensorValue1 + lightSensorValue2) / 2;
  tempVal = temperature();
  maxValue = max(lightSensorValue1, lightSensorValue2);
  maxValue = max(maxValue, frontVal); 
  hitDistance = obstacleDist();
  Serial.println("Obstacle =");
  Serial.println(hitDistance);
  
  if(tempVal <= 28 && hitDistance > 20 ){
    if (maxValue >= sittingValue){
      Serial.println("STOP");
      analogWrite(ML_PWM,0);
      analogWrite(MR_PWM,0);
    }
    else if(frontVal > tolerance && frontVal == maxValue){
      Serial.println("FRONT");
      digitalWrite(ML_Ctrl,LOW);
      analogWrite(ML_PWM,400);
      digitalWrite(MR_Ctrl,LOW);
      analogWrite(MR_PWM,365);
      delay(1000);
      Serial.println("STOP");
      analogWrite(ML_PWM,0);
      analogWrite(MR_PWM,0);
    }
    else if(lightSensorValue2 > tolerance && lightSensorValue2 == maxValue){ 
      Serial.println("LEFT");
      digitalWrite(ML_Ctrl,HIGH);
      analogWrite(ML_PWM,400);
      digitalWrite(MR_Ctrl,LOW);
      analogWrite(MR_PWM,400);
      delay(1000);
      Serial.println("STOP");
      analogWrite(ML_PWM,0);
      analogWrite(MR_PWM,0);
    }
    //right
    else if(lightSensorValue1 > tolerance && lightSensorValue1 == maxValue){
      Serial.println("RIGHT");
      digitalWrite(ML_Ctrl,LOW);
      analogWrite(ML_PWM,400);
      digitalWrite(MR_Ctrl,HIGH);
      analogWrite(MR_PWM,400);
      delay(1000);
      Serial.println("STOP");
      analogWrite(ML_PWM,0);
      analogWrite(MR_PWM,0);
    } else { 
      Serial.println("STOP");
      analogWrite(ML_PWM,0);
      analogWrite(MR_PWM,0);
    }
  }
  else {
    Serial.println("BACK");
    digitalWrite(ML_Ctrl,HIGH);
    analogWrite(ML_PWM,400); 
    digitalWrite(MR_Ctrl,HIGH);
    analogWrite(MR_PWM,400);

    Serial.println("STOP");
    analogWrite(ML_PWM,0);
    analogWrite(MR_PWM,0);

    tempVal = temperature();
  
    hitDistance = obstacleDist();
  }
   
  
  delay(1000);//delay in 2s
}
float temperature(){
   float temp = dht.readTemperature();
   Serial.println("TEMP = ");
   Serial.println(temp);
   return temp; 
}

void lightValues(){
  lightSensorValue1 = analogRead(lightInPin1);
  lightSensorValue2 = analogRead(lightInPin2);
//  Serial.println("Light1 = ");
//  Serial.println(lightSensorValue1);  
//  Serial.println("Light2 = ");
//  Serial.println(lightSensorValue2);
}

void checkTemp(){
  tempVal = temperature();
  digitalWrite(tempLEDPin, HIGH);
  if(tempVal > 30){
    delay(10);
  } else if (tempVal > 25){
    delay(200);
  } else if (tempVal > 10){
    delay(500); 
  } 
  digitalWrite(tempLEDPin, 0);
}
long obstacleDist() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  cm = (duration/2) / 29.1;
//  Serial.print(cm);
//  Serial.print("cm");
//  Serial.println();
  return cm;
}
