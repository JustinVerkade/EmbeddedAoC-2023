/*
 * Day01.c
 *
 *  Created on: 9 aug. 2023
 *      Author: J.Verkade
 */

#include "AoC/Days/Day14.h"

void printRocks(char *rocks, uint32_t width, uint32_t depth)
{
	for(uint32_t line=0; line<depth; line++)
	{
		uint32_t index = line * (width + 1);
		char tx_buffer[256];
		sprintf(tx_buffer, "%s\n", &rocks[index]);
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, tx_buffer, strlen(tx_buffer));
		HAL_Delay(10);
	}
}

void rollNorth(char *rocks, uint32_t width, uint32_t depth)
{
	// find rocks from top to bottom
	for(uint32_t d=1; d<depth; d++)
	{
		for(uint32_t w=0; w<width; w++)
		{
			// get current index
			uint32_t index = d * (width+1) + w;

			// abort if index is not a rolling rock
			if(rocks[index] != 'O')
				continue;

			// clear current index
			rocks[index] = '.';

			// find the destination position where the rock will land
			int32_t d2;
			for(d2=d; d2>0; d2--)
				if(rocks[(d2 - 1) * (width+1) + w] != '.')
					break;

			// reposition rock
			rocks[d2 * (width+1) + w] = 'O';
		}
	}
}

void rollWest(char *rocks, uint32_t width, uint32_t depth)
{
	// find rocks from top to bottom
	for(uint32_t w=1; w<width; w++)
	{
		for(uint32_t d=0; d<depth; d++)
		{
			// get current index
			uint32_t index = d * (width+1) + w;

			// abort if index is not a rolling rock
			if(rocks[index] != 'O')
				continue;

			// clear current index
			rocks[index] = '.';

			// find the destination position where the rock will land
			int32_t w2;
			for(w2=w; w2>0; w2--)
				if(rocks[d * (width+1) + (w2 - 1)] != '.')
					break;

			// reposition rock
			rocks[d * (width+1) + w2] = 'O';
		}
	}
}

void rollSouth(char *rocks, uint32_t width, uint32_t depth)
{
	// find rocks from top to bottom
	for(int32_t d=depth-2; d>=0; d--)
	{
		for(int32_t w=0; w<width; w++)
		{
			// get current index
			uint32_t index = d * (width+1) + w;

			// abort if index is not a rolling rock
			if(rocks[index] != 'O')
				continue;

			// clear current index
			rocks[index] = '.';

			// find the destination position where the rock will land
			int32_t d2;
			for(d2=d; d2<depth-1; d2++)
				if(rocks[(d2 + 1) * (width+1) + w] != '.')
					break;

			// reposition rock
			rocks[d2 * (width+1) + w] = 'O';
		}
	}
}

void rollEast(char *rocks, uint32_t width, uint32_t depth)
{
	// find rocks from top to bottom
	for(int32_t w=width-2; w>=0; w--)
	{
		for(uint32_t d=0; d<depth; d++)
		{
			// get current index
			uint32_t index = d * (width+1) + w;

			// abort if index is not a rolling rock
			if(rocks[index] != 'O')
				continue;

			// clear current index
			rocks[index] = '.';

			// find the destination position where the rock will land
			int32_t w2;
			for(w2=w; w2<width-1; w2++)
				if(rocks[d * (width+1) + (w2 + 1)] != '.')
					break;

			// reposition rock
			rocks[d * (width+1) + w2] = 'O';
		}
	}
}

uint64_t calculateLoad(char *rocks, uint32_t width, uint32_t depth)
{
	// calculate each rocks weight
	uint64_t load = 0;
	for(uint32_t d=0; d<depth; d++)
		for(uint32_t w=0; w<width; w++)
			if(rocks[d * (width+1) + w] == 'O')
				load += depth - d;
	return load;
}

