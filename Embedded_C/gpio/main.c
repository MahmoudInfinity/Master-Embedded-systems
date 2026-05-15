#include "gpio.h"

int main(void)
{
    GPIOB_Init_PB11_Output();
    GPIOB_Init_PB8_Input_PU();

    while (1)
    {
        if (GPIOB->IDR.bit.B8)   // Not pressed (pull-up)
        {
            GPIOB->ODR.bit.B11 = 0;
        }
        else                      // Pressed
        {
            GPIOB->ODR.bit.B11 = 1;
        }
    }
}
