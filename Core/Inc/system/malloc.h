/*
 * malloc.h
 *
 *  Created on: 29 nov. 2023
 *      Author: J.Verkade
 */

#ifndef INC_SYSTEM_MALLOC_H_
#define INC_SYSTEM_MALLOC_H_

#include "stdint.h"

void* sram_malloc(uint32_t bytes);
void* sram_calloc(uint32_t object_count, uint16_t object_size);
void sram_clrmem();

#endif /* INC_SYSTEM_MALLOC_H_ */
