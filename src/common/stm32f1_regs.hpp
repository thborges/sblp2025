/*
 * Part of the Robotics Language Standard Library, under the
 * Apache License v2.0.
 */

#pragma once

#include "cppdeps.h"

/*
 * Global Registers
 */

struct SYSTICK_CTRL_t {
	uint32_t enable:1;
	uint32_t tickint:1;
	uint32_t clksource:1;
	uint32_t _res1:5;
	uint32_t _res2:8;
	uint32_t countflag:16;
};

const uint32_t SCS_BASE = 0xE000E000;
extern volatile SYSTICK_CTRL_t *SYSTICK_CTRL;
extern volatile uint32_t *SYSTICK_LOAD;
extern volatile uint32_t *SYSTICK_VAL;
extern volatile uint32_t *SYSTICK_CALIB;

struct AHBENR_t {
	uint32_t dma1:1;		// DMA1 clock enable
	uint32_t dma2:1;		// DMA2 clock enable
	uint32_t sram:1;		// SRAM interface clock enable
	uint32_t _res1:1;
	uint32_t flitf:1;		// FLITF clock enable
	uint32_t _res2:1;
	uint32_t crc:1;		// CRC clock enable
	uint32_t _res3:1;
	uint32_t fsmc:1;		// FSMC clock enable
	uint32_t _res4:1;
	uint32_t sdio:1;		// SDIO clock enable
	uint32_t _res5:5;
	uint32_t _res6:16;
};

struct APB2EN_t {
	uint32_t afio:1;	// Alternate function I/O clock enable
	uint32_t _res1:1;	
	uint32_t iopa:1;	// I/O port A clock enable
	uint32_t iopb:1;	// I/O port B clock enable
	uint32_t iopc:1;	// I/O port C clock enable
	uint32_t iopd:1;	// I/O port D clock enable
	uint32_t iope:1;	// I/O port E clock enable
	uint32_t iopf:1;	// I/O port F clock enable
	uint32_t iopg:1;	// I/O port G clock enable
	uint32_t adc1:1;	// ADC1 interface clock enable
	uint32_t adc2:1;	// ADC2 interface clock enable
	uint32_t tim1:1;	// TIM1 Timer clock enable
	uint32_t spi1:1;   // SPI1 clock enable
	uint32_t tim8:1;   // TIM8 Timer clock enable
	uint32_t usart1:1; // USART1 clock enable
	uint32_t adc3:1;	// ADC3 interface clock enable
	uint32_t _res2:16;
};

struct APB1EN_t {
	uint32_t tim2:1;		// Timer 2 clock enable
	uint32_t tim3:1;		// Timer 3 clock enable
	uint32_t tim4:1;		// Timer 4 clock enable
	uint32_t tim5:1;		// Timer 5 clock enable
	uint32_t tim6:1;		// Timer 6 clock enable
	uint32_t tim7:1;		// Timer 7 clock enable
	uint32_t _res1:5;
	uint32_t wwdg:1;		// Window watchdog clock enable
	uint32_t _res2:2;
	uint32_t spi2:1;		// SPI2 clock enable
	uint32_t spi3:1;		// SPI3 clock enable
	uint32_t _res3:1;
	uint32_t usart2:1;		// USART 2 clock enable
	uint32_t usart3:1;		// USART 3 clock enable
	uint32_t usart4:1;		// USART 4 clock enable
	uint32_t usart5:1;		// USART 5 clock enable
	uint32_t i2c1:1;		// I2C 1 clock enable
	uint32_t i2c2:1;		// I2C 2 clock enable
	uint32_t usb:1;		// USB clock enable
	uint32_t _res4:1;		
	uint32_t can:1;		// CAN clock enable
	uint32_t _res5:1;
	uint32_t bkp:1;		// Backup interface clock enable
	uint32_t pwr:1;		// Power interface clock enable
	uint32_t dac:1;		// DAC interface clock enable
	uint32_t _res6:2;
};

// AHB peripheral clock enable register 
extern volatile AHBENR_t *AHBENR;
// APB1 peripheral clock enable register
extern volatile APB1EN_t *APB1EN;
// APB2 peripheral clock enable register
extern volatile APB2EN_t *APB2EN;


