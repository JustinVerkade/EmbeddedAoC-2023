/*
 * Day01.c
 *
 *  Created on: 9 aug. 2023
 *      Author: J.Verkade
 */

#include "AoC/Days/Day09.h"

void printSequence(int32_t *sequence, uint32_t sequence_length)
{
	char buffer[256];
	memset(buffer, 0x00, 256);
	for(uint32_t index=0; index<sequence_length; index++)
	{
		char value_str[16];
		if(index == 0)
			sprintf(value_str, "[%ld, ", sequence[index]);
		else if(index < sequence_length - 1)
			sprintf(value_str, "%ld, ", sequence[index]);
		else
			sprintf(value_str, "%ld]\n", sequence[index]);
		strcat(buffer, value_str);
	}
	AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, buffer, strlen(buffer));
}

int32_t recursion(int32_t *sequence, uint32_t sequence_length)
{
	// create buffer in memory
	uint32_t diff_sequence_length = sequence_length - 1;
	int32_t *diff_sequence = (int32_t*)sram_malloc(sizeof(int32_t) * diff_sequence_length);

	// fill with deltas
	for(uint8_t index=0; index<diff_sequence_length; index++)
		diff_sequence[index] = sequence[index + 1] - sequence[index];

	// check if all zeros
	uint8_t all_zeros = 1;
	for(uint8_t index=0; index<diff_sequence_length; index++)
	{
		if(diff_sequence[index])
		{
			all_zeros = 0;
			break;
		}
	}

	// end of line
	if(all_zeros)
		return 0;

	// do recursion
	return diff_sequence[diff_sequence_length - 1] + recursion(diff_sequence, diff_sequence_length);
}

int32_t reverseRecursion(int32_t *sequence, uint32_t sequence_length)
{
	// create buffer in memory
	uint32_t diff_sequence_length = sequence_length - 1;
	int32_t *diff_sequence = (int32_t*)sram_malloc(sizeof(int32_t) * diff_sequence_length);

	// fill with deltas
	for(uint8_t index=0; index<diff_sequence_length; index++)
		diff_sequence[index] = sequence[index + 1] - sequence[index];

	// check if all zeros
	uint8_t all_zeros = 1;
	for(uint8_t index=0; index<diff_sequence_length; index++)
	{
		if(diff_sequence[index])
		{
			all_zeros = 0;
			break;
		}
	}

	// end of line
	if(all_zeros)
		return 0;

	// do recursion
	return diff_sequence[0] - reverseRecursion(diff_sequence, diff_sequence_length);
}

AdventOfCode_return_t AoC_Day_09(AdventOfCode_config_t config)
{
	// implementation state (change to OK when the day is active)
	if(config == AOC_CONFIG_TEST)
		return AOC_RETURN_OK;

	{ // part 1
		// fetch file
		FIL file;
		uint8_t ret;
		if(config == AOC_CONFIG_DEBUG_VERBOSE)
			ret = f_open(&file, "day9t.txt", FA_READ);
		else ret = f_open(&file, "day9.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// for each line
		int64_t answer_value = 0;
		char buffer[256];
		while(f_gets(buffer, 256, &file))
		{
			// make token array
			char *tokens[64];
			uint32_t token_counter = 0;
			char *token = strtok(buffer, " ");
			while(token != NULL)
			{
				tokens[token_counter] = token;
				token = strtok(NULL, " ");
				token_counter++;
			}

			// create buffer in memory
			uint32_t sequence_length = token_counter;
			int32_t *sequence = (int32_t*)sram_malloc(sizeof(int32_t) * sequence_length);

			// fill buffer
			for(uint8_t token_index=0; token_index<sequence_length; token_index++)
				sequence[token_index] = atol(tokens[token_index]);

			// test sequence import
			int32_t next_sequence_value = recursion(sequence, sequence_length);
			next_sequence_value += sequence[sequence_length - 1];
			answer_value += next_sequence_value;
		}

		// output answer
		char answer[64];
		sprintf(answer, "    Part 1 answer: %llu\n", answer_value);
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
			ret = f_open(&file, "day9t.txt", FA_READ);
		else ret = f_open(&file, "day9.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// for each line
		int64_t answer_value = 0;
		char buffer[256];
		while(f_gets(buffer, 256, &file))
		{
			// make token array
			char *tokens[64];
			uint32_t token_counter = 0;
			char *token = strtok(buffer, " ");
			while(token != NULL)
			{
				tokens[token_counter] = token;
				token = strtok(NULL, " ");
				token_counter++;
			}

			// create buffer in memory
			uint32_t sequence_length = token_counter;
			int32_t *sequence = (int32_t*)sram_malloc(sizeof(int32_t) * sequence_length);

			// fill buffer
			for(uint8_t token_index=0; token_index<sequence_length; token_index++)
				sequence[token_index] = atol(tokens[token_index]);

			// test sequence import
			int32_t next_sequence_value = reverseRecursion(sequence, sequence_length);
			next_sequence_value -= sequence[0];
			answer_value -= next_sequence_value;
		}

		// output answer
		char answer[64];
		sprintf(answer, "    Part 1 answer: %lld\n", answer_value);
		AdventOfCode_print(config, answer, strlen(answer));

		// clean up
		f_close(&file);
		sram_clrmem();
	}

	// status code
	return AOC_RETURN_OK;
}
