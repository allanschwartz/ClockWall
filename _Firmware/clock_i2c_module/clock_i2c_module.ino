/*!
 *  @file       i2c_slaveNode.c
 *
 *  @brief      POC for multiple Arduino controller ...
 *              Send and receive messages over the I2C interface
 *              This is the SLAVE thread and application layer
 *
 *  @date       January 2, 2016
 *
 *  @author     Allan M. Schwartz, allans@CodeValue.net
 *
 *  @copyright  (c) 2016, by Allan M. Schwartz.  All rights reserved.
 */

#include <Wire.h>
#include "i2c_misc.h"
#include <SPI.h>
#include <stdarg.h>


#define LED_PIN        9                ///< Arduino digital Pin 9
#define BLINK_PIN      13               ///< Arduino digital Pin 13
#define SYNC_PIN       2                ///< Arduino digital Pin 2
#define MY_I2C_ADDRESS 13
#define DEBUG_SPI      0


extern volatile clock_t g_ms_on;        ///< milliseconds on, pulse size
extern volatile clock_t g_ms_off;       ///< milliseconds off, pulse size
extern volatile boolean g_PulsesActive; ///< generating pulses currently?


static boolean pulsing_high = false;    ///< if generating pulses, current HIGH/LOW
static clock_t pulse_timer = 0;         ///< timer used for timing the pulses


/*!
 *  @brief      setup code is called by main(), once
 */
void setup(void)
{
    // put your setup code here, to run once
    pinMode(LED_PIN, OUTPUT);
    pinMode(BLINK_PIN, OUTPUT);         // Blinking pattern (non-DEBUG version only)
    pinMode(SYNC_PIN, INPUT);           // use this PIN to be syncronize by master
    attachInterrupt(digitalPinToInterrupt(SYNC_PIN), sync_interruptHandler, FALLING);

    Wire.begin(MY_I2C_ADDRESS);         // begin the I2C (Two-Wire Interface)
    Wire.onReceive(i2c_receiveHandler);
    Wire.onRequest(i2c_requestHandler);

#if DEBUG_SPI
    SPI.begin();                        // we will use the SPI channel for debug printfs
    delay(1000);                        // wait a sec, so I can trigger the 'scope
    myputs("hello SPI");
#else
    Serial.begin(9600);
    Serial.write("hello world\n");
#endif
}


/*!
 *  @brief      loop code is called repeatedly, call our two
 *              two threads
 *
 *  @note       The I2C receiver and transmitter, running off of interrupts,
 *              is another thread of execution.
 */
void loop(void)
{
    // put your main code here, to run repeatedly:
    led_element_thread();
    blink_thread();
}


/*!
 *  @brief      thread of execution, controlling our LED Element,
 *              which is pulsed on/off according to some schedule
 */
void led_element_thread(void)
{
    if (g_PulsesActive) {
        // there are two cases because there are two different
        // intervals: g_ms_on, and g_ms_off
        if (pulsing_high == HIGH) {
            if ( (clock_t)(millis() - pulse_timer) >= g_ms_on ) {
                pulse_it(LOW);
                pulse_timer = millis();
            }
        }
        else {
            if ( (clock_t)(millis() - pulse_timer) >= g_ms_off ) {
                pulse_it(HIGH);
                pulse_timer = millis();
            }
        }
    }
}


/*!
 *  @brief      turn our LED Element on or off, and time a pulse
 */
void pulse_it(boolean high_or_low)
{
    pulse_timer = millis();
    pulsing_high = high_or_low;
    digitalWrite(LED_PIN, high_or_low);
}


/*!
 *  @brief      turn our LED Element on or off
 */
void flash_it(boolean high_or_low)
{
    digitalWrite(LED_PIN, high_or_low);
}

