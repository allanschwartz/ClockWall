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


byte    msgIn[MAX_MSG_SIZE];           ///< last message received IN from slave
byte    msgIn_len = 0;                 ///< size of message IN 


/*!
 *  @brief      send an I2C message (in the msgOut buffer) to a slave
 *  @param      slaveAddr: I2C Slave Address
 *  @returns    result code from endTransmission:
 *              0 .. success
 *              1 .. length to long for buffer
 *              2 .. address send, NACK received
 *              3 .. data send, NACK received
 *              4 .. other twi error (lost bus arbitration, bus error, ..)
 */
int i2c_sendRequestMsg(byte slaveAddr, byte msg[], byte msgLen)
{
    Wire.beginTransmission(slaveAddr);  // start condition

    Wire.write(msg, msgLen);        // write all bytes of the msg

    return Wire.endTransmission();    // stop transmitting
}


/*!
 *  @brief      received an I2C message, of a specific length
 *              from a specific I2C Slave
 *  @param      slaveAddress: I2C Slave Address
 *  @param      maxlen: the length of the received message
 *  @returns    number of bytes received
 */
int i2c_masterReceiveMsg(byte slaveAddress, byte maxlen)
{
    Wire.requestFrom(slaveAddress, maxlen);    // request len bytes from slave device

    msgIn_len = 0;
    // receive the message byte by byte
    while ( Wire.available() ) {
        msgIn[msgIn_len++] = Wire.read();
    }
    
    return msgIn_len;
}

