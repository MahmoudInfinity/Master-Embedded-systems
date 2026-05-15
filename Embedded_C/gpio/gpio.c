/*
 * gpio.c
 *
 *  Created on: May 15, 2026
 *      Author: Mahmoud Elmowafy
 */


#include "gpio.h"

/* Enable GPIOB clock */
static void GPIOB_Clock_Enable(void)
{
    RCC_APB2ENR |= (1 << 3);
}

/* PB11 Output */
void GPIOB_Init_PB11_Output(void)
{
    GPIOB_Clock_Enable();

    GPIOB->CRH.bit.MODE11 = 0x2; // Output 2 MHz
    GPIOB->CRH.bit.CNF11  = 0x0; // Push-pull
}

/* PB8 Input Pull-up */
void GPIOB_Init_PB8_Input_PU(void)
{
    GPIOB_Clock_Enable();

    GPIOB->CRH.bit.MODE8 = 0x0;  // Input
    GPIOB->CRH.bit.CNF8  = 0x2;  // Pull-up / pull-down

    GPIOB->ODR.bit.B8 = 1;       // Enable pull-up
}
