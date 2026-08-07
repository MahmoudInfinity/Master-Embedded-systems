/*
 * stm32f103x8_SPI_driver.c
 *
 *  Created on: Aug 2, 2026
 *      Author: Mahmoud Elmowafy
 */


#include "stm32f103x8_SPI_driver.h"



/*
 * =======================================================================================
 * 							Generic Variables
 * =======================================================================================
 */


SPI_Config * Global_SPI_Config[2] ={NULL , NULL};



/*
 * =======================================================================================
 * 							Generic Macros
 * =======================================================================================
 */


#define SPI1_Index 	0
#define SPI2_Index 	1

#define SPI_RXNE ((uint8_t)0x01)
#define SPI_TXE  ((uint8_t)0x02)







/*
 * =======================================================================================
 * 							Generic Functions
 * =======================================================================================
 */



/**================================================================

 * @Fn				- MCAL_SPI_Init
 * @brief 			- Initializes the SPI peripheral according to the specified parameters
 * @param [in] 		- SPIx: where x can be (1..2 depending on device used)
 * @param [in] 		- SPI_Config: pointer to SPI configuration structure
 * @retval 			- none
 * Note				- Configure SPI_CR1 and SPI_CR2 registers (Mode, BaudRate, CPOL, CPHA, NSS, IRQ...)
 *================================================================*/

void MCAL_SPI_Init(SPI_TypeDef * SPIx , SPI_Config  * SPI_Config)
{
	// ==============================
	// 0. Safety: Disable SPI first
	// ==============================
	SPIx->CR1 &= ~(1 << 6);   // CR1[SPE] = 0

	// register safety using temp vars
	uint16_t Temp_CR1 = 0;
	uint16_t Temp_CR2 = 0;

	// ==============================
	// 1. Enable Clock + NVIC
	// ==============================
	if (SPIx == SPI1)
	{
		RCC_SPI1_CLK_EN();
		Global_SPI_Config[SPI1_Index] = SPI_Config;

		if (SPI_Config->IRQ_Enable != SPI_IRQ_NONE)
		{
			NVIC_IRQ35_SPI1_Enable;
		}
	}
	else if (SPIx == SPI2)
	{
		RCC_SPI2_CLK_EN();
		Global_SPI_Config[SPI2_Index] = SPI_Config;

		if (SPI_Config->IRQ_Enable != SPI_IRQ_NONE)
		{
			NVIC_IRQ36_SPI2_Enable;
		}
	}

	// ==============================
	// 2. Build CR1 (core config)
	// ==============================
	Temp_CR1 |= SPI_Config->Device_mode;
	Temp_CR1 |= SPI_Config->Communication_mode;
	Temp_CR1 |= SPI_Config->Frame_format;
	Temp_CR1 |= SPI_Config->Data_Size;
	Temp_CR1 |= SPI_Config->ClckPolarity;
	Temp_CR1 |= SPI_Config->ClckPhase;
	Temp_CR1 |= SPI_Config->SPI_BAUDRATEPRESCALER;

	// ==============================
	// 3. NSS handling (CR1 + CR2 split)
	// ==============================

	if (SPI_Config->NSS == SPI_NSS_SOFT_SET)
	{
		// Software NSS → SSM=1, SSI=1
		Temp_CR1 |= (1 << 9); // SSM
		Temp_CR1 |= (1 << 8); // SSI
	}
	else if (SPI_Config->NSS == SPI_NSS_SOFT_RESET)
	{
		// Software NSS → SSM=1, SSI=0
		Temp_CR1 |= (1 << 9); // SSM
	}
	else if (SPI_Config->NSS == SPI_NSS_HARD_MASTER)
	{
		// Hardware NSS (Master)
		// SSM = 0 (default)
		Temp_CR2 |= (1 << 2); // CR2[SSOE]
	}
	else
	{
		// Hardware NSS Slave → nothing needed
		// SSM = 0 by default
	}

	// ==============================
	// 4. Build CR2 (IRQ)
	// ==============================
	Temp_CR2 |= SPI_Config->IRQ_Enable;

	// ==============================
	// 5. Write to registers
	// ==============================
	SPIx->CR1 = Temp_CR1;
	SPIx->CR2 = Temp_CR2;

	// ==============================
	// 6. Enable SPI
	// ==============================
	SPIx->CR1 |= (1 << 6);   // CR1[SPE] = 1
}



