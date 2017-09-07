/*!
 *  @file       i2c_misc.h
 *
 *  @brief      types and enumerations common to the Master and the Slave
 *
 *  @history    January 2, 2016
 *
 *  @author     Allan M. Schwartz, allans@CodeValue.net
 *
 *  @copyright  (c) 2016, by Allan M. Schwartz.  All rights reserved.
 */

//! my favorite types
typedef unsigned long clock_t;

//! command operations  (first byte of the request message)
enum cmd_byte {
    CMD_LED_ONOFF = 'O',
    CMD_LED_NOP = 'N',
};

//! response message types (first byte of the response message)
enum response_type {
    MSG_ACK = 'A',
    MSG_NAK = 'N',
};

//! the status of a received msg (second byte of the response message)
enum receive_status {
    I2C_MSG_OK = 0,
    UNKNOWN_CMD = 'u',
};

//! software millisecond timer, easier to use than 'millis()'
struct ms_timer {
    clock_t    start_time;              ///< from millis(), [ms] since boot
    clock_t    duration;                ///< timer duration, [ms]
};

#define MAX_MSG_SIZE    32              ///< specific to Arduino I2C implementation

//void myprintf(const char *format, ...);
