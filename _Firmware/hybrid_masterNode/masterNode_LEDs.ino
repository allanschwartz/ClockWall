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


extern byte    msgIn[MAX_MSG_SIZE];      ///< last received message (from a Slave)


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
int request_response(byte slaveAddr, byte msg[], byte msgLen)
{
    int rc;

    // SPI case
    if ( slaveAddr == 10 || slaveAddr == 11) {
        spi_request_response(slaveAddr-7, msg, msgLen);
        return msgIn[0];
    }

    // I2C case
    rc = i2c_sendRequestMsg(slaveAddr, msg, msgLen);
    if ( rc != 0 ) {
        myprintf("sendRequestMsg rc %d", rc );
        return -2;              // error return
    }
    delayMicroseconds(200);
    rc = i2c_masterReceiveMsg(slaveAddr, 4);
    delayMicroseconds(200);

    if ( rc != 4 ) {
        myprintf("RCV %d", rc );
        return -1;              // error return
    }
    return msgIn[0];            // return ACK or NAK
}


/*!
 *  @brief      send CMD_LED_ONOFF message to the addressed Slave,
 *              and return the response
 *  @param      slaveAddress: I2C Slave Address
 *  @param      onOff: true, turn the LED on, ...
 *  @param      onSync: the Slave waits for the SYNC signal before doing this
 *  @returns    result of the request_response, -1, ACK or NAK
 */
int send_LED_OnOff(byte slaveAddr, boolean onOff, boolean onSync)
{
    byte msg[6] = { CMD_LED_ONOFF, 7, onSync, LED_PIN, onOff, 0 };

    return request_response(slaveAddr, msg, sizeof msg);
}


/*!
 *  @brief      send CMD_LED_NOP message to the addressed Slave,
 *              and return the response
 *  @param      slaveAddr: I2C Slave Address...
 *  @param      onSync: the Slave waits for the SYNC signal before doing this
 *  @returns    result of the request_response, -1, ACK or NAK
 */
int send_LED_Nop(byte slaveAddr, boolean onSync)
{
    byte msg[6] = { CMD_LED_ONOFF, 7, onSync, 0, 0, 0};

    return request_response(slaveAddr, msg, sizeof msg);
}

