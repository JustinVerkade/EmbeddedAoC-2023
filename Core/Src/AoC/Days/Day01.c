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
			AdventOfCode_print(config, "\tCannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// process data
		char buffer[16];
		int32_t prev_value = -1;
		int32_t counter = 0;
		while(f_gets(buffer, 16, &file))
		{
			int32_t value = atoi(buffer);
			if(prev_value >= 0 && prev_value < value)
				counter++;
			prev_value = value;
		}

		// output answer
		char answer[32];
		sprintf(answer, "\tPart 1 answer: %ld\n", counter);
		AdventOfCode_print(config, answer, strlen(answer));

		// clean up
		f_close(&file);
	}

	{ // part 2
		// fetch file
		FIL file;
		uint8_t ret;
		if(config == AOC_CONFIG_DEBUG_VERBOSE)
			ret = f_open(&file, "day1t.txt", FA_READ);
		else ret = f_open(&file, "day1.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "\tCannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// process data
		char buffer[16];
		int32_t prev_value[] = {-1, -1};
		int32_t prev_sum = -1;
		int32_t counter = 0;
		while(f_gets(buffer, 16, &file))
		{
			int32_t value = atoi(buffer);
			if(prev_value[1] >= 0)
			{
				int32_t sum = value + prev_value[1] + prev_value[0];
				if(prev_sum >= 0 && prev_sum < sum)
					counter++;
				prev_sum = sum;
			}
			prev_value[1] = prev_value[0];
			prev_value[0] = value;
		}

		// output answer
		char answer[32];
		sprintf(answer, "\tPart 2 answer: %ld\n", counter);
		AdventOfCode_print(config, answer, strlen(answer));

		// clean up
		f_close(&file);
	}

	// status code
	return AOC_RETURN_OK;
}
