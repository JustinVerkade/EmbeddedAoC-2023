/*
 * Day01.c
 *
 *  Created on: 9 aug. 2023
 *      Author: J.Verkade
 */

#include "math.h"
#include "AoC/Days/Day07.h"

uint8_t cardValuePart1(char c)
{
	if(c >= '0' && c <= '9')
		return c - '0';
	switch(c)
	{
	case 'T': return 10;
	case 'J': return 11;
	case 'Q': return 12;
	case 'K': return 13;
	case 'A': return 14;
	default: return 0;
	};
}

uint8_t cardValuePart2(char c)
{
	if(c >= '0' && c <= '9')
		return c - '0';
	switch(c)
	{
	case 'T': return 10;
	case 'J': return 1; // lowest value
	case 'Q': return 12;
	case 'K': return 13;
	case 'A': return 14;
	default: return 0;
	};
}

uint8_t swappedHandsPart1(char *hand0, char *hand1)
{
	for(uint8_t i=0; i<5; i++)
	{
		uint8_t card_value_0 = cardValuePart1(hand0[i]);
		uint8_t card_value_1 = cardValuePart1(hand1[i]);
		if(card_value_0 > card_value_1)
			return 0;
		if(card_value_0 < card_value_1)
			return 1;
	}
	return 0;
}

uint8_t swappedHandsPart2(char *hand0, char *hand1)
{
	for(uint8_t i=0; i<5; i++)
	{
		uint8_t card_value_0 = cardValuePart2(hand0[i]);
		uint8_t card_value_1 = cardValuePart2(hand1[i]);
		if(card_value_0 > card_value_1)
			return 0;
		if(card_value_0 < card_value_1)
			return 1;
	}
	return 0;
}

uint8_t handValue(char *hand)
{
	// create card counter
	uint8_t card_count[16];
	memset(card_count, 0x00, 16);

	// fill card count
	card_count[cardValuePart1(hand[0])] += 1;
	card_count[cardValuePart1(hand[1])] += 1;
	card_count[cardValuePart1(hand[2])] += 1;
	card_count[cardValuePart1(hand[3])] += 1;
	card_count[cardValuePart1(hand[4])] += 1;

	// get highest reward
	for(uint8_t card_type=0; card_type<16; card_type++)
	{
		// five of a kind
		if(card_count[card_type] == 5)
			return 7;

		// four of a kind
		if(card_count[card_type] == 4)
			return 6;

		// full house
		if(card_count[card_type] == 3)
		{
			// check for full house
			for(uint8_t card_index=0; card_index<16; card_index++)
				if(card_count[card_index] == 2)
					return 5;

			// three of a kind
			return 4;
		}

		// pairs
		if(card_count[card_type] == 2)
		{
			// check for two pair
			for(uint8_t card_index=0; card_index<16; card_index++)
			{
				// two pair
				if(card_count[card_index] == 2 && card_index != card_type)
					return 3;

				// full house
				if(card_count[card_index] == 3)
					return 5;
			}

			// 1 pair
			return 2;
		}
	}

	// high card
	return 1;
}

char characters[] = "AKQT98765432";
uint8_t bestRankValue(char *hand)
{
	// get amount of jokers
	uint8_t joker_count = 0;
	for(uint8_t i=0; i<5; i++)
		if(hand[i] == 'J')
			joker_count++;

	// total possible combinations
	char best_hand[6];
	uint8_t best_rank = 0;
	uint32_t possiblities = powl(12, joker_count);
	for(uint32_t posibility=0; posibility<possiblities; posibility++)
	{
		// calculate replacement index
		uint8_t replacements[5];
		replacements[0] = posibility % 12;
		replacements[1] = (posibility / 12) % 12;
		replacements[2] = (posibility / 144) % 12;
		replacements[3] = (posibility / 1728) % 12;
		replacements[4] = (posibility / 20736) % 12;

		// replace letters
		char check[6];
		memcpy(check, hand, 6);
		uint8_t joker_index = 0;
		for(uint8_t i=0; i<5; i++)
			if(check[i] == 'J')
				check[i] = characters[replacements[joker_index++]];

		// if first test just copy
		if(!best_rank)
		{
			memcpy(best_hand, check, 6);
			best_rank = handValue(best_hand);
			continue;
		}

		// if rank is better
		uint8_t current_rank = handValue(check);
		if(current_rank > best_rank)
		{
			memcpy(best_hand, check, 6);
			best_rank = current_rank;
		}
		else if(current_rank == best_rank && swappedHandsPart2(best_hand, check))
		{
			memcpy(best_hand, check, 6);
		}
	}
	return best_rank;
}

typedef struct Hand_t Hand_t;
struct Hand_t
{
	char hand[6];
	uint32_t bid;
	uint8_t rank;
	uint32_t subrank;
};