/**================================================================

 * @Fn				- MCAL_SPI_DeInit
 * @brief 			- Resets the SPI peripheral registers
 * @param [in] 		- SPIx: where x can be (1..2 depending on device used)
 * @retval 			- none
 * Note				- Disable SPI and reset it using RCC peripheral reset register
 *================================================================*/

void MCAL_SPI_DeInit(SPI_TypeDef * SPIx)
{
	if (SPIx == SPI1)
	{
		// 1. Reset SPI1 peripheral
		RCC_SPI1_Reset();

		// 2. Disable NVIC interrupt
		NVIC_IRQ35_SPI1_Disable;

		// 3. Clear stored config
		Global_SPI_Config[SPI1_Index] = NULL;
	}
	else if (SPIx == SPI2)
	{
		// 1. Reset SPI2 peripheral
		RCC_SPI2_Reset();

		// 2. Disable NVIC interrupt
		NVIC_IRQ36_SPI2_Disable;

		// 3. Clear stored config
		Global_SPI_Config[SPI2_Index] = NULL;
	}
}



/**================================================================

 * @Fn				- MCAL_SPI_GPIO_Set_Pins
 * @brief 			- Initializes GPIO pins for SPI (SCK, MISO, MOSI, NSS)
 * @param [in] 		- SPIx: where x can be (1..2 depending on device used)
 * @retval 			- none
 * Note				- Should enable corresponding GPIO & AF clocks in RCC
 * ```
   			  Configure pins based on device mode (Master/Slave)
  ```

 *================================================================*/

void MCAL_SPI_GPIO_Set_Pins(SPI_TypeDef * SPIx)
{
	GPIO_PinConfig_t PinCfg;

	// ==============================
	// SPI1 Configuration
	// ==============================
	if (SPIx == SPI1)
	{
		// Enable GPIOA clock (if not already enabled)
		RCC_GPIOA_CLK_EN();

		// Check Master or Slave
		if (Global_SPI_Config[SPI1_Index]->Device_mode == SPI_DEVICE_MODE_MASTER)
		{
			// SCK -> PA5 (AF Push-Pull)
			PinCfg.GPIO_PinNumber   = GPIO_PIN_5;
			PinCfg.GPIO_MODE        = GPIO_MODE_OUTPUT_AF_PP;
			PinCfg.GPIO_Output_Speed= GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOA, &PinCfg);

			// MOSI -> PA7 (AF Push-Pull)
			PinCfg.GPIO_PinNumber   = GPIO_PIN_7;
			MCAL_GPIO_Init(GPIOA, &PinCfg);

			// MISO -> PA6 (Input Floating)
			PinCfg.GPIO_PinNumber   = GPIO_PIN_6;
			PinCfg.GPIO_MODE        = GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA, &PinCfg);

			// NSS
			if (Global_SPI_Config[SPI1_Index]->NSS == SPI_NSS_HARD_MASTER)
			{
				// NSS -> PA4 (AF Push-Pull)
				PinCfg.GPIO_PinNumber   = GPIO_PIN_4;
				PinCfg.GPIO_MODE        = GPIO_MODE_OUTPUT_AF_PP;
				MCAL_GPIO_Init(GPIOA, &PinCfg);
			}
		}
		else // Slave
		{
			// SCK -> Input Floating
			PinCfg.GPIO_PinNumber   = GPIO_PIN_5;
			PinCfg.GPIO_MODE        = GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA, &PinCfg);

			// MOSI -> Input Floating
			PinCfg.GPIO_PinNumber   = GPIO_PIN_7;
			PinCfg.GPIO_MODE        = GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA, &PinCfg);

			// MISO -> AF Push-Pull
			PinCfg.GPIO_PinNumber   = GPIO_PIN_6;
			PinCfg.GPIO_MODE        = GPIO_MODE_OUTPUT_AF_PP;
			PinCfg.GPIO_Output_Speed= GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOA, &PinCfg);

			// NSS -> Input Floating
			PinCfg.GPIO_PinNumber   = GPIO_PIN_4;
			PinCfg.GPIO_MODE        = GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA, &PinCfg);
		}
	}

	// ==============================
	// SPI2 Configuration
	// ==============================
	else if (SPIx == SPI2)
	{
		// Enable GPIOB clock
		RCC_GPIOB_CLK_EN();

		if (Global_SPI_Config[SPI2_Index]->Device_mode == SPI_DEVICE_MODE_MASTER)
		{
			// SCK -> PB13
			PinCfg.GPIO_PinNumber   = GPIO_PIN_13;
			PinCfg.GPIO_MODE        = GPIO_MODE_OUTPUT_AF_PP;
			PinCfg.GPIO_Output_Speed= GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOB, &PinCfg);

			// MOSI -> PB15
			PinCfg.GPIO_PinNumber   = GPIO_PIN_15;
			MCAL_GPIO_Init(GPIOB, &PinCfg);

			// MISO -> PB14
			PinCfg.GPIO_PinNumber   = GPIO_PIN_14;
			PinCfg.GPIO_MODE        = GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOB, &PinCfg);

			// NSS
			if (Global_SPI_Config[SPI2_Index]->NSS == SPI_NSS_HARD_MASTER)
			{
				PinCfg.GPIO_PinNumber   = GPIO_PIN_12;
				PinCfg.GPIO_MODE        = GPIO_MODE_OUTPUT_AF_PP;
				MCAL_GPIO_Init(GPIOB, &PinCfg);
			}
		}
		else // Slave
		{
			// SCK
			PinCfg.GPIO_PinNumber   = GPIO_PIN_13;
			PinCfg.GPIO_MODE        = GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOB, &PinCfg);

			// MOSI
			PinCfg.GPIO_PinNumber   = GPIO_PIN_15;
			PinCfg.GPIO_MODE        = GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOB, &PinCfg);

			// MISO
			PinCfg.GPIO_PinNumber   = GPIO_PIN_14;
			PinCfg.GPIO_MODE        = GPIO_MODE_OUTPUT_AF_PP;
			PinCfg.GPIO_Output_Speed= GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOB, &PinCfg);

			// NSS
			PinCfg.GPIO_PinNumber   = GPIO_PIN_12;
			PinCfg.GPIO_MODE        = GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOB, &PinCfg);
		}
	}
}


