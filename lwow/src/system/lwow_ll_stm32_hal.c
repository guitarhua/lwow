/**
 * \file            lwow_ll_stm32_hal.c
 * \brief           UART driver implementation for STM32 with HAL code
 */

/*
 * Copyright (c) 2020 Tilen MAJERLE
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of LwOW - Lightweight onewire library.
 *
 * Author:          Tilen MAJERLE <tilen@majerle.eu>
 * Version:         v2.0.0
 */

/*
 * How it works (general)
 *
 * https://docs.majerle.eu/projects/lwow/en/latest/user-manual/hw_connection.html#
 *
 * This specific driver is optimized for proejcts generated by STM32CubeMX or STM32CubeIDE with HAL drivers
 * It can be used w/ or w/o operating system and it uses interrupts & polling for data receive and data transmit.
 *
 * Application must pass pointer to UART handle as argument to ow_init function in order
 * to link OW instance with actual UART hardware used for OW instance.
 *
 * To use this driver, application must:
 * - Enable interrupt in CubeMX to allow HAL_UART_Receive_IT functionality
 * - Use pointer to UART handle when initializing ow with ow_init
 */
#include "lwow/lwow.h"
#include "main.h"                               /* Generated normally by CubeMX */

#if !__DOXYGEN__

static uint8_t init(void* arg);
static uint8_t deinit(void* arg);
static uint8_t set_baudrate(uint32_t baud, void* arg);
static uint8_t transmit_receive(const uint8_t* tx, uint8_t* rx, size_t len, void* arg);

/* STM32 LL driver for OW */
const lwow_ll_drv_t
lwow_ll_drv_stm32_hal = {
    .init = init,
    .deinit = deinit,
    .set_baudrate = set_baudrate,
    .tx_rx = transmit_receive,
};

static uint8_t
init(void* arg) {
    UART_HandleTypeDef* huart = arg;

    LWOW_ASSERT0("arg != NULL", arg != NULL);

    /* Initialize UART */
    HAL_UART_DeInit(huart);
    return HAL_UART_Init(huart) == HAL_OK;
}

static uint8_t
deinit(void* arg) {
    UART_HandleTypeDef* huart = arg;

    LWOW_ASSERT0("arg != NULL", arg != NULL);

    return HAL_UART_DeInit(huart);
}

static uint8_t
set_baudrate(uint32_t baud, void* arg) {
    UART_HandleTypeDef* huart = arg;

    LWOW_ASSERT0("arg != NULL", arg != NULL);

    huart->Init.BaudRate = baud;
    return init(huart);
}

static uint8_t
transmit_receive(const uint8_t* tx, uint8_t* rx, size_t len, void* arg) {
    UART_HandleTypeDef* huart = arg;
    uint32_t start;

    LWOW_ASSERT0("arg != NULL", arg != NULL);

    /* Get current HAL tick */
    start = HAL_GetTick();

    /* Start RX in interrupt mode */
    HAL_UART_Receive_IT(huart, rx, len);

    /* Process TX in polling mode */
    HAL_UART_Transmit(huart, (void*)tx, len, 100);

    /* Wait RX to finish */
    while (huart->RxState != HAL_UART_STATE_READY) {
        if (HAL_GetTick() - start > 100) {
            return 0;
        }
    }

    return 1;
}

#endif /* !__DOXYGEN__ */