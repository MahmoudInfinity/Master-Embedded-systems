#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

/* ================= Base Addresses ================= */
#define GPIOB_BASE  0x40010C00
#define RCC_BASE    0x40021000

#define RCC_APB2ENR (*(volatile uint32_t*)(RCC_BASE + 0x18))

/* ================= GPIO Struct ================= */

typedef union {
    struct {
        uint32_t B0  :1; uint32_t B1  :1;
        uint32_t B2  :1; uint32_t B3  :1;
        uint32_t B4  :1; uint32_t B5  :1;
        uint32_t B6  :1; uint32_t B7  :1;
        uint32_t B8  :1; uint32_t B9  :1;
        uint32_t B10 :1; uint32_t B11 :1;
        uint32_t B12 :1; uint32_t B13 :1;
        uint32_t B14 :1; uint32_t B15 :1;
        uint32_t RESERVED :16;
    } bit;
    uint32_t reg;
} GPIO_IDR_t;

typedef union {
    struct {
        uint32_t B0  :1; uint32_t B1  :1;
        uint32_t B2  :1; uint32_t B3  :1;
        uint32_t B4  :1; uint32_t B5  :1;
        uint32_t B6  :1; uint32_t B7  :1;
        uint32_t B8  :1; uint32_t B9  :1;
        uint32_t B10 :1; uint32_t B11 :1;
        uint32_t B12 :1; uint32_t B13 :1;
        uint32_t B14 :1; uint32_t B15 :1;
        uint32_t RESERVED :16;
    } bit;
    uint32_t reg;
} GPIO_ODR_t;

typedef union {
    struct {
        uint32_t MODE0 :2; uint32_t CNF0 :2;
        uint32_t MODE1 :2; uint32_t CNF1 :2;
        uint32_t MODE2 :2; uint32_t CNF2 :2;
        uint32_t MODE3 :2; uint32_t CNF3 :2;
        uint32_t MODE4 :2; uint32_t CNF4 :2;
        uint32_t MODE5 :2; uint32_t CNF5 :2;
        uint32_t MODE6 :2; uint32_t CNF6 :2;
        uint32_t MODE7 :2; uint32_t CNF7 :2;
    } bit;
    uint32_t reg;
} GPIO_CRL_t;

typedef union {
    struct {
        uint32_t MODE8  :2; uint32_t CNF8  :2;
        uint32_t MODE9  :2; uint32_t CNF9  :2;
        uint32_t MODE10 :2; uint32_t CNF10 :2;
        uint32_t MODE11 :2; uint32_t CNF11 :2;
        uint32_t MODE12 :2; uint32_t CNF12 :2;
        uint32_t MODE13 :2; uint32_t CNF13 :2;
        uint32_t MODE14 :2; uint32_t CNF14 :2;
        uint32_t MODE15 :2; uint32_t CNF15 :2;
    } bit;
    uint32_t reg;
} GPIO_CRH_t;

typedef struct {
    volatile GPIO_CRL_t CRL;
    volatile GPIO_CRH_t CRH;
    volatile GPIO_IDR_t IDR;
    volatile GPIO_ODR_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
} GPIO_TypeDef;

/* Peripheral pointer */
#define GPIOB ((GPIO_TypeDef*)GPIOB_BASE)

/* ================= API ================= */
void GPIOB_Init_PB11_Output(void);
void GPIOB_Init_PB8_Input_PU(void);

#endif