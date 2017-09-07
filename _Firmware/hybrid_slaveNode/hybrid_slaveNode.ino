/*!
 *  @file       hybrid_slaveNode.c
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
#include "misc.h"
#include <SPI.h>
#include <stdarg.h>


#define LED_PIN        9                ///< Arduino digital Pin 9
#define SYNC_PIN       3                ///< Arduino digital Pin 3 - INT 1
#define DEBUG_PIN      4                ///< Arduino digital Pin 4

#define MY_I2C_ADDRESS 10



/*!
 *  @brief      setup code is called by main(), once
 */
void setup(void)
{
    // put your setup code here, to run once
    pinMode(LED_PIN, OUTPUT);digitalWrite(LED_PIN, LOW);
    pinMode(DEBUG_PIN, OUTPUT);digitalWrite(DEBUG_PIN, LOW);
    pinMode(SYNC_PIN, INPUT);           // use this PIN to be syncronize by master
    
    attachInterrupt(digitalPinToInterrupt(SYNC_PIN), sync_interruptHandler, FALLING);

#if 0
    Wire.begin(MY_I2C_ADDRESS);         // begin the I2C (Two-Wire Interface)
    Wire.onReceive(i2c_receiveHandler);
    Wire.onRequest(i2c_requestHandler);
#endif

    SPIslaveInit();
    delay(200);               
#if 0
    Serial.begin(9600);
    Serial.write("hello world\n");
#endif

    // test DEBUG_PIN   ... also we wiggle this pin in a distinctive pattern, to identify reboot
    digitalWrite(DEBUG_PIN, HIGH); delay(1);
    digitalWrite(DEBUG_PIN, LOW);  delay(1);
    digitalWrite(DEBUG_PIN, HIGH); delay(1);
    digitalWrite(DEBUG_PIN, LOW);  delay(1);
    digitalWrite(DEBUG_PIN, HIGH);
    digitalWrite(DEBUG_PIN, LOW);

    // test LED_PIN
    digitalWrite(LED_PIN, HIGH);    delay(1);
    digitalWrite(LED_PIN, LOW);     delay(1);
    digitalWrite(LED_PIN, HIGH);    delay(1);
    digitalWrite(LED_PIN, LOW);     delay(1);
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(LED_PIN, LOW);
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
    //led_element_thread();
}


/*!
 *  @brief      turn our LED Element on or off
 */
void flash_it(boolean high_or_low)
{
    digitalWrite(LED_PIN, high_or_low);
}

