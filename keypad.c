
#include <mqx.h>
#include <bsp.h>
#include <io_gpio.h>


 FILE_PTR col1_port,col2_port,col3_port, rows_port;

#define keypad1 (GPIO_PORT_AN | GPIO_PIN2)
#define keypad2 (GPIO_PORT_AN | GPIO_PIN1)
#define keypad3 (GPIO_PORT_TC | GPIO_PIN3)
#define keypad4 (GPIO_PORT_AN | GPIO_PIN3)
#define keypad5 (GPIO_PORT_AN | GPIO_PIN4)
#define keypad6 (GPIO_PORT_AN | GPIO_PIN5)
// bad pin on microcontroller #define keypad7 (GPIO_PORT_AN | GPIO_PIN6)
#define keypad7 (GPIO_PORT_TC | GPIO_PIN0)
char key;
extern void clearLCD();
extern void printChar(char*,int);

   	GPIO_PIN_STRUCT col_1[] = {
   		keypad1|GPIO_PIN_STATUS_0,   	
      	GPIO_LIST_END
   	};
   	GPIO_PIN_STRUCT col_2[] = {
   		keypad3|GPIO_PIN_STATUS_0,
   			GPIO_LIST_END
   	};
	GPIO_PIN_STRUCT col_3[] = {
		keypad5|GPIO_PIN_STATUS_0,
			GPIO_LIST_END
	};
   	GPIO_PIN_STRUCT row_set[] = {
   		keypad2|GPIO_PIN_STATUS_0,
   		keypad4|GPIO_PIN_STATUS_0,
   		keypad6|GPIO_PIN_STATUS_0,
   		keypad7|GPIO_PIN_STATUS_0,
      	GPIO_LIST_END
   	};
   	


void keyInit()
{
	
	
	
	
	
	
	
	 if (NULL == (col1_port = fopen("gpio:write", (char_ptr) &col_1)))
   {
       printChar("col 1 not open",14);
      
   }if (NULL == (col2_port = fopen("gpio:write", (char_ptr) &col_2)))
   {
             printChar("col 2 not open",14);
  
   }if (NULL == (col3_port = fopen("gpio:write", (char_ptr) &col_3)))
   {
              printChar("col 3 not open",14);

   }
   
   if (NULL == (rows_port = fopen("gpio:read", (char_ptr) &row_set )))
   {
              printChar("row not open",12);
      
   }

	
	
	
	
	
	
	
	
}

