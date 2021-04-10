#include "DHT.h"
#define DHTPIN 7
#define DHTTYPE DHT11

#define ML_Ctrl 13 //define the direction control pin of left motor
#define ML_PWM 11  //define the PWM control pin of left motor
#define MR_Ctrl 12 //define direction control pin of right motor
#define MR_PWM 3   // define the PWM control pin of right motor

float tempVal = 0;
int lightInPin1 = A2;
int lightInPin2 = A4;
int lightSensorValue1 = 0;
int lightSensorValue2 = 0;
DHT dht(DHTPIN, DHTTYPE);
void setup()
{
    // put your setup code here, to run once:
    pinMode(ML_Ctrl, OUTPUT);
    pinMode(ML_PWM, OUTPUT);
    pinMode(MR_Ctrl, OUTPUT);
    pinMode(MR_PWM, OUTPUT);
    dht.begin();
    Serial.begin(9600);
}

void loop()
{
}

void temperature()
{
    Serial.println("Temperature = ");
    Serial.println(dht.readTemperature());
    Serial.println("Humidity = ");
    Serial.println(dht.readHumidity());
    delay(1000);
}

void lightValues()
{
    lightSensorValue1 = analogRead(lightInPin1);
    lightSensorValue2 = analogRead(lightInPin2);
    Serial.println(lightSensorValue1);
    delay(200);
    Serial.println(lightSensorValue2);
    delay(500);
}