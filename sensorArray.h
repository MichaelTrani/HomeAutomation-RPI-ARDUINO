#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>

#define ERRORCODE -1111
#define DHTTYPE DHT11  //Select DHT 11 configuration

// Digital Pins
#define ONE_WIRE_BUS 2  //Dallas Temperature
#define DHT_PIN 4        //DHT sensor
#define MIC_LED_PIN 8        //Microphone's LED
// Analog Pins
#define MIC_DATA_PIN A0  //Microphone's input
#define LIGHT_PIN A1
#define HALL_PIN A2

// Library builds
OneWire oneWire(ONE_WIRE_BUS);        // Establish oneWire instance
DallasTemperature sensors(&oneWire);  // Send oneWire reference to Dallas Temperature.
DHT dht(DHT_PIN, DHTTYPE);             // Establish DHT object

struct sensorArray {
  int lightAverage, hallVal, gasVal = ERRORCODE;
  float humidVal, tempAvg, noiseLevel = ERRORCODE;

  void sensorRun() {
    getTemp();
    getHumid();
    readLight();
    readHall();
    readGas();
    getNoiseLevel();
  }

  // Member function
  void getTemp() {
    float temp[2] = { -1111, -1111 };  // 0 = Dallas Temp, 1 = DHT Temp    int age;
    sensors.requestTemperatures();
    if (temp[0] != DEVICE_DISCONNECTED_C) {
      temp[0] = sensors.getTempFByIndex(0);
      //Serial.print("Dallas Temp: ");
      //Serial.println(temp[0]);
    } else {
      Serial.println("Dallas Temp: FAILURE");
    }

    temp[1] = dht.readTemperature(true);
    //Serial.print("DHT TEMP: ");
    //Serial.println(temp[1]);
    tempAvg = (temp[1] + temp[0]) / 2;
    Serial.print("Temperature: ");
    Serial.println(tempAvg);
  }

  void getHumid() {
    humidVal = dht.readHumidity();
    Serial.print("Humidity: ");
    Serial.println(humidVal);
  }

  void readLight() {
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
    Serial.print("Light: ");
    Serial.println(lightAverage);
  }

  void readHall() {
    hallVal = analogRead(HALL_PIN);
    hallVal = map(hallVal, 0, 1023, 0, 255);
    Serial.print("Magnetic: ");
    Serial.println(hallVal);
  }

  void readGas() {
    gasVal = analogRead(2);
    Serial.print("Gas: ");
    Serial.println(gasVal);
  }

  void getNoiseLevel() {
    noiseLevel = analogRead(MIC_DATA_PIN);
    if (noiseLevel > 30) {
      digitalWrite(MIC_LED_PIN, HIGH);
    } else {
      digitalWrite(MIC_LED_PIN, LOW);
    }
    Serial.print("Noise level: ");
    Serial.println(noiseLevel);
  }
};