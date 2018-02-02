/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved                       
*
*************************************************************************** 
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR 
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
* THE POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************
*
* $FileName: spi.c$
* $Version : 3.5.23.0$
* $Date    : Jan-13-2010$
*
* Comments:
*
*   This file contains the source for a simple strPayload of an
*   application that writes and reads the SPI memory using the SPI driver.
*   It's already configured for onboard SPI flash where available.
*
*END************************************************************************/

/*********************************************************************/
/****** THIS FILE HAS BEEN MODIFIED FOR EDUCATIONAL PURPOSE **********/
/********************************************************************* 	
		Senior Design Project, University of California, 2010
		Project: Wireless-Embedded Card Reader
		MCU: M52259DEMO KIT
		Port QS are opened automatically MQX.
***********************************************************************/


// Add prototype support.
#ifndef PROTO
#if defined (USE_PROTOTYPES) ? USE_PROTOTYPES : defined (__STDC__)
#define PROTO(ARGS) ARGS
#else
#define PROTO(ARGS) ()
#endif
#endif

// Include header files
#include <cstring>
#include <stdlib.h>
#include <mqx.h>
#include <bsp.h>
#include <spi.h>
#include "my_spi.h"
#include "nrf24l01.h"
#include "md5.h"

//#include "file_io_task.h"

#include "MFS_USB.h" //  to use file system on USB drive


// File pointer for SPI port
FILE_PTR    spifd;

// BSP requirements
#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif

#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif

#if ! BSPCFG_ENABLE_SPI0
#error This application requires BSPCFG_ENABLE_SPI0 defined non-zero in user_config.h. Please recompile kernel with this option.
#endif

#if BSP_SPI_MEMORY_GPIO_CS

#if ! BSPCFG_ENABLE_GPIODEV
#error This application requires BSPCFG_ENABLE_GPIODEV defined non-zero in user_config.h. Please recompile BSP with this option.
#endif

#endif

// Define PAYLOAD 
typedef unsigned short deviceIDType;	// 2 byte
typedef unsigned short eventNoType;		// 2 byte
typedef char studentIDType[16]; 		// CHANGE BACK TO unsigned WHEN MD5 WORKS !?
typedef char expireCodeType[8]; 		// 8 byte
typedef int timeStampType;				// 4 byte

TIME_STRUCT time_mqx1;

// Define struct for payload
struct PayloadStruct{
	deviceIDType deviceID;
	eventNoType eventNo;
	studentIDType studentID;
	expireCodeType expireCode;
	timeStampType timeStamp;
	};
typedef struct PayloadStruct Payload;

// Define struct for ack
/*
struct AckStruct
{
	deviceIDType deviceID;
	eventNoType eventNo;
	timeStampType timeStamp;
};
*/
int activeEventNo = 0; //the event number

// Functions Prototype
extern void main_task (uint_32);
extern void printChar(char *,int);
extern void print1Char(unsigned char);
extern void clearLCD();
extern char* mag_read();
extern char ScanKeypad();
extern void keyInit();
extern doorMan(char_ptr id);
extern appendFile(uchar_ptr id);
extern void unlockDoor();

extern int checkRoster(unsigned char* id, uchar roster[300][33], int size);
boolean checkAcknowledment(uchar payloadRX[64]);
void initialize();
void sync();
void comm();
void scan();
//void saveRoster(char* input);
//extern void checkDoor(char *);

boolean transmit(Payload *);

// constants
#define TX_RX_LEN 32
#define ENHANCED_SHOCKBURST true
const char * salt = "/cardreader/listen";

// Variables
boolean SYNC_MODE = false;		
boolean COMM_MODE = false;
boolean SCAN_MODE = false;
//boolean KEYPAD_MODE = false;
Payload strPayload;
unsigned char studentHex[16];	// encrypted student id_with_salt	
int roster_size = 0;
uchar roster[300] [33];			// roster's size by default is 300
char id_with_salt[8+sizeof(salt)];		
uchar payloadRX[33];
uchar payloadTX[33];
char LCDmessage[25];
boolean flag, flag2=false, flag3=false;

//eventNoType event;

// Task struct
const TASK_TEMPLATE_STRUCT  MQX_template_list[] = 
{
   /* Task Index,   Function,     Stack,  Priority,   Name,   Attributes,          Param, Time Slice */
    //{ 11L,      	Shell_Task, 	4000,   10,       "Shell",  MQX_AUTO_START_TASK, 0,     0 },
    { 12L,			USB_task,		3000L,  8,		  "USB", 	MQX_AUTO_START_TASK, 0, 	0 },
    { 13L,          main_task, 		1000L,  6L,       "main", 	MQX_AUTO_START_TASK, 0,     0 },
    { 0 }
};