AdventOfCode_return_t AoC_Day_14(AdventOfCode_config_t config)
{
	// implementation state (change to OK when the day is active)
	if(config == AOC_CONFIG_TEST)
		return AOC_RETURN_OK;

	{ // part 1
		// fetch file
		FIL file;
		uint8_t ret;
		if(config == AOC_CONFIG_DEBUG_VERBOSE)
			ret = f_open(&file, "day14t.txt", FA_READ);
		else ret = f_open(&file, "day14.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// get file length
		int32_t file_size = f_size(&file);

		// extract data
		char *rocks = (char*)sram_malloc(file_size + 1);

		// insert data
		f_read(&file, rocks, file_size + 1, NULL);

		// close file
		f_close(&file);

		// get space dimensions and insert line ends
		char *token = strtok(rocks, "\n");
		uint32_t width = strlen(token);
		uint32_t depth = 1;
		while(strtok(NULL, "\n"))
			depth++;

		// roll rocks
		rollNorth(rocks, width, depth);

		// calculate load
		uint64_t load = calculateLoad(rocks, width, depth);

		// output answer
		char answer[64];
		sprintf(answer, "    Part 1 answer: %llu\n", load);
		AdventOfCode_print(config, answer, strlen(answer));

		// clean up
		sram_clrmem();
	}

	{ // part 2
		// fetch file
		FIL file;
		uint8_t ret;
		if(config == AOC_CONFIG_DEBUG_VERBOSE)
			ret = f_open(&file, "day14t.txt", FA_READ);
		else ret = f_open(&file, "day14.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// get file length
		int32_t file_size = f_size(&file);

		// extract data
		char *rocks = (char*)sram_malloc(file_size + 1);

		// insert data
		f_read(&file, rocks, file_size + 1, NULL);

		// close file
		f_close(&file);

		// get space dimensions and insert line ends
		char *token = strtok(rocks, "\n");
		uint32_t width = strlen(token);
		uint32_t depth = 1;
		while(strtok(NULL, "\n"))
			depth++;

		// cache memory
		uint32_t *memory = (uint32_t*)sram_malloc(sizeof(uint32_t) * 10000);
		uint32_t memory_ptr = 0;

		uint32_t sequence_start = 0;
		uint32_t sequence_length = 0;
		uint32_t cycles = 10000;
		for(uint32_t cycle=0; cycle<cycles; cycle++)
		{
			// roll rocks
			rollNorth(rocks, width, depth);
			rollWest(rocks, width, depth);
			rollSouth(rocks, width, depth);
			rollEast(rocks, width, depth);

			// calculate load
			uint32_t load = calculateLoad(rocks, width, depth);
			memory[memory_ptr] = load;
			memory_ptr++;

			// check if sequence
			if(cycle < 8)
				continue;

			uint8_t exit = 0;
			for(uint32_t seq_len=2; seq_len<memory_ptr/2; seq_len++)
			{
				uint8_t flag = 0;
				uint32_t index0 = memory_ptr - 1 - seq_len * 0;
				uint32_t index1 = memory_ptr - 1 - seq_len * 1;
				for(uint32_t length=0; length<seq_len; length++)
				{
					if(memory[index0 - length] != memory[index1 - length])
					{
						flag = 1;
						break;
					}
				}

				if(!flag)
				{
					sequence_start = index1 - seq_len + 1;
					sequence_length = seq_len;
					exit = 1;
					break;
				}
			}
			if(exit)
				break;
		}

		// calculcate pattern leftovers
		uint32_t leftover_rolls = (1000000000 - sequence_start) % sequence_length;
		for(uint32_t roll=0; roll<leftover_rolls; roll++)
		{
			rollNorth(rocks, width, depth);
			rollWest(rocks, width, depth);
			rollSouth(rocks, width, depth);
			rollEast(rocks, width, depth);
		}

		// calculate load
		uint64_t load = calculateLoad(rocks, width, depth);

		// output answer
		char answer[64];
		sprintf(answer, "    Part 2 answer: %llu\n", load);
		AdventOfCode_print(config, answer, strlen(answer));

		// clean up
		sram_clrmem();

	}

	// status code
	return AOC_RETURN_OK;
}