AdventOfCode_return_t AoC_Day_07(AdventOfCode_config_t config)
{
	// implementation state (change to OK when the day is active)
	if(config == AOC_CONFIG_TEST)
		return AOC_RETURN_OK;

	{ // part 1
		// fetch file
		FIL file;
		uint8_t ret;
		if(config == AOC_CONFIG_DEBUG_VERBOSE)
			ret = f_open(&file, "day7t.txt", FA_READ);
		else ret = f_open(&file, "day7.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// creat game hands
		int32_t hand_ptr = 0;
		Hand_t *hands = (Hand_t*)sram_calloc(1024, sizeof(Hand_t));

		// fill hands
		char buffer[128];
		while(f_gets(buffer, 128, &file))
		{
			char *hand_str = strtok(buffer, " ");
			memcpy(hands[hand_ptr].hand, hand_str, 5);
			hands[hand_ptr].bid = atoi(strtok(NULL, " "));
			hands[hand_ptr].rank = handValue(hand_str);
			hands[hand_ptr].subrank =
					cardValuePart1(hand_str[0]) * 65536 +
					cardValuePart1(hand_str[1]) * 4096 +
					cardValuePart1(hand_str[2]) * 256 +
					cardValuePart1(hand_str[3]) * 16 +
					cardValuePart1(hand_str[4]);
			hand_ptr++;
		}

		// sort hands
		uint32_t timeout = 0xFFFF;
		while(timeout--)
		{
			// go over each hand
			uint8_t sorted_flag = 1;
			for(uint16_t hand=0; hand<hand_ptr-1; hand++)
			{
				// order is swapped
				if(hands[hand+0].rank < hands[hand+1].rank)
				{
					// swap items
					Hand_t temp = hands[hand+0];
					hands[hand+0] = hands[hand+1];
					hands[hand+1] = temp;
					sorted_flag = 0;
				}

				// if the rank is the same value by card order
				else if(hands[hand+0].rank == hands[hand+1].rank && hands[hand+0].subrank < hands[hand+1].subrank)
				{
					// swap items
					Hand_t temp = hands[hand+0];
					hands[hand+0] = hands[hand+1];
					hands[hand+1] = temp;
					sorted_flag = 0;
				}
			}
			if(sorted_flag)
				break;
		}

		// check timeout
		if(!timeout)
		{
			AdventOfCode_print(config, "Timeout!\n", strlen("Timeout!\n"));
		}

		// go over each hand
		uint64_t sum_of_values = 0;
		for(int32_t hand=0; hand<hand_ptr; hand++)
			sum_of_values += hands[hand].bid * (hand_ptr - hand);

		// output answer
		char answer[64];
		sprintf(answer, "    Part 1 answer: %llu\n", sum_of_values);
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
			ret = f_open(&file, "day7t.txt", FA_READ);
		else ret = f_open(&file, "day7.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// creat game hands
		int32_t hand_ptr = 0;
		Hand_t *hands = (Hand_t*)sram_calloc(1024, sizeof(Hand_t));

		// fill hands
		char buffer[128];
		while(f_gets(buffer, 128, &file))
		{
			char *hand_str = strtok(buffer, " ");
			memcpy(hands[hand_ptr].hand, hand_str, 5);
			hands[hand_ptr].bid = atoi(strtok(NULL, " "));
			hands[hand_ptr].rank = bestRankValue(hand_str);
			hands[hand_ptr].subrank =
					cardValuePart2(hand_str[0]) * 65536 +
					cardValuePart2(hand_str[1]) * 4096 +
					cardValuePart2(hand_str[2]) * 256 +
					cardValuePart2(hand_str[3]) * 16 +
					cardValuePart2(hand_str[4]);
			hand_ptr++;
		}

		// sort hands
		uint32_t timeout = 0xFFFF;
		while(timeout--)
		{
			// go over each hand
			uint8_t sorted_flag = 1;
			for(uint16_t hand=0; hand<hand_ptr-1; hand++)
			{
				// order is swapped
				if(hands[hand+0].rank < hands[hand+1].rank)
				{
					// swap items
					Hand_t temp = hands[hand+0];
					hands[hand+0] = hands[hand+1];
					hands[hand+1] = temp;
					sorted_flag = 0;
				}

				// if the rank is the same value by card order
				else if(hands[hand+0].rank == hands[hand+1].rank && hands[hand+0].subrank < hands[hand+1].subrank)
				{
					// swap items
					Hand_t temp = hands[hand+0];
					hands[hand+0] = hands[hand+1];
					hands[hand+1] = temp;
					sorted_flag = 0;
				}
			}
			if(sorted_flag)
				break;
		}

		// check timeout
		if(!timeout)
		{
			AdventOfCode_print(config, "Timeout!\n", strlen("Timeout!\n"));
		}

		// go over each hand
		uint64_t sum_of_values = 0;
		for(int32_t hand=0; hand<hand_ptr; hand++)
			sum_of_values += hands[hand].bid * (hand_ptr - hand);

		// output answer
		char answer[64];
		sprintf(answer, "    Part 2 answer: %llu\n", sum_of_values);
		AdventOfCode_print(config, answer, strlen(answer));

		// clean up
		f_close(&file);
		sram_clrmem();
	}

	// status code
	return AOC_RETURN_OK;
}
