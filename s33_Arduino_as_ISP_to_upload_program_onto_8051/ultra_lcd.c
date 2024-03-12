#include <reg51.h>
#include <intrins.h>

#define lcd_data P2

sfr16 DPTR = 0x82;

sbit trig = P3^5;
sbit echo = P3^2;
sbit led = P3^1;

sbit rs = P0^0;
sbit rw = P0^1;
sbit en = P0^2;

void lcd_init();
void cmd(unsigned char a);
void dat(unsigned char b);
void show(unsigned char *s);
void lcd_delay();

void lcd_init()
{
    cmd(0x38);
    cmd(0x0e);
    cmd(0x06);
    cmd(0x0c);
    cmd(0x80);
}

void cmd(unsigned char a)
{
    lcd_data = a;
    rs = 0;
    rw = 0;
    en = 1;
    lcd_delay();
    en = 0;
}

void dat(unsigned char b)
{
    lcd_data = b;
    rs = 1;
    rw = 0;
    en = 1;
    lcd_delay();
    en = 0;
}

void show(unsigned char *s)
{
    while (*s)
    {
        dat(*s++);
    }
}

void lcd_delay()
{
    unsigned int i;
    for (i = 0; i <= 1000; i++)
        ;
}

void send_pulse(void)
{
    TH0 = TL0 = 0x00;
    trig = 1;
    _nop_();_nop_();_nop_();_nop_();_nop_();
    _nop_();_nop_();_nop_();_nop_();_nop_();
    trig = 0;
}

unsigned char ultrasonic()
{
    unsigned char get;
    send_pulse();
    while (!echo)
        ;
    while (echo)
        ;
    DPH = TH0;
    DPL = TL0;
    TH0 = TL0 = 0xff;
    if (DPTR < 38000)
        get = DPTR / 59;
    else
        get = 0;
    return get;
}

void main()
{
    TMOD = 0x09;
    TH0 = TL0 = 0x00;
    TR0 = 1;
    lcd_init();
    cmd(0x84);
    show("Distance");
    cmd(0xc9);
    show("cm");
    P3 |= (1 << 2);
    while (1)
    {
        short dist;
        char i;
        dist = ultrasonic();
        if (dist < 9)
            led = 1;
        else
            led = 0;
        cmd(0xc5);
        for (i = 4; i; i--)
        {
            dat('0' + dist % 10);
            dist /= 10;
        }
    }
}