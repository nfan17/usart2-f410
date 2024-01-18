
#pragma once


#ifndef USART2_H
#define USART2_H


#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"
// #include "stm32f410rx.h"
#include <stdint.h>


uint32_t calculateUartDiv(uint32_t pclk, uint32_t baudrate, uint8_t osrBit);
void usart2Init(void);
void usart2Send(uint8_t* data, uint16_t len);
void usart2ReadString(uint8_t * data, uint8_t end_char, uint16_t max_size, uint32_t max_tries);
void usart2SendString(char* data);

#ifdef __cplusplus
}
#endif

#endif // USART2_H
