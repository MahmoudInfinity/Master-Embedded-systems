/*
 * stm32f103x8_ADC_driver.c
 *
 *  Created on: Aug 7, 2026
 *      Author: Mahmoud Elmowafy
 */

// ============================================================
// INCLUDES
// ============================================================

#include "stm32f103x8_ADC_driver.h"

// ============================================================
// PRIVATE VARIABLES
// ============================================================

static ADC_Config *g_ADC_Config[3] = {NULL, NULL, NULL};  // Store config for each ADC

// ============================================================
// PRIVATE HELPER FUNCTIONS
// ============================================================

/**
 * @brief   Calculate sampling time register position for given channel
 * @param   Channel: ADC channel number (0-17)
 * @retval  uint8_t: Bit position in SMPR register
 */
static uint8_t ADC_Get_SMPRx_Bit_Position(uint8_t Channel)
{
	if (Channel < 10)
	{
		return (Channel * 3);  // SMPR2 register channels 0-9
	}
	else
	{
		return ((Channel - 10) * 3);  // SMPR1 register channels 10-17
	}
}

/**
 * @brief   Set sampling time for specified channel
 * @param   ADCx    : ADC peripheral
 * @param   Channel : Channel number
 * @param   SmpTime : Sampling time value
 * @retval  None
 */
static void ADC_Set_Sampling_Time(ADC_TypeDef *ADCx, uint8_t Channel, uint8_t SmpTime)
{
	uint8_t bit_pos = ADC_Get_SMPRx_Bit_Position(Channel);

	if (Channel < 10)
	{
		// SMPR2 for channels 0-9
		ADCx->SMPR2 &= ~(0x7 << bit_pos);
		ADCx->SMPR2 |= (SmpTime << bit_pos);
	}
	else
	{
		// SMPR1 for channels 10-17
		ADCx->SMPR1 &= ~(0x7 << bit_pos);
		ADCx->SMPR1 |= (SmpTime << bit_pos);
	}
}

/**
 * @brief   Set regular sequence configuration
 * @param   ADCx                : ADC peripheral
 * @param   Channel             : Channel number to convert
 * @param   Number_Of_Conversions: Total number of conversions
 * @retval  None
 */
static void ADC_Set_Regular_Sequence(ADC_TypeDef *ADCx, uint8_t Channel, uint8_t Number_Of_Conversions)
{
	// Set sequence length L[3:0] in SQR1
	ADCx->SQR1 &= ~(0xF << 20);
	ADCx->SQR1 |= ((Number_Of_Conversions - 1) << 20);

	// Set channel in first position (SQ1) in SQR3
	ADCx->SQR3 &= ~(0x1F << 0);  // Clear SQ1 (5 bits)
	ADCx->SQR3 |= (Channel << 0);
}

/**
 * @brief   Wait for ADC stabilization time (tSTAB)
 * @retval  None
 */
static void ADC_Wait_Stabilization(void)
{
	uint32_t i;
	for (i = 0; i < 2000; i++);  // Delay for stabilization
}

/**
 * @brief   Get ADC index from ADC peripheral pointer
 * @param   ADCx    : ADC peripheral
 * @retval  uint8_t : ADC index (0 for ADC1, 1 for ADC2, 2 for ADC3)
 */
static uint8_t ADC_Get_Index(ADC_TypeDef *ADCx)
{
	if (ADCx == ADC1)
		return 0;
	else if (ADCx == ADC2)
		return 1;
	else if (ADCx == ADC3)
		return 2;
	return 0;
}

/**
 * @brief   Configure GPIO pins for ADC inputs based on selected channel
 * @param   ADCx    : ADC peripheral
 * @param   Channel : Channel number
 * @retval  None
 */
static void ADC_Configure_GPIO_Pin(ADC_TypeDef *ADCx, uint8_t Channel)
{
	GPIO_PinConfig_t PinConfig;

	// Set pin to analog input mode
	PinConfig.GPIO_MODE = GPIO_MODE_ANALOG;

	// ADC1 and ADC2 use PA0-PA7 and PB0-PB1 for channels 0-9
	// ADC3 uses PF6-PF10 for channels 0-4

	if ((ADCx == ADC1) || (ADCx == ADC2))
	{
		switch (Channel)
		{
			case ADC_CHANNEL_0:
			case ADC_CHANNEL_1:
			case ADC_CHANNEL_2:
			case ADC_CHANNEL_3:
			case ADC_CHANNEL_4:
			case ADC_CHANNEL_5:
			case ADC_CHANNEL_6:
			case ADC_CHANNEL_7:
				// PA0-PA7
				PinConfig.GPIO_PinNumber = Channel;
				MCAL_GPIO_Init(GPIOA, &PinConfig);
				break;

			case ADC_CHANNEL_8:
			case ADC_CHANNEL_9:
				// PB0-PB1
				PinConfig.GPIO_PinNumber = (Channel - 8);
				MCAL_GPIO_Init(GPIOB, &PinConfig);
				break;

			case ADC_CHANNEL_10:
			case ADC_CHANNEL_11:
			case ADC_CHANNEL_12:
			case ADC_CHANNEL_13:
			case ADC_CHANNEL_14:
			case ADC_CHANNEL_15:
				// PC0-PC5
				PinConfig.GPIO_PinNumber = (Channel - 10);
				MCAL_GPIO_Init(GPIOC, &PinConfig);
				break;

			case ADC_CHANNEL_16:
			case ADC_CHANNEL_17:
				// Temperature sensor and Vrefint (internal channels, no GPIO)
				break;
		}
	}
}