// APB1 peripheral reset register
struct APB1RSTR_t {
	uint32_t tim2rst:1;
	uint32_t tim3rst:1;
	uint32_t tim4rst:1;
	uint32_t tim5rst:1;
	uint32_t tim6rst:1;
	uint32_t tim7rst:1;
	uint32_t _res1:5;
	uint32_t wwdgrst:1;
	uint32_t _res2:2;
	uint32_t spi2rst:1;
	uint32_t spi3rst:1;
	uint32_t _res3:1;
	uint32_t usart2rst:1;
	uint32_t usart3rst:1;
	uint32_t uart4rst:1;
	uint32_t uart5rst:1;
	uint32_t i2c1rst:1;
	uint32_t i2c2rst:1;
	uint32_t usbrst:1;
	uint32_t _res4:1;
	uint32_t canrst:1;
	uint32_t _res5:1;
	uint32_t bkprst:1;
	uint32_t pwrrst:1;
	uint32_t dacrst:1;
	uint32_t _res6:2;
};

extern volatile APB1RSTR_t *APB1RSTR;

/*struct GPIO_t {
	uint32_t CRL;      // GPIO port configuration register low,      Address offset: 0x00
	uint32_t CRH;      // GPIO port configuration register high,     Address offset: 0x04
	uint32_t IDR;      // GPIO port input data register,             Address offset: 0x08
	uint32_t ODR;      // GPIO port output data register,            Address offset: 0x0C
	uint32_t BSRR;     // GPIO port bit set/reset register,          Address offset: 0x10
	uint32_t BRR;      // GPIO port bit reset register,              Address offset: 0x14
	uint32_t LCKR;     // GPIO port configuration lock register,     Address offset: 0x18
}*/

struct RCC_t {
	uint32_t cr      ; // RCC clock control register,                Address offset: 0x00
	uint32_t cfgr    ; // RCC clock configuration register,          Address offset: 0x04
	uint32_t cir     ; // RCC clock interrupt register,              uint32_t Address offset: 0x08
	uint32_t apb2rstr; // RCC APB2 peripheral reset register,        uint32_t Address offset: 0x0C
	uint32_t apb1rstr; // RCC APB1 peripheral reset register,        uint32_t Address offset: 0x10
	uint32_t ahbenr  ; // RCC AHB peripheral clock enable register,  uint32_t Address offset: 0x14
	uint32_t apb2enr ; // RCC APB2 peripheral clock enable register, uint32_t Address offset: 0x18
	uint32_t apb1enr ; // RCC APB1 peripheral clock enable register, uint32_t Address offset: 0x1C
	uint32_t bdcr    ; // RCC backup domain control register,        uint32_t Address offset: 0x20
	uint32_t csr     ; // RCC control/status register,               uint32_t Address offset: 0x24
	uint32_t ahbrstr ; // RCC AHB peripheral clock reset register,   uint32_t Address offset: 0x28
	uint32_t cfgr2   ; // RCC clock configuration register2,         Address offset: 0x2C
};

struct RCC_CR_t {
	uint32_t hsi_on:1;
	uint32_t hsi_rdy:1;
	uint32_t _res1:1;
	uint32_t hsi_trim:5;
	uint32_t hsi_call:8;
	uint32_t hse_on:1;
	uint32_t hse_rdy:1;
	uint32_t hse_byp:1;
	uint32_t css_on:1;
	uint32_t _res2:4;
	uint32_t pll_on:1;
	uint32_t pll_rdy:1;
	uint32_t _res3:6;
};

struct RCC_CFGR_t {
	uint32_t sw:2;		// System clock switch
	uint32_t sws:2;
	uint32_t hpre:4;
	uint32_t ppre1:3;
	uint32_t ppre2:3;
	uint32_t adc_pre:2;
	uint32_t pll_src:1;
	uint32_t pll_xtpre:1;
	uint32_t pll_mul:4;
	uint32_t usb_pre:1;
	uint32_t _res1:1;
	uint32_t mco:3;
	uint32_t _res2:5;
};

//
extern volatile RCC_t *RCC;
// Clock control register
extern volatile RCC_CR_t *RCC_CR;
// Clock configuration register
extern volatile RCC_CFGR_t *RCC_CFGR;


struct FLASH_t {
  uint32_t acr    ;
  uint32_t keyr   ;
  uint32_t optkeyr;
  uint32_t sr     ;
  uint32_t cr     ;
  uint32_t ar     ;
  uint32_t _res   ;
  uint32_t obr    ;
  uint32_t wrpr   ;
};

