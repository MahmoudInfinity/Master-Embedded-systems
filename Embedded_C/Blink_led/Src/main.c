#include <stdint.h>

#define RCC_BASE    0x40021000
#define RCC_APB2ENR (*(volatile uint32_t*)(RCC_BASE + 0x18))

#define GPIOB_BASE  0x40010C00
#define GPIOB_CRH   (*(volatile uint32_t*)(GPIOB_BASE + 0x04))
#define GPIOB_ODR   (*(volatile uint32_t*)(GPIOB_BASE + 0x0C))

int main(void)
{
    // 1. Enable clock for GPIOB (IOPBEN is bit 3)
    RCC_APB2ENR |= (1 << 3);

    // 2. Configure PB11 as output push-pull (50 MHz)
    GPIOB_CRH &= ~(0xF << 12);  // clear CNF11 + MODE11
    GPIOB_CRH |=  (0x3 << 12);  // output 50MHz push-pull

    while(1)
    {
        // 3. Toggle PB11
        GPIOB_ODR ^= (1 << 11);

        for(volatile int i = 0; i < 1000000; i++); // delay
    }
}
