#include <mqx.h>
#include <bsp.h>
#include <fio.h>
#include <io_gpio.h>

static pointer lcd;

void initLCD();
void clearLCD();
void printChar(char *a, int num);
void print1Char(unsigned char x);

void initLCD()
{
	 
   	 
	fflush(stdout);

	lcd = (pointer)fopen("ittyc:", BSP_DEFAULT_IO_OPEN_MODE);
	if (lcd== NULL) 
	{

	} 
	else 
	{
		fflush(stdout);
	}
   
}


void printChar( char *a,int num)
{ 
		_time_delay(20);
	initLCD();
		_time_delay(20);
	write(lcd,(pointer)a,num);
		_time_delay(20);
	fclose(lcd);
		_time_delay(20);
}


void print1Char(unsigned char x)
{ 
		_time_delay(20);
	initLCD();
		_time_delay(20);
	write(lcd,(pointer)&x,1);
		_time_delay(20);
	fclose(lcd);
		_time_delay(20);
}


void clearLCD()
{
	
	unsigned char Z=0xFE;
	unsigned char L=0x01;


	print1Char(Z);
	print1Char(L);	
	
}
