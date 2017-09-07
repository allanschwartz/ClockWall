
/*
 Stepper Motor Control - one revolution

 This program drives a unipolar or bipolar stepper motor.
 The motor is attached to digital pins 8 - 11 of the Arduino.

 The motor should revolve one revolution in one direction, then
 one revolution in the other direction.


 Created 11 Mar. 2007
 Modified 30 Nov. 2009
 by Tom Igoe

 */

#include <Stepper.h>
#define D4 4
#define D5 5
#define D7 7
#define D8 8
#define D9 9
#define D10 10
#define D11 11
#define D12 12

const int stepsPerRevolution = 720;  // change this to fit the number of steps per revolution
// for your motor

// one instance of the 'A' motor stepper on digital pins 4, 5, 7, 8
Stepper stepperA(stepsPerRevolution, D4, D5, D7, D8);
// one instance of the 'B' motor stepper digital pins 9, 10, 11, 12
Stepper stepperB(stepsPerRevolution, D9, D10, D11, D12);

void setup()
{
    // set the speed at 60 rpm:
    stepperA.setSpeed(60);
    stepperB.setSpeed(60);
    // initialize the serial port:
    Serial.begin(9600);
}

void loop()
{
    // step one revolution  in one direction:
    Serial.println("clockwise");
    stepperA.step(stepsPerRevolution);
    stepperB.step(stepsPerRevolution);
        stepperA.step(stepsPerRevolution);
    stepperB.step(stepsPerRevolution);
        stepperA.step(stepsPerRevolution);
    stepperB.step(stepsPerRevolution);
        stepperA.step(stepsPerRevolution);
    stepperB.step(stepsPerRevolution);
    delay(500);

    // step one revolution in the other direction:
    Serial.println("counterclockwise");
    stepperA.step(-stepsPerRevolution);
    stepperB.step(-stepsPerRevolution);
        stepperA.step(-stepsPerRevolution);
    stepperB.step(-stepsPerRevolution);
        stepperA.step(-stepsPerRevolution);
    stepperB.step(-stepsPerRevolution);
        stepperA.step(-stepsPerRevolution);
    stepperB.step(-stepsPerRevolution);
    delay(500);
}

