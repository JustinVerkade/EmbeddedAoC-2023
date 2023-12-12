/*
 * Day01.c
 *
 *  Created on: 9 aug. 2023
 *      Author: J.Verkade
 */

#include "AoC/Days/Day05.h"

typedef struct ConversionRange_t ConversionRange_t;
struct ConversionRange_t
{
	uint64_t input_from;
	uint64_t input_to;
	int64_t difference;
	uint64_t target;
	uint64_t from;
	uint64_t length;
};

typedef struct Conversion_t Conversion_t;
struct Conversion_t
{
	ConversionRange_t ranges[64];
	uint64_t range_count;
};

typedef struct Range_t Range_t;
struct Range_t
{
	uint64_t start;
	uint64_t length;
};

AdventOfCode_return_t AoC_Day_05(AdventOfCode_config_t config)
{
	// implementation state (change to OK when the day is active)
	if(config == AOC_CONFIG_TEST)
		return AOC_RETURN_OK;

	{ // part 1
		// fetch file
		FIL file;
		uint8_t ret;
		if(config == AOC_CONFIG_DEBUG_VERBOSE)
			ret = f_open(&file, "day5t.txt", FA_READ);
		else ret = f_open(&file, "day5.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// list of cards
		Conversion_t *conversions = (Conversion_t*)sram_malloc(sizeof(Conversion_t) * 8);
		memset(conversions, 0x00, sizeof(Conversion_t) * 8);

		// process initial seeds
		uint64_t initial_seed_count = 0;
		uint64_t initial_seeds[32];
		memset(initial_seeds, 0x00, sizeof(uint64_t) * 32);

		// get first line
		char buffer[256];
		f_gets(buffer, 256, &file);

		// tokenize data
		char token_buffer[32][32];
		memset(token_buffer, 0x00, 32 * 32);
		char *token = strtok(buffer, " ");
		uint8_t token_counter = 0;
		while(token != NULL && token_counter < 32)
		{
			memcpy(token_buffer[token_counter], token, strlen(token));
			token = strtok(NULL, " ");
			token_counter++;
		}

		// fill data in buffer
		initial_seed_count = token_counter - 1;
		for(uint8_t index=1; index<token_counter; index++)
			initial_seeds[index - 1] = atol(token_buffer[index]);

		// process rest of the data
		int64_t conversion_ptr = -1;
		while(f_gets(buffer, 256, &file))
		{
			// reset if newline
			if(buffer[0] == '\n')
			{
				conversion_ptr++;
				continue;
			}

			// if text
			if(buffer[0] < '0' || buffer[0] > '9')
			{

			}

			// is number
			else
			{
				// tokenize data
				char token_buffer[3][32];
				memset(token_buffer, 0x00, 3 * 32);
				char *token = strtok(buffer, " ");
				uint8_t token_counter = 0;
				while(token != NULL && token_counter < 3)
				{
					memcpy(token_buffer[token_counter], token, strlen(token));
					token = strtok(NULL, " ");
					token_counter++;
				}

				// fetch numbers
				uint64_t value_1 = atoll(token_buffer[0]);
				uint64_t value_2 = atoll(token_buffer[1]);
				uint64_t value_3 = atoll(token_buffer[2]);

				// insert numbers
				conversions[conversion_ptr].ranges[conversions[conversion_ptr].range_count].input_from = value_2;
				conversions[conversion_ptr].ranges[conversions[conversion_ptr].range_count].input_to = value_2 + value_3 - 1;
				conversions[conversion_ptr].ranges[conversions[conversion_ptr].range_count].difference = (int64_t)value_1 - (int64_t)value_2;

				// increment range count
				conversions[conversion_ptr].range_count++;
			}
		}
		conversion_ptr++;

		// process seeds
		for(uint8_t conversion_index=0; conversion_index<conversion_ptr; conversion_index++)
		{
			// convert values
			for(uint8_t value_index=0; value_index<initial_seed_count; value_index++)
			{
				// search if there is a range in which the seed exists
				for(uint8_t range_index=0; range_index<conversions[conversion_index].range_count; range_index++)
				{
					uint64_t input_from = conversions[conversion_index].ranges[range_index].input_from;
					uint64_t input_to = conversions[conversion_index].ranges[range_index].input_to;
					if(initial_seeds[value_index] >= input_from && initial_seeds[value_index] <= input_to)
					{
						initial_seeds[value_index] += conversions[conversion_index].ranges[range_index].difference;
						break;
					}
				}
			}
		}

		// find lowest value
		uint64_t lowest = 0x0FFFFFFFFFFFFFFF;
		for(uint8_t value_index=0; value_index<initial_seed_count; value_index++)
			if(initial_seeds[value_index] < lowest)
				lowest = initial_seeds[value_index];

		// output answer
		char answer[64];
		sprintf(answer, "    Part 1 answer: %lld\n", lowest);
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
			ret = f_open(&file, "day5t.txt", FA_READ);
		else ret = f_open(&file, "day5.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// list of cards
		Conversion_t *conversions = (Conversion_t*)sram_malloc(sizeof(Conversion_t) * 8);
		memset(conversions, 0x00, sizeof(Conversion_t) * 8);

		// process initial seeds
		uint64_t initial_seed_count = 0;
		uint64_t initial_seeds[32];
		memset(initial_seeds, 0x00, sizeof(uint64_t) * 32);

		// get first line
		char buffer[256];
		f_gets(buffer, 256, &file);

		// tokenize data
		char token_buffer[32][32];
		memset(token_buffer, 0x00, 32 * 32);
		char *token = strtok(buffer, " ");
		uint8_t token_counter = 0;
		while(token != NULL && token_counter < 32)
		{
			memcpy(token_buffer[token_counter], token, strlen(token));
			token = strtok(NULL, " ");
			token_counter++;
		}

		// fill data in buffer
		initial_seed_count = token_counter - 1;
		for(uint8_t index=1; index<token_counter; index++)
			initial_seeds[index - 1] = atoll(token_buffer[index]);

		// process rest of the data
		int64_t conversion_ptr = -1;
		while(f_gets(buffer, 256, &file))
		{
			// reset if newline
			if(buffer[0] == '\n')
			{
				conversion_ptr++;
				continue;
			}

			// if text
			if(buffer[0] < '0' || buffer[0] > '9')
			{

			}

			// is number
			else
			{
				// tokenize data
				char token_buffer[3][32];
				memset(token_buffer, 0x00, 3 * 32);
				char *token = strtok(buffer, " ");
				uint8_t token_counter = 0;
				while(token != NULL && token_counter < 3)
				{
					memcpy(token_buffer[token_counter], token, strlen(token));
					token = strtok(NULL, " ");
					token_counter++;
				}

				// fetch numbers
				uint64_t value_1 = atoll(token_buffer[0]);
				uint64_t value_2 = atoll(token_buffer[1]);
				uint64_t value_3 = atoll(token_buffer[2]);

				// insert numbers
				conversions[conversion_ptr].ranges[conversions[conversion_ptr].range_count].target = value_1;
				conversions[conversion_ptr].ranges[conversions[conversion_ptr].range_count].from = value_2;
				conversions[conversion_ptr].ranges[conversions[conversion_ptr].range_count].length = value_3;

				// increment range count
				conversions[conversion_ptr].range_count++;
			}
		}
		conversion_ptr++;

		// create partial buffer
		uint32_t buffer_size = 1000; // this should not work but it does
		Range_t *ranges = (Range_t*)sram_malloc(sizeof(Range_t) * buffer_size);
		Range_t *next_ranges = (Range_t*)sram_malloc(sizeof(Range_t) * buffer_size);
		uint32_t range_write_ptr = 0;
		memset(ranges, 0x00, sizeof(Range_t) * buffer_size);

		// fil range with current values
		uint64_t sum_of_lengths = 0;
		for(uint16_t seed_index=0; seed_index<initial_seed_count; seed_index+=2)
		{
			ranges[range_write_ptr].start = initial_seeds[seed_index];
			ranges[range_write_ptr].length = initial_seeds[seed_index+1];
			sum_of_lengths += initial_seeds[seed_index+1];
			range_write_ptr++;
		}

		// for each
		for(uint8_t conversion_index=0; conversion_index<conversion_ptr; conversion_index++)
		{
			// current layer conversion
			Conversion_t conversion = conversions[conversion_index];

			// create temporary next ranges
			uint32_t next_range_write_ptr = 0;
			memset(next_ranges, 0x00, sizeof(Range_t) * buffer_size);

			// for each range in current range
			for(uint32_t seed_index=0; seed_index<range_write_ptr; seed_index++)
			{
				// get current range
				Range_t current_range = ranges[seed_index];
				uint64_t start = current_range.start;
				uint64_t length = current_range.length;
				uint64_t end = start + length;
				uint8_t transformed = 0;

				// for each range
				uint64_t range_count = conversion.range_count;
				for(uint64_t range_index=0; range_index<range_count; range_index++)
				{
					// get curent conversion range
					ConversionRange_t conversion_range = conversion.ranges[range_index];
					uint64_t conversion_start = conversion_range.from;
					uint64_t conversion_end = conversion_start + conversion_range.length;
					uint64_t conversion_target = conversion_range.target;

					// discard if no overlap is occuring
					if(start >= conversion_end || end <= conversion_start)
					{
						if(range_index == range_count - 1 && end >= conversion_end)
							transformed = 0;
						continue;
					}

					// confirm transformation
					transformed = 1;

					// add input range if start starts before conversion range
					if(start < conversion_start)
					{
						next_ranges[next_range_write_ptr].start = start;
						next_ranges[next_range_write_ptr].length = conversion_start - start;
						next_range_write_ptr++;

						if(next_range_write_ptr == buffer_size)
							AdventOfCode_print(config, "Overflow!\n", strlen("Overflow!\n"));
					}

					// calculate overlapping part
					uint64_t overlap_start = (start > conversion_start) ? start : conversion_start;
					uint64_t overlap_end = (end < conversion_end) ? end : conversion_end;
					uint64_t overlap_length = overlap_end - overlap_start;
					uint64_t target_start = conversion_target + (overlap_start - conversion_start);

					// add overlap range
					next_ranges[next_range_write_ptr].start = target_start;
					next_ranges[next_range_write_ptr].length = overlap_length;
					next_range_write_ptr++;

					if(next_range_write_ptr == buffer_size)
						AdventOfCode_print(config, "Overflow!\n", strlen("Overflow!\n"));

					// add overlap exit if present
					if(end > conversion_end)
					{
						start = conversion_end;
						length = end - conversion_end;
					}
				}

				// if not transformed add it back into queue
				if(!transformed)
				{
					next_ranges[next_range_write_ptr].start = start;
					next_ranges[next_range_write_ptr].length = length;
					next_range_write_ptr++;

					if(next_range_write_ptr == buffer_size)
						AdventOfCode_print(config, "Overflow!\n", strlen("Overflow!\n"));
				}
			}

			// copy ranges into next
			memcpy(ranges, next_ranges, sizeof(Range_t) * buffer_size);
			range_write_ptr = next_range_write_ptr;
		}

		// get lowest range
		uint64_t end_total_size = 0;
		uint64_t lowest = 0x7FFFFFFFFFFFFFFF;
		for(uint32_t range=0; range<range_write_ptr; range++)
		{
			if(ranges[range].start < lowest && ranges[range].start)
			{
				lowest = ranges[range].start;
			}
			end_total_size += ranges[range].length;
		}

		// output answer
		char answer[64];
		sprintf(answer, "    Part 2 answer: %lld\n", lowest);
		AdventOfCode_print(config, answer, strlen(answer));

		// clean up
		f_close(&file);
		sram_clrmem();
	}

	// status code
	return AOC_RETURN_OK;
}
