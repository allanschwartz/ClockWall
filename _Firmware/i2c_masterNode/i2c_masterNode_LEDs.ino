/*!
 * @file        i2c_masterNode_LEDs.c
 *
 * @brief       the LED layer for the Master multi-Arduino controller
 *
 * @history     January 2, 2016
 *
 * @author      Allan M. Schwartz, allans@CodeValue.net
 *
 * @copyright   (c) 2016, by Allan M. Schwartz. All rights reserved.
 */


extern _u8    msgIn[MAX_MSG_SIZE];      ///< last received message (from a Slave)


/*!
 * @brief       send a pulse (clocked of the falling edge) to the Slave Arduinos
 */
void sync_Pulse(void)
{
    digitalWrite(SYNC_PIN, LOW);        // active low
    delayMicroseconds(100);
    digitalWrite(SYNC_PIN, HIGH);
}


/*!
 *  @brief      send an I2C request message, and then receive the response message
 *  @param      slave_addr: the I2C Slave Address
 *  @param      cmd: command byte
 *  @param      payloadLen: length of the message to send
 *  @param      payload: the message to send
 *  @returns    return code
 *                ACK: received an ACK from the Slave
 *                NAK: received a NAK from the Slave
 *                -1: there was no response from the Slave
 *                -2: there was an error sending the message
 *
 *  @TODO:      we could keep counts of the ACKs, NAKs (and failure reasons),
 *              no responses, sending errors
 *
 *              Run millions of these, and print the results once an hour
 */
int request_response(byte slave_addr, byte cmd, byte payloadLen, _u8 payload[4])
{
    _u8 naddr[2] = {'X', slave_addr};  // our Element Node address

    int rc = i2c_sendRequestMsg(cmd, naddr, payloadLen, payload);
    if ( rc != 0 ) {
        // error return
        myprintf("sendRequestMsg rc %d", rc );
        return -2;
    }
    delayMicroseconds(200);
    rc = i2c_masterReceiveMsg(slave_addr, 4);
    delayMicroseconds(200);
    if ( rc != 4 ) {
        myprintf("RCV %d", rc );
    }
    if ( rc == 4 )
        return msgIn[0];
    else
        return -1;
}


/*!
 *  @brief      send CMD_LED_PATTERN message to the addressed Slave,
 *              and return the response
 *  @param      slaveAddress: I2C Slave Address
 *  @param      ms_on: milliseconds on
 *  @param      ms_off: milliseconds off
 *  @returns    result of the request_response, -1, ACK or NAK
 */
int send_LEDpattern(_u8 slave_addr, int ms_on, int ms_off)
{
    _u8 payload[] = { highByte(ms_on), lowByte(ms_on), highByte(ms_off), lowByte(ms_off) };

    return request_response(slave_addr, CMD_LED_PATTERN, sizeof payload, payload);
}


/*!
 *  @brief      send CMD_LED_ONOFF message to the addressed Slave,
 *              and return the response
 *  @param      slaveAddress: I2C Slave Address
 *  @param      onOff: true, turn the LED on, ...
 *  @param      onSync: the Slave waits for the SYNC signal before doing this
 *  @returns    result of the request_response, -1, ACK or NAK
 */
int send_LED_OnOff(_u8 slave_addr, boolean onOff, boolean onSync)
{
    _u8 payload[] = { 0, onSync, 0, onOff };

    return request_response(slave_addr, CMD_LED_ONOFF, sizeof payload, payload);
}


/*!
 *  @brief      send a "CMD_LED_PULSES" message to the addressed Slave,
 *              and return the response
 *  @param      slaveAddress: I2C Slave Address
 *  @param      startStop: true, start the LED pulses pattern, ...
 *  @param      onSync: the Slave waits for the SYNC signal before doing this
 *  @returns    result of the request_response, -1, ACK or NAK
 */
int send_LED_Pulses(_u8 slave_addr, boolean startStop, boolean onSync)
{

    _u8 payload[] = { 0, onSync, 0, startStop };

    return request_response(slave_addr, CMD_LED_PULSES, sizeof payload, payload);
}

