/*
 * Day01.c
 *
 *  Created on: 9 aug. 2023
 *      Author: J.Verkade
 */

#include "AoC/Days/Day08.h"

typedef struct Execution_t Execution_t;
struct Execution_t
{
	char id[4];
	char left[4];
	uint16_t left_index;
	char right[4];
	uint16_t right_index;
};

AdventOfCode_return_t AoC_Day_08(AdventOfCode_config_t config)
{
	// implementation state (change to OK when the day is active)
	if(config == AOC_CONFIG_TEST)
		return AOC_RETURN_OK;

	{ // part 1
		// fetch file
		FIL file;
		uint8_t ret;
		if(config == AOC_CONFIG_DEBUG_VERBOSE)
			ret = f_open(&file, "day8t.txt", FA_READ);
		else ret = f_open(&file, "day8.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// fetch the command string
		char command_buffer[512];
		f_gets(command_buffer, 512, &file);
		uint32_t command_length = strlen(command_buffer) - 1;

		// skip white line
		char buffer[32];
		f_gets(buffer, 32, &file);

		// partse input data
		uint32_t execution_ptr = 0;
		Execution_t *executions = (Execution_t*)sram_calloc(2000, sizeof(Execution_t));
		while(f_gets(buffer, 32, &file))
		{
			// fetch IDs from input data
			memcpy(executions[execution_ptr].id, buffer, 3);
			memcpy(executions[execution_ptr].left, buffer + 7, 3);
			memcpy(executions[execution_ptr].right, buffer + 12, 3);
			execution_ptr++;
		}

		// process pre process IDs
		for(uint32_t index=0; index<execution_ptr; index++)
		{
			// find left index
			char *left_id = executions[index].left;
			for(uint32_t search=0; search<execution_ptr; search++)
			{
				if(!memcmp(left_id, executions[search].id, 3))
				{
					executions[index].left_index = search;
					break;
				}
			}
			// find right marker
			char *right_id = executions[index].right;
			for(uint32_t search=0; search<execution_ptr; search++)
			{
				if(!memcmp(right_id, executions[search].id, 3))
				{
					executions[index].right_index = search;
					break;
				}
			}
		}

		// find end condition index
		uint32_t start_ptr = 0;
		for(; start_ptr<execution_ptr; start_ptr++)
			if(!memcmp("AAA", executions[start_ptr].id, 3))
				break;

		// find end condition index
		uint32_t end_ptr = 0;
		for(; end_ptr<execution_ptr; end_ptr++)
			if(!memcmp("ZZZ", executions[end_ptr].id, 3))
				break;

		uint32_t instruction_ptr = start_ptr;
		uint32_t counter = 0;
		while(counter < 0xFFFFFFFF)
		{
			// get command
			uint32_t command_index = counter % command_length;
			char c = command_buffer[command_index];

			// process data
			if(c == 'R')
				instruction_ptr = executions[instruction_ptr].right_index;
			if(c == 'L')
				instruction_ptr = executions[instruction_ptr].left_index;
			counter++;

			// check if end
			if(instruction_ptr == end_ptr)
				break;
		}

		// output answer
		char answer[64];
		sprintf(answer, "    Part 1 answer: %lu\n", counter);
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
			ret = f_open(&file, "day8t2.txt", FA_READ);
		else ret = f_open(&file, "day8.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// fetch the command string
		char command_buffer[512];
		f_gets(command_buffer, 512, &file);
		uint32_t command_length = strlen(command_buffer) - 1;

		// skip white line
		char buffer[32];
		f_gets(buffer, 32, &file);

		// partse input data
		uint32_t execution_ptr = 0;
		Execution_t *executions = (Execution_t*)sram_calloc(2000, sizeof(Execution_t));
		while(f_gets(buffer, 32, &file))
		{
			// fetch IDs from input data
			memcpy(executions[execution_ptr].id, buffer, 3);
			memcpy(executions[execution_ptr].left, buffer + 7, 3);
			memcpy(executions[execution_ptr].right, buffer + 12, 3);
			execution_ptr++;
		}

		// process pre process IDs
		for(uint32_t index=0; index<execution_ptr; index++)
		{
			// find left index
			char *left_id = executions[index].left;
			for(uint32_t search=0; search<execution_ptr; search++)
			{
				if(!memcmp(left_id, executions[search].id, 3))
				{
					executions[index].left_index = search;
					break;
				}
			}
			// find right marker
			char *right_id = executions[index].right;
			for(uint32_t search=0; search<execution_ptr; search++)
			{
				if(!memcmp(right_id, executions[search].id, 3))
				{
					executions[index].right_index = search;
					break;
				}
			}
		}

		// find end condition index
		uint32_t start_ptrs = 0;
		uint32_t line_a[100];
		for(uint32_t index=0; index<execution_ptr; index++)
		{
			// abort if not ending on A
			if(executions[index].id[2] != 'A')
				continue;

			// find distance till first interaction with Z
			uint32_t start_ptr = index;
			uint32_t counter = 0;
			while(counter < 0xFFFFFFFF)
			{
				// get command
				uint32_t command_index = counter % command_length;
				char c = command_buffer[command_index];

				// process data
				if(c == 'R')
					start_ptr = executions[start_ptr].right_index;
				if(c == 'L')
					start_ptr = executions[start_ptr].left_index;
				counter++;

				// check if end
				if(executions[start_ptr].id[2] == 'Z')
					break;
			}
			// save counter to A
			line_a[start_ptrs] = counter;
			start_ptrs++;
		}

		// match solutions
		uint32_t largest = 0;
		uint64_t multiple = 1;
		for(uint8_t line=0; line<start_ptrs; line++)
		{
			if(largest < line_a[line])
				largest = line_a[line];
			multiple *= line_a[line];
		}

		// 271 is prime multiple
		uint32_t GCD;
		if(config == AOC_CONFIG_DEBUG_VERBOSE)
			GCD = 1;
		else
			GCD = 271;

		// LCM
		uint64_t lcm_out = line_a[0];
		for(uint8_t line=1; line<start_ptrs; line++)
		{
			lcm_out = (lcm_out * line_a[line]) / GCD;
		}

		// 1658425115 to low

		// output answer
		char answer[64];
		sprintf(answer, "    Part 2 answer: %llu\n", lcm_out);
		AdventOfCode_print(config, answer, strlen(answer));

		// clean up
		f_close(&file);
		sram_clrmem();
	}

	// status code
	return AOC_RETURN_OK;
}
