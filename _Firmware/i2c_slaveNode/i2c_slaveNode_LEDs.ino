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
        
volatile clock_t g_ms_on = 10;          ///< pulsing n milliseconds on
volatile clock_t g_ms_off = 10;         ///< pulsing n milliseconds off
volatile boolean g_PulsesActive = false; ///< is pulsing active right now
volatile byte sync_command;             ///< the next command to do, with SYNC
volatile byte sync_command_arg;         ///< and that command's argument


/*!
 *  @brief      receive an 'SYNC' interrupt from a falling edge on PIN D2
 * 
 *  @note       the semantics is we do a "scheduled" event
 */
void sync_interruptHandler(void)
{
    //debugPuts("SYNC");
    switch ( sync_command ) {
        case CMD_LED_PULSES:
            if (sync_command_arg) {
                g_PulsesActive = true;
                pulse_it(HIGH);
            }
            else {
                g_PulsesActive = false;
                pulse_it(LOW);
            }
            break;
            
        case CMD_LED_ONOFF:
            flash_it(sync_command_arg);
            break;
    }
}


/*!
 *  @brief      Setup a LED pulse pattern
 *  @param      ms_on: milliseconds on
 *  @param      ms_off: milliseconds off
 */
void LEDpattern(int ms_on, int ms_off)
{
    //myprintf("%s ms_on %d ms_off %d", __FUNCTION__, ms_on, ms_off);
    debugPuts("PATTERN");
    g_ms_on = ms_on;
    g_ms_off = ms_off;
}


/*!
 *  @brief      Turn the LED on or off
 *  @param      onOff: true sets the LED ON
 *  @param      onSync: if true, defer until we receive a SYNC interrupt
 */
void LED_OnOff(boolean onOff, boolean onSync)
{
    //myprintf("%s %d %d", __FUNCTION__, onOff, onSync);
    debugPuts("LED_ON");
    if ( onSync ) {
        sync_command = CMD_LED_ONOFF;
        sync_command_arg = onOff;
        return;
    }
    flash_it(onOff);
}


/*!
 *  @brief      Begin a pulsing of the LED at the predefined frequency
 *  @param      startStop: true starts the pulsing, false stops it
 *  @param      onSync: if true, defer until we receive a SYNC interrupt
 */
void LED_Pulses(boolean startStop, boolean onSync)
{
    //myprintf("%s %d %d", __FUNCTION__, startStop, onSync);
    debugPuts("PULSES");
    if ( onSync ) {
        sync_command = CMD_LED_PULSES;
        sync_command_arg = startStop;
        return;
    }
    if (startStop) {
        g_PulsesActive = true;
        pulse_it(HIGH);
    }
    else {
        g_PulsesActive = false;
        pulse_it(LOW);
    }
}

