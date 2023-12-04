/*
 * Day01.c
 *
 *  Created on: 9 aug. 2023
 *      Author: J.Verkade
 */

#include "AoC/Days/Day02.h"

AdventOfCode_return_t AoC_Day_02(AdventOfCode_config_t config)
{
	// implementation state (change to OK when the day is active)
	if(config == AOC_CONFIG_TEST)
		return AOC_RETURN_OK;

	{ // part 1
		// fetch file
		FIL file;
		uint8_t ret;
		if(config == AOC_CONFIG_DEBUG_VERBOSE)
			ret = f_open(&file, "day2t.txt", FA_READ);
		else ret = f_open(&file, "day2.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// init counter variables
		const uint32_t red_cubes_limit = 12;
		const uint32_t blue_cubes_limit = 14;
		const uint32_t green_cubes_limit = 13;

		// process data
		char buffer[256];
		int32_t sum_of_values = 0;
		while(f_gets(buffer, 256, &file))
		{
			// tokenize data
			char token_buffer[64][16];
			memset(token_buffer, 0x00, 64 * 16);
			char *token = strtok(buffer, " ");
			uint8_t token_counter = 0;
			while(token != NULL && token_counter < 64)
			{
				memcpy(token_buffer[token_counter], token, strlen(token));
				token = strtok(NULL, " ");
				token_counter++;
			}

			if(token_counter == 0)
				continue;

			// counter cubes
			uint32_t red_cubes = 0;
			uint32_t blue_cubes = 0;
			uint32_t green_cubes = 0;

			// process tokens
			uint8_t valid_flag = 1;
			uint8_t token_ptr = 2;
			while(token_ptr <= token_counter)
			{
				// get value
				uint32_t cubes = atoi(token_buffer[token_ptr]);

				// add to specific color
				if(!memcmp("green", token_buffer[token_ptr+1], 5))
					green_cubes += cubes;
				else if(!memcmp("blue", token_buffer[token_ptr+1], 4))
					blue_cubes += cubes;
				else if(!memcmp("red", token_buffer[token_ptr+1], 3))
					red_cubes += cubes;

				// check if end of game
				char end_token = token_buffer[token_ptr+1][strlen(token_buffer[token_ptr+1])-1];
				if(end_token == ';' || end_token == '\0')
				{
					if(red_cubes > red_cubes_limit || blue_cubes > blue_cubes_limit || green_cubes > green_cubes_limit)
					{
						valid_flag = 0;
						break;
					}

					// reset cubes
					red_cubes = 0;
					blue_cubes = 0;
					green_cubes = 0;
				}
				token_ptr += 2;
			}

			// check if valid game
			if(valid_flag)
			{
				uint32_t id = atoi(token_buffer[1]);
				sum_of_values += id;
			}
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
			ret = f_open(&file, "day2t.txt", FA_READ);
		else ret = f_open(&file, "day2.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// process data
		char buffer[256];
		int32_t sum_of_values = 0;
		while(f_gets(buffer, 256, &file))
		{
			// tokenize data
			char token_buffer[64][16];
			memset(token_buffer, 0x00, 64 * 16);
			char *token = strtok(buffer, " ");
			uint8_t token_counter = 0;
			while(token != NULL && token_counter < 64)
			{
				memcpy(token_buffer[token_counter], token, strlen(token));
				token = strtok(NULL, " ");
				token_counter++;
			}

			if(token_counter == 0)
				continue;

			// minimum cubes
			uint32_t red_cubes_minimum = 0;
			uint32_t blue_cubes_minimum = 0;
			uint32_t green_cubes_minimum = 0;

			// counter cubes
			uint32_t red_cubes = 0;
			uint32_t blue_cubes = 0;
			uint32_t green_cubes = 0;

			// process tokens
			uint8_t token_ptr = 2;
			while(token_ptr <= token_counter)
			{
				// get value
				uint32_t cubes = atoi(token_buffer[token_ptr]);

				// add to specific color
				if(!memcmp("green", token_buffer[token_ptr+1], 5))
					green_cubes += cubes;
				else if(!memcmp("blue", token_buffer[token_ptr+1], 4))
					blue_cubes += cubes;
				else if(!memcmp("red", token_buffer[token_ptr+1], 3))
					red_cubes += cubes;

				// check if end of game
				char end_token = token_buffer[token_ptr+1][strlen(token_buffer[token_ptr+1])-1];
				if(end_token == ';' || end_token == '\0')
				{
					// set minimum cubes needed
					if(red_cubes > red_cubes_minimum)
						red_cubes_minimum = red_cubes;
					if(blue_cubes > blue_cubes_minimum)
						blue_cubes_minimum = blue_cubes;
					if(green_cubes > green_cubes_minimum)
						green_cubes_minimum = green_cubes;

					// reset cubes
					red_cubes = 0;
					blue_cubes = 0;
					green_cubes = 0;
				}
				token_ptr += 2;
			}

			// sum of game power
			uint32_t game_power = red_cubes_minimum * blue_cubes_minimum * green_cubes_minimum;
			sum_of_values += game_power;
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
