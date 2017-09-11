
#define ID_BIT0 A0
#define ID_BIT1 A1
#define ID_BIT2 A2
#define ID_BIT3 A3

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println("reset");
    pinMode(ID_BIT0, INPUT_PULLUP);
    pinMode(ID_BIT1, INPUT_PULLUP);
    pinMode(ID_BIT2, INPUT_PULLUP);
    pinMode(ID_BIT3, INPUT_PULLUP);
}

void loop()
{
    // put your main code here, to run repeatedly:
    byte val = read_switch1();
    display_switch1_value(val);
    delay(2000);
}

void display_switch1_value(byte val)
{
    Serial.print("\r\nSwitch1:");
    Serial.print("0x");
    Serial.print(val, 16);
    Serial.print(", or 0b");
    Serial.print(val, 2);
}

byte read_switch1()
{

    byte b0 = digitalRead( ID_BIT0 );
    byte b1 = digitalRead( ID_BIT1 );
    byte b2 = digitalRead( ID_BIT2 );
    byte b3 = digitalRead( ID_BIT3 );
    byte v = (b3<<3) | (b2<<2) | (b1<<1) | b0;
    return ~v & 0x0F;
}

