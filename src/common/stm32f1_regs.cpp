
#include "../../../common/stm32f1_regs.hpp"

volatile SYSTICK_CTRL_t *SYSTICK_CTRL = (volatile SYSTICK_CTRL_t *)0xE000E010;
volatile uint32_t *SYSTICK_LOAD  = (volatile uint32_t *)0xE000E014;
volatile uint32_t *SYSTICK_VAL   = (volatile uint32_t *)0xE000E018;
volatile uint32_t *SYSTICK_CALIB = (volatile uint32_t *)0xE000E01C;

// AHB peripheral clock enable register 
volatile AHBENR_t *AHBENR = (volatile AHBENR_t *)0x40021014;
// APB1 peripheral clock enable register
volatile APB1EN_t *APB1EN = (volatile APB1EN_t *)0x4002101C;
// APB2 peripheral clock enable register
volatile APB2EN_t *APB2EN = (volatile APB2EN_t *)0x40021018;


volatile APB1RSTR_t *APB1RSTR = (volatile APB1RSTR_t *)0x40021010;

//
volatile RCC_t *RCC = (volatile RCC_t *)0x40021000;
// Clock control register
volatile RCC_CR_t *RCC_CR = (volatile RCC_CR_t *)0x40021000;
// Clock configuration register
volatile RCC_CFGR_t *RCC_CFGR = (volatile RCC_CFGR_t *)0x40021004;


volatile FLASH_t *FLASH = (volatile FLASH_t *)0x40022000;

// PORT A Control Register Low
volatile GPIOx_CRL_t *GPIOA_CRL = (volatile GPIOx_CRL_t *)0x40010800;
// PORT A Control Register High
volatile GPIOx_CRH_t *GPIOA_CRH = (volatile GPIOx_CRH_t *)0x40010804;
// PORT A Input Data Register
volatile GPIOx_IDR_t *GPIOA_IDR = (volatile GPIOx_IDR_t *)0x40010808;
// PORT A Output Data Register
volatile GPIOx_ODR_t *GPIOA_ODR = (volatile GPIOx_ODR_t *)0x4001080C;

// PORT B Control Register Low
volatile GPIOx_CRL_t *GPIOB_CRL = (volatile GPIOx_CRL_t *)0x40010C00;
// PORT B Control Register High
volatile GPIOx_CRH_t *GPIOB_CRH = (volatile GPIOx_CRH_t *)0x40010C04;
// PORT B Input Data Register
volatile GPIOx_IDR_t *GPIOB_IDR = (volatile GPIOx_IDR_t *)0x40010C08;
// PORT B Output Data Register
volatile GPIOx_ODR_t *GPIOB_ODR = (volatile GPIOx_ODR_t *)0x40010C0C;

// PORT C Control Register Low
volatile GPIOx_CRL_t *GPIOC_CRL = (volatile GPIOx_CRL_t *)0x40011000;
// PORT C Control Register High
volatile GPIOx_CRH_t *GPIOC_CRH = (volatile GPIOx_CRH_t *)0x40011004;
// PORT C Input Data Register
volatile GPIOx_IDR_t *GPIOC_IDR = (volatile GPIOx_IDR_t *)0x40011008;
// PORT C Output Data Register
volatile GPIOx_ODR_t *GPIOC_ODR = (volatile GPIOx_ODR_t *)0x4001100C;

// 
volatile MAPR_t *MAPR = (volatile MAPR_t *)0x40010004;


 // Status register
volatile SR_t *USART2_SR = (volatile SR_t *)0x40004400;

 // Data register
volatile DR_t *USART2_DR = (volatile DR_t *)0x40004404;

 // Baud rate register
volatile BRR_t *USART2_BRR = (volatile BRR_t *)0x40004408;

 // Control register 1
volatile CR1_t *USART2_CR1 = (volatile CR1_t *)0x4000440C;


// I2C 1 Control 1 Register
volatile I2C_CR1_t *I2C1_CR1 = (volatile I2C_CR1_t *)0x40005400;
// I2C 1 Control 2 Register
volatile I2C_CR2_t *I2C1_CR2 = (volatile I2C_CR2_t *)0x40005404;
// I2C 1 Data Register (8 bits)
volatile uint16_t *I2C1_DR = (volatile uint16_t*)0x40005410;
// I2C 1 Status 1 Register
volatile I2C_SR1_t *I2C1_SR1 = (volatile I2C_SR1_t *)0x40005414;
// I2C 1 Status 2 Register
volatile I2C_SR2_t *I2C1_SR2 = (volatile I2C_SR2_t *)0x40005418;
// I2C 1 Clock control register
volatile I2C_CCR_t *I2C1_CCR = (volatile I2C_CCR_t *)0x4000541C;
// I2C 1 T RISE register
volatile uint16_t *I2C1_TRISE = (volatile uint16_t *)0x40005420;

// I2C 2 Control 1 Register
volatile I2C_CR1_t *I2C2_CR1 = (volatile I2C_CR1_t *)0x40005800;
// I2C 2 Control 2 Register
volatile I2C_CR2_t *I2C2_CR2 = (volatile I2C_CR2_t *)0x40005804;
// I2C 2 Data Register (8 bits)
volatile uint16_t *I2C2_DR  = (volatile uint16_t *)0x40005810;
// I2C 2 Status 1 Register
volatile I2C_SR1_t *I2C2_SR1 = (volatile I2C_SR1_t *)0x40005814;
// I2C 2 Status 2 Register
volatile I2C_SR2_t *I2C2_SR2 = (volatile I2C_SR2_t *)0x40005818;
// I2C 2 Clock control register
volatile I2C_CCR_t *I2C2_CCR = (volatile I2C_CCR_t *)0x4000581C;
// I2C 2 T RISE register
volatile uint16_t *I2C2_TRISE = (volatile uint16_t *)0x40005820;

 // SPI1 
volatile SPI_CR1_t *SPI1_CR1 = (volatile SPI_CR1_t *)0x40013000;

 // SPI1 
volatile CR2_t *SPI1_CR2 = (volatile CR2_t *)0x40013004;


 // SPI1 
volatile SPI_SR_t *SPI1_SR = (volatile SPI_SR_t *)0x40013008;

 // SPI1 
volatile SPI_DR_t *SPI1_DR = (volatile SPI_DR_t *)0x4001300C;

 // SPI1 
volatile CRCPR_t *SPI1_CRCPR = (volatile CRCPR_t *)0x40013010;

 // SPI1 
volatile RXCRCR_t *SPI1_RXCRCR = (volatile RXCRCR_t *)0x40013014;

 // SPI1 
volatile TXCRCR_t *SPI1_TXCRCR = (volatile TXCRCR_t *)0x40013018;

 // SPI1 
volatile I2SCFGR_t *SPI1_I2SCFGR = (volatile I2SCFGR_t *)0x4001301C;

 // SPI1 
volatile I2SPR_t *SPI1_I2SPR = (volatile I2SPR_t *)0x40013020;
