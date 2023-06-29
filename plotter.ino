#include <AccelStepper.h>
#include <MultiStepper.h>
#define MOTOR1_STEP_PIN 2
#define MOTOR1_DIR_PIN 3
#define MOTOR2_STEP_PIN 4
#define MOTOR2_DIR_PIN 5
const int dataSize = 60;
AccelStepper motor1(AccelStepper::DRIVER, MOTOR1_STEP_PIN, MOTOR1_DIR_PIN);
AccelStepper motor2(AccelStepper::DRIVER, MOTOR2_STEP_PIN, MOTOR2_DIR_PIN);
MultiStepper steppers;

int recPoints[dataSize];
int first = 0;
int firstP = 1;
int turnedOff = 0;
int index;
long positions[2]; // Array of desired stepper positions

String receivedChar;
void setup() {
  Serial.begin(9600);
  Serial.setTimeout(50);
 
  // Configure each stepper
  motor1.setMaxSpeed(60);
  motor2.setMaxSpeed(60);
  steppers.addStepper(motor1);
  steppers.addStepper(motor2);
}

void doThing() {
  for (int i = 0; i < index / 2; i ++) {
    //Serial.println("Going To ");
    positions[0] = -recPoints[i * 2];
    positions[1] = recPoints[i * 2 + 1];
    // Serial.println(positions[0]);
    // Serial.println(positions[1]);
    steppers.moveTo(positions);
    steppers.runSpeedToPosition(); // Blocks until all are in position
  }
}

void loop() {
  if (turnedOff == 0) {
    if (Serial.available() > 0) { // Check if data is available to read
      
      receivedChar = Serial.readString(); // Read the incoming character
      if (first == 1) {
        Serial.println(receivedChar);
        index = 0;
        while (receivedChar.length() > 0) {
          int delimiterIndex = receivedChar.indexOf("|");  // Find the delimiter position
          String numberString;  // String to store each number
          
          if (delimiterIndex != -1) {
            numberString = receivedChar.substring(0, delimiterIndex);  // Extract number substring
            receivedChar = receivedChar.substring(delimiterIndex + 1);  // Remove the processed part from the input string
          } else {
            numberString = receivedChar;  // If no delimiter found, treat the remaining string as the last number
            receivedChar = "";  // Set input to empty string to exit the loop
          }
          
          recPoints[index] = numberString.toInt();  // Convert the number string to integer and store in array
          index++;  // Increment the index
        }
        if (firstP) {
          motor1.setCurrentPosition(-recPoints[0]);
          motor2.setCurrentPosition(recPoints[1]);
          firstP = 0; 
        }
        Serial.println(index);
        doThing();
        Serial.println("Give me more");
      } else {
        Serial.println("START");
      }
      first = 1;
    }
  }
}
