#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define NEO_PIN 6

#define NEO_NUM_LEDS 12

#define BRIGHTNESS 50

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEO_NUM_LEDS, NEO_PIN, NEO_GRB + NEO_KHZ800);


void setup()
{

    strip.setBrightness(BRIGHTNESS);
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}

void loop()
{
    // Some example procedures showing how to display to the pixels:
    colorWipe(strip.Color(255, 0, 0), 50); // Red
    strip.show(); // Initialize all pixels to 'off'
    delay(1000);
    colorWipe(strip.Color(0, 255, 0), 50); // Green
    colorWipe(strip.Color(255, 0, 0), 50); // Red
    strip.show(); // Initialize all pixels to 'off'
    delay(1000);
    colorWipe(strip.Color(0, 0, 255), 50); // Blue
    colorWipe(strip.Color(255, 0, 0), 50); // Red
    strip.show(); // Initialize all pixels to 'off'
    delay(1000);
    colorWipe(strip.Color(0, 0, 0, 255), 50); // White
    colorWipe(strip.Color(255, 0, 0), 50); // Red
    strip.show(); // Initialize all pixels to 'off'
    delay(1000);

    whiteOverRainbow(20, 75, 5);

    colorWipe(strip.Color(0, 0, 0, 255), 50); // White

    rainbowFade2White(3, 3, 1);
    delay(5000);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait)
{
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}



void rainbowFade2White(uint8_t wait, int rainbowLoops, int whiteLoops)
{
    float fadeMax = 100.0;
    int fadeVal = 0;
    uint32_t wheelVal;
    int redVal, greenVal, blueVal;

    for (int k = 0 ; k < rainbowLoops ; k ++) {

        for (int j = 0; j < 256; j++) { // 5 cycles of all colors on wheel

            for (int i = 0; i < strip.numPixels(); i++) {

                wheelVal = Wheel(((i * 256 / strip.numPixels()) + j) & 255);

                redVal = red(wheelVal) * float(fadeVal / fadeMax);
                greenVal = green(wheelVal) * float(fadeVal / fadeMax);
                blueVal = blue(wheelVal) * float(fadeVal / fadeMax);

                strip.setPixelColor( i, strip.Color( redVal, greenVal, blueVal ) );

            }

            //First loop, fade in!
            if (k == 0 && fadeVal < fadeMax - 1) {
                fadeVal++;
            }

            //Last loop, fade out!
            else if (k == rainbowLoops - 1 && j > 255 - fadeMax ) {
                fadeVal--;
            }

            strip.show();
            delay(wait);
        }

    }



    delay(500);

}

void whiteOverRainbow(uint8_t wait, uint8_t whiteSpeed, uint8_t whiteLength )
{

    if (whiteLength >= strip.numPixels()) whiteLength = strip.numPixels() - 1;

    int head = whiteLength - 1;
    int tail = 0;

    int loops = 3;
    int loopNum = 0;

    static unsigned long lastTime = 0;


    while (true) {
        for (int j = 0; j < 256; j++) {
            for (uint16_t i = 0; i < strip.numPixels(); i++) {
                if ((i >= tail && i <= head) || (tail > head && i >= tail) || (tail > head && i <= head) ) {
                    strip.setPixelColor(i, strip.Color(0, 0, 0, 255 ) );
                } else {
                    strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
                }

            }

            if (millis() - lastTime > whiteSpeed) {
                head++;
                tail++;
                if (head == strip.numPixels()) {
                    loopNum++;
                }
                lastTime = millis();
            }

            if (loopNum == loops) return;

            head %= strip.numPixels();
            tail %= strip.numPixels();
            strip.show();
            delay(wait);
        }
    }

}
void fullWhite()
{

    for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0, 255 ) );
    }
    strip.show();
}


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait)
{
    uint16_t i, j;

    for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
        for (i = 0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        }
        strip.show();
        delay(wait);
    }
}

void rainbow(uint8_t wait)
{
    uint16_t i, j;

    for (j = 0; j < 256; j++) {
        for (i = 0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel((i + j) & 255));
        }
        strip.show();
        delay(wait);
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85) {
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3, 0);
    }
    if (WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3, 0);
    }
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0, 0);
}

uint8_t red(uint32_t c)
{
    return (c >> 16);
}
uint8_t green(uint32_t c)
{
    return (c >> 8);
}
uint8_t blue(uint32_t c)
{
    return (c);
}

