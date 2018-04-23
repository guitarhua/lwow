/**
 * \file            ow.h
 * \brief           OneWire-UART library
 */
 
/*
 * Copyright (c) 2018 Tilen Majerle
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
 * This file is part of OneWire library.
 *
 * Author:          Tilen MAJERLE <tilen@majerle.eu>
 */
#ifndef __ONEWIRE_H
#define __ONEWIRE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stdint.h"
#include "stddef.h"

#include "ow_config.h"

/**
 * \defgroup        OW API functions
 * \brief           OneWire API
 * \{
 */

/**
 * \brief			OneWire result enumeration
 */
typedef enum {
    owOK = 0x00,                                /*!< Device returned OK */
    owERRPRESENCE = -1,                         /*!< Presence was not successful */
    owERRNODEV = -2,                            /*!< No device connected, maybe device removed during scan? */
    owERR,                                      /*!< General-Purpose error */
} owr_t;

/**
 * \brief           1-Wire structure
 */
typedef struct {    
#if OW_CFG_OS || __DOXYGEN__
    OW_CFG_OS_MUTEX_HANDLE mutex;               /*!< Mutex handle */
#endif /* OW_USE_RTOS || __DOXYGEN__ */
    
    uint8_t rom[8];                             /*!< ROM address of last device found.
                                                     When searching for new devices, we always need last found address,
                                                     to be able to decide which way to go next time during scan. */
    uint8_t disrepancy;                         /*!< Disrepancy value on last search */
	void* arg;                                  /*!< User custom argument */
} ow_t;

#define OW_UNUSED(x)                ((void)(x))
    
#define OW_CMD_RSCRATCHPAD          0xBE
#define OW_CMD_WSCRATCHPAD          0x4E
#define OW_CMD_CPYSCRATCHPAD        0x48
#define OW_CMD_RECEEPROM            0xB8
#define OW_CMD_RPWRSUPPLY           0xB4
#define OW_CMD_SEARCHROM            0xF0
#define OW_CMD_READROM              0x33
#define OW_CMD_MATCHROM             0x55
#define OW_CMD_SKIPROM              0xCC

owr_t       ow_init(ow_t* ow, void* arg);

owr_t       ow_protect(ow_t* ow);
owr_t       ow_unprotect(ow_t* ow);

owr_t       ow_reset(ow_t* ow);
uint8_t     ow_write_byte(ow_t* ow, uint8_t b);
uint8_t     ow_read_byte(ow_t* ow);
uint8_t     ow_read_bit(ow_t* ow);

owr_t       ow_search_reset(ow_t* ow);
owr_t       ow_search(ow_t* ow, uint8_t *id);

uint8_t     ow_match_rom(ow_t* ow, uint8_t* rom_id);
uint8_t     ow_skip_rom(ow_t* ow);
uint8_t     ow_crc(const void *in, size_t len);

/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ONEWIRE_H__ */