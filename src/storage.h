#ifndef STORAGE_H
#define STORAGE_H

#include <stdint.h>

void     storage_init();
void     storage_save_counter(uint16_t value);
uint16_t storage_load_counter();

#endif // STORAGE_H
