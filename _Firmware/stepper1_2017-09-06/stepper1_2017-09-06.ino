#include "cvStepper.h"

// change this to the number of steps on your motor
#define STEPS 180
#define INC 1

// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
//Stepper(int n_steps, int shift_reg_offset, bool hibyte);

#define NUM_MOTORS  1


Stepper stepper(STEPS,0);


void setup()
{
    Serial.begin(115200);
    Serial.println("Stepper test!");

    //    // set the speed of the motor to 30 RPMs


    stepper.setSpeed(60);


    delay(2 * 1000);
}

void loop()
{
    Serial.println("Forward");    
    delay(2 * 1000);
    for (int i = 0; i < INC * STEPS; i += INC) {

        stepper.step(INC);

    }
}
