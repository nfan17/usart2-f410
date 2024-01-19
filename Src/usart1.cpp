#include "usart1.hpp"

constexpr uint32_t baud = 115200;

void usartInit(void) {

    // GPIO AF, No pupdr, high speed, AF7 | PA9 TX, PA10 RX
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER &= ~(GPIO_MODER_MODE9 | GPIO_MODER_MODE10);
    GPIOA->MODER |= GPIO_MODER_MODE9_1 | GPIO_MODER_MODE10_1;

    GPIOA->OTYPER &= (GPIO_OTYPER_OT9 | GPIO_OTYPER_OT10);
    
    GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED9 | GPIO_OSPEEDR_OSPEED10);
    GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED9_1 | GPIO_OSPEEDR_OSPEED10_1;
    
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD9 | GPIO_PUPDR_PUPD10);
    
    GPIOA->AFR[1] |= GPIO_AFRH_AFSEL9 | GPIO_AFRH_AFSEL10;
    GPIOA->AFR[1] &= ~(GPIO_AFRH_AFSEL9_3 | GPIO_AFRH_AFSEL10_3);


    // UART
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    /*
     * TIMING (standard)
     * BAUD = Fclk / (8 * (2 - OVER8) * USARTDIV)
     * when OVER8 = 0, fraction is 4 bits in DIV_FRACTION[3:0] in BRR
     * when OVER8 = 1, it is 3 bits DIV_FRACTION[2:0] and [3] must be cleared
     * fraction = DIV_FRACTION / (OVER8 * 8), so for OVER8 = 0 DIV_FRACTION / 16
     * 
     */
    USART1->CR1 &= ~(USART_CR1_OVER8 | USART_CR1_M);

    uint16_t uartdiv = SystemCoreClock / baud;

    USART1->BRR = ( ( ( uartdiv / 16 ) << USART_BRR_DIV_Mantissa_Pos ) |
                    ( ( uartdiv % 16 ) << USART_BRR_DIV_Fraction_Pos ) );

    USART1->CR3 |= USART_CR3_EIE;
    USART1->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;

}

void usartSend(uint8_t* data, uint16_t len) {

    USART1->CR1 |= USART_CR1_TE;

    for (uint16_t i = 0; i < len; ++i) {
        
        while (!(USART1->SR & USART_SR_TXE));
        USART1->DR = static_cast<uint8_t>(data[i]);

    }

    while(!(USART1->SR & USART_SR_TC));

    // USART2->CR1 &= ~USART_CR1_TE; // I think stmHAL keeps TE 1 at all times tbh

}

uint8_t usartReadString(uint8_t * data, uint8_t end_char, uint16_t max_size, uint16_t max_tries) {
    USART1->CR1 |= USART_CR1_RE;

    uint16_t i, j = 0;
    while (i < max_size && j++ <= max_tries) {
        if (USART1->SR & USART_SR_RXNE) {
            j = 0;
            data[i] = USART1->DR;
            if (data[i] == end_char) {
                data[i] = '\0';
                return 1;
            }
            i++;
        }
    }
    return 0;
}

void usartSendString(char *data) {
    
    USART1->CR1 |= USART_CR1_TE;

    int cont = 1;
    while(cont) {
        
        if (*data == '\0') {
            cont = 0;
        }
        while (!(USART1->SR & USART_SR_TXE));
        USART1->DR = static_cast<uint8_t>(*(data++));

    }

    while(!(USART1->SR & USART_SR_TC));

}