extern volatile FLASH_t *FLASH;

/*
 * PORT registers
 */

// PORT x Control Register Low (x=A..G)
struct GPIOx_CRL_t {
	uint32_t mode0:2;
	uint32_t cnf0 :2;
	uint32_t mode1:2;
	uint32_t cnf1 :2;
	uint32_t mode2:2;
	uint32_t cnf2 :2;
	uint32_t mode3:2;
	uint32_t cnf3 :2;
	uint32_t mode4:2;
	uint32_t cnf4 :2;
	uint32_t mode5:2;
	uint32_t cnf5 :2;
	uint32_t mode6:2;
	uint32_t cnf6 :2;
	uint32_t mode7:2;
	uint32_t cnf7 :2;
};

// PORT x Control Register High (x=A..G)
struct GPIOx_CRH_t {
	uint32_t mode8 :2;
	uint32_t cnf8  :2;
	uint32_t mode9 :2;
	uint32_t cnf9  :2;
	uint32_t mode10:2;
	uint32_t cnf10 :2;
	uint32_t mode11:2;
	uint32_t cnf11 :2;
	uint32_t mode12:2;
	uint32_t cnf12 :2;
	uint32_t mode13:2;
	uint32_t cnf13 :2;
	uint32_t mode14:2;
	uint32_t cnf14 :2;
	uint32_t mode15:2;
	uint32_t cnf15 :2;
};

// Port x output data register (x=A..G)
struct GPIOx_ODR_t {
	uint32_t odr0:1;
	uint32_t odr1:1;
	uint32_t odr2:1;
	uint32_t odr3:1;
	uint32_t odr4:1;
	uint32_t odr5:1;
	uint32_t odr6:1;
	uint32_t odr7:1;
	uint32_t odr8:1;
	uint32_t odr9:1;
	uint32_t odr10:1;
	uint32_t odr11:1;
	uint32_t odr12:1;
	uint32_t odr13:1;
	uint32_t odr14:1;
	uint32_t odr15:1;
	uint32_t _res:16;
};

// Port x input data register (x=A..G)
struct GPIOx_IDR_t {
	uint32_t idr0:1;
	uint32_t idr1:1;
	uint32_t idr2:1;
	uint32_t idr3:1;
	uint32_t idr4:1;
	uint32_t idr5:1;
	uint32_t idr6:1;
	uint32_t idr7:1;
	uint32_t idr8:1;
	uint32_t idr9:1;
	uint32_t idr10:1;
	uint32_t idr11:1;
	uint32_t idr12:1;
	uint32_t idr13:1;
	uint32_t idr14:1;
	uint32_t idr15:1;
	uint32_t _res:16;
};


// GPIO PORT A base address
const uint32_t GPIO_PORTA_BASE = 0x40010800;
// PORT A Control Register Low
extern volatile GPIOx_CRL_t *GPIOA_CRL;
// PORT A Control Register High
extern volatile GPIOx_CRH_t *GPIOA_CRH;
// PORT A Input Data Register
extern volatile GPIOx_IDR_t *GPIOA_IDR;
// PORT A Output Data Register
extern volatile GPIOx_ODR_t *GPIOA_ODR;


// GPIO PORT B base address
const uint32_t GPIO_PORTB_BASE = 0x40010C00;
// PORT B Control Register Low
extern volatile GPIOx_CRL_t *GPIOB_CRL;
// PORT B Control Register High
extern volatile GPIOx_CRH_t *GPIOB_CRH;
// PORT B Input Data Register
extern volatile GPIOx_IDR_t *GPIOB_IDR;
// PORT B Output Data Register
extern volatile GPIOx_ODR_t *GPIOB_ODR;


// GPIO PORT C base address
const uint32_t GPIO_PORTC_BASE = 0x40011000;
// PORT C Control Register Low
extern volatile GPIOx_CRL_t *GPIOC_CRL;
// PORT C Control Register High
extern volatile GPIOx_CRH_t *GPIOC_CRH;
// PORT C Input Data Register
extern volatile GPIOx_IDR_t *GPIOC_IDR;
// PORT C Output Data Register
extern volatile GPIOx_ODR_t *GPIOC_ODR;


