/*!
 *  @file       slaveNode_msg.c
 *
 *  @brief      send and receive messages over the I2C interface
 *
 *  @date       January 2, 2016
 *
 *  @author     Allan M. Schwartz, allans@CodeValue.net
 *
 *  @copyright  (c) 2016, by Allan M. Schwartz.  All rights reserved.
 */


static byte   msgIn[MAX_MSG_SIZE] = {0};      ///< last message received IN from Master
static byte   msgIn_len = 0;            ///< size of message IN
static byte   spi_buffer_index = 0;
static boolean spi_transferring = false;



#if 0
/*!
 *  @brief      respond to an I2C read request, with an I2C message
 */
void i2c_requestHandler(void)
{
    Wire.write(msgOut, msgOut_len);
}
#endif


//Initialize SPI slave.
void SPIslaveInit(void)
{
    // Initialize SPI pins.
    pinMode(SCK, INPUT);
    pinMode(MOSI, INPUT);
    pinMode(MISO, INPUT);       // hi impedance mode
    pinMode(SS, INPUT);
    //
    // turn on SPI in slave mode
    SPCR |= bit(SPE);

    SPI.attachInterrupt();
}


/*!
 *  @brief      once signaled that we have received an I2C message,
 *              read that message
 */
int i2c_slaveReceiveMsg(void)
{
    //debugPuts("RECV");
    msgIn_len = 0;
    // receive the message, byte by byte
    while ( Wire.available() ) {
        msgIn[msgIn_len++] = Wire.read();
    }

    return msgIn_len;
}


#if 0
// the following code was lifted from
// /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/libraries/SPI/SPI.h
inline static byte SPItransfer(byte data)
{
    SPDR = data;
    asm volatile("nop");
    while (!(SPSR & _BV(SPIF))) ; // wait
    return SPDR;
}
#endif

#if 0
void spi_SelectEdge_ISR(void )
{
    digitalWrite(DEBUG_PIN, HIGH);
    digitalWrite(DEBUG_PIN, LOW);
    digitalWrite(DEBUG_PIN, HIGH);

    if ( !digitalRead(SS) ) {
        spi_buffer_index = 0;
        spi_transferring = true;
    } else {
        spi_transferring = false;
    }
    digitalWrite(DEBUG_PIN, LOW);
    digitalWrite(DEBUG_PIN, HIGH);
    digitalWrite(DEBUG_PIN, LOW);
}
#endif

ISR(SPI_STC_VECT)
{
    byte c = SPDR;  // grab byte from SPI Data Register
        
    digitalWrite(DEBUG_PIN, HIGH);
    //digitalWrite(DEBUG_PIN, LOW);
    
#if 0
    // receive the message, byte by byte
    if ( spi_buffer_index == 0 )
        SDDR = SPItransfer(msgOut[spi_buffer_index]);
    else {
        msgIn[spi_buffer_index - 1] = SPDR;
        SPDR = msgOut[spi_buffer_index];
    }
    spi_buffer_index++;
#endif

    //digitalWrite(DEBUG_PIN, HIGH);
    digitalWrite(DEBUG_PIN, LOW);
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

    int rc2 = i2c_checkMsg();
    if (rc2 != I2C_MSG_OK) {
        // on error, print the error code
        //myprintf("checkMsg rc %d", rc2);
        //i2c_sendResponseMsg(MSG_NAK, rc2);
        return;
    }

    byte cmd = msgIn[0];
    byte len = msgIn[1];
    if ( len != 7 ) {
        //i2c_sendResponseMsg(MSG_NAK, BAD_PAYLOAD_LEN);
        return;
    }

#if 0

    switch (cmd) {

        case CMD_LED_ONOFF: {
                byte onSync = msgIn[2];
                byte pin = msgIn[3];
                boolean onOff = msgIn[4];
                LED_OnOff(pin, onOff, onSync);
                break;
            }
        case CMD_LED_NOP: {
                byte onSync = msgIn[2];
                LED_Nop(onSync);
                break;
            }

        default:
            //i2c_sendResponseMsg(MSG_NAK, UNKNOWN_CMD);
            return;
    }

#endif

    //i2c_sendResponseMsg(MSG_ACK, I2C_MSG_OK);
}

