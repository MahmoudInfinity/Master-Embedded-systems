/*
 * stm32_F103C6_gpio_driver.h
 *
 *  Created on: Jun 8, 2026
 *      Author: pc
 */

#ifndef INC_STM32_F103C6_GPIO_DRIVER_H_
#define INC_STM32_F103C6_GPIO_DRIVER_H_

// INCLUDES

#include "stm32_F103C6.h"


// Config struct

typedef struct
{
	uint16_t GPIO_PinNumber ; // specifies the GPIO pin to be configured
	                          //this parameter can be value of @ref GPIO PIN define

	uint8_t GPIO_PinMode ;  // specifies the pin mode
	                         //can be value of @ref PIN Mode

    uint8_t GPIO_output_speed; //specifes the speed of the pin when selected as output
                                  //can be value of @ref output speed

}GPIO_PinConfig_t;




//@ref GPIO pin state

#define GPIO_PinSet 1
#define GPIO_PinReSet 0




//@ref GPIO PIN define

/*
 * Each GPIO pin is represented by a single bit in a 16-bit register.
 * Bit position = pin number
 * Example:
 *   Pin 0 → bit 0 → 0x0001
 *   Pin 1 → bit 1 → 0x0002
 *   ...
 *   Pin 15 → bit 15 → 0x8000
 */

#define GPIO_PIN_0   ((uint16_t)0x0001)  // 0000 0000 0000 0001 → selects pin 0
#define GPIO_PIN_1   ((uint16_t)0x0002)  // 0000 0000 0000 0010 → selects pin 1
#define GPIO_PIN_2   ((uint16_t)0x0004)  // 0000 0000 0000 0100 → selects pin 2
#define GPIO_PIN_3   ((uint16_t)0x0008)  // 0000 0000 0000 1000 → selects pin 3
#define GPIO_PIN_4   ((uint16_t)0x0010)  // 0000 0000 0001 0000 → selects pin 4
#define GPIO_PIN_5   ((uint16_t)0x0020)  // 0000 0000 0010 0000 → selects pin 5
#define GPIO_PIN_6   ((uint16_t)0x0040)  // 0000 0000 0100 0000 → selects pin 6
#define GPIO_PIN_7   ((uint16_t)0x0080)  // 0000 0000 1000 0000 → selects pin 7
#define GPIO_PIN_8   ((uint16_t)0x0100)  // 0000 0001 0000 0000 → selects pin 8
#define GPIO_PIN_9   ((uint16_t)0x0200)  // 0000 0010 0000 0000 → selects pin 9
#define GPIO_PIN_10  ((uint16_t)0x0400)  // 0000 0100 0000 0000 → selects pin 10
#define GPIO_PIN_11  ((uint16_t)0x0800)  // 0000 1000 0000 0000 → selects pin 11
#define GPIO_PIN_12  ((uint16_t)0x1000)  // 0001 0000 0000 0000 → selects pin 12
#define GPIO_PIN_13  ((uint16_t)0x2000)  // 0010 0000 0000 0000 → selects pin 13
#define GPIO_PIN_14  ((uint16_t)0x4000)  // 0100 0000 0000 0000 → selects pin 14
#define GPIO_PIN_15  ((uint16_t)0x8000)  // 1000 0000 0000 0000 → selects pin 15

/*
 * Mask for all GPIO pins (16 pins)
 * Useful when you want to configure or reset the entire port at once
 */
#define GPIO_PIN_ALL ((uint16_t)0xFFFF)  // 1111 1111 1111 1111 → selects all pins

//==============================================================================
//@ref PIN Mode
//==============================================================================



/* ================= GPIO MODE BITS =================
 * MODE[1:0] defines input/output speed or input mode selection
 * CNF[1:0]  defines configuration of the pin
 * ================================================== */

/* ---------------- INPUT MODES (MODE = 00) ---------------- */
#define GPIO_MODE_INPUT_ANALOG        	 0x0  //  Analog mode
#define GPIO_MODE_INPUT_FLOATING         0x1  //  Floating input (reset state)
#define GPIO_MODE_INPUT_PULL_UP       	 0x2  //  Input with pull-up
#define GPIO_MODE_INPUT_PULL_DOWN     	 0x3  //  Input with  pull-down


/* ---------------- OUTPUT MODES (MODE > 00) ---------------- */

/* General purpose output */
#define GPIO_MODE_OUTPUT_PP                0x4  // Push-pull
#define GPIO_MODE_OUTPUT_OD                0x5  // Open-drain

/* Alternate function output */
#define GPIO_MODE_AF_OD                    0x6  //  Alternate function open-drain
#define GPIO_MODE_AF_PP               	   0x7  //  Alternate function input
#define GPIO_MODE_AF_Input      		   0x8  // AF Input

//=========================================================
//@ref output speed
//=========================================================

#define Speed_10MHZ 	0x1;
#define Speed_2MHZ  	0x2;
#define Speed_50MHZ 	0x3;

//=========================================================
//@ref GPIO_return_lock
//=========================================================

#define GPIO_return_lock_OK 	1
#define GPIO_return_lock_ERROR 	0


//==========================================================
//APIS supported by MACAL GPIO Driver
//==========================================================


void MCAL_GPIO_Init(GPIO_TypeDef * GPIOx , GPIO_PinConfig_t * PinConfig );
void MCAL_GPIO_DeInit(GPIO_TypeDef * GPIOx );
uint8_t MCAL_GPIO_ReadPin(GPIO_TypeDef * GPIOx , uint16_t PinNumber);
uint16_t MCAL_GPIO_ReadPort(GPIO_TypeDef * GPIOx );
void MCAL_GPIO_WritePin(GPIO_TypeDef * GPIOx , uint16_t PinNumber , uint8_t value);
void MCAL_GPIO_WritePort(GPIO_TypeDef * GPIOx ,  uint16_t value);
void MCAL_GPIO_TogglePin(GPIO_TypeDef * GPIOx , uint16_t PinNumber);
uint8_t MCAL_GPIO_LockPin(GPIO_TypeDef * GPIOx , uint16_t PinNumber);



#endif /* INC_STM32_F103C6_GPIO_DRIVER_H_ */
