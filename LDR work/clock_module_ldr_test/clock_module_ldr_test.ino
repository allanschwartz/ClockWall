/*
 *
 Stepper Motor Control - one revolution, with LDR testing

 This program drives a unipolar or bipolar stepper motor.

 The motor should revolve one revolution in one direction, then
 one revolution in the other direction.

 */

#include "Time.h"
#include <Stepper.h>
#define D3 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7
#define D8 8
#define D9 9
#define D10 10
#define D11 11
#define D12 12
#define D13 13

#define V1_4    1
#if V1_1
#define M_A1  D5
#define M_A2  D4
#define M_A3  D7
#define M_A4  D8
#define M_B1  D9
#define M_B2  D10
#define M_B3  D11
#define M_B4  D12
#define ID_BIT8  A3
#define ID_BIT4  A2
#define ID_BIT2  A1
#define ID_BIT1  A0
#endif

#if V1_3 || V1_4
#define M_A1  D4
#define M_A2  D5
#define M_A3  D8
#define M_A4  D9
#define M_B1  D12
#define M_B2  D10
#define M_B3  D6
#define M_B4  D7
const int BUTTONS = A0;
const int LDR1 = A6;
const int LDR2 = A7;
#endif

// Enumerate Two Motors

enum {MotorA, MotorB};

// D8 Pin connected to enable pin (OE) of 74HC595
const int enablePin = D3;
// D10 Pin connected to latch pin (ST_CP) of 74HC595
const int latchPin = A1;
// D11 Pin connected to DataIn (DS) of 74HC595
const int dataPin = D11;
// D13 Pin connected to clock pin (SH_CP) of 74HC595
const int clockPin = D13;

// specific for this motor
const int StepsPerDegree = 2;
const int StepsPerRevolution = 720;  // change this to fit the number of steps per revolution


// The 'A' motor is the 1mm motor shaft and rotates opposite
// The 'A' motor instance, on digital pins M_A1, M_A2, M_A3, M_A4
Stepper stepperA(StepsPerRevolution, M_A1, M_A2, M_A3, M_A4);
// The 'B' motor is the 4mm motor shaft.
// The 'B' motor instance, on digital pins M_B1, M_B2, M_B3, M_B4
Stepper stepperB(StepsPerRevolution, M_B1, M_B2, M_B3, M_B4);


void setup()
{
    //set pins to output because they are addressed in the main loop
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(enablePin, OUTPUT);
    Serial.begin(115200);
    Serial.println("reset");

    // set the rotational speed to 30 rpm:
    stepperA.setSpeed(30);
    stepperB.setSpeed(30);

#if V1_1
    readIDSwitch();
#endif
#if V1_4
    readIDSwitch();
#endif
    BlueBits_blink(75);

    alignment_procedure();
    alignment_procedure();
}

#if V1_1
byte readIDSwitch()
{
    byte id;

    pinMode(ID_BIT8, INPUT_PULLUP);
    byte id_bit8 = !digitalRead(ID_BIT8);
    //Serial.println("ID_BIT8 " + String(id_bit8 ? "ON" : "OFF") );

    pinMode(ID_BIT4, INPUT_PULLUP);
    byte id_bit4 = !digitalRead(ID_BIT4);
    //Serial.println("ID_BIT4 " + String(id_bit4 ? "ON" : "OFF") );

    pinMode(ID_BIT2, INPUT_PULLUP);
    byte id_bit2 = !digitalRead(ID_BIT2);
    //Serial.println("ID_BIT2 " + String(id_bit2 ? "ON" : "OFF") );

    pinMode(ID_BIT1, INPUT_PULLUP);
    byte id_bit1 = !digitalRead(ID_BIT1);
    //Serial.println("ID_BIT1 " + String(id_bit1 ? "ON" : "OFF") );

    id = id_bit8 << 3 | id_bit4 << 2 | id_bit2 << 1 | id_bit1;
    Serial.println("id " + String(id) );
    return id;
}
#endif

