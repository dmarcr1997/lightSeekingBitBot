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

int lightSensorValue1 = 0;
int lightSensorValue2 = 0;
float tempVal;
float humVal;
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
  checkTemp();
  moveRover();
}
void moveRover(){
  lightSensorValue1 = analogRead(lightInPin1);
  lightSensorValue2 = analogRead(lightInPin2);
  
  tempVal = temperature();
  humVal = humidity();
  
  hitDistance = obstacleDist();
  Serial.println("Obstacle =");
  Serial.println(hitDistance);
  if(humVal <=70 && tempVal <= 80 && hitDistance > 5 ){
    //front
    if((lightSensorValue1 - lightSensorValue2) < 40){ 
      Serial.println("FRONT");
      digitalWrite(ML_Ctrl,LOW);
      analogWrite(ML_PWM,400);
      digitalWrite(MR_Ctrl,LOW);
      analogWrite(MR_PWM,365);
    }
    else if(lightSensorValue1 > 200 || lightSensorValue2 > 200){
      Serial.println("STOP");
      analogWrite(ML_PWM,0);
      analogWrite(MR_PWM,0);
    }
    else if(lightSensorValue1 < lightSensorValue2 && (lightSensorValue1 - lightSensorValue2 < 100)){ 
      Serial.println("LEFT");
      digitalWrite(ML_Ctrl,HIGH);
      analogWrite(ML_PWM,400);
      digitalWrite(MR_Ctrl,LOW);
      analogWrite(MR_PWM,400);
    }
    //right
    else if(lightSensorValue1 > lightSensorValue2 && (lightSensorValue1 - lightSensorValue2 < 100)){
      Serial.println("RIGHTT");
      digitalWrite(ML_Ctrl,LOW);
      analogWrite(ML_PWM,400);
      digitalWrite(MR_Ctrl,HIGH);
      analogWrite(MR_PWM,400);
    }
    //stop
    else {
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
  }
   //left
  
  delay(2000);//delay in 2s
}
float temperature(){
   float temp = dht.readTemperature();
   Serial.println("TEMP = ");
   Serial.println(temp);
   return temp; 
}

float humidity(){
    float humidity = dht.readHumidity();
    Serial.println("HUM = ");
    Serial.println(humidity);
    return humidity;
}

void lightValues(){
  lightSensorValue1 = analogRead(lightInPin1);
  lightSensorValue2 = analogRead(lightInPin2);
  Serial.println("Light1 = ");
  Serial.println(lightSensorValue1);  
  Serial.println("Light2 = ");
  Serial.println(lightSensorValue2);
}

void checkTemp(){
  tempVal = temperature();
  humVal = humidity();
  if(tempVal > 100 && humVal > 70){
    digitalWrite(tempLEDPin, 255);
  } else if (tempVal > 70 || humVal > 50){
    digitalWrite(tempLEDPin, 150);
  } else if (tempVal > 30 ||   humVal > 30){
    digitalWrite(tempLEDPin, 50); 
  } else {
    analogWrite(tempLEDPin, 0);
  }
}
long obstacleDist() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  cm = (duration/2) / 29.1;
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  delay(250);
  return cm;
}

