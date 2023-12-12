/*
 * Day01.c
 *
 *  Created on: 9 aug. 2023
 *      Author: J.Verkade
 */

#include "math.h"
#include "AoC/Days/Day06.h"

typedef struct Game_t Game_t;
struct Game_t
{
	uint64_t duration;
	uint64_t distance;
};

AdventOfCode_return_t AoC_Day_06(AdventOfCode_config_t config)
{
	// implementation state (change to OK when the day is active)
	if(config == AOC_CONFIG_TEST)
		return AOC_RETURN_OK;

	{ // part 1
		// fetch file
		FIL file;
		uint8_t ret;
		if(config == AOC_CONFIG_DEBUG_VERBOSE)
			ret = f_open(&file, "day6t.txt", FA_READ);
		else ret = f_open(&file, "day6.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// create games
		uint64_t total_games = 0;
		Game_t games[20];
		memset(games, 0x00, sizeof(Game_t) * 20);

		{
			// process duration
			char buffer[256];
			f_gets(buffer, 256, &file);

			// tokenize data
			char token_buffer[8][16];
			memset(token_buffer, 0x00, 8 * 16);
			char *token = strtok(buffer, " ");
			uint8_t token_counter = 0;
			while(token != NULL && token_counter < 8)
			{
				memcpy(token_buffer[token_counter], token, strlen(token));
				token = strtok(NULL, " ");
				token_counter++;
			}

			// insert game size
			total_games = token_counter - 1;

			// for each token after the first insert to game
			for(uint8_t index=1; index<token_counter; index++)
			{
				games[index - 1].duration = atoll(token_buffer[index]);
			}
		}

		{
			// process distance
			char buffer[256];
			f_gets(buffer, 256, &file);

			// tokenize data
			char token_buffer[8][16];
			memset(token_buffer, 0x00, 8 * 16);
			char *token = strtok(buffer, " ");
			uint8_t token_counter = 0;
			while(token != NULL && token_counter < 8)
			{
				memcpy(token_buffer[token_counter], token, strlen(token));
				token = strtok(NULL, " ");
				token_counter++;
			}

			// for each token after the first insert to game
			for(uint8_t index=1; index<token_counter; index++)
			{
				games[index - 1].distance = atoll(token_buffer[index]);
			}
		}

		// calculate the distances
		uint64_t mul = 1;
		for(uint8_t index=0; index<total_games; index++)
		{
			uint64_t lower = (int32_t)ceil(0.5f * (games[index].duration - sqrtf(games[index].duration * games[index].duration - 4.0f * ((float)games[index].distance + 0.001f))));
			uint64_t upper = (int32_t)floor(0.5f * (games[index].duration + sqrtf(games[index].duration * games[index].duration - 4.0f * ((float)games[index].distance + 0.001f))));
			uint64_t length = upper - lower + 1;
			mul *= length;
		}

		// output answer
		char answer[64];
		sprintf(answer, "    Part 1 answer: %llu\n", mul);
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
			ret = f_open(&file, "day6t2.txt", FA_READ);
		else ret = f_open(&file, "day62.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// create games
		Game_t games[20];
		memset(games, 0x00, sizeof(Game_t) * 20);

		{
			// process duration
			char buffer[256];
			f_gets(buffer, 256, &file);

			// tokenize data
			char token_buffer[8][16];
			memset(token_buffer, 0x00, 8 * 16);
			char *token = strtok(buffer, " ");
			uint8_t token_counter = 0;
			while(token != NULL && token_counter < 8)
			{
				memcpy(token_buffer[token_counter], token, strlen(token));
				token = strtok(NULL, " ");
				token_counter++;
			}

			// for each token after the first insert to game
			for(uint8_t index=1; index<token_counter; index++)
			{
				games[index - 1].duration = atoll(token_buffer[index]);
			}
		}

		{
			// process distance
			char buffer[256];
			f_gets(buffer, 256, &file);

			// tokenize data
			char token_buffer[8][16];
			memset(token_buffer, 0x00, 8 * 16);
			char *token = strtok(buffer, " ");
			uint8_t token_counter = 0;
			while(token != NULL && token_counter < 8)
			{
				memcpy(token_buffer[token_counter], token, strlen(token));
				token = strtok(NULL, " ");
				token_counter++;
			}

			// for each token after the first insert to game
			for(uint8_t index=1; index<token_counter; index++)
			{
				games[index - 1].distance = atoll(token_buffer[index]);
			}
		}

		{
			char answer[64];
			sprintf(answer, "Duration: %llu Distance: %llu\n", games[0].duration, games[0].distance);
			AdventOfCode_print(config, answer, strlen(answer));
		}

		// calculate the distances
		uint64_t lower = (uint64_t)ceil(0.5 * (games[0].duration - sqrt(games[0].duration * games[0].duration - 4.0 * ((double)games[0].distance + 0.001))));
		uint64_t upper = (uint64_t)floor(0.5 * (games[0].duration + sqrt(games[0].duration * games[0].duration - 4.0 * ((double)games[0].distance + 0.001))));
		uint64_t length = upper - lower + 1;

		// output answer
		char answer[64];
		sprintf(answer, "    Part 2 answer: %llu\n", length);
		AdventOfCode_print(config, answer, strlen(answer));

		// clean up
		f_close(&file);
		sram_clrmem();
	}


	// status code
	return AOC_RETURN_OK;
}
