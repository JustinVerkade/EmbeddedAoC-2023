/*
 * Day01.c
 *
 *  Created on: 9 aug. 2023
 *      Author: J.Verkade
 */

#include "AoC/Days/Day01.h"

AdventOfCode_return_t AoC_Day_01(AdventOfCode_config_t config)
{
	// implementation state (change to OK when the day is active)
	if(config == AOC_CONFIG_TEST)
		return AOC_RETURN_OK;

	{ // part 1
		// fetch file
		FIL file;
		uint8_t ret;
		if(config == AOC_CONFIG_DEBUG_VERBOSE)
			ret = f_open(&file, "day1t.txt", FA_READ);
		else ret = f_open(&file, "day1.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// process data
		char buffer[128];
		int32_t sum_of_values = 0;
		while(f_gets(buffer, 64, &file))
		{
			char *buffer_ptr = buffer;
			uint8_t concat_ptr = 0;
			char concat_buffer[128];
			memset(concat_buffer, 0x00, 128);
			char c = *buffer_ptr++;
			while(c != 0x00)
			{
				if(c >= '0' && c <= '9')
				{
					concat_buffer[concat_ptr] = c;
					if(concat_ptr == 0)
						concat_buffer[concat_ptr + 1] = c;
					concat_ptr = 1;
				}
				c = *buffer_ptr++;
			}
			uint32_t value = atol(concat_buffer);
			sum_of_values += value;
		}

		// output answer
		char answer[32];
		sprintf(answer, "    Part 1 answer: %ld\n", sum_of_values);
		AdventOfCode_print(config, answer, strlen(answer));

		// clean up
		f_close(&file);
	}

	{ // part 2
		// fetch file
		FIL file;
		uint8_t ret;
		if(config == AOC_CONFIG_DEBUG_VERBOSE)
			ret = f_open(&file, "day1t2.txt", FA_READ);
		else ret = f_open(&file, "day1.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// process data
		char buffer[128];
		int32_t sum_of_values = 0;
		while(f_gets(buffer, 64, &file))
		{
			char *buffer_ptr = buffer;
			uint8_t concat_ptr = 0;
			char concat_buffer[128];
			memset(concat_buffer, 0x00, 128);
			while(*buffer_ptr != 0x00)
			{
				char c = *buffer_ptr;

				// check if value
				if(c >= '0' && c <= '9')
				{
					concat_buffer[concat_ptr] = c;
					if(concat_ptr == 0)
						concat_buffer[concat_ptr + 1] = c;
					concat_ptr = 1;
				}
				// check if text
				char txt = 0x00;
				if(!memcmp("one", buffer_ptr, 3))
					txt = '1';
				else if(!memcmp("two", buffer_ptr, 3))
					txt = '2';
				else if(!memcmp("three", buffer_ptr, 5))
					txt = '3';
				else if(!memcmp("four", buffer_ptr, 4))
					txt = '4';
				else if(!memcmp("five", buffer_ptr, 4))
					txt = '5';
				else if(!memcmp("six", buffer_ptr, 3))
					txt = '6';
				else if(!memcmp("seven", buffer_ptr, 5))
					txt = '7';
				else if(!memcmp("eight", buffer_ptr, 5))
					txt = '8';
				else if(!memcmp("nine", buffer_ptr, 4))
					txt = '9';

				if(txt != 0x00)
				{
					concat_buffer[concat_ptr] = txt;
					if(concat_ptr == 0)
						concat_buffer[concat_ptr + 1] = txt;
					concat_ptr = 1;
				}

				c = *++buffer_ptr;
			}
			uint32_t value = atol(concat_buffer);
			sum_of_values += value;
		}

		// output answer
		char answer[32];
		sprintf(answer, "    Part 2 answer: %ld\n", sum_of_values);
		AdventOfCode_print(config, answer, strlen(answer));

		// clean up
		f_close(&file);
	}

	// status code
	return AOC_RETURN_OK;
}
