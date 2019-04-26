#ifndef INCLUDE_KEYBOARD_H
#define INCLUDE_KEYBOARD_H

#include "types.h"

#define KB_CTR_PORT 0x64  // kbd controller status port(I)
#define KB_DIB 0x01       // kbd data in buffer
#define KB_DATA_PORT 0x60 // kbd data port(I)

#define INPUT_BUF 128

void init_kbd();

int8_t get_scan_code();

#endif