enum PIN_MODE {INPUT = 0x0, OUTPUT10MHz = 0x1, OUTPUT2MHz = 0x2, OUTPUT50MHz = 0x3 };
enum INPUT_MODE {ANALOG = 0x0, FLOATING = 0x1, PULLUPDOWN = 0x2 };
enum OUTPUT_MODE {PUSHPULL = 0x0, OPENDRAIN = 0x1, APUSH_PULL = 0x2, AOPENDRAIN = 0x3 };


/*
 * REMAP registers
 */

struct MAPR_t {
	uint32_t SPI1_REMAP        :1;              // 
	uint32_t I2C1_REMAP        :1;              // 
	uint32_t USART1_REMAP      :1;              // 
	uint32_t USART2_REMAP      :1;              // 
	uint32_t USART3_REMAP      :2;           // 
	uint32_t TIM1_REMAP        :2;           // 
	uint32_t TIM2_REMAP        :2;           // 
	uint32_t TIM3_REMAP        :2;           // 
	uint32_t TIM4_REMAP        :1;              // 
	uint32_t CAN_REMAP         :2;           // 
	uint32_t PD01_REMAP        :1;              // 
	uint32_t TIM5CH4_IREMAP    :1;              // 
	uint32_t ADC1_ETRGINJ_REMAP:1;              // 
	uint32_t ADC1_ETRGREG_REMAP:1;              // 
	uint32_t ADC2_ETRGINJ_REMAP:1;              // 
	uint32_t ADC2_ETRGREG_REMAP:1;              // 
	uint32_t _res1             :3;           // Reserved, 3 bits
	uint32_t SWJ_CFG           :3;           // 
	uint32_t _res2             :5;           // Reserved, 5 bits
 };

 // 
 extern volatile MAPR_t *MAPR;


/*
 * I2C Registers
 */

struct I2C_CR1_t {
	uint32_t pe:1;		   // peripheral enable
	uint32_t smbus:1;	   // true = SMBus mode = true, false = I2C
	uint32_t _res1:1;
	uint32_t smbtype:1;   // false = SMBus device, SMBus Host
	uint32_t enarp:1;     // ARP enable
	uint32_t enpec:1;     // PEC enable
	uint32_t engc:1;      // General call enable
	uint32_t nostretch:1; // Clock stretching disable (0=enabled)
	uint32_t start:1;     // Start generation
	uint32_t stop:1;      // Stop generation
	uint32_t ack:1;       // Acknowledge enable
	uint32_t pos:1;       // Acknowledge/PEC Position (for data reception)
	uint32_t pec:1;       // Packet error checking
	uint32_t alert:1;     // SMBus alert
	uint32_t _res2:1;
	uint32_t swrst:1;     // Software reset
};

struct I2C_CR2_t {
	uint16_t freq:6;	// Peripheral clock frequency (in Mhz, from 2 to 36)
	uint16_t _res1:2;
	uint16_t iterren:1;	// Error interrupt enable
	uint16_t itevten:1;	// Event interrupt enable
	uint16_t itbufen:1;	// Buffer interrupt enable
	uint16_t dmaen:1;		// DMA requests enable
	uint16_t last:1;		// DMA last transfer
	uint16_t _res2:3;
};

struct I2C_SR1_t {
	uint16_t sb:1;			// Start bit (Master mode)
	uint16_t addr:1;		// Address sent (mastermode) / matched (slave mode)
	uint16_t btf:1;		// Byte transfer finished
	uint16_t add10:1;		// 10-bit header sent (Master mode)
	uint16_t stopf:1;		// Stop detection (slavemode)
	uint16_t _res1:1;
	uint16_t rxne:1;		// Data register not empty (receivers)
	uint16_t txe:1;		// Data register empty (transmitters)
	uint16_t berr:1;		// Bus error
	uint16_t arlo:1;		// Arbitration lost (master mode)
	uint16_t af:1;			// Acknowledge failure
	uint16_t ovr:1;		// Overrun/Underrun
	uint16_t pecerr:1;		// PEC Error in reception
	uint16_t _res2:1;
	uint16_t timeout:1;	// Timeout or Tlow error
	uint16_t smbalert:1;	// SMBus alert
};

