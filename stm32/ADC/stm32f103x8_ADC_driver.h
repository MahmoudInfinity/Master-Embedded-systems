/*
 * stm32f103x8_ADC_driver.h
 *
 *  Created on: Aug 7, 2026
 *      Author: Mahmoud Elmowafy
 */

#ifndef INC_STM32F103X8_ADC_DRIVER_H_
#define INC_STM32F103X8_ADC_DRIVER_H_

// ============================================================
// INCLUDES
// ============================================================

#include "stm32f103x8.h"
#include "stm32f103x8_gpio_driver.h"

// ============================================================
// ENUMS
// ============================================================

typedef enum
{
	Enable,
	Disable
} ADC_Polling_Mechanism;

// ============================================================
// STRUCTURES
// ============================================================

// IRQ Source Structure
typedef struct
{
	uint16_t EOC:1;      // End of Conversion
	uint16_t JEOC:1;     // End of Injected Conversion
	uint16_t JSTRT:1;    // Injected group start
	uint16_t STRT:1;     // Regular group start
	uint16_t OVR:1;      // Overrun flag
	uint16_t AWD:1;      // Analog watchdog flag
	uint16_t reserved:10;

} S_ADC_IRQ_SRC;

// ADC Configuration Structure
typedef struct
{
	uint16_t Conversion_Mode;        // Single or Continuous @ref ADC_Conversion_Mode

	uint16_t Data_Alignment;         // Right or Left aligned @ref ADC_Data_Alignment

	uint16_t External_Trigger_Conv;  // External trigger for regular group @ref ADC_External_Trigger

	uint16_t Sampling_Time;          // ADC sampling time @ref ADC_Sampling_Time

	uint16_t Channel;                // ADC channel (0-17) @ref ADC_Channels

	uint16_t Number_Of_Conversions;  // 1 to 16 conversions in sequence

	uint16_t IRQ_Enable;             // IRQ Enable @ref ADC_IRQ_Enable

	uint16_t DMA_Enable;             // Enable/Disable DMA

	void (*p_IRQ_Callback)(S_ADC_IRQ_SRC IRQ_SRC);  // IRQ Callback function pointer

} ADC_Config;

// ============================================================
// REFERENCE MACROS
// ============================================================

// ===============================
// @ref ADC_Conversion_Mode
// ===============================
// CR2 register:
// Bit 1 (CONT):
// 0 → Single conversion mode
// 1 → Continuous conversion mode

#define ADC_MODE_SINGLE       (0)          // CR2[CONT] = 0
#define ADC_MODE_CONTINUOUS   (1 << 1)     // CR2[CONT] = 1

// ===============================
// @ref ADC_Data_Alignment
// ===============================
// CR2 register:
// Bit 11 (ALIGN):
// 0 → Right aligned
// 1 → Left aligned

#define ADC_ALIGN_RIGHT       (0)          // CR2[ALIGN] = 0
#define ADC_ALIGN_LEFT        (1 << 11)    // CR2[ALIGN] = 1

// ===============================
// @ref ADC_External_Trigger
// ===============================
// CR2 register:
// Bits 19:17 (EXTSEL[2:0]):
// 000 → Timer 1 CC1
// 001 → Timer 1 CC2
// 010 → Timer 1 CC3
// 011 → Timer 2 CC2
// 100 → Timer 3 TRGO
// 101 → Timer 4 CC4
// 110 → EXTI Line 11
// 111 → SWSTART (software start)



#define ADC_EXTSEL_TIM1_CC1   (0x0 << 17)            // Timer 1 CC1  (000)
#define ADC_EXTSEL_TIM1_CC2   (0x1 << 17)            // Timer 1 CC2  (001)
#define ADC_EXTSEL_TIM1_CC3   (0x2 << 17)            // Timer 1 CC3  (010)
#define ADC_EXTSEL_TIM2_CC2   (0x3 << 17)            // Timer 2 CC2  (011)
#define ADC_EXTSEL_TIM3_TRGO  (0x4 << 17)            // Timer 3 TRGO (100)
#define ADC_EXTSEL_TIM4_CC4   (0x5 << 17)            // Timer 4 CC4  (101)
#define ADC_EXTSEL_EXTI11     (0x6 << 17)            // EXTI Line 11 (110)
#define ADC_EXTSEL_SWSTART    (0x7 << 17)            // Software start (111)

// ===============================
// @ref ADC_Sampling_Time
// ===============================
// SMPR1/SMPR2 registers:
// Bits for each channel:
// 000 → 1.5 cycles
// 001 → 7.5 cycles
// 010 → 13.5 cycles
// 011 → 28.5 cycles
// 100 → 41.5 cycles
// 101 → 55.5 cycles
// 110 → 71.5 cycles
// 111 → 239.5 cycles

