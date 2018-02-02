/* 	
		Senior Design Project, University of California, 2010
		Project: Wireless-Embedded Card Reader
		MCU: M52259DEMO KIT
*/

#ifndef __my_spi_h__
#define __my_spi_h__

#include <mqx.h>
#include <bsp.h>
#include <string.h>
#include <spi.h>

// add spi functions

//FILE_PTR    spifd;
extern uchar spi0_send_read_byte(uchar byte);



typedef volatile uint_8      vuint8;  //  8 bits 
typedef volatile uint_16    vuint16;


// Register read/write macros 
#define MCF_QSPI_QMR                         (*(vuint16*)(0x40000340))
#define MCF_QSPI_QDLYR                       (*(vuint16*)(0x40000344))
#define MCF_QSPI_QWR                         (*(vuint16*)(0x40000348))
#define MCF_QSPI_QIR                         (*(vuint16*)(0x4000034C))
#define MCF_QSPI_QAR                         (*(vuint16*)(0x40000350))
#define MCF_QSPI_QDR                         (*(vuint16*)(0x40000354))


// Bit definitions and macros for MCF_QSPI_QMR 
#define MCF_QSPI_QMR_BAUD(x)                 (((x)&0xFF)<<0)
#define MCF_QSPI_QMR_CPHA                    (0x100)
#define MCF_QSPI_QMR_CPOL                    (0x200)
#define MCF_QSPI_QMR_BITS(x)                 (((x)&0xF)<<0xA)
#define MCF_QSPI_QMR_DOHIE                   (0x4000)
#define MCF_QSPI_QMR_MSTR                    (0x8000)


// General Purpose I/O (GPIO)
// Register read/write macros
#define MCF_GPIO_PORTQS                      (*(vuint8 *)(0x4010000C))
#define MCF_GPIO_DDRQS                       (*(vuint8 *)(0x40100024))
#define MCF_GPIO_SETQS                       (*(vuint8 *)(0x4010003C))
#define MCF_GPIO_CLRQS                       (*(vuint8 *)(0x40100054))
#define MCF_GPIO_PQSPAR                      (*(vuint16*)(0x4010006C))

#define MCF_GPIO_PORTAN                      (*(vuint8 *)(0x4010000A))
#define MCF_GPIO_DDRAN                       (*(vuint8 *)(0x40100022))
#define MCF_GPIO_SETAN                       (*(vuint8 *)(0x4010003A))
#define MCF_GPIO_CLRAN                       (*(vuint8 *)(0x40100052))
#define MCF_GPIO_PANPAR                      (*(vuint8 *)(0x4010006A))

#define MCF_GPIO_SETNQ                       (*(vuint8 *)(0x40100038))

// Bit definitions and macros for MCF_GPIO_PORTQS 
#define MCF_GPIO_PORTQS_PORTQS0              (0x1)
#define MCF_GPIO_PORTQS_PORTQS1              (0x2)
#define MCF_GPIO_PORTQS_PORTQS2              (0x4)
#define MCF_GPIO_PORTQS_PORTQS3              (0x8)
#define MCF_GPIO_PORTQS_PORTQS4              (0x10)
#define MCF_GPIO_PORTQS_PORTQS5              (0x20)
#define MCF_GPIO_PORTQS_PORTQS6              (0x40)

// Bit definitions and macros for MCF_GPIO_PORTAN 
#define MCF_GPIO_PORTAN_PORTAN0              (0x1)
#define MCF_GPIO_PORTAN_PORTAN1              (0x2)
#define MCF_GPIO_PORTAN_PORTAN2              (0x4)
#define MCF_GPIO_PORTAN_PORTAN3              (0x8)
#define MCF_GPIO_PORTAN_PORTAN4              (0x10)
#define MCF_GPIO_PORTAN_PORTAN5              (0x20)
#define MCF_GPIO_PORTAN_PORTAN6              (0x40)
#define MCF_GPIO_PORTAN_PORTAN7              (0x80)

// Bit definitions and macros for MCF_GPIO_SETNQ 
#define MCF_GPIO_SETNQ_SETNQ1                (0x2)
#define MCF_GPIO_SETNQ_SETNQ2                (0x4)
#define MCF_GPIO_SETNQ_SETNQ3                (0x8)
#define MCF_GPIO_SETNQ_SETNQ4                (0x10)
#define MCF_GPIO_SETNQ_SETNQ5                (0x20)
#define MCF_GPIO_SETNQ_SETNQ6                (0x40)
#define MCF_GPIO_SETNQ_SETNQ7                (0x80)

