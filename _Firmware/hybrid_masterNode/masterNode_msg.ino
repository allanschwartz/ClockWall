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
int i2c_sendRequestMsg(byte slaveAddr, byte msg[], byte msgLen)
{
    byte crc = 0;
    for (byte i = 0; i < msgLen; i++)
        crc ^= msg[i];
        
    Wire.beginTransmission(slaveAddr);

    Wire.write(msg, msgLen);          // write all the bytes of the msg
    Wire.write(crc);                  // add the CRC

    return Wire.endTransmission();    // stop transmitting
}

void spi_sendRequestMsg(byte slaveSS_pin, byte msg[], byte msgLen, bool withCRC)
{
    if ( withCRC ) {
        byte crc = 0;
        for (byte i = 0; i < msgLen; i++)
            crc ^= msg[i];
        msg[msgLen] = crc;
        msgLen++;
    }
        
    digitalWrite(slaveSS_pin, LOW);     // SS enable

    SPI.transfer(msg, msgLen);          // write all the bytes of the msg

    digitalWrite(slaveSS_pin, HIGH);     // SS disable 
}

void spi_receiveResponseMsg(byte slaveSS_pin, byte msgLen)
{
    digitalWrite(slaveSS_pin, LOW);     // SS enable

    const char *p = "0123456789";
    for (byte i = 0; i < msgLen; i++) {
        msgIn[i] = SPI.transfer(*p);          // read all the bytes of the msg
        p++;
    }

    digitalWrite(slaveSS_pin, HIGH);     // SS disable
}

void spi_request_response(byte slaveSS_pin, byte msg[], byte msgLen)
{
    spi_sendRequestMsg(slaveSS_pin, msg, msgLen, true);
    
    spi_receiveResponseMsg(slaveSS_pin, 8);
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

