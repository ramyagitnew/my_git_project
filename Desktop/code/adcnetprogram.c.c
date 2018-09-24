#include<lpc214x.h>
//#include "LCD.h"
//#include "ADC.h"
 #define bit(x) (1<<x)
 unsigned int adc(int,int);
unsigned int val;
void lcd_init(void);
void cmd(unsigned char a);
void dat(unsigned char b);
void show(unsigned char *s);
void lcd_delay(void);
 
int main()
{
    IO1DIR=0xffffffff;
    IO0DIR=0x00000000;
    PINSEL0=0x0300;//selected ad0.7 as analog input
    VPBDIV=0x02;
    lcd_init();
    show("ADC Value : ");
    while(1) {
        cmd(0x8b);
        val=adc(0,6);
        dat((val/1000)+48);
        dat(((val/100)%10)+48);
        dat(((val/10)%10)+48);
        dat((val%10)+48);
    }
}
unsigned int adc(int no,int ch)
{
    switch(no)                                    //select adc
    {
        case 0: AD0CR=0x00200600|(1<<ch);       //select channel channel 1 adc0 selected
                AD0CR|=(1<<24);                 //start conversion
                while((AD0GDR& (1<<31))==0);
                val=AD0GDR;
                break;
 
        case 1: AD1CR=0x00200600|(1<<ch);       //select channel
                AD1CR|=(1<<24);                 //start conversion
                while((AD1GDR&(1<<31))==0);
                val=AD1GDR;
                break;
    }
    val=(val >> 6) & 0x03FF;                   // bit 6:15 is 10 bit AD value
 
    return val;
}

 
void lcd_init()
{
    cmd(0x38);
    cmd(0x0e);
    cmd(0x01);
    cmd(0x06);
    cmd(0x0c);
    cmd(0x80);
}
 
void cmd(unsigned char a)
{
    IO1CLR=0xFF070000;
    IO1SET=(a<<24);
    IO1CLR=bit(16);             //rs=0
    IO1CLR=bit(17);             //rw=0
    IO1SET=bit(18);             //en=1
    lcd_delay();
    IO1CLR=bit(18);             //en=0
}
 
void dat(unsigned char b)
{
    IO1CLR=0xFF070000;
    IO1SET=(b<<24);
    IO1SET=bit(16);             //rs=1
    IO1CLR=bit(17);             //rw=0
    IO1SET=bit(18);             //en=1
    lcd_delay();
    IO1CLR=bit(18);             //en=0
}
 
void show(unsigned char *s)
{
    while(*s) {
        dat(*s++);
    }
}
 
void lcd_delay()
{
    unsigned int i;
    for(i=0;i<=3000;i++);
}