struct I2C_SR2_t {
	uint16_t msl:1;			// Master/slave
	uint16_t busy:1;			// Bus busy
	uint16_t tra:1;			// Transmitter/receiver false=data bytes received, true=data bytes transmited
	uint16_t _res:1;
	uint16_t gencall:1;		// General call address (Slave mode)
	uint16_t smbdefault:1;		// SMBus device default address (Slave mode)
	uint16_t smbhost:1;		// SMBus host header (Slave mode)
	uint16_t dualf:1;			// Dual flag (Slave mode)
	uint16_t pec:8;			// Packet error checking register
};

struct I2C_CCR_t {
	uint16_t ccr:12;	// Clock control register in Fast/Standard mode (Master mode)
	uint16_t _res:2;
	uint16_t duty:1;		// Fast mode duty cycle 
	uint16_t fs:1;			// I2C master mode selection false=Standard, true=Fast
};


// I2C 1 Control 1 Register
extern volatile I2C_CR1_t *I2C1_CR1;
// I2C 1 Control 2 Register
extern volatile I2C_CR2_t *I2C1_CR2;
// I2C 1 Data Register (8 bits)
extern volatile uint16_t *I2C1_DR;
// I2C 1 Status 1 Register
extern volatile I2C_SR1_t *I2C1_SR1;
// I2C 1 Status 2 Register
extern volatile I2C_SR2_t *I2C1_SR2;
// I2C 1 Clock control register
extern volatile I2C_CCR_t *I2C1_CCR;
// I2C 1 T RISE register
extern volatile uint16_t *I2C1_TRISE;

// I2C 2 Control 1 Register
extern volatile I2C_CR1_t *I2C2_CR1;
// I2C 2 Control 2 Register
extern volatile I2C_CR2_t *I2C2_CR2;
// I2C 2 Data Register (8 bits)
extern volatile uint16_t *I2C2_DR;
// I2C 2 Status 1 Register
extern volatile I2C_SR1_t *I2C2_SR1;
// I2C 2 Status 2 Register
extern volatile I2C_SR2_t *I2C2_SR2;
// I2C 2 Clock control register
extern volatile I2C_CCR_t *I2C2_CCR;
// I2C 2 T RISE register
extern volatile uint16_t *I2C2_TRISE;

/*
 * USART types
 */

 struct SR_t {
	uint32_t PE   :1;              // Parity error
	uint32_t FE   :1;              // Framing error
	uint32_t NE   :1;              // Noise error flag
	uint32_t ORE  :1;              // Overrun error
	uint32_t IDLE :1;              // IDLE line detected
	uint32_t RXNE :1;              // Read data register not empty
	uint32_t TC   :1;              // Transmission complete
	uint32_t TXE  :1;              // Transmit data register empty
	uint32_t LBD  :1;              // LIN break detection flag
	uint32_t CTS  :1;              // CTS flag
	uint32_t _res1:16;          // Reserved, 22 bits
	uint32_t _res2:6;           // 
 };

 struct CR1_t {
	uint32_t SBK   :1;              // Send break
	uint32_t RWU   :1;              // Receiver wakeup
	uint32_t RE    :1;              // Receiver enable
	uint32_t TE    :1;              // Transmitter enable
	uint32_t IDLEIE:1;              // IDLE interrupt enable
	uint32_t RXNEIE:1;              // RXNE interrupt enable
	uint32_t TCIE  :1;              // Transmission complete interrupt enable
	uint32_t TXEIE :1;              // TXE interrupt enable
	uint32_t PEIE  :1;              // PE interrupt enable
	uint32_t PS    :1;              // Parity selection
	uint32_t PCE   :1;              // Parity control enable
	uint32_t WAKE  :1;              // Wakeup method
	uint32_t M     :1;              // Word length
	uint32_t UE    :1;              // USART enable
	uint32_t _res1 :16;          // Reserved, 18 bits
	uint32_t _res2 :2;           // 
 };

 struct DR_t {
	uint32_t DR   :9;           // Data value
	uint32_t _res1:16;          // Reserved, 23 bits
	uint32_t _res2:7;           // 
 };

 struct BRR_t {
	uint32_t DIV_Fraction:4;           // fraction of USARTDIV
	uint32_t DIV_Mantissa:12;          // mantissa of USARTDIV
	uint32_t _res1       :16;          // Reserved, 16 bits
 };

 // Status register
 extern volatile SR_t *USART2_SR;

 // Data register
 extern volatile DR_t *USART2_DR;

 // Baud rate register
 extern volatile BRR_t *USART2_BRR;

 // Control register 1
 extern volatile CR1_t *USART2_CR1;