/**================================================================

 * @Fn				- MCAL_SPI_Send_Data
 * @brief 			- Sends data through SPI
 * @param [in] 		- SPIx: where x can be (1..2 depending on device used)
 * @param [in] 		- pTxBuffer: pointer to data buffer to be transmitted
 * @param [in] 		- PollingEnable: enable or disable polling mechanism
 * @retval 			- none
 * Note				- Polling on TXE flag if polling is enabled
 *================================================================*/



void MCAL_SPI_Send_Data(SPI_TypeDef * SPIx , uint16_t * pTxBuffer , enum Polling_mechism PollingEnable)
{
	if (PollingEnable == Enable)
	{
		// ==============================
		// Polling mode (same as before)
		// ==============================
		while (!(SPIx->SR & SPI_TXE));

		if (SPIx->CR1 & (1 << 11)) // 16-bit
		{
			SPIx->DR = *pTxBuffer;
		}
		else // 8-bit
		{
			*((volatile uint8_t*)&SPIx->DR) = *((uint8_t*)pTxBuffer);
		}
	}



}


/**================================================================

 * @Fn				- MCAL_SPI_Receive_Data
 * @brief 			- Receives data through SPI
 * @param [in] 		- SPIx: where x can be (1..2 depending on device used)
 * @param [out] 	- pRxBuffer: pointer to buffer to store received data
 * @param [in] 		- PollingEnable: enable or disable polling mechanism
 * @retval 			- none
 * Note				- Polling on RXNE flag if polling is enabled
 *================================================================*/
