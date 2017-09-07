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
#include <SPI.h>
#include "i2c_misc.h"


#define LED_PIN        9                ///< Arduino digital Pin 9
#define BLINK_PIN      13               ///< Arduino digital Pin 13
#define SYNC_PIN       2                ///< Arduino digital Pin 2
#define MY_I2C_ADDRESS 12
#define DEBUG_SPI      0


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
    // we run a standard blinking on D13
    blink_thread();

    // all other activity occurs at interrupt time, either off 
    // the i2c_receiveHandler() or the sync_interruptHandler().
}


/*!
 *  @brief      thread of execution, controlling our
 *              blinking D13 LED
 *
 *  @note       this is non-functional if SPI-debugging is on
 *              because D13 is also used as the SPI CLK
 */
void blink_thread()
{
    static clock_t t0 = 0;
    static boolean led_on = false;

    if ( (clock_t)(millis() - t0) > 500 ) {
        t0 = millis();

        digitalWrite(BLINK_PIN, led_on );
        led_on = !led_on;
    }
}


/*!
 *  @brief      our debug-only 'fputs()'-like function
 *              the characters flow either to the SPI bus or a debug console
 */
void debugPuts(char *str)
{
#if DEBUG_SPI
    SPI.transfer(str, strlen(str));
#else
    Serial.write(str, strlen(str));
#endif
}

