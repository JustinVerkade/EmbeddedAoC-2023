/*
 * Day01.c
 *
 *  Created on: 9 aug. 2023
 *      Author: J.Verkade
 */
#include "math.h"

#include "AoC/Days/Day12.h"

typedef struct CacheObject_t CacheObject_t;
struct CacheObject_t
{
	char *string;
	uint8_t *sequence;
	uint64_t count;
};

uint32_t cache_size = 0x800000llu / sizeof(CacheObject_t) - 1;
uint32_t cache_ptr = 0;
CacheObject_t *cache;

static uint64_t recursion(char *string, uint8_t *reference_sequence, uint8_t sequence_length, uint32_t dept)
{
	// cache check
	for(uint32_t ptr=0; ptr<cache_ptr; ptr++)
		if(cache[ptr].string == string && cache[ptr].sequence == reference_sequence)
			return cache[ptr].count;

	// verify if current sequence is correct
	if(strlen(string) == 0)
		return sequence_length == 0;
	if(sequence_length == 0)
	{
		for(uint8_t i=0; i<strlen(string); i++)
			if(string[i] == '#')
				return 0;
		return 1;
	}

	uint64_t counter = 0;
	if(string[0] == '.' || string[0] == '?')
		counter += recursion(string+1, reference_sequence, sequence_length, dept + 1);

	if(string[0] == '#' || string[0] == '?')
	{
		if(reference_sequence[0] <= strlen(string) && (reference_sequence[0] == strlen(string) || string[reference_sequence[0]] != '#'))
		{
			// verify no operational springs in block
			uint8_t flag = 0;
			for(uint8_t i=0; i<reference_sequence[0]; i++)
			{
				if(string[i] == '.')
				{
					flag = 1;
					break;
				}
			}
			if(!flag)
				counter += recursion(string+reference_sequence[0] + 1, reference_sequence + 1, sequence_length - 1, dept + 1);
		}
	}

	if(cache_ptr != cache_size && dept != 0)
	{
		cache[cache_ptr].string = string;
		cache[cache_ptr].sequence = reference_sequence;
		cache[cache_ptr].count = counter;
		cache_ptr++;
	}

	return counter;
}

AdventOfCode_return_t AoC_Day_12(AdventOfCode_config_t config)
{
	// implementation state (change to OK when the day is active)
	if(config == AOC_CONFIG_TEST)
		return AOC_RETURN_OK;

	{ // part 1
		// fetch file
		FIL file;
		uint8_t ret;
		if(config == AOC_CONFIG_DEBUG_VERBOSE)
			ret = f_open(&file, "day12t.txt", FA_READ);
		else ret = f_open(&file, "day12.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// build cache
		cache_ptr = 0;
		cache = (CacheObject_t*)sram_malloc(cache_size * sizeof(CacheObject_t));

		// for each line
		uint32_t total = 0;
		char buffer[256];
		while(f_gets(buffer, 256, &file))
		{
			// split status code and sequences
			char *status_code = strtok(buffer, " ");
			char *sequence_list = strtok(NULL, " ");

			// create array of sequences
			uint8_t length = 0;
			uint8_t sequence[64];
			char *value_str = strtok(sequence_list, ",");
			while(value_str != NULL)
			{
				sequence[length] = atoi(value_str);
				value_str = strtok(NULL, ",");
				length++;
			}

			// calculate answer
			cache_ptr = 0;
			uint32_t count = recursion(status_code, sequence, length, 0);
			total += count;
		}

		// output answer
		char answer[64];
		sprintf(answer, "    Part 1 answer: %lu\n", total);
		AdventOfCode_print(config, answer, strlen(answer));

		// clean up
		f_close(&file);
		sram_clrmem();
	}

	{ // part 2
		// fetch file
		FIL file;
		uint8_t ret;
		if(config == AOC_CONFIG_DEBUG_VERBOSE)
			ret = f_open(&file, "day12t.txt", FA_READ);
		else ret = f_open(&file, "day12.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// build cache
		cache_ptr = 0;
		cache = (CacheObject_t*)sram_malloc(cache_size * sizeof(CacheObject_t));

		// for each line
		uint32_t counter = 0;
		uint64_t total = 0;
		char buffer[256];
		while(f_gets(buffer, 256, &file))
		{
			// split status code and sequences
			char *status_code = strtok(buffer, " ");
			char *sequence_list = strtok(NULL, " ");

			// create array of sequences
			uint8_t length = 0;
			uint8_t sequence[64];
			char *value_str = strtok(sequence_list, ",");
			while(value_str != NULL)
			{
				sequence[length] = atoi(value_str);
				value_str = strtok(NULL, ",");
				length++;
			}

			// create 5x status code string
			char new_status_code[256];
			memset(new_status_code, 0x00, 256);
			for(uint8_t i=0; i<5; i++)
			{
				strcat(new_status_code, status_code);
				if(i != 4) strcat(new_status_code, "?");
			}

			// create 5x sequence length count
			for(uint8_t i=0; i<4; i++)
				memcpy(sequence + (i+1)*length, sequence + (i)*length, length);
			length *= 5;

			// calculate answer
			cache_ptr = 0;
			uint64_t count = recursion(new_status_code, sequence, length, 0);
			{
				char answer[256];
				sprintf(answer, "Line[%4lu]: %llu <> [%lu %u] <> [%lu]\n", ++counter, count, (uint32_t)strlen(new_status_code), length, cache_ptr);
				AdventOfCode_print(config, answer, strlen(answer));
			}
			total += count;
		}

		//1426673570795 false
		//

		// output answer
		char answer[128];
		sprintf(answer, "    Part 2 answer: %llu\n", total);
		AdventOfCode_print(config, answer, strlen(answer));

		// clean up
		f_close(&file);
		sram_clrmem();
	}

	// status code
	return AOC_RETURN_OK;
}
