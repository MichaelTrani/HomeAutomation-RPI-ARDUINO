// Michael Trani February 2,2023
#include "sensorArray.h" 

#define timeInterval 300000 // Five min interval, 1800000 for half hour
int cycle = 1;  		// Track how many times this has run
sensorArray arrayPrime;

void setup() {
  Serial.begin(9600);
  sensors.begin();
  dht.begin();
  pinMode(MIC_LED_PIN, OUTPUT);
  Serial.println("--Initialized");
}

void loop() {
  Serial.print("# Cycle: ");
  Serial.println(cycle);
  arrayPrime.sensorRun();
  delay(timeInterval);  
  ++cycle;
}
