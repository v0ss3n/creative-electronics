/* Code by Michelle Vossen and Elvis Dubra 
A secret knock that opens a box full of secrets 
Special thanks to pretty much all of the student assistants 
April, 2017 */

#include <Stepper.h>
#define STEPS_PER_MOTOR_REVOLUTION 32
#define STEPS_PER_OUTPUT_REVOLUTION 32 * 64  //2048  
Stepper small_stepper(STEPS_PER_MOTOR_REVOLUTION, 8, 10, 9, 11);

// Variables
unsigned long currenttime = 0;
int amountofknocks = 0;
int stepstaken;
int ledPin = 13;
int knockSensor = 0;
int inputPiezo = 0;
int statePin = LOW;
int threshold = 250;

void setup() {
  Serial.begin(9600);
}

void loop() {
  //Serial.println(analogRead(A0));
  inputPiezo = analogRead(knockSensor);
  //Serial.println(inputPiezo);
  if (inputPiezo <= threshold) {
    if (amountofknocks == 0) {
      currenttime = millis();
      amountofknocks++;
      Serial.println("Knock 1");
    }
    else if (amountofknocks == 1) {
      Serial.println(millis());
      Serial.println(currenttime);
      if ((millis() > (currenttime + 300)) && (millis() < (currenttime + 1500))) {
        amountofknocks++;
        currenttime = millis();
        Serial.println("Knock 2");
      }
      else {
        amountofknocks = 0;
      }
    }

    else if (amountofknocks == 2) {
      Serial.println(millis());
      Serial.println(currenttime);
      if ((millis() > (currenttime + 1700)) && (millis() < (currenttime + 5000))) {
        statePin = !statePin;
        digitalWrite(ledPin, statePin);
        Serial.println("Knock 3, Open!");   // After the third correct knock, the door will open

        stepstaken  =  STEPS_PER_OUTPUT_REVOLUTION / 4 ;
        small_stepper.setSpeed(500);
        small_stepper.step(stepstaken);
        delay(5000);    // You have 5 seconds to get your stuff out of the box

        stepstaken  =  - STEPS_PER_OUTPUT_REVOLUTION / 4; 
        small_stepper.setSpeed(700);
        small_stepper.step(stepstaken);
        
        delay(150);
        amountofknocks = 0;
        currenttime = millis();
      }
      else {
        amountofknocks = 0;
      }
    }
    while (analogRead(knockSensor) < 1000) {}
    delay(100);
  }
}
