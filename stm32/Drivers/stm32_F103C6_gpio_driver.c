/*
 * stm32_F103C6_gpio_driver.c
 *
 *  Created on: Jun 8, 2026
 *      Author: pc
 */
//===============================================================
//INCLUDES
//===============================================================
#include "stm32_F103C6.h"
#include "stm32_F103C6_gpio_driver.h"


uint8_t  Get_CRLH_PinPosition(uint16_t GPIO_PinNumber)
{


	switch (GPIO_PinNumber)
	{

	case GPIO_PIN_0:

		return 0;

	case GPIO_PIN_1:

		return 4;

	case GPIO_PIN_2:

		return 8;
	case GPIO_PIN_3:

		return 12;
	case GPIO_PIN_4:

		return 16;
	case GPIO_PIN_5:

		return 20;

	case GPIO_PIN_6:

		return 24;

	case GPIO_PIN_7:

		return 28;


	case GPIO_PIN_8:

		return 0;

	case GPIO_PIN_9:

		return 4;

	case GPIO_PIN_10:

		return 8;
	case GPIO_PIN_11:

		return 12;
	case GPIO_PIN_12:

		return 16;
	case GPIO_PIN_13:

		return 20;

	case GPIO_PIN_14:

		return 24;

	case GPIO_PIN_15:

		return 28;




	}

	return 0;
}








/*==========================================================================
 *@Fn 			-  MCAL GPIO Init
 *@brief		    -  initializes GPIOx PIN according to parameters in PinConfig
 *@param [in]	-  GPIOx  (x: x =  A ... E depending on the device )
 *@param [in]	-  PinConfig is a pointer to struct to configure the selected GPIO
 *@retval		-  none
 *note 			-  stm32F103C6 has GPIOs from A to E but the used fabricated chip (LQFP48) has Ports A, B and part of C, D
===============================================================================
 */


void MCAL_GPIO_Init(GPIO_TypeDef * GPIOx , GPIO_PinConfig_t * PinConfig )
{
	//Port configuration register low (GPIOx_CRL) (x=A..G)
	//Port configuration register high (GPIOx_CRL) (x=A..G)

	volatile uint32_t * configregister = NULL;
	uint8_t PIN_CNFG = 0;

	configregister = (PinConfig->GPIO_PinNumber < GPIO_PIN_8)?  &GPIOx->CRL : &GPIOx->CRH;

	// clear MODEx[1:0] and CNFG[1:0]

	(*configregister) &= ~(0xF << Get_CRLH_PinPosition(PinConfig-> GPIO_PinNumber));

	// if PIN is output

	if ( (PinConfig-> GPIO_PinMode == GPIO_MODE_OUTPUT_PP ) || (PinConfig-> GPIO_PinMode == GPIO_MODE_OUTPUT_OD)
			||(PinConfig-> GPIO_PinMode == GPIO_MODE_AF_OD )|| (PinConfig-> GPIO_PinMode == GPIO_MODE_AF_PP ))

	{
		// set MODEx[1:0] and CNFGx[1:0]
		PIN_CNFG = (((PinConfig->GPIO_PinMode - 4 )<< 2) | PinConfig-> GPIO_output_speed ) & 0x0f;



	}

	// if PIN is input
	else
	{
		if(PinConfig-> GPIO_PinMode == GPIO_MODE_INPUT_ANALOG || PinConfig-> GPIO_PinMode == GPIO_MODE_INPUT_FLOATING)

		{
			// set MODEx[1:0] and CNFGx[1:0]
			PIN_CNFG = (((PinConfig->GPIO_PinMode  )<< 2) |0x0 ) & 0x0f;


		}

		else if(PinConfig-> GPIO_PinMode == GPIO_MODE_AF_Input)// cosider it as input floating
		{

			// set MODEx[1:0] and CNFGx[1:0]
			PIN_CNFG = ((( GPIO_MODE_INPUT_FLOATING )<< 2) |0x0 ) & 0x0f;

		}

		else  // input pull up or pull down
		{
			PIN_CNFG = ((( GPIO_MODE_INPUT_PULL_UP )<< 2) |0x0 ) & 0x0f;
			if(PinConfig-> GPIO_PinMode == GPIO_MODE_INPUT_PULL_UP)
			{
				// table 20   PxODR 1
				GPIOx->ODR |= PinConfig->GPIO_PinNumber;
			}

			else
			{   // For pull down
				// table 20   PxODR 0

				GPIOx->ODR &= ~ (PinConfig->GPIO_PinNumber);


			}
		}
	}


	(*configregister) |= ((PIN_CNFG) << Get_CRLH_PinPosition(PinConfig->GPIO_PinNumber));
}


/*==========================================================================
 *@Fn 			-  MCAL GPIO DeInit
 *@brief		-  Reset GPIOx PINs
 *@param [in]	-  GPIOx  (x: x =  A ... E depending on the device )
 *@retval		-  none
 *note 			- none
=============================================================================
 */
void MCAL_GPIO_DeInit(GPIO_TypeDef * GPIOx )
{

	uint8_t bit_position = 0;

	if (GPIOx == GPIOA)       bit_position = 2;
	else if (GPIOx == GPIOB)  bit_position = 3;
	else if (GPIOx == GPIOC)  bit_position = 4;
	else if (GPIOx == GPIOD)  bit_position = 5;
	else if (GPIOx == GPIOE)  bit_position = 6;

	RCC->APB2RSTR |=  (1 << bit_position);   // Assert reset
	RCC->APB2RSTR &= ~(1 << bit_position);   // Release reset


}