void MCAL_SPI_Receive_Data(SPI_TypeDef * SPIx , uint16_t * pRxBuffer , enum Polling_mechism PollingEnable)
{
	// ==============================
	// 1. Wait until RXNE = 1
	// ==============================
	if (PollingEnable == Enable)
	{
		while (!(SPIx->SR & SPI_RXNE));
	}

	// ==============================
	// 2. Read data from DR
	// ==============================

	if (SPIx->CR1 & (1 << 11))   // 16-bit mode (DFF = 1)
	{
		*pRxBuffer = SPIx->DR;
	}
	else // 8-bit mode
	{
		*((uint8_t*)pRxBuffer) = *((volatile uint8_t*)&SPIx->DR);
	}
}

/**================================================================

 * @Fn				- MCAL_SPI_Tx_Rx
 * @brief 			- Transmits and receives data simultaneously (Full-Duplex)
 * @param [in] 		- SPIx: where x can be (1..2 depending on device used)
 * @param [in/out] 	- pTxRxBuffer: pointer to buffer used for both transmission and reception
 * @param [in] 		- PollingEnable: enable or disable polling mechanism
 * @retval 			- none
 * Note				- Send data and read received data in the same transaction
 *================================================================*/


void MCAL_SPI_Tx_Rx(SPI_TypeDef * SPIx , uint16_t * pTxRxBuffer , enum Polling_mechism PollingEnable)

{}


/************************************************************************
 * IRQ
 ************************************************************************
 */
void SPI1_IRQHandler(void)        			/* SPI1 global interrupt                            */
{
	uint16_t SR  = SPI1->SR;
	uint16_t CR2 = SPI1->CR2;

	struct S_IRQ_SRC irq_src = {
			.TXE  = (SR & (1 << 1)) && (CR2 & (1 << 7)),
			.RXNE = (SR & (1 << 0)) && (CR2 & (1 << 6)),
			.ERRI = (SR & ((1 << 6)|(1 << 5)|(1 << 4))) && (CR2 & (1 << 5))
	};

	// ==============================
	// Error handling (CRITICAL)
	// ==============================
	if (irq_src.ERRI)
	{
		// OVR (Overrun)
		if (SR & (1 << 6))
		{
			volatile uint16_t temp;
			temp = SPI1->DR;
			temp = SPI1->SR;
			(void)temp;
		}

		// MODF (Mode fault)
		if (SR & (1 << 5))
		{
			volatile uint16_t temp;
			temp = SPI1->SR;
			SPI1->CR1 |= (1 << 6); // Re-enable SPI
			(void)temp;
		}

		// CRCERR
		if (SR & (1 << 4))
		{
			SPI1->SR &= ~(1 << 4);
		}
	}

	// ==============================
	// Safe callback
	// ==============================
	if (Global_SPI_Config[SPI1_Index] &&
			Global_SPI_Config[SPI1_Index]->p_IRQ_callback)
	{
		Global_SPI_Config[SPI1_Index]->p_IRQ_callback(irq_src);
	}
}




void SPI2_IRQHandler(void)           			/* SPI2 global interrupt  */
{

	uint16_t SR  = SPI2->SR;
	uint16_t CR2 = SPI2->CR2;

	struct S_IRQ_SRC irq_src = {
			.TXE  = (SR & (1 << 1)) && (CR2 & (1 << 7)),
			.RXNE = (SR & (1 << 0)) && (CR2 & (1 << 6)),
			.ERRI = (SR & ((1 << 6)|(1 << 5)|(1 << 4))) && (CR2 & (1 << 5))
	};

	// ==============================
			// Error handling (CRITICAL)
	// ==============================
	if (irq_src.ERRI)
	{
		// OVR (Overrun)
		if (SR & (1 << 6))
		{
			volatile uint16_t temp;
			temp = SPI2->DR;
			temp = SPI2->SR;
			(void)temp;
		}

		// MODF (Mode fault)
		if (SR & (1 << 5))
		{
			volatile uint16_t temp;
			temp = SPI1->SR;
			SPI2->CR1 |= (1 << 6); // Re-enable SPI
			(void)temp;
		}

		// CRCERR
		if (SR & (1 << 4))
		{
			SPI2->SR &= ~(1 << 4);
		}
	}

	// ==============================
	// Safe callback
	// ==============================
	if (Global_SPI_Config[SPI2_Index] &&
			Global_SPI_Config[SPI2_Index]->p_IRQ_callback)
	{
		Global_SPI_Config[SPI2_Index]->p_IRQ_callback(irq_src);
	}

}