#if V1_4
byte readIDSwitch()
{
    byte id;
    // this table is derived from measuring voltages from
    // the resistor ladder circuit
    // ... it probably could be a equation; however a table lookup is more accurate
    const int id_table[] = { 224, /* id = 0 */
                             296, /* id = 1 */
                             357, /* id = 2 */
                             405, /* id = 3 */
                             449, /* id = 5 */
                             486, /* id = 5 */
                             520, /* id = 6 */
                             550, /* id = 7 */
                             576, /* id = 8 */
                             598, /* id = 9 */
                             621, /* id = 10 */
                             639, /* id = 11*/
                             656, /* id = 12 */
                             672, /* id = 13 */
                             688, /* id = 14 */
                             701  /* id = 15 */
                           };
    const int epsilon = 10;

    pinMode(BUTTONS, INPUT_PULLUP);
    short v = analogRead(BUTTONS);

    Serial.println("buttons " + String(v) );

    // use a lookup table, because the voltage is not linear
    for (int i = 0 ; i < 15; i++ ) {
        if ( (id_table[i] - epsilon < v) &&
                (v < id_table[i] + epsilon) ) {
            id = i;
            Serial.println("id " + String(id) );
            return id;
        }
    }
    return 0;
}
#endif

static byte bluebits[2] = { 0, 0 };
#define BB_BIT1  B00000100  // Q2
#define BB_BIT2  B00001000  // Q3
#define BB_BIT3  B00010000  // Q4
#define BB_BIT4  B00100000  // Q5
#define BB_BIT5  B01000000  // Q6
#define BB_BIT6  B10000000  // Q7
#define BB_BIT7  BB_BIT1
#define BB_BIT8  BB_BIT2
#define BB_BIT9  BB_BIT3
#define BB_BIT10 BB_BIT4
#define BB_BIT11 BB_BIT5
#define BB_BIT12 BB_BIT6
#define BB_ALL   B01111110




void  BlueBits_Clear()
{
    bluebits[0] = 0; bluebits[1] = 0;
}

void  BlueBits_Clear(byte n)
{
    switch (n) {
        case 1:     bluebits[0] &= ~BB_BIT1;   break;
        case 2:     bluebits[0] &= ~BB_BIT2;   break;
        case 3:     bluebits[0] &= ~BB_BIT3;   break;
        case 4:     bluebits[0] &= ~BB_BIT4;   break;
        case 5:     bluebits[0] &= ~BB_BIT5;   break;
        case 6:     bluebits[0] &= ~BB_BIT6;   break;
        case 7:     bluebits[1] &= ~BB_BIT7;   break;
        case 8:     bluebits[1] &= ~BB_BIT8;   break;
        case 9:     bluebits[1] &= ~BB_BIT9;   break;
        case 10:    bluebits[1] &= ~BB_BIT10;   break;
        case 11:    bluebits[1] &= ~BB_BIT11;   break;
        case 12:    bluebits[1] &= ~BB_BIT12;   break;
        default:
            ;
    }
}

void  BlueBits_Set()
{
    bluebits[0] = BB_ALL; bluebits[1] = BB_ALL;
}

void  BlueBits_Set(byte n)
{
    switch (n) {
        case 1:     bluebits[0] |= BB_BIT1;   break;
        case 2:     bluebits[0] |= BB_BIT2;   break;
        case 3:     bluebits[0] |= BB_BIT3;   break;
        case 4:     bluebits[0] |= BB_BIT4;   break;
        case 5:     bluebits[0] |= BB_BIT5;   break;
        case 6:     bluebits[0] |= BB_BIT6;   break;
        case 7:     bluebits[1] |= BB_BIT7;   break;
        case 8:     bluebits[1] |= BB_BIT8;   break;
        case 9:     bluebits[1] |= BB_BIT9;   break;
        case 10:    bluebits[1] |= BB_BIT10;   break;
        case 11:    bluebits[1] |= BB_BIT11;   break;
        case 12:    bluebits[1] |= BB_BIT12;   break;
        default:
            ;
    }
}

void   BlueBits_Show()
{
    // This method sends bits to the shift register:
    digitalWrite(clockPin, LOW);        // SH_CP
    //digitalWrite(enablePin, HIGH);    // OE_
    digitalWrite(latchPin, LOW);        // ST_CP

    // shift the bits out:
    shiftOut(dataPin, clockPin, MSBFIRST, bluebits[1]);
    shiftOut(dataPin, clockPin, MSBFIRST, bluebits[0]);
    delay(20);

    // latch and turn on the output so the LEDs can light up:
    digitalWrite(latchPin, HIGH);       // ST_CP
    digitalWrite(enablePin, LOW);       // OE_
}

