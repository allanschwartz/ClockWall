/*!
 *  @file       i2c_slaveNode_msg.c
 *
 *  @brief      send and receive messages over the I2C interface
 *
 *  @date       January 2, 2016
 *
 *  @author     Allan M. Schwartz, allans@CodeValue.net
 *
 *  @copyright  (c) 2016, by Allan M. Schwartz.  All rights reserved.
 */


static byte   msgIn[MAX_MSG_SIZE];      ///< last message received IN from Master
static byte   msgIn_len = 0;            ///< size of message IN
static byte   msgOut[MAX_MSG_SIZE];     ///< next message to go OUT to Master
static byte   msgOut_len = 0;           ///< size of message OUT


/*!
 *  @brief      prepare an I2C response message (of fixed length)
 *  @param      cmd: command byte
 *  @param      info: info byte
 */
static void i2c_sendResponseMsg(byte cmd, byte info)
{
    msgOut_len = 0;            // semaphore, or sorts
    byte *p = msgOut;
    byte crc = 0;
    byte len = 4;

    *p = cmd;
    crc ^= *p++;

    *p = len;
    crc ^= *p++;

    *p = info;
    crc ^= *p++;

    *p = crc;
    msgOut_len = len;            // ready to transmit
}


/*!
 *  @brief      respond to an I2C read request, with an I2C message
 */
void i2c_requestHandler(void)
{
    Wire.write(msgOut, msgOut_len);
    msgOut_len = 0;            // so we don't resend it
}


/*!
 *  @brief      once signaled that we have received an I2C message,
 *              read that message
 */
int i2c_slaveReceiveMsg(void)
{
    debugPuts("RECV");
    byte len = 0;
    msgIn_len = 0;
    // receive the message, byte by byte
    while ( Wire.available() ) {
        msgIn[len++] = Wire.read();
    }
    msgIn_len = len;

    return len;
}


/*!
 *  @brief      check the I2C message from the master for correctness
 */
int i2c_checkMsg(void)
{
    // check the message length
    if (msgIn[1] != msgIn_len) {
        return I2C_LEN_WRONG;
    }

    // check that the message is addressed to this I2C Slave Address
    byte naddr = msgIn[3];
    if (naddr != MY_I2C_ADDRESS)
        return I2C_BAD_NADDR;

    // check that the message CRC's o.k.
    byte crc = 0;
    for (byte i = 0; i < msgIn_len; i++) {
        crc ^= msgIn[i];
    }
    if (crc != 0 )
        return I2C_BAD_CRC;

    return I2C_MSG_OK;
}


/*!
 *  @brief      an call-back routine, that signals us that we
 *              have received an I2C message, and can 'read' it
 *  @param      _: not used
 */
void i2c_receiveHandler(int _)
{
    int rc1 = i2c_slaveReceiveMsg();
    if (rc1 != 9) {
        // we expect to receive only 9-byte messages
        myprintf("receive msg rc %d", rc1);
    }
    
    if (rc1 == 0) {
        i2c_sendResponseMsg(MSG_NAK, 0);
        return;
    }

    int rc2 = i2c_checkMsg();
    if (rc2 != I2C_MSG_OK) {
        // on error, print the error code
        myprintf("checkMsg rc %d", rc2);
        i2c_sendResponseMsg(MSG_NAK, rc2);
        return;
    }

    byte cmd = msgIn[0];
    byte len = msgIn[1];
    if ( len != 9 ) {
        i2c_sendResponseMsg(MSG_NAK, BAD_PAYLOAD_LEN);
        return;
    }

    switch (cmd) {

        case CMD_LED_PATTERN: {
                _u16 t_on = makeWord(msgIn[4], msgIn[5]);
                _u16 t_off = makeWord(msgIn[6], msgIn[7]);

                LEDpattern(t_on, t_off);
                i2c_sendResponseMsg(MSG_ACK, I2C_MSG_OK);
                break;
            }
        case CMD_LED_ONOFF:
            LED_OnOff(msgIn[7], msgIn[5]);
            i2c_sendResponseMsg(MSG_ACK, I2C_MSG_OK);
            break;

        case CMD_LED_PULSES:
            LED_Pulses(msgIn[7], msgIn[5]);
            i2c_sendResponseMsg(MSG_ACK, I2C_MSG_OK);
            break;

        default:
            i2c_sendResponseMsg(MSG_NAK, UNKNOWN_CMD);
            break;
    }
}

