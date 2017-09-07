/*!
 *  @file       i2c_slaveNode_LEDs.h
 *
 *  @brief      the LED layer for the Slave
 *
 *  @date       January 2, 2016
 *
 *  @author     Allan M. Schwartz, allans@CodeValue.net
 *
 *  @copyright  (c) 2016, by Allan M. Schwartz.  All rights reserved.
 */


// all volatile, because I2C handlers are called from an interrupt

volatile byte sync_command;             ///< the next command to do, with SYNC
volatile byte sync_command_arg1;        ///< and that command's argument1
volatile byte sync_command_arg2;        ///< and that command's argument2


/*!
 *  @brief      receive an 'SYNC' interrupt from a falling edge on PIN D2
 * 
 *  @note       the semantics is we do a "scheduled" event
 */
void sync_interruptHandler(void)
{
    //debugPuts("SYNC");
    switch ( sync_command ) {
            
        case CMD_LED_ONOFF:
            digitalWrite(sync_command_arg1, sync_command_arg2);
            break;
            
        case CMD_LED_NOP:
            break;
    }
}


/*!
 *  @brief      Turn the LED on or off
 *  @param      onOff: true sets the LED ON
 *  @param      onSync: if true, defer until we receive a SYNC interrupt
 */
void LED_OnOff(byte pin, boolean onOff, boolean onSync)
{
    debugPuts("LED_ON");
    if ( onSync ) {
        sync_command = CMD_LED_ONOFF;
        sync_command_arg1 = pin;
        sync_command_arg2 = onOff;
        return;
    }
    digitalWrite(pin, onOff);
}


/*!
 *  @brief      At the next SYNC command, do nothing
 *  @param      onSync: if true, defer until we receive a SYNC interrupt
 */
void LED_Nop(boolean onSync)
{
    //debugPuts("LED_Nop");
    if ( onSync ) {
        sync_command = CMD_LED_NOP;
    }
}