char ScanKeypad()
{
//printf("keypad button triggers number\n");
 
   key = 'x';
   		
   		if (IO_OK != ioctl(col1_port, GPIO_IOCTL_WRITE_LOG1, (char_ptr) &col_1[0]))
           {	printChar("error col1",10);
              // printf("Setting pin1 to log 1 failed.\n");
              //_mqx_exit(-1);
           }                   
       // read row pin status to pin list /
       // read row pin 2 status
       if (IO_OK != ioctl(rows_port, GPIO_IOCTL_READ, (char_ptr) &row_set[0]))
       {	printChar("error",1);
       		clearLCD();
           //printf("Reading pin status from kpfile2 to kppins2 list failed.\n");
          //_mqx_exit(-1);
       }      
       if (row_set[0] & GPIO_PIN_STATUS_1)
       {	//printChar("2",1);
       key='2';
           //printf("2"); 
       }
       else 
       {
          // printf("");
       }
       
       //read row pin 4 status
       if (IO_OK != ioctl(rows_port, GPIO_IOCTL_READ, (char_ptr) &row_set[1]))
       {//	printChar("0",1);
          // printf("Reading pin status from kpfile2 to kppins2 list failed.\n");
          //_mqx_exit(-1);
          key = '0';
       }      
       if (row_set[1] & GPIO_PIN_STATUS_1)
       {	printChar("0",1);
           //printf("0"); 
       }
       else 
       {
           //printf("");
       }
       
       //read row pin 6 status
       if (IO_OK != ioctl(rows_port, GPIO_IOCTL_READ, (char_ptr) &row_set[2]))
       {	printChar("0",1);
           //printf("Reading pin status from kpfile2 to kppins2 list failed.\n");
          //_mqx_exit(-1);
       }      
       if (row_set[2] & GPIO_PIN_STATUS_1)
       {//	printChar("8",1);
           //printf("8"); 
           key = '8';
       }
       else 
       {
           //printf("");
       }
       
       //read row pin 7 status
       if (IO_OK != ioctl(rows_port, GPIO_IOCTL_READ, (char_ptr) &row_set[3]))
       {	printChar("0",1);
          // printf("Reading pin status from kpfile2 to kppins2 list failed.\n");
          //_mqx_exit(-1);
       }      
       if (row_set[3] & GPIO_PIN_STATUS_1)
       {//	printChar("5",1);
           //printf("5"); 
           key = '5';
       }
       else 
       {
           //printf(""); 
       }
       
       //clear pin1 back to 0
       
      if (IO_OK != ioctl(col1_port, GPIO_IOCTL_WRITE_LOG0, NULL))
           {	printChar("0",1);
              // printf("clear pin1 to 0 failed.\n");
              //_mqx_exit(-1);
           }
      //printf("loop check\n");
       _time_delay(100);
          
 
   		if (IO_OK != ioctl(col2_port, GPIO_IOCTL_WRITE_LOG1, (char_ptr) &col_2[0]))
           {	printChar("0",1);
               //printf("Setting pin1 to log 1 failed.\n");
              //_mqx_exit(-1);
           }                   
       // read row pin status to pin list //
       // read row pin 2 status
       if (IO_OK != ioctl(rows_port, GPIO_IOCTL_READ, (char_ptr) &row_set[0]))
       {	printChar("0",1);
           //printf("Reading pin status from kpfile2 to kppins2 list failed.\n");
         // _mqx_exit(-1);
       }      
       if (row_set[0] & GPIO_PIN_STATUS_1)
       {
           //printf("1");
          // printChar("1",1); 
          key = '1';
       }
       else 
       {
          // printf("");
       }
       
       //read row pin 4 status
       if (IO_OK != ioctl(rows_port, GPIO_IOCTL_READ, (char_ptr) &row_set[1]))
       {	printChar("0",1);
          // printf("Reading pin status from kpfile2 to kppins2 list failed.\n");
          //_mqx_exit(-1);
       }      
       if (row_set[1] & GPIO_PIN_STATUS_1)
       {
           //printf("*"); 
        //   printChar("*",1);
        key = '*';
       }
       else 
       {
           //printf("");
       }
       
       //read row pin 6 status
       if (IO_OK != ioctl(rows_port, GPIO_IOCTL_READ, (char_ptr) &row_set[2]))
       {	printChar("0",1);
           //printf("Reading pin status from kpfile2 to kppins2 list failed.\n");
          //_mqx_exit(-1);
       }      
       if (row_set[2] & GPIO_PIN_STATUS_1)
       {//	printChar("7",1);
           //printf("7"); 
           key = '7';
       }
       else 
       {
          // printf("");
       }
       
       //read row pin 7 status
       if (IO_OK != ioctl(rows_port, GPIO_IOCTL_READ, (char_ptr) &row_set[3]))
       {	printChar("0",1);
           //printf("Reading pin status from kpfile2 to kppins2 list failed.\n");
          //_mqx_exit(-1);
       }      
       if (row_set[3] & GPIO_PIN_STATUS_1)
       {
           //printf("");
          // printChar("4",1); 
          key = '4';
       }
       else 
       {	//printChar("0",1);
           //printf("4"); 
       }
       
       //clear pin1 back to 0
       
      if (IO_OK != ioctl(col2_port, GPIO_IOCTL_WRITE_LOG0, NULL))
           {	printChar("0",1);
               //printf("clear pin1 to 0 failed.\n");
              //_mqx_exit(-1);
           }
           
      //printf("loop check\n");
       _time_delay(100);
       
       

   		
   		if (IO_OK != ioctl(col3_port, GPIO_IOCTL_WRITE_LOG1, (char_ptr) &col_3[0]))
           {	printChar("0",1);
               //printf("Setting pin1 to log 1 failed.\n");
              //_mqx_exit(-1);
           }                   
       // read row pin status to pin list /
       // read row pin 2 status
       if (IO_OK != ioctl(rows_port, GPIO_IOCTL_READ, (char_ptr) &row_set[0]))
       {	printChar("0",1);
           //printf("Reading pin status from kpfile2 to kppins2 list failed.\n");
          //_mqx_exit(-1);
       }      
       if (row_set[0] & GPIO_PIN_STATUS_1)
       {//printChar("3",1);
           //printf("3"); 
             key = '3';
       }
       else 
       {
           //printf("");
       }
       
       //read row pin 4 status
       if (IO_OK != ioctl(rows_port, GPIO_IOCTL_READ, (char_ptr) &row_set[1]))
       {
       		printChar("0",1);
           //printf("Reading pin status from kpfile2 to kppins2 list failed.\n");
          //_mqx_exit(-1);
       }      
       if (row_set[1] & GPIO_PIN_STATUS_1)
       {//printChar("#",1);
           //printf("#"); 
             key = '#';
       }
       else 
       {
           //printf("");
       }
       
       //read row pin 6 status
       if (IO_OK != ioctl(rows_port, GPIO_IOCTL_READ, (char_ptr) &row_set[2]))
       {	printChar("0",1);
           //printf("Reading pin status from kpfile2 to kppins2 list failed.\n");
         // _mqx_exit(-1);
       }      
       if (row_set[2] & GPIO_PIN_STATUS_1)
       {//printChar("9",1);
          // printf("9"); 
            key = '9';
       }
       else 
       {
          // printf("");
       }
       
       //read row pin 7 status
       if (IO_OK != ioctl(rows_port, GPIO_IOCTL_READ, (char_ptr) &row_set[3]))
       {	printChar("0",1);
           //printf("Reading pin status from kpfile2 to kppins2 list failed.\n");
         // _mqx_exit(-1);
       }      
       if (row_set[3] & GPIO_PIN_STATUS_1)
       {//printChar("6",1);
           //printf("6"); 
             key = '6';
       }
       else 
       {
           //printf(""); 
       }
       
       //clear pin1 back to 0
       
      if (IO_OK != ioctl(col3_port, GPIO_IOCTL_WRITE_LOG0, (char_ptr) NULL))
           {	printChar("0",1);
               //printf("clear pin1 to 0 failed.\n");
             // _mqx_exit(-1);
           }
       
      
     // printf("loop check\n");
       _time_delay(100);
       
     
    return key;
   	
}
  


 



 

