#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "STM32F103x8.h"
#include "stm32f103x8_gpio_driver.h"

#include "stm32f103x8_EXTI_driver.h"
#include "stm32f103x8_USART_driver.h"

//====================================================
// Global UART config
//====================================================
UART_Config UART1_Config;

//====================================================
// EXTI config objects
//====================================================
EXTI_PinConfig_t EXTI8_Config;
EXTI_PinConfig_t EXTI4_Config;
EXTI_PinConfig_t EXTI5_Config;

//====================================================
// Global event variable
// 0 = no event
// 1 = PB8 pressed
// 2 = PB4 pressed
// 3 = PB5 pressed
//====================================================
volatile uint8_t g_button_event = 0;
volatile uint8_t g_busy = 0;

//====================================================
// Simple delay
//====================================================
void delay_simple(volatile uint32_t time)
{
	for (volatile uint32_t i = 0; i < time; i++);
}

//====================================================
// UART send one char
//====================================================
void UART_SendChar(USART_TypeDef *USARTx, char c)
{
	uint16_t data = c;
	MCAL_UART_SendData(USARTx, &data, enable);
	MCAL_UART_WAIT_TC(USARTx);
}

//====================================================
// Buzzer control
//====================================================
void Buzzer_Init(void)
{
	GPIO_PinConfig_t pinCfg;

	RCC_GPIOB_CLK_EN();

	pinCfg.GPIO_PinNumber = GPIO_PIN_9;
	pinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	pinCfg.GPIO_Output_Speed = GPIO_SPEED_10M;
	MCAL_GPIO_Init(GPIOB, &pinCfg);

	MCAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
}

void Buzzer_On(void)
{
	MCAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
}

void Buzzer_Off(void)
{
	MCAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
}

//====================================================
// Buzzer patterns
//====================================================
void Buzzer_Sound_1(void)
{
	Buzzer_On();
	delay_simple(30000);
	Buzzer_Off();
	Buzzer_On();
	delay_simple(30000);
	Buzzer_Off();
	Buzzer_On();
	delay_simple(30000);
	Buzzer_Off();

}

void Buzzer_Sound_2(void)
{
	Buzzer_On();
	delay_simple(20000);
	Buzzer_Off();
	delay_simple(15000);
	Buzzer_On();
	delay_simple(20000);
	Buzzer_Off();
	Buzzer_On();
	delay_simple(20000);
	Buzzer_Off();
	delay_simple(15000);
	Buzzer_On();
	delay_simple(20000);
	Buzzer_Off();
	Buzzer_On();
	delay_simple(20000);
	Buzzer_Off();
	delay_simple(15000);
	Buzzer_On();
	delay_simple(20000);
	Buzzer_Off();
	Buzzer_On();
	delay_simple(20000);
	Buzzer_Off();
	delay_simple(15000);
	Buzzer_On();
	delay_simple(20000);
	Buzzer_Off();
	Buzzer_On();
	delay_simple(20000);
	Buzzer_Off();
	delay_simple(15000);
	Buzzer_On();
	delay_simple(20000);
	Buzzer_Off();
	Buzzer_On();
	delay_simple(20000);
	Buzzer_Off();
	delay_simple(15000);
	Buzzer_On();
	delay_simple(20000);
	Buzzer_Off();
	Buzzer_On();
	delay_simple(20000);
	Buzzer_Off();
	delay_simple(15000);
	Buzzer_On();
	delay_simple(20000);
	Buzzer_Off();
	Buzzer_On();
	delay_simple(20000);
	Buzzer_Off();
	delay_simple(15000);
	Buzzer_On();
	delay_simple(20000);
	Buzzer_Off();
	Buzzer_On();
	delay_simple(20000);
	Buzzer_Off();
	delay_simple(15000);
	Buzzer_On();
	delay_simple(20000);
	Buzzer_Off();
	Buzzer_On();
	delay_simple(20000);
	Buzzer_Off();
	delay_simple(15000);
	Buzzer_On();
	delay_simple(20000);
	Buzzer_Off();
	Buzzer_On();
	delay_simple(20000);
	Buzzer_Off();


}

void Buzzer_Sound_3(void)
{

	Buzzer_On();
	delay_simple(30000);
	Buzzer_Off();
	delay_simple(20000);
	Buzzer_Off();
	Buzzer_On();
	delay_simple(30000);
	Buzzer_Off();
	delay_simple(20000);
	Buzzer_Off();
	Buzzer_On();
	delay_simple(30000);
	Buzzer_Off();
	delay_simple(20000);
	Buzzer_Off();
	Buzzer_On();
	delay_simple(30000);
	Buzzer_Off();
	delay_simple(20000);
	Buzzer_Off();
}

