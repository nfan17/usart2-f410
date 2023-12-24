

#include "usart2.hpp"

constexpr uint32_t baud = 115200;

uint32_t calculateUartDiv(uint32_t pclk, uint32_t baudrate, uint8_t osrBit) {

    uint32_t temp = static_cast<uint32_t> (
        static_cast<uint64_t>(pclk) * 25U / (2U * (2U - osrBit) * static_cast<uint64_t>(baudrate))
    );

    uint32_t mantissa = temp / 100U;
    uint32_t fraction = (((temp - (mantissa * 100U)) * (8U * (2U - osrBit))) + 50U) / 100U;

    if (osrBit == 0) { // osr = 16
        fraction &= 0xFFU;
    }
    else { // osr = 8
        fraction = (fraction & 0xF8U) << 1U | (fraction & 0x7U);
    }

    return (mantissa << 4U) | (fraction);
}

void usart2Init(void) {

    // GPIO AF, No pupdr, high speed, AF7 | PA2, PA3
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER &= ~(GPIO_MODER_MODE2 | GPIO_MODER_MODE3);
    GPIOA->MODER |= GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1;

    GPIOA->OTYPER &= (GPIO_OTYPER_OT2 | GPIO_OTYPER_OT3);
    
    GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED2 | GPIO_OSPEEDR_OSPEED3);
    GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED2_1 | GPIO_OSPEEDR_OSPEED3_1;
    
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD2 | GPIO_PUPDR_PUPD3);
    
    GPIOA->AFR[0] |= GPIO_AFRL_AFRL2 | GPIO_AFRL_AFRL3;
    GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL2_3 | GPIO_AFRL_AFRL3_3);


    // UART
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    /*
     * TIMING (standard)
     * BAUD = Fclk / (8 * (2 - OVER8) * USARTDIV)
     * when OVER8 = 0, fraction is 4 bits in DIV_FRACTION[3:0] in BRR
     * when OVER8 = 1, it is 3 bits DIV_FRACTION[2:0] and [3] must be cleared
     * fraction = DIV_FRACTION / (OVER8 * 8), so for OVER8 = 0 DIV_FRACTION / 16
     * 
     */
    USART2->CR1 &= ~(USART_CR1_OVER8 | USART_CR1_M);

    uint8_t osr = USART2->CR1 & USART_CR1_OVER8; // over sampling rate
    uint32_t pclk = SystemCoreClock >> APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1)>> RCC_CFGR_PPRE1_Pos];

    USART2->BRR |= calculateUartDiv(pclk, baud, osr);

    USART2->CR3 |= USART_CR3_EIE;
    USART2->CR1 |= USART_CR1_UE;
}

void usart2Send(uint8_t* data, uint16_t len) {

    USART2->CR1 |= USART_CR1_TE;

    for (uint16_t i = 0; i < len; ++i) {
        
        while (!(USART2->SR & USART_SR_TXE));
        USART2->DR |= static_cast<uint8_t>(data[i]);

    }

    while(!(USART2->SR & USART_SR_TC));

    USART2->CR1 &= ~USART_CR1_TE; // I think stmHAL keeps TE 1 at all times tbh

}