// ============================================================
// PUBLIC API FUNCTIONS
// ============================================================

/**
 * @fn      MCAL_ADC_Init
 * @brief   Initialize ADC peripheral
 */
void MCAL_ADC_Init(ADC_TypeDef *ADCx, ADC_Config *ADC_Config)
{
	uint8_t adc_index = ADC_Get_Index(ADCx);

	// Store configuration
	g_ADC_Config[adc_index] = ADC_Config;

	// Enable ADC clock (APB2)
	if (ADCx == ADC1)
	{
		RCC->APB2ENR |= (1 << 9);  // ADC1 clock enable
	}
	else if (ADCx == ADC2)
	{
		RCC->APB2ENR |= (1 << 10);  // ADC2 clock enable
	}
	else if (ADCx == ADC3)
	{
		RCC->APB2ENR |= (1 << 11);  // ADC3 clock enable
	}

	// Configure GPIO pins
	MCAL_ADC_GPIO_Set_Pins(ADCx);

	// Disable ADC before configuration
	ADCx->CR2 &= ~(1 << 0);  // Clear ADON bit

	// Configure ADC_CR1
	ADCx->CR1 = 0;  // Reset CR1
	if (ADC_Config->IRQ_Enable != ADC_IRQ_NONE)
	{
		ADCx->CR1 |= ADC_Config->IRQ_Enable;
	}

	// Configure ADC_CR2
	ADCx->CR2 = 0;  // Reset CR2
	ADCx->CR2 |= ADC_Config->Conversion_Mode;      // Single or continuous
	ADCx->CR2 |= ADC_Config->Data_Alignment;       // Data alignment
	ADCx->CR2 |= ADC_Config->External_Trigger_Conv; // External trigger
	ADCx->CR2 |= ADC_Config->DMA_Enable;           // DMA enable/disable

	// Set sampling time
	ADC_Set_Sampling_Time(ADCx, ADC_Config->Channel, ADC_Config->Sampling_Time);

	// Set regular sequence
	ADC_Set_Regular_Sequence(ADCx, ADC_Config->Channel, ADC_Config->Number_Of_Conversions);

	// Power-up ADC
	// First ADON write - wake up from power down
	ADCx->CR2 |= (1 << 0);  // Set ADON bit

	// Wait for stabilization (tSTAB)
	ADC_Wait_Stabilization();

	// Second ADON write - start conversion (if not using external trigger)
	if (ADC_Config->Conversion_Mode == ADC_MODE_SINGLE)
	{
		// For single mode, don't start yet, let user start via MCAL_ADC_Start_Conversion
		ADCx->CR2 &= ~(1 << 0);  // Clear ADON
		ADCx->CR2 |= (1 << 0);   // Set ADON again to complete startup
	}
}

/**
 * @fn      MCAL_ADC_DeInit
 * @brief   Reset ADC to default state
 */
void MCAL_ADC_DeInit(ADC_TypeDef *ADCx)
{
	// Disable ADC
	ADCx->CR2 &= ~(1 << 0);  // Clear ADON

	// Reset all registers
	ADCx->CR1 = 0x0;
	ADCx->CR2 = 0x0;
	ADCx->SMPR1 = 0x0;
	ADCx->SMPR2 = 0x0;
	ADCx->SQR1 = 0x0;
	ADCx->SQR2 = 0x0;
	ADCx->SQR3 = 0x0;

	uint8_t adc_index = ADC_Get_Index(ADCx);
	g_ADC_Config[adc_index] = NULL;
}

/**
 * @fn      MCAL_ADC_GPIO_Set_Pins
 * @brief   Configure GPIO pins for ADC
 */
void MCAL_ADC_GPIO_Set_Pins(ADC_TypeDef *ADCx)
{
	uint8_t adc_index = ADC_Get_Index(ADCx);

	if (g_ADC_Config[adc_index] != NULL)
	{
		ADC_Configure_GPIO_Pin(ADCx, g_ADC_Config[adc_index]->Channel);
	}
}

/**
 * @fn      MCAL_ADC_Start_Conversion
 * @brief   Start ADC conversion
 */
void MCAL_ADC_Start_Conversion(ADC_TypeDef *ADCx)
{
	// Set ADON bit to start conversion in single mode
	ADCx->CR2 |= (1 << 0);
}

/**
 * @fn      MCAL_ADC_Read_Value
 * @brief   Read ADC value with optional polling
 */
uint16_t MCAL_ADC_Read_Value(ADC_TypeDef *ADCx, ADC_Polling_Mechanism Polling)
{
	if (Polling == Enable)
	{
		// Poll for End of Conversion flag
		while (!(ADCx->SR & (1 << 1)));  // Wait for EOC flag
	}

	// Clear EOC flag
	ADCx->SR &= ~(1 << 1);

	// Return 12-bit result
	return (ADCx->DR & 0x0FFF);
}

/**
 * @fn      MCAL_ADC_Get_Conversion_Status
 * @brief   Get ADC conversion status
 */
uint8_t MCAL_ADC_Get_Conversion_Status(ADC_TypeDef *ADCx)
{
	return ((ADCx->SR & (1 << 1)) >> 1);  // Return EOC flag status
}

/**
 * @fn      MCAL_ADC_Disable
 * @brief   Disable ADC
 */
void MCAL_ADC_Disable(ADC_TypeDef *ADCx)
{
	ADCx->CR2 &= ~(1 << 0);  // Clear ADON bit
}
