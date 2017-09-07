#include "cvStepper.h"

// change this to the number of steps on your motor
#define STEPS 180
#define INC 1

// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
//Stepper(int n_steps, int shift_reg_offset, bool hibyte);

#define NUM_MOTORS  8
//Stepper *steppers;


Stepper stepper0(STEPS,0);
Stepper stepper1(STEPS, 1);
Stepper stepper2(STEPS, 2);
Stepper stepper3(STEPS, 3);
Stepper stepper4(STEPS, 4);
//Stepper stepper5(STEPS, 5);
//Stepper stepper6(STEPS, 6);
//Stepper stepper7(STEPS, 7);

void setup()
{
    Serial.begin(115200);
    Serial.println("Stepper test!");

    //    steppers = new Stepper[8](STEPS);


    //    for (int i = 0; i < NUM_MOTORS; i++) {
    //        steppers[i] = new Stepper(STEPS, i);
    //    }

    //    delay(2 * 1000);
    //
    //    // set the speed of the motor to 30 RPMs
    //    for (int i = 0; i < NUM_MOTORS; i++) {
    //        steppers[i].setSpeed(60);
    //    }

    stepper0.setSpeed(60);
    stepper1.setSpeed(60);
    stepper2.setSpeed(60);
    stepper3.setSpeed(60);
    stepper4.setSpeed(60);
//    stepper5.setSpeed(60);
//    stepper6.setSpeed(60);
//    stepper7.setSpeed(60);

    delay(2 * 1000);
}

void loop()
{
    Serial.println("Forward");    delay(2 * 1000);
    for (int i = 0; i < 4 * STEPS; i += INC) {
        //        for (auto i = 0; i < NUM_MOTORS; i++) {
        //            steppers[i].step(INC);
        //        }
        //
        //        stepper0.step(INC);

//        stepper0.step(1);
//        stepper1.step(1);
        stepper2.step(1);
//        stepper3.step(INC);
        stepper4.step(INC);
//        stepper5.step(INC);
//        stepper6.step(INC);
//        stepper7.step(INC);
    }
}
