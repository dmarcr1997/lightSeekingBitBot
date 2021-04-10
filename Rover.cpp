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
    lightValues();
    moveRover();
}
void moveRover()
{
    lightSensorValue1 = analogRead(lightInPin1);
    lightSensorValue2 = analogRead(lightInPin2);

    float tempVal = temperature();
    float humVal = humidity();

    if (humVal <= 70 && tempVal <= 50)
    {
        //front
        if ((lightSensorValue1 - lightSensorValue2) < 40)
        {
            Serial.println("FRONT");
            digitalWrite(ML_Ctrl,LOW
            analogWrite(ML_PWM,400);
            digitalWrite(MR_Ctrl,LOW);
            analogWrite(MR_PWM,365);
        }
        else if (lightSensorValue1 > 200 || lightSensorValue2 > 200)
        {
            Serial.println("STOP");
            analogWrite(ML_PWM, 0);
            analogWrite(MR_PWM, 0);
        }
        else if (lightSensorValue1 < lightSensorValue2 && (lightSensorValue1 - lightSensorValue2 < 100))
        {
            Serial.println("LEFT");
            digitalWrite(ML_Ctrl, HIGH);
            analogWrite(ML_PWM, 400);
            digitalWrite(MR_Ctrl, LOW);
            analogWrite(MR_PWM, 400);
        }
        //right
        else if (lightSensorValue1 > lightSensorValue2 && (lightSensorValue1 - lightSensorValue2 < 100))
        {
            Serial.println("RIGHTT");
            digitalWrite(ML_Ctrl, LOW);
            analogWrite(ML_PWM, 400);
            digitalWrite(MR_Ctrl, HIGH);
            analogWrite(MR_PWM, 400);
        }
        //stop
        else
        {
            analogWrite(ML_PWM, 0);
            analogWrite(MR_PWM, 0);
        }
    }
    else
    {
        Serial.println("BACK");
        digitalWrite(ML_Ctrl, HIGH);
        analogWrite(ML_PWM, 400);
        digitalWrite(MR_Ctrl, HIGH);
        analogWrite(MR_PWM, 365);
    }
    //left

    delay(2000); //delay in 2s
}
float temperature()
{
    float temp = dht.readTemperature();
    return temp;
}

float humidity()
{
    float humidity = dht.readHumidity();
    return humidity;
}

void lightValues()
{
    lightSensorValue1 = analogRead(lightInPin1);
    lightSensorValue2 = analogRead(lightInPin2);
    Serial.println("Light1 = ");
    Serial.println(lightSensorValue1);
    Serial.println("Light2 = ");
    Serial.println(lightSensorValue2);
}