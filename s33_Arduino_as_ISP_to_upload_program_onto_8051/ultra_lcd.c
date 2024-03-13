#include <reg51.h>
#include <math.h>
#include <intrins.h>

#define lcd_data P2

sfr16 DPTR = 0x82;

sbit trig = P3^5;
sbit echo = P3^2;
sbit led = P1^1;

short dist=0;

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
	cmd(0x02);
    cmd(0x28);
    cmd(0x06);
    cmd(0x0c);
	cmd(0x01);
    cmd(0x80);
}

void cmd(unsigned char a)
{
    lcd_data = (a & 0xF0);
    rs = 0;
    rw = 0;
    en = 1;
    lcd_delay();
    en = 0;
	lcd_delay();
	
	lcd_data = (a << 4);
    en = 1;
    lcd_delay();
    en = 0;
}

void dat(unsigned char b)
{
    lcd_data = (b & 0xF0);
    rs = 1;
    rw = 0;
    en = 1;
    lcd_delay();
    en = 0;
	lcd_delay();
	
	lcd_data = (b << 4);
    en = 1;
    lcd_delay();
    en = 0;
}

void show(unsigned char *s)
{
    while (*s) dat(*s++);
}

void lcd_delay()
{
    unsigned int i;
    for (i = 0; i <= 1000; i++);
}

void send_pulse(void)
{
    TH0 = TL0 = 0x00;
    trig = 1;
    _nop_();_nop_();_nop_();_nop_();_nop_();
    _nop_();_nop_();_nop_();_nop_();_nop_();
    trig = 0;
}

short ultrasonic()
{
    short get;
    send_pulse();
    while (!echo);
    while (echo);
    DPH = TH0;
    DPL = TL0;
    TH0 = TL0 = 0xff;
    if (DPTR < 38000) get = DPTR / 59;
    else get = 0;
    return get;
}

void main()
{
	char i;
    TMOD = 0x09;
    TH0 = TL0 = 0x00;
    TR0 = 1;
    lcd_init();
    cmd(0x81);
    show("Distance:");
    cmd(0x8d);
    show("cm");
    P3 |= (1 << 2);
    while (1)
    {
        dist = ultrasonic();
        if (dist > 400){
			led = 0;
			show("---");
			continue;
		}
        led = 1;
		cmd(0x8a);
		dat((dist/100)%100+48);
		dat((dist/10)%10+48);
		dat((dist%10)+48);
		dist=sqrt(dist*1.5625);
		cmd(0xc0);
		for(i=1;i<=16;i++)
			i<=dist ? dat(255):dat(' ');
    }
}