//====================================================
// UART1 init
//====================================================
void UART1_Init(void)
{
	RCC_GPIOA_CLK_EN();

	UART1_Config.USART_Mode      = UART_MODE_TX;
	UART1_Config.BaudRate        = UART_BaudRate_9600;
	UART1_Config.Payload_Length  = UART_Payload_Length_8B;
	UART1_Config.Parity          = UART_Parity__NONE;
	UART1_Config.StopBits        = UART_StopBits__1;
	UART1_Config.HwFlowCtl       = UART_HwFlowCtl_NONE;
	UART1_Config.IRQ_Enable      = UART_IRQ_Enable_NONE;
	UART1_Config.P_IRQ_CallBack  = 0;

	MCAL_UART_Init(USART1, &UART1_Config);
	MCAL_UART_GPIO_Set_Pins(USART1);
}

//====================================================
// EXTI callbacks
// only set the event flag
//====================================================
void Button8_Callback(void)
{
	if (!g_busy)
		g_button_event = 1;
}

void Button4_Callback(void)
{
	if (!g_busy)
		g_button_event = 2;
}

void Button5_Callback(void)
{
	if (!g_busy)
		g_button_event = 3;
}

//====================================================
// EXTI init
//====================================================
void Buttons_EXTI_Init(void)
{
	RCC_GPIOB_CLK_EN();
	RCC_AFIO_CLK_EN();

	// PB8 -> EXTI8
	EXTI8_Config.EXTI_PIN.GPIO_Port = GPIOB;
	EXTI8_Config.EXTI_PIN.GPIO_PIN = GPIO_PIN_8;
	EXTI8_Config.EXTI_PIN.ETI_InputLineNumber = 8;
	EXTI8_Config.Trigger_Case = EXTI_Trigger_FALLING;
	EXTI8_Config.IRQ_EN = EXTI_IRQ_Enable;
	EXTI8_Config.P_IRQ_CallBack = Button8_Callback;
	MCAL_EXTI_GPIO_Init(&EXTI8_Config);

	// PB4 -> EXTI4
	EXTI4_Config.EXTI_PIN.GPIO_Port = GPIOB;
	EXTI4_Config.EXTI_PIN.GPIO_PIN = GPIO_PIN_4;
	EXTI4_Config.EXTI_PIN.ETI_InputLineNumber = 4;
	EXTI4_Config.Trigger_Case = EXTI_Trigger_FALLING;
	EXTI4_Config.IRQ_EN = EXTI_IRQ_Enable;
	EXTI4_Config.P_IRQ_CallBack = Button4_Callback;
	MCAL_EXTI_GPIO_Init(&EXTI4_Config);

	// PB5 -> EXTI5
	EXTI5_Config.EXTI_PIN.GPIO_Port = GPIOB;
	EXTI5_Config.EXTI_PIN.GPIO_PIN = GPIO_PIN_5;
	EXTI5_Config.EXTI_PIN.ETI_InputLineNumber = 5;
	EXTI5_Config.Trigger_Case = EXTI_Trigger_FALLING;
	EXTI5_Config.IRQ_EN = EXTI_IRQ_Enable;
	EXTI5_Config.P_IRQ_CallBack = Button5_Callback;
	MCAL_EXTI_GPIO_Init(&EXTI5_Config);
}

//====================================================
// Main
//====================================================
int main(void)
{
	UART1_Init();
	Buzzer_Init();
	Buttons_EXTI_Init();

	while (1)
	{
		if (g_button_event != 0)
		{
			g_busy = 1;

			if (g_button_event == 1)
			{  for(int i =0 ; i< 10000;++i)
			{
				UART_SendChar(USART1, 'A');
			}
			Buzzer_Sound_1();

			while (MCAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8) == GPIO_PIN_RESET);
			delay_simple(30000);
			}
			else if (g_button_event == 2)
			{
				for(int i =0 ; i< 10000;++i)
				{
					UART_SendChar(USART1, 'B');
				}
				Buzzer_Sound_2();

				while (MCAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == GPIO_PIN_RESET);
				delay_simple(30000);
			}
			else if (g_button_event == 3)
			{
				for(int i =0 ; i< 10000;++i)
				{
					UART_SendChar(USART1, 'C');
				}
				Buzzer_Sound_3();

				while (MCAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == GPIO_PIN_RESET);
				delay_simple(30000);
			}

			g_button_event = 0;
			g_busy = 0;
		}
	}
}
