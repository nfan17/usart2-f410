
#pragma once


#ifndef USART2_H
#define USART2_H


#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"
#include <stdint.h>


uint32_t calculateUartDiv(uint32_t pclk, uint32_t baudrate, uint8_t osrBit);
void usart2Init(void);
void usart2Send(uint8_t* data, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif // USART2_H