/* Types and registers for SPI1: Serial peripheral interface
 */

 struct SPI_CR1_t {
	uint32_t CPHA    :1;              // 
	uint32_t CPOL    :1;              // 
	uint32_t MSTR    :1;              // 
	uint32_t BR      :3;           // 
	uint32_t SPE     :1;              // 
	uint32_t LSBFIRST:1;              // 
	uint32_t SSI     :1;              // 
	uint32_t SSM     :1;              // 
	uint32_t RXONLY  :1;              // 
	uint32_t DFF     :1;              // 
	uint32_t CRCNEXT :1;              // 
	uint32_t CRCEN   :1;              // 
	uint32_t BIDIOE  :1;              // 
	uint32_t BIDIMODE:1;              // 
	uint32_t _res1   :16;          // Reserved, 16 bits
 };

 // SPI1 
 extern volatile SPI_CR1_t *SPI1_CR1;

 struct CR2_t {
	uint32_t RXDMAEN:1;              // 
	uint32_t TXDMAEN:1;              // 
	uint32_t SSOE   :1;              // 
	uint32_t _res1  :2;           // Reserved, 2 bits
	uint32_t ERRIE  :1;              // 
	uint32_t RXNEIE :1;              // 
	uint32_t TXEIE  :1;              // 
	uint32_t _res2  :16;          // Reserved, 24 bits
	uint32_t _res3  :8;           // 
 };

 // SPI1 
 extern volatile CR2_t *SPI1_CR2;

 struct SPI_SR_t {
	uint32_t RXNE  :1;              // 
	uint32_t TXE   :1;              // 
	uint32_t CHSIDE:1;              // 
	uint32_t UDR   :1;              // 
	uint32_t CRCERR:1;              // 
	uint32_t MODF  :1;              // 
	uint32_t OVR   :1;              // 
	uint32_t BSY   :1;              // 
	uint32_t _res1 :16;          // Reserved, 24 bits
	uint32_t _res2 :8;           // 
 };

 // SPI1 
 extern volatile SPI_SR_t *SPI1_SR;

 struct SPI_DR_t {
	uint32_t DR   :16;          // 
	uint32_t _res1:16;          // Reserved, 16 bits
 };

 // SPI1 
 extern volatile SPI_DR_t *SPI1_DR;

 struct CRCPR_t {
	uint32_t CRCPOLY:16;          // 
	uint32_t _res1  :16;          // Reserved, 16 bits
 };

 // SPI1 
 extern volatile CRCPR_t *SPI1_CRCPR;

 struct RXCRCR_t {
	uint32_t RxCRC:16;          // 
	uint32_t _res1:16;          // Reserved, 16 bits
 };

 // SPI1 
 extern volatile RXCRCR_t *SPI1_RXCRCR;

 struct TXCRCR_t {
	uint32_t TxCRC:16;          // 
	uint32_t _res1:16;          // Reserved, 16 bits
 };

 // SPI1 
 extern volatile TXCRCR_t *SPI1_TXCRCR;

 struct I2SCFGR_t {
	uint32_t CHLEN  :1;              // 
	uint32_t DATLEN :2;           // 
	uint32_t CKPOL  :1;              // 
	uint32_t I2SSTD :2;           // 
	uint32_t _res1  :1;              // Reserved, 1 bits
	uint32_t PCMSYNC:1;              // 
	uint32_t I2SCFG :2;           // 
	uint32_t I2SE   :1;              // 
	uint32_t I2SMOD :1;              // 
	uint32_t _res2  :16;          // Reserved, 20 bits
	uint32_t _res3  :4;           // 
 };

 // SPI1 
 extern volatile I2SCFGR_t *SPI1_I2SCFGR;

 struct I2SPR_t {
	uint32_t I2SDIV:8;           // 
	uint32_t ODD   :1;              // 
	uint32_t MCKOE :1;              // 
	uint32_t _res1 :16;          // Reserved, 22 bits
	uint32_t _res2 :6;           // 
 };

 // SPI1 
 extern volatile I2SPR_t *SPI1_I2SPR;
