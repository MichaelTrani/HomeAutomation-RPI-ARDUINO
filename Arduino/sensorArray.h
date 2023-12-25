#include "HardwareSerial.h"
#include "Arduino.h"
#include "waterPump.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>

#define ERRORCODE -1111
#define PLANT_COUNT 5
#define DHTTYPE DHT11  //Select DHT 11 configuration

// Digital Pins
#define ONE_WIRE_BUS 2  //Dallas Temperature
#define DHT_PIN 4        //DHT sensor
#define MIC_LED_PIN 8        //Microphone's LED
// Analog Pins
#define MIC_DATA_PIN A0  //Microphone's input
#define LIGHT_PIN A1
#define HALL_PIN A2

#define Plant_0 A3
#define Plant_1 A4
#define Plant_2 A5
#define Plant_3 A6
#define Plant_4 A7

#define Plant_Water_Point_0


// Library builds
OneWire oneWire(ONE_WIRE_BUS);        // Establish oneWire instance
DallasTemperature sensors(&oneWire);  // Send oneWire reference to Dallas Temperature.
DHT dht(DHT_PIN, DHTTYPE);             // Establish DHT object

struct sensorArray {
  int humidVal, lightAverage, noiseLevel, hallVal, gasVal = ERRORCODE;
  float tempAvg = ERRORCODE;
  int plantMoisture[PLANT_COUNT];

  void sensorRun() {
    getTemp();
    getHumid();
    getLight();
    getHall();
    getGas();
    getNoiseLevel();
    getMoisture();
  }

  // Member function
  void getMoisture(){
    plantMoisture[0] = analogRead(Plant_0);
    plantMoisture[1] = analogRead(Plant_1);
    plantMoisture[2] = analogRead(Plant_2);
    plantMoisture[3] = analogRead(Plant_3);
    plantMoisture[4] = analogRead(Plant_4);

    for(int i=0; i < PLANT_COUNT; i++){
      Serial.print("PlantMoisture:");
      Serial.print(i);
      Serial.print(",");
      Serial.println(plantMoisture[i]);
     
      int waterCheck = Serial.read(); 
      waterPump(waterCheck, Plant_Water_Point_0)

    }
  }


  void getTemp() {
    float temp[2] = { -1111, -1111 };  // 0 = Dallas Temp, 1 = DHT Temp    int age;
    sensors.requestTemperatures();
    temp[0] = sensors.getTempFByIndex(0);
    temp[1] = dht.readTemperature(true);

    tempAvg = (temp[1] + temp[0]) / 2;
    
    Serial.print("Temperature:");
    Serial.println(tempAvg);
  }

  void getHumid() {
    humidVal = dht.readHumidity();
    Serial.print("Humidity:");
    Serial.println(humidVal);
  }

  void getLight() {
    int lightReadings[10];
    int lightMax = 0;
    int lightMin = 1024;

    for (int i = 0; i < 10; i++) {
      lightReadings[i] = analogRead(LIGHT_PIN);
      lightMax = max(lightMax, lightReadings[i]);
      lightMin = min(lightMin, lightReadings[i]);
    }

    lightAverage = (lightMax + lightMin) / 2;
    lightAverage = map(lightAverage, 0, 1023, 0, 255);
    Serial.print("Light:");
    Serial.println(lightAverage);
  }

  void getHall() {
    hallVal = analogRead(HALL_PIN);
    hallVal = map(hallVal, 0, 1023, 0, 255);
    Serial.print("Magnetic:");
    Serial.println(hallVal);
  }

  void getGas() {
    gasVal = analogRead(2);
    Serial.print("Gas:");
    Serial.println(gasVal);
  }

  void getNoiseLevel() {
    noiseLevel = analogRead(MIC_DATA_PIN);
    if (noiseLevel > 30) {
      digitalWrite(MIC_LED_PIN, HIGH);
    } else {
      digitalWrite(MIC_LED_PIN, LOW);
    }

    Serial.print("Noise level:");
    Serial.println(noiseLevel);
  }
};
