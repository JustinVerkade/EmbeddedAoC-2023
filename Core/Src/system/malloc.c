/*
 * malloc.h
 *
 *  Created on: 29 nov. 2023
 *      Author: J.Verkade
 */

#include "System/malloc.h"
#include "string.h"

// SDRAM defines
#define SRAM_MEMORY_ADDRESS 0x60000000llu
#define SRAM_MEMORY_SIZE 0x800000llu

// local memory data
static uint8_t *sram_memory = (uint8_t*)SRAM_MEMORY_ADDRESS;
static uint32_t sram_ptr = 0x00000000llu;

void* sram_malloc(uint32_t bytes)
{
	// get allocation ptr;
	void *alloc_ptr = sram_memory + sram_ptr;

	// check for overrun
	uint32_t block_size = sram_ptr + bytes;
	if(block_size >= SRAM_MEMORY_SIZE)
		return NULL;

	// increment memory ptr;
	sram_ptr += bytes;

	// return allocation address
	return alloc_ptr;
}

void* sram_calloc(uint32_t object_count, uint16_t object_size)
{
	// create memory block
	uint32_t block_size = object_count * object_size;
	void *alloc_ptr = sram_malloc(block_size);

	// check if allocation completes
	if(alloc_ptr == NULL)
		return NULL;

	// empty alloction block
	memset(alloc_ptr, 0x00, block_size);

	// return cleared allocation addres
	return alloc_ptr;
}

void sram_clrmem()
{
	// reset allocation ptr
	sram_ptr = 0;
}