void BlueBits_blink(clock_t delay_ms)
{
    BlueBits_Clear();
    BlueBits_Show();
    for ( int j = 0; j < 2; j++ ) {
        for ( int i = 1; i <= 12; i++ ) {
            BlueBits_Set(i);
            BlueBits_Show();
        }
        delay(delay_ms * 4);
        BlueBits_Clear();
        BlueBits_Show();
        delay(delay_ms);
    }
}

bool test_LDR_levels(Stepper *motor, bool ledOn, char whichMotor)
{
    int min_v1 = 1024;
    int min_v2 = 1024;
    int min_v1_degrees = 0;
    int min_v2_degrees = 0;

    delay(1000);
    if ( ledOn ) {
        BlueBits_Clear();
        BlueBits_Set(12);
        BlueBits_Show();
    }
    else {
        BlueBits_Clear();
        BlueBits_Show();
    }
    delay(1000);


    int epsilon = 1;        // degrees of movement between measurements
    if (whichMotor == 'A') {
        for ( int theta = 0; theta <= 360; theta += 1 ) {
            delay(10);
            int v1 = analogRead(LDR1);
            delay(10);
            int v2 = analogRead(LDR2);

            if ( v1 < min_v1) {
                min_v1 = v1; min_v1_degrees = theta;
            }
            if ( v2 < min_v2) {
                min_v2 = v2; min_v2_degrees = theta;
            }
            delay(10);
            motor->step(-epsilon * StepsPerDegree);
        }
    }
 

    if (whichMotor == 'B') {
        for ( int theta = 0; theta <= 360; theta += epsilon ) {
            delay(10);
            int v1 = analogRead(LDR1);
            delay(10);
            int v2 = analogRead(LDR2);

            if ( v1 < min_v1) {
                min_v1 = v1; min_v1_degrees = theta;
            }
            if ( v2 < min_v2) {
                min_v2 = v2; min_v2_degrees = theta;
            }
            delay(10);
            motor->step(epsilon * StepsPerDegree);
        }
    }
    String LDRxResult = "Motor " + String(whichMotor) + " LDR result:";


if (whichMotor == 'A') {
    Serial.println( LDRxResult + " min_v1: " + String(min_v1) +
                    " at " + String(min_v1_degrees) + " degrees");
}
if (whichMotor == 'B') {
    Serial.println( LDRxResult + " min_v2: " + String(min_v2) +
                    " at " + String(min_v2_degrees) + " degrees");
}
    if ( min_v1_degrees > 300 ) {
        min_v1_degrees = -(360 - min_v1_degrees);
    }
    if ( min_v2_degrees > 300 ) {
        min_v2_degrees = -(360 - min_v2_degrees);
    }
    int degrees_off;
    if (whichMotor == 'A') {
        degrees_off = min_v1_degrees;
    }
    if (whichMotor == 'B') {
        degrees_off = min_v2_degrees;
    }
    
    Serial.println( LDRxResult + " degrees_off: " + String(degrees_off) + " degrees" );
    delay(3000);
    
    // move to 12oclock.
    int correction = 0;

    if (whichMotor == 'A') {
        correction = 5-degrees_off;
    }

    if (whichMotor == 'B') {
        correction = degrees_off-5;
    }
    Serial.println ("correction: " + String(correction) );
    motor->step(StepsPerDegree*correction);

    delay(5000);
    if ( degrees_off > 9 ) return false;
    return true;
}

void alignment_procedure(void)
{
    //stepperA.step(45 * StepsPerDegree);
    //stepperB.step(-45 * StepsPerDegree);
    if (! test_LDR_levels(&stepperA, false, 'A') )
        (void)test_LDR_levels(&stepperA, false, 'A');
    //    //stepperA.step(45 * StepsPerDegree);
    if (! test_LDR_levels(&stepperB, false, 'B') )
        (void)test_LDR_levels(&stepperB, false, 'B');
    //stepperA.step(-45 * StepsPerDegree);
}

void loop()
{
    // step one revolution  in one direction:
    Serial.println("clockwise");
    //stepperA.step(180 * StepsPerDegree);
    stepperB.step(180 * StepsPerDegree);

    delay(1000);

    // step one revolution in the other direction:
    Serial.println("counterclockwise");
    //stepperA.step(-180 * StepsPerDegree);
    stepperB.step(-180 * StepsPerDegree);

    delay(1000);

    BlueBits_blink(75);
}

