/*!
 *  @file        i2c_slaveNode_msg.c
 *
 *  @brief       send and receive messages over the I2C interface
 *
 *  @date        January 2, 2016
 *
 *  @author      Allan M. Schwartz, allans@CodeValue.net
 *
 *  @copyright   (c) 2016, by Allan M. Schwartz.  All rights reserved.
 */


_u8    msgIn[MAX_MSG_SIZE];             ///< last message received IN from slave
_u8    msgIn_len = 0;                   ///< size of message IN 
_u8    msgOut[MAX_MSG_SIZE];            ///< next request message to go OUT
_u8    msgOut_len = 0;                  ///< size of message OUT


/*!
 *  @brief      prepare a Request Msg, in the MsgOut buffer
 *  @param      cmd: command byte
 *  @param      naddr: two-byte address of the Slave-device
 *  @param      payloadLen: length
 *  @param      payload: the payload portion of the message
 *  @returns    result code from endTransmission:
 *                0 .. success
 *                1 .. length to long for buffer
 *                2 .. address send, NACK received
 *                3 .. data send, NACK received
 *                4 .. other twi error (lost bus arbitration, bus error, ..)
 */
int i2c_sendRequestMsg(byte cmd, byte naddr[], byte payloadLen, _u8 payload[4])
{
    msgOut_len = 0;                     // semaphore, don't transmit while defining
    _u8 *p = (_u8 *)msgOut;
    _u8 crc = 0;
    _u8 len = payloadLen + 5;

    *p++ = cmd;
    crc ^= cmd;

    *p++ = len;
    crc ^= len;

    _u8 t;
    t = naddr[0];
    *p++ = t;
    crc ^= t;

    t = naddr[1];
    *p++ = t;
    crc ^= t;

    for (int i = 0; i < payloadLen; i++ ) {
        t = payload[i];
        *p++ = t;
        crc ^= t;
    }
    *p = crc;
    msgOut_len = len;                   // ready to transmit
    return i2c_masterSendMsg(naddr[1]);
}


/*!
 *  @brief      send an I2C message (in the msgOut buffer) to a slave
 *  @param      slaveAddress: I2C Slave Address
 *  @returns    result code from endTransmission:
 *              0 .. success
 *              1 .. length to long for buffer
 *              2 .. address send, NACK received
 *              3 .. data send, NACK received
 *              4 .. other twi error (lost bus arbitration, bus error, ..)
 */
int i2c_masterSendMsg(_u8 slaveAddress)
{
    Wire.beginTransmission(slaveAddress);

    Wire.write(msgOut, msgOut_len);

    return Wire.endTransmission();    // stop transmitting
}


/*!
 *  @brief      received an I2C message, of a specific length
 *              from a specific I2C Slave
 *  @param      slaveAddress: I2C Slave Address
 *  @param      maxlen: the length of the received message
 *  @returns    number of bytes received
 */
int i2c_masterReceiveMsg(_u8 slaveAddress, _u8 maxlen)
{
    Wire.requestFrom(slaveAddress, maxlen);    // request len bytes from slave device

    _u8 len = 0;
    msgIn_len = 0;
    // receive the message byte by byte
    while ( Wire.available() ) {
        msgIn[len++] = Wire.read();
    }
    msgIn_len = len;
    
    return len;
}