/*
// Bit definitions and macros for MCF_GPIO_DDRAN 
#define MCF_GPIO_DDRAN_DDRAN0                (0x1)
#define MCF_GPIO_DDRAN_DDRAN1                (0x2)
#define MCF_GPIO_DDRAN_DDRAN2                (0x4)
#define MCF_GPIO_DDRAN_DDRAN3                (0x8)
#define MCF_GPIO_DDRAN_DDRAN4                (0x10)
#define MCF_GPIO_DDRAN_DDRAN5                (0x20)
#define MCF_GPIO_DDRAN_DDRAN6                (0x40)
#define MCF_GPIO_DDRAN_DDRAN7                (0x80)

// Bit definitions and macros for MCF_GPIO_PANPAR 
#define MCF_GPIO_PANPAR_PANPAR0              (0x1)
#define MCF_GPIO_PANPAR_AN0_GPIO             (0)
#define MCF_GPIO_PANPAR_AN0_AN0              (0x1)
#define MCF_GPIO_PANPAR_PANPAR1              (0x2)
#define MCF_GPIO_PANPAR_AN1_GPIO             (0)
#define MCF_GPIO_PANPAR_AN1_AN1              (0x2)
#define MCF_GPIO_PANPAR_PANPAR2              (0x4)
#define MCF_GPIO_PANPAR_AN2_GPIO             (0)
#define MCF_GPIO_PANPAR_AN2_AN2              (0x4)
#define MCF_GPIO_PANPAR_PANPAR3              (0x8)
#define MCF_GPIO_PANPAR_AN3_GPIO             (0)
#define MCF_GPIO_PANPAR_AN3_AN3              (0x8)
#define MCF_GPIO_PANPAR_PANPAR4              (0x10)
#define MCF_GPIO_PANPAR_AN4_GPIO             (0)
#define MCF_GPIO_PANPAR_AN4_AN4              (0x10)
#define MCF_GPIO_PANPAR_PANPAR5              (0x20)
#define MCF_GPIO_PANPAR_AN5_GPIO             (0)
#define MCF_GPIO_PANPAR_AN5_AN5              (0x20)
#define MCF_GPIO_PANPAR_PANPAR6              (0x40)
#define MCF_GPIO_PANPAR_AN6_GPIO             (0)
#define MCF_GPIO_PANPAR_AN6_AN6              (0x40)
#define MCF_GPIO_PANPAR_PANPAR7              (0x80)
#define MCF_GPIO_PANPAR_AN7_GPIO             (0)
#define MCF_GPIO_PANPAR_AN7_AN7              (0x80)

// Bit definitions and macros for MCF_GPIO_PQSPAR 
#define MCF_GPIO_PQSPAR_PQSPAR0(x)           (((x)&0x3)<<0)
#define MCF_GPIO_PQSPAR_QSPI_DOUT_GPIO       (0)
#define MCF_GPIO_PQSPAR_QSPI_DOUT_DOUT       (0x1)
#define MCF_GPIO_PQSPAR_QSPI_DOUT_SCL1       (0x2)
#define MCF_GPIO_PQSPAR_QSPI_DOUT_UTXD1      (0x3)
#define MCF_GPIO_PQSPAR_PQSPAR1(x)           (((x)&0x3)<<0x2)
#define MCF_GPIO_PQSPAR_QSPI_DIN_GPIO        (0)
#define MCF_GPIO_PQSPAR_QSPI_DIN_DIN         (0x4)
#define MCF_GPIO_PQSPAR_QSPI_DIN_SDA1        (0x8)
#define MCF_GPIO_PQSPAR_QSPI_DIN_URXD1       (0xC)
#define MCF_GPIO_PQSPAR_PQSPAR2(x)           (((x)&0x3)<<0x4)
#define MCF_GPIO_PQSPAR_QSPI_CLK_GPIO        (0)
#define MCF_GPIO_PQSPAR_QSPI_CLK_CLK         (0x10)
#define MCF_GPIO_PQSPAR_QSPI_CLK_SCL0        (0x20)
#define MCF_GPIO_PQSPAR_QSPI_CLK_URTS1       (0x30)
#define MCF_GPIO_PQSPAR_PQSPAR3(x)           (((x)&0x3)<<0x6)
#define MCF_GPIO_PQSPAR_QSPI_CS0_GPIO        (0)
#define MCF_GPIO_PQSPAR_QSPI_CS0_CS0         (0x40)
#define MCF_GPIO_PQSPAR_QSPI_CS0_SDA0        (0x80)
#define MCF_GPIO_PQSPAR_QSPI_CS0_UCTS1       (0xC0)
#define MCF_GPIO_PQSPAR_PQSPAR4(x)           (((x)&0x3)<<0x8)
#define MCF_GPIO_PQSPAR_PQSPAR5(x)           (((x)&0x3)<<0xA)
#define MCF_GPIO_PQSPAR_QSPI_CS2_GPIO        (0)
#define MCF_GPIO_PQSPAR_QSPI_CS2_CS2         (0x400)
#define MCF_GPIO_PQSPAR_QSPI_CS2_SYNCB       (0x800)
#define MCF_GPIO_PQSPAR_QSPI_CS2_USB_DM_PDOWN (0xC00)
#define MCF_GPIO_PQSPAR_PQSPAR6(x)           (((x)&0x3)<<0xC)
#define MCF_GPIO_PQSPAR_QSPI_CS3_GPIO        (0)
#define MCF_GPIO_PQSPAR_QSPI_CS3_CS3         (0x1000)
#define MCF_GPIO_PQSPAR_QSPI_CS3_SYNCA       (0x2000)
#define MCF_GPIO_PQSPAR_QSPI_CS3_USB_DP_PDOWN (0x3000)
*/

#endif