#define ADC_SAMPLETIME_1_5    (0)          // 1.5 cycles
#define ADC_SAMPLETIME_7_5    (1)          // 7.5 cycles
#define ADC_SAMPLETIME_13_5   (2)          // 13.5 cycles
#define ADC_SAMPLETIME_28_5   (3)          // 28.5 cycles
#define ADC_SAMPLETIME_41_5   (4)          // 41.5 cycles
#define ADC_SAMPLETIME_55_5   (5)          // 55.5 cycles
#define ADC_SAMPLETIME_71_5   (6)          // 71.5 cycles
#define ADC_SAMPLETIME_239_5  (7)          // 239.5 cycles

// ===============================
// @ref ADC_Channels
// ===============================
// Regular sequence bits in SQR3, SQR2, SQR1

#define ADC_CHANNEL_0         (0)
#define ADC_CHANNEL_1         (1)
#define ADC_CHANNEL_2         (2)
#define ADC_CHANNEL_3         (3)
#define ADC_CHANNEL_4         (4)
#define ADC_CHANNEL_5         (5)
#define ADC_CHANNEL_6         (6)
#define ADC_CHANNEL_7         (7)
#define ADC_CHANNEL_8         (8)
#define ADC_CHANNEL_9         (9)
#define ADC_CHANNEL_10        (10)
#define ADC_CHANNEL_11        (11)
#define ADC_CHANNEL_12        (12)
#define ADC_CHANNEL_13        (13)
#define ADC_CHANNEL_14        (14)
#define ADC_CHANNEL_15        (15)
#define ADC_CHANNEL_16        (16)  // Temperature sensor
#define ADC_CHANNEL_17        (17)  // Vrefint

// ===============================
// @ref ADC_IRQ_Enable
// ===============================
// CR1 register:
// Bit 5 (EOCIE):  End of conversion interrupt
// Bit 7 (JEOCIE): End of injected conversion interrupt
// Bit 9 (AWDIE):  Analog watchdog interrupt
// Bit 8 (OVRIE):  Overrun interrupt

#define ADC_IRQ_NONE          (0)
#define ADC_IRQ_EOC           (1 << 5)     // CR1[EOCIE]
#define ADC_IRQ_JEOC          (1 << 7)     // CR1[JEOCIE]
#define ADC_IRQ_AWD           (1 << 9)     // CR1[AWDIE]
#define ADC_IRQ_OVR           (1 << 8)     // CR1[OVRIE]

// ===============================
// @ref ADC_DMA_Enable
// ===============================
// CR2 register:
// Bit 8 (DMA):
// 0 → DMA disabled
// 1 → DMA enabled

#define ADC_DMA_DISABLE       (0)
#define ADC_DMA_ENABLE        (1 << 8)

// ============================================================
// APIs SUPPORTED BY ADC DRIVER
// ============================================================

/**
 * @fn      MCAL_ADC_Init
 * @brief   Initialize ADC peripheral with given configuration
 * @param   ADCx        : ADC peripheral (ADC1, ADC2, ADC3)
 * @param   ADC_Config  : Pointer to ADC configuration structure
 * @retval  None
 */
void MCAL_ADC_Init(ADC_TypeDef *ADCx, ADC_Config *ADC_Config);

/**
 * @fn      MCAL_ADC_DeInit
 * @brief   Reset ADC peripheral to default state
 * @param   ADCx        : ADC peripheral (ADC1, ADC2, ADC3)
 * @retval  None
 */
void MCAL_ADC_DeInit(ADC_TypeDef *ADCx);

/**
 * @fn      MCAL_ADC_GPIO_Set_Pins
 * @brief   Configure GPIO pins for ADC input
 * @param   ADCx        : ADC peripheral (ADC1, ADC2, ADC3)
 * @retval  None
 */
void MCAL_ADC_GPIO_Set_Pins(ADC_TypeDef *ADCx);

/**
 * @fn      MCAL_ADC_Start_Conversion
 * @brief   Start ADC conversion
 * @param   ADCx        : ADC peripheral (ADC1, ADC2, ADC3)
 * @retval  None
 */
void MCAL_ADC_Start_Conversion(ADC_TypeDef *ADCx);

/**
 * @fn      MCAL_ADC_Read_Value
 * @brief   Read ADC converted value with polling mechanism
 * @param   ADCx        : ADC peripheral (ADC1, ADC2, ADC3)
 * @param   Polling     : Enable or Disable polling
 * @retval  uint16_t    : 12-bit ADC value (0-4095)
 */
uint16_t MCAL_ADC_Read_Value(ADC_TypeDef *ADCx, ADC_Polling_Mechanism Polling);

/**
 * @fn      MCAL_ADC_Get_Conversion_Status
 * @brief   Check if conversion is complete
 * @param   ADCx        : ADC peripheral (ADC1, ADC2, ADC3)
 * @retval  uint8_t     : 1 if EOC flag is set, 0 otherwise
 */
uint8_t MCAL_ADC_Get_Conversion_Status(ADC_TypeDef *ADCx);

/**
 * @fn      MCAL_ADC_Disable
 * @brief   Disable ADC peripheral
 * @param   ADCx        : ADC peripheral (ADC1, ADC2, ADC3)
 * @retval  None
 */
void MCAL_ADC_Disable(ADC_TypeDef *ADCx);

#endif /* INC_STM32F103X8_ADC_DRIVER_H_ */
