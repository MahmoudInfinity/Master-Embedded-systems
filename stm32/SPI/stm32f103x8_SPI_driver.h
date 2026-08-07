/*
 * stm32f103x8_SPI_driver.h
 *
 *  Created on: Aug 2, 2026
 *      Author: Mahmoud Elmowafy
 */

#ifndef INC_STM32F103X8_SPI_DRIVER_H_
#define INC_STM32F103X8_SPI_DRIVER_H_

//includes

#include "stm32f103x8.h"
#include "stm32f103x8_gpio_driver.h"
// polling enum
enum Polling_mechism{

	Enable,

	Disable

};



// SPI interrupt request source
struct S_IRQ_SRC
{
	uint16_t TXE:1;  //  transmitter buffer is empty
	uint16_t RXNE:1;  //receiver buffer is not empty
	uint16_t ERRI:1;  // error occured
	uint16_t reserved:5;  // reserved

};






//configuration

typedef struct
{
	uint16_t Device_mode;           // specifies the mode of the device, master or slave @ref SPI_Device_Mode

	uint16_t Communication_mode;   // specifies the SPI bidirectional @ref SPI_communication_Mode

	uint16_t Frame_format;        // specifies the LSB or MSB @ref SPI_Frame_Format

	uint16_t  Data_Size;         // @ref SPI_Data_Size

	uint16_t ClckPolarity;     	// @ref SPI_Clck_Polarity

	uint16_t ClckPhase;  	  // @ref SPI_Clck_phase

	uint16_t NSS;        	// slave select (managed by software or hardware) @ref SPI_NSS


	uint16_t SPI_BAUDRATEPRESCALER;    //  @ref SPI_BAUDRATEPRESCALER

	uint16_t IRQ_Enable;    //@ref SPI_IRQ_Enable


	void (* p_IRQ_callback)(struct S_IRQ_SRC IRQ_SRC)  //  An address of c function to be set

}SPI_Config;


//**********************************************************************************************
//REference MACROS
//**********************************************************************************************


// ===============================
// @ref SPI_Device_Mode
// ===============================
// SPI_CR1 register:
// Bit 2 (MSTR):
// 0 → Slave mode
// 1 → Master mode

#define SPI_DEVICE_MODE_MASTER    (1 << 2)   // CR1[MSTR] = 1 → Master
#define SPI_DEVICE_MODE_SLAVE     (0)        // CR1[MSTR] = 0 → Slave



// ===============================
// @ref SPI_communication_Mode
// ===============================
// SPI_CR1 register:
// Bit 15 (BIDIMODE): 1 → 1-line bidirectional, 0 → 2-line
// Bit 14 (BIDIOE):   1 → Output enabled (Tx), 0 → Input (Rx)
// Bit 10 (RXONLY):   1 → Receive only mode (2-line)

#define SPI_DIRECTION_2LINES      (0)                         // Full duplex (BIDIMODE=0, RXONLY=0)
#define SPI_DIRECTION_2LINES_RX   (1 << 10)                   // CR1[RXONLY] = 1 → 2-line receive only
#define SPI_DIRECTION_1LINE_RX    (1 << 15)                   // CR1[BIDIMODE]=1, BIDIOE=0 → 1-line Rx
#define SPI_DIRECTION_1LINE_TX    ((1 << 15) | (1 << 14))     // CR1[BIDIMODE]=1, BIDIOE=1 → 1-line Tx



// ===============================
// @ref SPI_Frame_Format
// ===============================
// SPI_CR1 register:
// Bit 7 (LSBFIRST):
// 0 → MSB transmitted first
// 1 → LSB transmitted first

#define SPI_FRAME_FORMAT_MSB_FIRST     (0)         // CR1[LSBFIRST] = 0
#define SPI_FRAME_FORMAT_LSB_FIRST     (1 << 7)    // CR1[LSBFIRST] = 1



// ===============================
// @ref SPI_Data_Size
// ===============================
// SPI_CR1 register:
// Bit 11 (DFF):
// 0 → 8-bit data frame
// 1 → 16-bit data frame

#define SPI_DATASIZE_8BIT      (0)         // CR1[DFF] = 0 → 8-bit
#define SPI_DATASIZE_16BIT     (1 << 11)   // CR1[DFF] = 1 → 16-bit



// ===============================
// @ref SPI_IRQ_Enable
// ===============================
// SPI_CR2 register:
// Bit 7 (TXEIE):  Interrupt enable for TXE
// Bit 6 (RXNEIE): Interrupt enable for RXNE
// Bit 5 (ERRIE):  Interrupt enable for errors (OVR, MODF, CRCERR)

