/*
 * Day01.c
 *
 *  Created on: 9 aug. 2023
 *      Author: J.Verkade
 */

#include "AoC/Days/Day04.h"

typedef struct Card_t Card_t;
struct Card_t
{
	uint8_t id;
	uint8_t winning[16];
	uint8_t winning_ptr;
	uint8_t numbers[32];
	uint8_t numbers_ptr;
	uint32_t instances;
};

AdventOfCode_return_t AoC_Day_04(AdventOfCode_config_t config)
{
	// implementation state (change to OK when the day is active)
	if(config == AOC_CONFIG_TEST)
		return AOC_RETURN_OK;

	{ // part 1
		// fetch file
		FIL file;
		uint8_t ret;
		if(config == AOC_CONFIG_DEBUG_VERBOSE)
			ret = f_open(&file, "day4t.txt", FA_READ);
		else ret = f_open(&file, "day4.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// list of cards
		Card_t *cards = (Card_t*)sram_malloc(sizeof(Card_t) * 210);
		uint8_t card_index = 0;
		memset(cards, 0x00, sizeof(Card_t) * 210);

		// iterate lines
		char buffer[128];
		while(f_gets(buffer, 128, &file))
		{
			// tokenize data
			char token_buffer[48][8];
			memset(token_buffer, 0x00, 48 * 8);
			char *token = strtok(buffer, " ");
			uint8_t token_counter = 0;
			while(token != NULL && token_counter < 48)
			{
				memcpy(token_buffer[token_counter], token, strlen(token));
				token = strtok(NULL, " ");
				token_counter++;
			}

			// insert card id
			cards[card_index].id = atoi(token_buffer[1]);

			// process winning numbers
			uint8_t winning_number_index;
			memset(cards[card_index].winning, 0x00, 16);
			for(winning_number_index=2; token_buffer[winning_number_index][0]!='|' && winning_number_index < 16; winning_number_index++)
			{
				uint8_t value = atoi(token_buffer[winning_number_index]);
				cards[card_index].winning[winning_number_index - 2] = value;
			}
			cards[card_index].winning_ptr = winning_number_index - 2;

			// process card numbers
			uint8_t card_number_index = winning_number_index + 1;
			memset(cards[card_index].numbers, 0x00, 32);
			for(; token_buffer[card_number_index][0]!=0x00 && (card_number_index - winning_number_index - 1) < 32; card_number_index++)
			{
				uint8_t value = atoi(token_buffer[card_number_index]);
				cards[card_index].numbers[card_number_index - winning_number_index - 1] = value;
			}
			cards[card_index].numbers_ptr = card_number_index - winning_number_index - 1;
			card_index++;
		}

		// process cards
		uint32_t sum_of_values = 0;
		for(uint8_t index=0; index<card_index; index++)
		{
			// for each number card
			uint32_t card_worth = 0;
			for(uint8_t numbers_index=0; numbers_index<cards[index].numbers_ptr; numbers_index++)
			{
				// check if in list
				for(uint8_t winning_index=0; winning_index<cards[index].winning_ptr; winning_index++)
				{
					if(cards[index].numbers[numbers_index] == cards[index].winning[winning_index])
					{
						if(!card_worth)
							card_worth = 1;
						else card_worth *= 2;
					}
				}
			}
			sum_of_values += card_worth;
		}

		// output answer
		char answer[32];
		sprintf(answer, "    Part 1 answer: %ld\n", sum_of_values);
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
			ret = f_open(&file, "day4t.txt", FA_READ);
		else ret = f_open(&file, "day4.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("    Cannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// list of cards
		Card_t *cards = (Card_t*)sram_malloc(sizeof(Card_t) * 210);
		uint8_t card_index = 0;
		memset(cards, 0x00, sizeof(Card_t) * 210);

		// iterate lines
		char buffer[128];
		while(f_gets(buffer, 128, &file))
		{
			// tokenize data
			char token_buffer[48][8];
			memset(token_buffer, 0x00, 48 * 8);
			char *token = strtok(buffer, " ");
			uint8_t token_counter = 0;
			while(token != NULL && token_counter < 48)
			{
				memcpy(token_buffer[token_counter], token, strlen(token));
				token = strtok(NULL, " ");
				token_counter++;
			}

			// insert card id
			cards[card_index].id = atoi(token_buffer[1]);

			// process winning numbers
			uint8_t winning_number_index;
			memset(cards[card_index].winning, 0x00, 16);
			for(winning_number_index=2; token_buffer[winning_number_index][0]!='|' && winning_number_index < 16; winning_number_index++)
			{
				uint8_t value = atoi(token_buffer[winning_number_index]);
				cards[card_index].winning[winning_number_index - 2] = value;
			}
			cards[card_index].winning_ptr = winning_number_index - 2;

			// process card numbers
			uint8_t card_number_index = winning_number_index + 1;
			memset(cards[card_index].numbers, 0x00, 32);
			for(; token_buffer[card_number_index][0]!=0x00 && (card_number_index - winning_number_index - 1) < 32; card_number_index++)
			{
				uint8_t value = atoi(token_buffer[card_number_index]);
				cards[card_index].numbers[card_number_index - winning_number_index - 1] = value;
			}
			cards[card_index].numbers_ptr = card_number_index - winning_number_index - 1;
			card_index++;
		}

		// set instances to 1
		for(uint8_t index=0; index<210; index++)
			cards[index].instances = 1;

		// calculate answer
		uint32_t sum_of_values = 0;
		for(int16_t index=card_index-1; index>=0; index--)
		{
			// calculate matching cards
			uint8_t matches = 0;
			for(uint8_t winning_index=0; winning_index<cards[index].winning_ptr; winning_index++)
				for(uint8_t numbers_index=0; numbers_index<cards[index].numbers_ptr; numbers_index++)
					if(cards[index].numbers[numbers_index] == cards[index].winning[winning_index])
						matches++;

			// for the matches below the current match save instances
			for(uint8_t next_index=0; next_index<matches; next_index++)
				cards[index].instances += cards[next_index + index + 1].instances;

			// increment total
			sum_of_values += cards[index].instances;
		}


		// output answer
		char answer[32];
		sprintf(answer, "    Part 2 answer: %ld\n", sum_of_values);
		AdventOfCode_print(config, answer, strlen(answer));

		// clean up
		f_close(&file);
		sram_clrmem();
	}

	// status code
	return AOC_RETURN_OK;
}
