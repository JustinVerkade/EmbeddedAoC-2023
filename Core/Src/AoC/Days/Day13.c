/*
 * Day01.c
 *
 *  Created on: 9 aug. 2023
 *      Author: J.Verkade
 */

#include "AoC/Days/Day13.h"

#define PROCESS_STATE_FETCHING 0
#define PROCESS_STATE_PROCESSING 1
#define PROCESS_STATE_RESETTING 2

int32_t getVertical(char *map, uint32_t width, uint32_t depth, uint32_t exeption)
{
	// check for vertical mirror
	uint32_t vertical = 0;
	for(uint32_t w=1; w<width; w++)
	{
		if(w == exeption)
			continue;

		// get maximum width
		uint32_t max_mirror_width = ((width - w) < (w)) ? width - w: w;

		// check rows for simularity
		uint8_t mirrored = 1;
		for(uint8_t i=0; i<max_mirror_width; i++)
		{
			for(uint8_t j=0; j<depth; j++)
			{
				// get compare indices
				uint32_t index0 = (j * (width + 1)) + (w - i - 1);
				uint32_t index1 = (j * (width + 1)) + (w + i + 0);
				if(map[index0] != map[index1])
				{
					mirrored = 0;
					break;
				}
			}
			if(!mirrored)
				break;
		}
		if(mirrored)
		{
			vertical = w;
			break;
		}
	}
	return vertical;
}

int32_t getHorizontal(char *map, uint32_t width, uint32_t depth, uint32_t exeption)
{
	// check for horizontal mirror
	int32_t horizontal = 0;
	for(uint32_t d=1; d<depth; d++)
	{
		if(d == exeption)
			continue;

		// get maximum width
		uint32_t max_mirror_depth = ((depth - d) < (d)) ? depth - d: d;

		// check rows for simularity
		uint8_t mirrored = 1;
		for(uint8_t i=0; i<max_mirror_depth; i++)
		{
			for(uint8_t j=0; j<width; j++)
			{
				// get compare indices
				uint32_t index0 = ((d - i - 1) * (width + 1)) + (j);
				uint32_t index1 = ((d + i + 0) * (width + 1)) + (j);
				if(map[index0] != map[index1])
				{
					mirrored = 0;
					break;
				}
			}
			if(!mirrored)
				break;
		}
		if(mirrored)
		{
			horizontal = d;
			break;
		}
	}
	return horizontal;
}

AdventOfCode_return_t AoC_Day_13(AdventOfCode_config_t config)
{
	// implementation state (change to OK when the day is active)
	if(config == AOC_CONFIG_TEST)
		return AOC_RETURN_OK;

	{ // part 1
		// fetch file
		FIL file;
		uint8_t ret;
		if(config == AOC_CONFIG_DEBUG_VERBOSE)
			ret = f_open(&file, "day13t.txt", FA_READ);
		else ret = f_open(&file, "day13.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// create character buffer
		char *map = (char*)sram_calloc(512, 1);

		// process data
		uint32_t sum = 0;
		uint8_t state = PROCESS_STATE_FETCHING;
		char buffer[64];
		while(f_gets(buffer, 64, &file))
		{
			// fetching
			if(state == PROCESS_STATE_FETCHING)
			{
				if(buffer[0] == '\n')
					state = PROCESS_STATE_PROCESSING;
				else strcat(map, buffer);
				if(map[strlen(map) - 1] != '\n')
				{
					strcat(map, "\n");
					state = PROCESS_STATE_PROCESSING;
				}
			}

			// processing
			if(state == PROCESS_STATE_PROCESSING)
			{
				// get map dimension
				uint32_t width = 0;
				for(; map[width]!='\n'; width++);
				uint32_t depth = strlen(map) / (width + 1);

				int32_t vertical = getVertical(map, width, depth, 0);
				int32_t horizontal = getHorizontal(map, width, depth, 0);

				// total
				uint32_t total = vertical + 100 * horizontal;
				sum += total;

				state = PROCESS_STATE_RESETTING;
			}

			// resetting
			if(state == PROCESS_STATE_RESETTING)
			{
				memset(map, 0x00, 256);
				state = PROCESS_STATE_FETCHING;
			}
		}

		// output answer
		char answer[64];
		sprintf(answer, "    Part 1 answer: %lu\n", sum);
		AdventOfCode_print(config, answer, strlen(answer));

		// clean up
		f_close(&file);
		sram_clrmem();
	}

	{ // part 1
		// fetch file
		FIL file;
		uint8_t ret;
		if(config == AOC_CONFIG_DEBUG_VERBOSE)
			ret = f_open(&file, "day13t.txt", FA_READ);
		else ret = f_open(&file, "day13.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// create character buffer
		char *map = (char*)sram_calloc(512, 1);

		// process data
		uint32_t sum = 0;
		uint8_t state = PROCESS_STATE_FETCHING;
		char buffer[64];
		while(f_gets(buffer, 64, &file))
		{
			// fetching
			if(state == PROCESS_STATE_FETCHING)
			{
				if(buffer[0] == '\n')
					state = PROCESS_STATE_PROCESSING;
				else strcat(map, buffer);
				if(map[strlen(map) - 1] != '\n')
				{
					strcat(map, "\n");
					state = PROCESS_STATE_PROCESSING;
				}
			}

			// processing
			if(state == PROCESS_STATE_PROCESSING)
			{
				// get map dimension
				uint32_t width = 0;
				for(; map[width]!='\n'; width++);
				uint32_t depth = strlen(map) / (width + 1);

				// get current result
				int32_t vertical = getVertical(map, width, depth, 0);
				int32_t horizontal = getHorizontal(map, width, depth, 0);
				int32_t new_vertical = 0;
				int32_t new_horizontal = 0;

				// go over each line and row index
				char clone[512];
				uint8_t flag = 0;
				for(uint8_t w=0; w<width; w++)
				{
					for(uint8_t d=0; d<depth; d++)
					{
						// make a clone of the target
						memcpy(clone, map, strlen(map) + 1);

						// swap smudge
						uint32_t index = d * (width + 1) + w;
						clone[index] = clone[index] == '.' ? '#' : '.';

						// get current result
						new_vertical = getVertical(clone, width, depth, vertical);
						new_horizontal = getHorizontal(clone, width, depth, horizontal);

						// abort if failed
						if(!new_vertical && !new_horizontal)
							continue;

						flag = 1;
						break;
					}
					if(flag)
						break;
				}

				if(!flag)
				{
					AdventOfCode_print(config, "Failed!\n", strlen("Failed!\n"));
				}

				// total
				if(new_vertical != vertical)
					sum += new_vertical;
				if(new_horizontal != horizontal)
					sum += 100 * new_horizontal;

				state = PROCESS_STATE_RESETTING;
			}

			// resetting
			if(state == PROCESS_STATE_RESETTING)
			{
				memset(map, 0x00, 256);
				state = PROCESS_STATE_FETCHING;
			}
		}

		// output answer
		char answer[64];
		sprintf(answer, "    Part 2 answer: %lu\n", sum);
		AdventOfCode_print(config, answer, strlen(answer));

		// clean up
		f_close(&file);
		sram_clrmem();
	}

	// status code
	return AOC_RETURN_OK;
}
