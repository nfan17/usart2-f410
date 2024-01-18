
#pragma once


#ifndef USART1_H
#define USART1_H


#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"
#include "stm32f410rx.h"
#include <stdint.h>


uint32_t calculateUartDiv(uint32_t pclk, uint32_t baudrate, uint8_t osrBit);
void usartInit(void);
// bool sendByte(uint8_t* data); // come back to this later (low layer interface)
// bool readByte(uint8_t* data);
void usartSend(uint8_t* data, uint16_t len);
void usartReadString(uint8_t * data, uint8_t end_char, uint16_t max_size, uint32_t max_tries);
void usartSendString(char* data);

#ifdef __cplusplus
}
#endif

#endif // USART1_H
