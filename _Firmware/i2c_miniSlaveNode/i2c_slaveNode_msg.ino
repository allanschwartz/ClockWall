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
 *  @brief      prepare an I2C 2-byte response message
 *  @param      cmd: command byte
 *  @param      info: info byte
 */
static void i2c_responseMsg(byte cmd, byte info)
{
    msgOut[0] = cmd;
    msgOut[1] = info;
    msgOut_len = 2;            // ready to transmit
}


/*!
 *  @brief      respond to an I2C read request, with an I2C message
 */
void i2c_requestHandler(void)
{
    Wire.write(msgOut, msgOut_len);
}


/*!
 *  @brief      once signaled that we have received an I2C message,
 *              read that message
 */
int i2c_slaveReceiveMsg(void)
{
    msgIn_len = 0;
    // receive the message, byte by byte
    while ( Wire.available() ) {
        msgIn[msgIn_len++] = Wire.read();
    }

    return msgIn_len;
}


/*!
 *  @brief      an call-back routine, that signals us that we
 *              have received an I2C message, and can 'read' it
 *  @param      _: not used
 */
void i2c_receiveHandler(int _)
{
    int rc1 = i2c_slaveReceiveMsg();

    if (rc1 == 0 || rc1 < 4) {
        i2c_responseMsg(MSG_NAK, rc1);
        return;
    }

    byte cmd = msgIn[0];

    switch (cmd) {

        case CMD_LED_ONOFF: {
            byte onSync = msgIn[1];
            byte pin = msgIn[2];
            boolean onOff = msgIn[3];
            LED_OnOff(pin, onOff, onSync);
            break;
        }
        case CMD_LED_NOP: {
            byte onSync = msgIn[1];
            LED_Nop(onSync);
            break;
        }
        default:
            i2c_responseMsg(MSG_NAK, UNKNOWN_CMD);
            return;

    }
    i2c_responseMsg(MSG_ACK, I2C_MSG_OK);
}

