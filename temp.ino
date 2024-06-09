#include <OneWire.h>
#include <DallasTemperature.h>

// DS18B20 data wire is connected to pin 2
#define ONE_WIRE_BUS 2
// MOSFET Gate is connected to pin 5
#define MOSFET_GATE 5

// Initialize the oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass the oneWire reference to Dallas Temperature
DallasTemperature sensors(&oneWire);

// Target temperature in degrees Celsius
const float targetTemp = 25.0;

void setup(void) {
  // Start serial communication for debugging purposes
  Serial.begin(9600);
  // Start the temperature sensor library
  sensors.begin();
  // Set MOSFET_GATE pin as output
  pinMode(MOSFET_GATE, OUTPUT);
  // Initialize the MOSFET_GATE pin to LOW (turn off Peltier module)
  digitalWrite(MOSFET_GATE, LOW);
}

void loop(void) {
  // Request temperature readings from the sensor
  sensors.requestTemperatures();
  // Fetch the temperature in Celsius
  float currentTemp = sensors.getTempCByIndex(0);

  // Print the current temperature to the serial monitor
  Serial.print("Current Temperature: ");
  Serial.print(currentTemp);
  Serial.println(" *C");

  // Control the Peltier module based on the current temperature
  if (currentTemp < targetTemp) {
    // Turn on the Peltier module
    digitalWrite(MOSFET_GATE, HIGH);
    Serial.println("Peltier module ON");
  } else {
    // Turn off the Peltier module
    digitalWrite(MOSFET_GATE, LOW);
    Serial.println("Peltier module OFF");
  }

  // Wait for 1 second before the next loop
  delay(1000);
}