/*==========================================================================
 *@Fn 			-  MCAL GPIO ReadPin
 *@brief		    -  Reads GPIO Pin
 *@param [in]	-  GPIOx  (x: x =  A ... E depending on the device )
 *@param [in]	- The number of the pin to be read from 0 to 15 according @ref GPIO PIN define
 *@retval		-  The input pin value  @ref GPIO pin state
 *note 			-  none
===============================================================================
 */
uint8_t MCAL_GPIO_ReadPin(GPIO_TypeDef * GPIOx , uint16_t PinNumber)
{
	uint8_t pinStatus;

	if((GPIOx->IDR & PinNumber) != (uint32_t)GPIO_PinReSet )

	{ pinStatus = GPIO_PinSet ;}

	else
	{ pinStatus = GPIO_PinReSet ; }


	return pinStatus;

	return 0;
}



/*==========================================================================
 *@Fn 			-  MCAL GPIO ReadPort
 *@brief		    -  Reads Gpio Port
 *@param [in]	-  GPIOx  (x: x =  A ... E depending on the device )
 *@retval		-  The Port value GPIOx
 *note 			-  none
===============================================================================
 */


uint16_t MCAL_GPIO_ReadPort(GPIO_TypeDef * GPIOx )
{
	uint16_t portValue;

	portValue = (uint16_t)GPIOx->IDR;

	return portValue;
}


/*==========================================================================
 *@Fn 			-  MCAL GPIO WritePin
 *@brief		    -  Writes GPIO Pin
 *@param [in]	-  GPIOx  (x: x =  A ... E depending on the device )
 *@param [in]	- The number of the pin to be written from 0 to 15 according @ref GPIO PIN define
 *@param [in]	- The value to be written wether high or low @ref Pin state
 *@retval		-  none
 *note 			-  none
===============================================================================
 */

void MCAL_GPIO_WritePin(GPIO_TypeDef * GPIOx , uint16_t PinNumber , uint8_t value)
{
	if(value != GPIO_PinReSet )

		{ GPIOx->BSRR = (uint32_t)PinNumber ;}

		else
		{ GPIOx->BRR = (uint32_t)PinNumber ; }

}


/*==========================================================================
 *@Fn 			-  MCAL GPIO WritePort
 *@brief		    -  Writes GPIO Port
 *@param [in]	-  GPIOx  (x: x =  A ... E depending on the device )
 *@param [in]	- The value to be written on port
 *@retval		-  none
 *note 			-  none
===============================================================================
 */



void MCAL_GPIO_WritePort(GPIO_TypeDef * GPIOx ,  uint16_t value)
{

  GPIOx-> ODR = (uint32_t)value;

}








/*==========================================================================
 *@Fn 			-  MCAL GPIO Toggle Pin
 *@brief		    -  Toggles GPIO Pin
 *@param [in]	-  GPIOx  (x: x =  A ... E depending on the device )
 *@param [in]	- The number of the pin to be toggled from 0 to 15 according @ref GPIO PIN define
 *@retval		-  none
 *note 			-  none
===============================================================================
 */

void MCAL_GPIO_TogglePin(GPIO_TypeDef * GPIOx , uint16_t PinNumber)
{
  GPIOx->ODR ^=  (uint32_t)PinNumber;

}


/*==========================================================================
 *@Fn 			-  MCAL GPIO Lock Pin
 *@brief		    -  Lock GPIO Pin
 *@param [in]	-  GPIOx  (x: x =  A ... E depending on the device )
 *@param [in]	- The number of the pin to be locked from 0 to 15 according @ref GPIO PIN define
 *@retval		-  returns wether lock succeded (OK ^^ ERROR) @ref GPIO_return_lock
 *note 			-  none
===============================================================================
 */
uint8_t MCAL_GPIO_LockPin(GPIO_TypeDef * GPIOx , uint16_t PinNumber)
{

	/*Bit 16 LCKK[16]: Lock key
	This bit can be read anytime. It can only be modified using the Lock Key Writing Sequence.
	0: Port configuration lock key not active
	1: Port configuration lock key active. GPIOx_LCKR register is locked until the next reset.
	LOCK key writing sequence:
	Write 1
	Write 0
	Write 1
	Read 0
	Read 1 (this read is optional but confirms that the lock is active)
	Note: During the LOCK Key Writing sequence, the value of LCK[15:0] must not change.
	Any error in the lock sequence will abort the lock.
	Bits 15:0 LCKy: Port x Lock bit y (y= 0 .. 15)
	These bits are read write but can only be written when the LCKK bit is 0.
	0: Port configuration not locked
	1: Port configuration locked.*/

	volatile uint32_t temp = (1 << 16) | PinNumber;  // LCKK = 1 + pins

	    // Step 1: Write 1
	    GPIOx->LCKR = temp;

	    // Step 2: Write 0
	    GPIOx->LCKR = PinNumber;

	    // Step 3: Write 1
	    GPIOx->LCKR = temp;

	    // Step 4: Read (dummy)
	    temp = GPIOx->LCKR;

	    // Step 5: Read again (lock active)
	    if (GPIOx->LCKR & (1 << 16))
	    {
	        return GPIO_return_lock_OK;
	    }
	    else
	    {
	    	return GPIO_return_lock_ERROR;

	    }
}