#define SPI_IRQ_NONE          0
#define SPI_IRQ_TXE           (1 << 7)   // CR2[TXEIE]
#define SPI_IRQ_RXNE          (1 << 6)   // CR2[RXNEIE]
#define SPI_IRQ_ERR           (1 << 5)   // CR2[ERRIE]


// ===============================
// @ref SPI_CLK_Polarity
// ===============================
// SPI_CR1 register:
// Bit 1 (CPOL):
// 0 → Clock idle LOW
// 1 → Clock idle HIGH

#define SPI_CLK_POLARITY_LOW     (0)        // CR1[CPOL] = 0 → Idle = 0
#define SPI_CLK_POLARITY_HIGH    (1 << 1)   // CR1[CPOL] = 1 → Idle = 1



// ===============================
// @ref SPI_CLK_Phase
// ===============================
// SPI_CR1 register:
// Bit 0 (CPHA):
// 0 → Data captured on first clock edge
// 1 → Data captured on second clock edge

#define SPI_CLK_PHASE_1EDGE      (0)        // CR1[CPHA] = 0 → First edge
#define SPI_CLK_PHASE_2EDGE      (1 << 0)   // CR1[CPHA] = 1 → Second edge


// ===============================
// @ref SPI_NSS
// ===============================
// SPI_CR1 register:
// Bit 9 (SSM): 1 → Software management, 0 → Hardware NSS
// Bit 8 (SSI): Used when SSM = 1 to set internal NSS level
//
// SPI_CR2 register:
// Bit 2 (SSOE): 1 → NSS output enabled in master mode

// -------- Software NSS --------
#define SPI_NSS_SOFT_SET      ((1 << 9) | (1 << 8))   // CR1[SSM]=1, CR1[SSI]=1 → NSS internally HIGH
#define SPI_NSS_SOFT_RESET    (1 << 9)                // CR1[SSM]=1, CR1[SSI]=0 → NSS internally LOW

// -------- Hardware NSS --------
#define SPI_NSS_HARD_MASTER   (1 << 2)                // CR2[SSOE]=1 → NSS driven by hardware (master)
#define SPI_NSS_HARD_SLAVE    (0)                     // CR1[SSM]=0 → NSS controlled by external pin


// ===============================
// @ref SPI_BAUDRATEPRESCALER
// ===============================
// SPI_CR1 register:
// Bits 5:3 (BR[2:0]):
// 000 → fPCLK / 2
// 001 → fPCLK / 4
// 010 → fPCLK / 8
// 011 → fPCLK / 16
// 100 → fPCLK / 32
// 101 → fPCLK / 64
// 110 → fPCLK / 128
// 111 → fPCLK / 256

#define SPI_BAUDRATEPRESCALER_2     (0 << 3)   // CR1[BR]=000 → fPCLK/2
#define SPI_BAUDRATEPRESCALER_4     (1 << 3)   // CR1[BR]=001 → fPCLK/4
#define SPI_BAUDRATEPRESCALER_8     (2 << 3)   // CR1[BR]=010 → fPCLK/8
#define SPI_BAUDRATEPRESCALER_16    (3 << 3)   // CR1[BR]=011 → fPCLK/16
#define SPI_BAUDRATEPRESCALER_32    (4 << 3)   // CR1[BR]=100 → fPCLK/32
#define SPI_BAUDRATEPRESCALER_64    (5 << 3)   // CR1[BR]=101 → fPCLK/64
#define SPI_BAUDRATEPRESCALER_128   (6 << 3)   // CR1[BR]=110 → fPCLK/128
#define SPI_BAUDRATEPRESCALER_256   (7 << 3)   // CR1[BR]=111 → fPCLK/256


//********************************************************************

// APIS supported with "MCAL SPI Driver"

//********************************************************************



void MCAL_SPI_Init(SPI_TypeDef * SPIx , SPI_Config  * SPI_Config);

void MCAL_SPI_DeInit(SPI_TypeDef * SPIx );

void MCAL_SPI_GPIO_Set_Pins(SPI_TypeDef * SPIx );

void MCAL_SPI_Send_Data(SPI_TypeDef * SPIx , uint16_t * pTxBuffer , enum Polling_mechism PollingEnable);

void MCAL_SPI_Receive_Data(SPI_TypeDef * SPIx , uint16_t * pRxBuffer , enum Polling_mechism PollingEnable);

void MCAL_SPI_Tx_Rx(SPI_TypeDef * SPIx , uint16_t * pTxRxBuffer , enum Polling_mechism PollingEnable);





#endif /* INC_STM32F103X8_SPI_DRIVER_H_ */
