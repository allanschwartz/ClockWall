/*!
 *  @file       i2c_masterNode.c
 *
 *  @brief      POC for multiple Arduino controller ...
 *              Send and receive messages over the I2C interface
 *              This is the MASTER thread and application layer
 *
 *  @date       January 2, 2016
 *
 *  @author     Allan M. Schwartz, allans@CodeValue.net
 *
 *  @copyright (c) 2016, by Allan M. Schwartz.  All rights reserved.
 */

#include <Wire.h>
#include "misc.h"
#include <SPI.h>
#include <stdarg.h>


#define BLINK_PIN      13               ///< Arduino digital Pin 13
#define SYNC_PIN       2                ///< Arduino digital Pin 2
#define DEBUG_SPI      1
#define LED_PIN        9


//! the I2C Slave Addresses on the bus
const byte slaveAddresses[] = {
    10, 11, 12, 13
};
//! the I2C Slave Addresses on the bus
const byte SS_Pins[] = {
    3, 4, 5, 6,
};
const byte nSlaves = sizeof slaveAddresses; ///< number of I2C Slave Addresses


/*!
 *  @brief      setup code is called by main(), once
 */
void setup()
{
    // put your setup code here, to run once:
    Wire.begin();                       // begin the I2C (Two-Wire Interface)
    pinMode(BLINK_PIN, OUTPUT);         // Blinking pattern (non-DEBUG version only)
    pinMode(SYNC_PIN, OUTPUT);          // we will use this PIN to syncronize the Slaves
    digitalWrite(SYNC_PIN, HIGH);       // active low, so start in HIGH
    for ( int i = 0; i < nSlaves; i++ ) {
        pinMode(SS_Pins[i], OUTPUT);
        digitalWrite(SS_Pins[i], HIGH); // active low
    }

    SPI.begin();                        // we will use the SPI channel for debug printfs

    // Slow down the master a bit, to 1Mhz (instead of the default 4Mhz)
    SPI.setClockDivider(SPI_CLOCK_DIV16);
  
    delay(1000);                        // wait a sec, so I can trigger the 'scope
#if DEBUG_SPI
    debugPuts("hello SPI");
#else
    Serial.begin(9600);
    Serial.write("hello world\n");
#endif

    delay(100);
}


/*!
 *  @brief      loop code to perform our timing experiments
 */
void loop()
{
    //   ----- experiment #1 -----
    // just a 50 ms pulse, manually timed
    // do this 3 times, so we can see it clearly on the logic analyzer
    for ( int k = 0; k < 3; k++ ) {

        //debugPuts("exp 1 pulse");
        non_syncronized_pulse(50, 100);
    }

    delay(2000);


    //   ----- experiment #2 -----
    //  a 75 ms pulse, syncronized

    // do this 3 times, so we can see it clearly on the logic analyzer
    for ( int k = 0; k < 3; k++ ) {

        //debugPuts("exp 2 pulse");
        syncronized_pulse(75, 150);
    }

    delay(4000);
}


/*!
 *  @brief      create a timed, non-syncronized pulse on each slave
 */
void non_syncronized_pulse(clock_t ms_on, clock_t ms_off)
{
    struct ms_timer blip_timer;         // timer to measure the LED on time
    struct ms_timer off_timer;          // timer to measure the LED off time

    // manually pulse one LED for 50 ms
    ms_timer_set(&blip_timer, ms_on);

    // send the commands to all the Slaves
    for ( int i = 0; i < nSlaves; i++ ) {
        send_LED_OnOff(slaveAddresses[i], HIGH, false);
    }

    ms_timer_delay(&blip_timer);        // wait out ms_on

    ms_timer_set(&off_timer, ms_off);

    for ( int i = 0; i < nSlaves; i++ ) {
        send_LED_OnOff(slaveAddresses[i], LOW, false);
    }

    ms_timer_delay(&off_timer);         // wait out ms_off
}


/*!
 *  @brief      create a timed, syncronized pulse on each slave
 */
void syncronized_pulse(clock_t ms_on, clock_t ms_off)
{
    static struct ms_timer blip_timer = { 0 };  // timer for the LED on time
    static struct ms_timer off_timer = { 0 };   // timer for the LED off time

    // send the commands to all the Slaves
    for ( int i = 0; i < nSlaves; i++ ) {
        send_LED_OnOff(slaveAddresses[i], HIGH, true);
    }

    ms_timer_delay(&off_timer);         // if the off_timer was set, wait it out

    ms_timer_set(&blip_timer, ms_on);   // set a timer for pulse-on

    sync_Pulse();                       // sync all the slaves in one instant

    delay(ms_on - 3 * nSlaves);         // wait for most of the timer

    for ( int i = 0; i < nSlaves; i++ ) {
        send_LED_OnOff(slaveAddresses[i], LOW, true);
    }
    ms_timer_delay(&blip_timer);        // wait out ms_on

    sync_Pulse();                       // sync all the slaves in one instant

    ms_timer_set(&off_timer, ms_off);   // set a timer for pulse-off

    delay(ms_off - 6 * nSlaves);        // wait for most of the timer.
}