/***************************************************************************/
/*MAIN PROGRAM*---------------------------------------------------------------------
* 
* Comments  : ...
* 	
*	
*
*END*----------------------------------------------------------------------*/
void main_task (uint_32 dummy)
{

	clearLCD();
	// Initialize
	printChar("Wait...", 7);
	_time_delay(1111);
	clearLCD();
	initialize();
	

	// GO TO SYNC_MODE
	// Ack struct contains: gateway ID (hardcode), event No, timestamp 
	// can we cast a payload to ack struct?
	
	SYNC_MODE =false;
	if (SYNC_MODE) sync();

	// GO TO COMM_MODE
	//if (COMM_MODE) comm();

	// GO TO SCAN_MODE
	SCAN_MODE = true;
		printChar("scan...", 7);
	_time_delay(1111);
	clearLCD();
	if (SCAN_MODE) while (1) scan();
	

} // end of main





/****************************************/
/************** FUNCTIONS ***************/
/****************************************/


// Check Acknowledgement from Gateway 
boolean checkAcknowledment(uchar payloadRX[33]) 
{
	uchar str[7];
	int i;

	for (i=0;i<7;i++)
		str[i]=payloadRX[i];

	if (strcmp((char *)str, "EventID") ) // temporarily use "EventID" to identify the gateway
		return true;
	else 
		return false;

}

// this function send and read a byte from M552259DEMO's SPI port
uchar spi0_send_read_byte(uchar byte)
{
	uint_32 ret = 0;

	// Read a data byte from memory
	// ret = memory_read_byte (spifd, SPI_MEMORY_ADDR1);
	// printChar ("b4 read: 0x%x\n", byte);
	ret = read(spifd, &byte, 1);
	fflush (spifd);
	if (ret != 1) 
	{
			clearLCD();
			strcpy(LCDmessage, "ERROR");
			printChar (LCDmessage, sizeof(LCDmessage));;
	}
	else
	{
			// printChar ("read: 0x%x\n", byte);
	}
  
	return byte;
}

// this function initialize the system
void initialize()
{
	// SPI's variables
	uint_32                i=0;
	uint_32                param;
   	
   	// open LCD
   	
   	
	// Open SPI port
	clearLCD();
	printChar ("Starting ...", 12);
	
	// Open the SPI controller 
	spifd = fopen ("spi0:", NULL);
	if (NULL == spifd) 
	{
		_time_delay (200L);
		_mqx_exit (-1);
	}

	// Set baud rate 
	param = 2000000;
	if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_SET_BAUD, &param)) {}
	else {}

	// Set clock mode 
	param = SPI_CLK_POL_PHA_MODE0;
	if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_SET_MODE, &param)) {} 
	else {}

	// Set big endian 
	param = SPI_DEVICE_BIG_ENDIAN;
	if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_SET_ENDIAN, &param)) {} 
	else {}

	// Set transfer mode 
	param = SPI_DEVICE_MASTER_MODE;
	if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_SET_TRANSFER_MODE, &param)) {} 
	else {}

	// Clear statistics 
	if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_CLEAR_STATS, NULL)) {} 
	else {}

	// Initialize transceiver 
	nrf24l01_initialize_debug(false, TX_RX_LEN, ENHANCED_SHOCKBURST); // transmit mode by default with ES
	
	// System initialized
	
	clearLCD();
	printChar("Initialization  done", 20);
}

// this function sync magstripe with gateway
void sync()
{
	boolean accepted = false;
	
	nrf24l01_irq_clear_all();
	nrf24l01_set_as_rx(true);

	clearLCD();
	printChar("Acknowledgement ...", 19);
	
	while (!(nrf24l01_irq_pin_active() && nrf24l01_irq_rx_dr_active())); // wait for ack
	
	clearLCD();
	printChar("Ack received!", 13);
	
	nrf24l01_read_rx_payload(payloadRX, TX_RX_LEN);	// read packet to "payloadRX"
	accepted = checkAcknowledment(payloadRX);

	if (accepted) { 
		char* ptr;
		clearLCD();
		printChar("Ack is valid!", 13);
		_time_delay(500);
		//EventID=2
		ptr = (char*)payloadRX + strlen("EventID=");
		activeEventNo = atoi(ptr);
		//event = atoi((char*)payloadRX[8]);
	}
	else {
		clearLCD();
		printChar("Ack is not valid!", 17);
	}
	
	nrf24l01_irq_clear_all();

	SYNC_MODE = false; // disable sync_mode
	
	if (accepted)
		COMM_MODE = true; // enable comm_mode
	else 
	{
		clearLCD();
		printChar("Sync failed", 11);				
	}
	
}

