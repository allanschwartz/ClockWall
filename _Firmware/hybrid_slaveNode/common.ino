/*!
 *  @file       common.c
 *
 *  @brief      POC for multiple Arduino controller ...
 *              some common utility functions, common to both
 *              the MASTER and SLAVE
 *
 *  @date       January 2, 2016
 *
 *  @author     Allan M. Schwartz, allans@CodeValue.net
 *
 *  @copyright (c) 2016, by Allan M. Schwartz.  All rights reserved.
 */
 

#if 0
/*!
 *  @brief      thread of execution, controlling our
 *              blinking D13 LED
 *
 *  @note       this is non-functional if SPI-debugging is on
 *              because D13 is also used as the SPI CLK
 */
void blink_thread()
{
    static clock_t t0 = 0;
    static boolean led_on = false;

    if ( (clock_t)(millis() - t0) > 500 ) {
        t0 = millis();
        if ( led_on ) {
            digitalWrite(BLINK_PIN, HIGH );
            led_on = false;
        } else {
            digitalWrite(BLINK_PIN, LOW );
            led_on = true;
        }
    }
}
#endif


/*!
 *  @brief      our debug-only 'printf'-like function
 *              except the characters flow to the SPI MOSI line,
 *              instead of to a debug console
 */
#define PRINTF_BUF 80 // define the tmp buffer size (change if desired)
void myprintf(const char *format, ...)
{
    char buf[PRINTF_BUF];
    va_list ap;
    va_start(ap, format);
    vsnprintf(buf, sizeof(buf), format, ap);

    va_end(ap);
    debugPuts(buf);
}


/*!
 *  @brief      our debug-only 'fputs()'-like function
 *              the characters flow either to the SPI bus or a debug console
 */
void debugPuts(char *str)
{
#if DEBUG_SPI
    SPI.transfer(str, strlen(str));
#else
    Serial.write(str, strlen(str));
#endif
}


/*!
 *  @brief      set a software millisecond timer
 *  @param      timer: pointer to control structure
 *  @param      duration: timer duration in milliseconds
 */
void ms_timer_set(struct ms_timer *timer, clock_t duration)
{
    timer->start_time = millis();
    timer->duration = duration;
}


/*!
 *  @brief      test whether a software millisecond timer has expired
 *  @param      timer: pointer to control structure
 *  @returns    true if expired, false if not yet expired
 */
boolean ms_timer_expired(struct ms_timer *timer)
{
    if ((clock_t)(millis() - timer->start_time) >= timer->duration)
        return true;
    else
        return false;
}


/*!
 *  @brief      delay until a software millisecond timer expires
 *  @param      timer: pointer to control structure
 */
void ms_timer_delay(struct ms_timer *timer)
{
    while ( !ms_timer_expired(timer) )
        ;
}

