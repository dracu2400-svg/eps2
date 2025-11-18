/*
 * tlv.h - MSP430 TLV (Tag-Length-Value) HAL Compatibility Stub
 * 
 * Copyright (C) 2019, SpaceLab.
 */

#ifndef HAL_TLV_H_
#define HAL_TLV_H_

#include <stdint.h>

/* TLV tags */
#define TLV_TAG_Reserved1   0x01
#define TLV_TAG_Reserved2   0x02
#define TLV_TAG_Reserved3   0x03
#define TLV_TAG_Reserved4   0x04
#define TLV_TAG_LDTAG       0x08
#define TLV_TAG_PDTAG       0x10
#define TLV_TAG_Reserved5   0x11
#define TLV_TAG_Reserved6   0x12
#define TLV_TAG_BLANK       0xFE

/* Peripheral descriptors */
#define TLV_DEVICE_ID_0     0x1A04
#define TLV_DEVICE_ID_1     0x1A05

/* Stub function prototypes */
static inline void TLV_getInfo(uint8_t tag, uint8_t instance, uint8_t *length, uint16_t **data_address) { 
    if (length) *length = 0;
    if (data_address) *data_address = (uint16_t*)0;
}

#endif /* HAL_TLV_H_ */