// this function communicate with gateway, download roster
void comm()
{
	// variables
	//int i;
	char buff[5];
	int counter = 0;
	
	clearLCD();
	printChar("Downloading...", 14);
	// Start downloading roster
	while(1)
	{
		while (!(nrf24l01_irq_pin_active() && nrf24l01_irq_rx_dr_active()));
	
		nrf24l01_read_rx_payload(payloadRX, TX_RX_LEN);
		
		if (strcmp((char*)payloadRX, "DONE") == 0)
		{
			break;	
		}
		strcpy((char *)roster[counter], (char *)payloadRX);
		roster[counter][32] = '\0';		// 32 or 33 !?
		counter++; 
		nrf24l01_irq_clear_all();
	}
	sprintf(buff,"%d",counter);
	clearLCD();
	printChar("Roster size: ", 15); printChar(buff,3);
	
	roster_size = counter;
	COMM_MODE = false; 		// disable comm_mode
	SCAN_MODE = true;		// enable scan mode
	
}

// this function wait for a swipe and send to gateway
void scan() 
{

//	unsigned char checksum[16];
	char input;
	// Scanner's variables
	extern char id[9];
	extern char expiration[5];
	int ii;
		char res;
	
	unsigned char result[8];
	//int s,r;
	//char input;
	int s2;
	int r;
	
	if(flag3==false)
	{	keyInit();
		flag3=true;
	}


	// ask user which mode to use
	clearLCD();
	printChar("1.Keypad ",9);
	printChar("2.Swipe ",8);
//	printChar("3.Door ",7);
	_time_delay(1111);
	
	res = 'x';
	while(res=='x')
	{
		res = ScanKeypad();
	}

	

	if (res =='1')
	{
		
			int i = 0;
			flag=TRUE;
		
			clearLCD();
			printChar("Type Now:",9);
				
				for( i = 0; i<8;)
				{
					res = 'x';
					while(res=='x')
					{
						res = ScanKeypad();
					
					}
					
					if (res !='x')
					{
						i++;
						id[i]=res;
					
		   			print1Char(id[i]);
					}
		   		}
		   	printChar("break",5);
			id[8]='\0';
			strcpy(id_with_salt,id);
			strcat(id_with_salt,salt);
			md5(id_with_salt,studentHex);
			//protect result for subsequent if statements
			res = 'x';		
				if(flag2==FALSE)
		{
			
		
			sync();
			comm();
			flag2=TRUE;
		}
			
	}
			
		if (res == '2')
		{
			
			flag = TRUE;
	
		// wait for a swipe
			clearLCD();
			printChar("Swipe now:",9);
			_time_delay(1000);
			clearLCD();
			printChar("ID: ",4);


		// Swipe 
			mag_read();
			for (ii=0;ii<8;ii++)
			{
		   		print1Char(id[ii]);
			}
			strcpy(id_with_salt,id);
			strcat(id_with_salt,salt);
		   
			// encrypt in MD5 format 
			//protect result for subsequent if statements
			md5(id_with_salt,studentHex);
			res ='x';
			if(flag2==FALSE)
		{
			
		
			sync();
			comm();
			flag2=TRUE;
		}
		}
		
		if (res == '3')
		{
			 //char* test = id;
		flag = FALSE;
	
		
			clearLCD();
			printChar("Swipe now:",9);
			_time_delay(1000);
			clearLCD();
			printChar("ID: ",4);


		mag_read();
			for (ii=0;ii<8;ii++)
			{
		   		print1Char(id[ii]);
			}
			/*
			id[0]='6';
			id[1]='4';
			id[2]='8';
			id[3]='5';
			id[4]='6';
			id[5]='7';
			id[6]='8';
			id[7]='7';
			id[8]='\n';
			*/
		//	strcpy(id_with_salt,id);
		//	strcat(id_with_salt,salt);
		   
			// encrypt in MD5 format 
			//protect result for subsequent if statements
		//	md5(id_with_salt,studentHex);
		
		//doorMan(id);
			unlockDoor();
		
		
		
		//	checkDoor( id, roster,  roster_size);
		
			
		}
	
		if(res=='4')
		{
		;
			flag = FALSE;
			clearLCD();
			printChar("add id",6);
				_time_delay(2000);
			id[0]='5';
			id[1]='9';
			id[2]='3';
			id[3]='8';
			id[4]='4';
			id[5]='2';
			id[6]='2';
			id[7]='1';
			id[8]='\0';
			
					//	id = "59384221";
			strcpy(id_with_salt,id);
			strcat(id_with_salt,salt);
			md5(id_with_salt,studentHex);
			printChar("hash",4);
				_time_delay(2000);
			appendFile(studentHex);
		//	id = "64856787";
			id[0]='6';
			id[1]='4';
			id[2]='8';
			id[3]='5';
			id[4]='6';
			id[5]='7';
			id[6]='8';
			id[7]='7';
			id[8]='\0';
			strcpy(id_with_salt,id);
			strcat(id_with_salt,salt);
			md5(id_with_salt,studentHex);
			appendFile(studentHex);
			clearLCD();
			printChar("done USB",8);
				_time_delay(2000);
				
		}
	/*	else
		{
			
		
		// wait for a swipe
			clearLCD();
			printChar("else",4);
			printChar("Swipe now:",9);
			_time_delay(1000);
			clearLCD();
			printChar("ID: ",4);


		// Swipe 
			mag_read();
			for (ii=0;ii<8;ii++)
			{
		   		print1Char(id[ii]);
			}
		
				strcat(id_with_salt,salt);
		   
			// encrypt in MD5 format 
			//protect result for subsequent if statements
			md5(id_with_salt,studentHex);
		
		}
	
*/
	
if(flag)
{
	printChar("roster check",12);
	_time_delay(1000);

	
	if ( checkRoster(studentHex, roster, roster_size) )
	{
		

		//_time_get(&time_mqx1); // get timestamp
		s2 = time_mqx1.SECONDS;

	//	strcpy(id_with_salt,id);
	//	strcat(id_with_salt,salt);
		   
		// encrypt in MD5 format 
	//	md5(id_with_salt,studentHex);
		  
		// make Payload
		
		// need to assign correct deviceID, eventNo and timeStamp
		
		strPayload.deviceID = 1;

		strPayload.eventNo = activeEventNo;
		

		strPayload.timeStamp = 0;
		
		// 
		strcpy(strPayload.studentID,(char*)studentHex);

		strcpy(strPayload.expireCode,expiration);

		//
		clearLCD();
		printChar("Wait...", 7);
		//while ( ! transmit(&strPayload) );
		transmit(&strPayload);
		
		// receive feedback
		nrf24l01_irq_clear_all();
		nrf24l01_set_as_rx(true);
		while (!(nrf24l01_irq_pin_active() && nrf24l01_irq_rx_dr_active()));
		nrf24l01_read_rx_payload(payloadRX, TX_RX_LEN);
		
		strcpy((char*)result,(char *) payloadRX); // should receive and print out "Received"
		
		clearLCD();
		printChar((char*)result, 10);
		_time_delay(1000);
		
		clearLCD();
		printChar((char*)(strPayload.expireCode),4);
		_time_delay(1000);
	
	
	}
	else
	{
		clearLCD();
		printChar("ID is not valid", 15);
		_time_delay(1000);
	}
}

}
// keypad function
/*
char keypad()
{
		
	char input;
	keyInit();
	
	clearLCD();
	while(input!='0'||input!='1'||input!='2'||input!='3'||
	input!='4'||input!='5'||input!='6'||input!='7'||input!='8'||input!='9')
	{
		
		input = ScanKeypad();
		print1Char(input);
	
	}
	return input;
}
*/

// transmit function
boolean transmit(Payload * pl)
{
	nrf24l01_irq_clear_all();
	_time_delay(50);
   	nrf24l01_set_as_tx();
   	nrf24l01_write_tx_payload((unsigned char*)(pl), TX_RX_LEN, true);
   	while(!(nrf24l01_irq_pin_active() && (nrf24l01_irq_tx_ds_active()) || nrf24l01_irq_max_rt_active()));
	return true;
}


int checkRoster(unsigned char* id, uchar roster[300][33], int size)
{
	int i;
	char asciiID[33];
	md5toAscii(id,asciiID);
	if(size == 0) //no roster, open event
	{
		return 1;
	}
	
	
	for (i=0;i<size;i++)
	{
		char asciiRosterID[33];
		md5toAscii(roster[i],asciiRosterID);
		if (strcmp(asciiID, asciiRosterID) == 0)
			return 1;
	}
	return 0;
}
