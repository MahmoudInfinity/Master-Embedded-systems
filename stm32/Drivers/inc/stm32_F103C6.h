/*
 * stm32_F103C6.h
 *
 *  Created on: Jun 8, 2026
 *      Author: Mahmoud Elmowafy
 */

#ifndef INC_STM32_F103C6_H_
#define INC_STM32_F103C6_H_

//INCLUDE

#include "stdlib.h"
#include <stdint.h>

//==============================================================
//Base addresses for Memory

#define Flash_Memory_base						0x8000000UL

#define System_Memory_base						0x1FFFF000UL

#define Sram_Memory_base						0x20000000UL

#define Peripherals_base						0x4000000UL

#define Cortex_M3_internal_peripherals_base		0xE000000UL



//Base addresses for AHB peripherals


#define RCC_Base 	0x40021000UL


//Base addresses for ABB2 peripherals


//GPIO

#define GPIOA_Base 	0x40010800UL
#define GPIOB_Base 	0x40010C00UL
#define GPIOC_Base 	0x40011000UL
#define GPIOD_Base 	0x40011400UL
#define GPIOE_Base 	0x40011800UL


//EXTI

#define EXTI_Base 0x40010400UL

//AFIO


#define AFIO_Base 0x40010000UL


//==============================================================
// Peripheral Registers: RCC
//==============================================================

typedef struct

{

	volatile uint32_t CR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t APB1RSTR;
	volatile uint32_t AHBENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t APB1ENR;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;


}RCC_TypeDef;


// Peripheral Registers: GPIO

typedef struct
{

	volatile uint32_t CRL;
	volatile uint32_t CRH;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t BRR;
	volatile uint32_t LCKR;


}GPIO_TypeDef;

// Peripheral Registers: AFIO

typedef struct
{

	volatile uint32_t EVCR;
	volatile uint32_t MAPR;
	volatile uint32_t EXTICR1;
	volatile uint32_t EXTICR2;
	volatile uint32_t EXTICR3;
	volatile uint32_t EXTICR4;
	volatile uint32_t RESERVED0;
	volatile uint32_t MAPR2;

}AFIO_TypeDef;
// Peripheral Registers: EXTI

typedef struct
{

	volatile uint32_t IMR;
	volatile uint32_t EMR;
	volatile uint32_t RTSR;
	volatile uint32_t FTSR;
	volatile uint32_t SWIER;
	volatile uint32_t PR;



}EXTI_TypeDef;



//==============================================================
// Peripheral Instants
//==============================================================


#define GPIOA 	((GPIO_TypeDef *) GPIOA_Base)
#define GPIOB 	((GPIO_TypeDef *) GPIOB_Base)
#define GPIOC 	((GPIO_TypeDef *) GPIOC_Base)
#define GPIOD 	((GPIO_TypeDef *) GPIOD_Base)
#define GPIOE 	((GPIO_TypeDef *) GPIOE_Base)

#define RCC 	((RCC_TypeDef *) RCC_Base)
#define AFIO 	((AFIO _TypeDef *) AFIO_Base)
#define EXTI 	((EXTI _TypeDef *) EXTI_Base)


//============================================================
//clock enable macros
//============================================================

#define RCC_GPIOA_CLK_EN() (RCC -> APBB2EN |= 1 << 2)
#define RCC_GPIOB_CLK_EN() (RCC -> APBB2EN |= 1 << 3)
#define RCC_GPIOC_CLK_EN() (RCC -> APBB2EN |= 1 << 4)
#define RCC_GPIOD_CLK_EN() (RCC -> APBB2EN |= 1 << 5)
#define RCC_GPIOE_CLK_EN() (RCC -> APBB2EN |= 1 << 6)

#define RCC_AFIO_CLK_EN() (RCC -> APBB2EN |= 1 << 0)


#endif /* INC_STM32_F103C6_H_ */


