
const int LASER_PINS[][2] = {{6, 7}, {9, 8}, {10, 11}}; // Pins for each laser (AN, TTL)
bool ttlState[3] = {false, false, false}; // Array to track TTL state for each laser

void setup() {
  Serial.begin(9600);
  
  for (int i = 0; i < 3; i++) {
    pinMode(LASER_PINS[i][0], OUTPUT); // AN pin
    pinMode(LASER_PINS[i][1], OUTPUT); // TTL pin
    digitalWrite(LASER_PINS[i][0], LOW); // Ensure analog pins are initially low
    digitalWrite(LASER_PINS[i][1], LOW); // Ensure TTL pins are initially low
  }
}

void updateLaserState(int laser) {
  int analogPin = LASER_PINS[laser - 1][0];
  
  if (ttlState[laser - 1]) {
    digitalWrite(analogPin, HIGH); // If TTL is on, make the analog signal visible
  } else {
    digitalWrite(analogPin, LOW); // If TTL is off, make the analog signal invisible
  }
}

void turnAnalogOn(int laser) {
  int analogPin = LASER_PINS[laser - 1][0];
  analogWrite(analogPin, 255); // Full voltage
  ttlState[laser - 1] = true; // TTL is always on when analog is on
  
  Serial.print("Analog control for laser ");
  Serial.print(laser);
  Serial.println(" turned on.");
}

void turnAnalogOff(int laser) {
  int analogPin = LASER_PINS[laser - 1][0];
  analogWrite(analogPin, 0); // Turn off analog output
  
  ttlState[laser - 1] = false;
  
  Serial.print("Analog control for laser ");
  Serial.print(laser);
  Serial.println(" turned off.");
}

void setAnalog(int laser, float voltage) {
  // Ensure voltage is within the range [0, 5]
  voltage = constrain(voltage, 0.0, 5.0);
  
  int analogPin = LASER_PINS[laser - 1][0];
  int analogValue = map(voltage, 0, 5, 0, 255); // Map voltage to PWM range (0-255)
  analogWrite(analogPin, analogValue);
  ttlState[laser - 1] = true; // TTL is always on when setting analog voltage
  
  Serial.print("Analog voltage set to ");
  Serial.print(voltage);
  Serial.print("V (");
  Serial.print(analogValue);
  Serial.print(" PWM) for laser ");
  Serial.println(laser);
}

void turnTTLOn(int laser) {
  int ttlPin = LASER_PINS[laser - 1][1];
  
  digitalWrite(ttlPin, HIGH);
  ttlState[laser - 1] = true;
  updateLaserState(laser);
  
  Serial.print("TTL control for laser ");
  Serial.print(laser);
  Serial.println(" turned on.");
}

void turnTTLOff(int laser) {
  int ttlPin = LASER_PINS[laser - 1][1];
  
  digitalWrite(ttlPin, LOW);
  ttlState[laser - 1] = false;
  updateLaserState(laser);
  
  Serial.print("TTL control for laser ");
  Serial.print(laser);
  Serial.println(" turned off.");
}

void handleCommands() {
  while (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.startsWith("turnAnalogOn")) {
      int laser = command.charAt(12) - '0';
      turnAnalogOn(laser);
    } else if (command.startsWith("turnAnalogOff")) {
      int laser = command.charAt(13) - '0';
      turnAnalogOff(laser);
    } else if (command.startsWith("setAnalog")) {
      int laser = command.charAt(9) - '0';
      float voltage = command.substring(10).toFloat();
      setAnalog(laser, voltage);
    } else if (command.startsWith("turnTTLOn")) {
      int laser = command.charAt(9) - '0';
      turnTTLOn(laser);
    } else if (command.startsWith("turnTTLOff")) {
      int laser = command.charAt(10) - '0';
      turnTTLOff(laser);
    } else {
      Serial.println("Invalid command.");
    }
  }
}

void loop() {
  handleCommands();
  // Add any additional non-blocking code here
}