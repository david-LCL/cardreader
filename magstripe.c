#include <mqx.h>
#include <bsp.h>
#include <fio.h>

static pointer mag;

extern void printChar(char*,int);
extern void clearLCD();
extern void print1Char(char);

extern void fixSwipe();
void mag_init();
void mag_read();

char buffin[50];
char wholeid[21];


char id[9];
char expiration[5];

char buffsend[21];

void mag_init()
{

 	uint_32 param;
 
 

	fflush(stdout);


	mag = (pointer)fopen("ttya:", BSP_DEFAULT_IO_OPEN_MODE);
	if (mag== NULL) 
	{

	}
	 else 
	{	 
		fflush(stdout);

	    param = 9600;	// should comment this to test with HyperTerminal
		ioctl(mag,IO_IOCTL_SERIAL_SET_BAUD,&param);


	}
	
	
}


//void mag_read(char* idPtr, char* expirePtr)
void mag_read()
{

   int i;

   char * buffinptr;
   char * idptr;
   idptr=id;
   buffinptr=buffin;

   mag_init();

for (i = 0; i < 26; i++)
{
	

   if(TRUE)		
   {
	
		
   		read(mag,(pointer)buffinptr++,1);
	
  
   }
   else
   	{
   		
  
   	
   	}

}
   
	fclose(mag);	


	_time_delay(33);


	buffinptr=buffin;
	fixSwipe();
	buffinptr=wholeid;

	id[8] = '\0';
	expiration[4]='\0';


}

void fixSwipe()
{
	int i,j;
	int l=0;

	for(i=0;i<50;i++)
	{
		if(buffin[i]==';'&&	buffin[i+10]=='='&&buffin[i+15]=='='&&l==0) 
		{
		
					
				for(j=0;j<21;j++)
				{
						
					wholeid[j]=buffin[i+j];
					
					if(j>1 && j<10)
					{
						
						id[j-2]=wholeid[j];
					}
					
					if(j>10 && j<15)
					{
						
						expiration[j-11]=wholeid[j]; 
					}
					
				
					
					

				}
		
			l=1;
			
		}
			
	}
		

}
