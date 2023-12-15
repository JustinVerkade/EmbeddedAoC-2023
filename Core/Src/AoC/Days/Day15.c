/*
 * Day01.c
 *
 *  Created on: 9 aug. 2023
 *      Author: J.Verkade
 */

#include "AoC/Days/Day15.h"

typedef struct Box_t Box_t;
struct Box_t
{
	uint8_t lenses[256][16];
	uint8_t lens_ptr;
};

uint8_t getHash(uint8_t *hash, uint8_t len)
{
	uint8_t target = 0;
	for(uint8_t i=0; i<len; i++)
	{
		target += hash[i];
		target *= 17;
	}
	return target;
}

void pushLens(Box_t *box, uint8_t *label, uint8_t focal_length)
{
	// create box string
	char box_label[16];
	sprintf(box_label, "%s%u", (char*)label, focal_length);

	// check if already in the list
	uint8_t index;
	for(index=0; index<box->lens_ptr; index++)
		if(!memcmp(box->lenses[index], label, strlen((char*)label)))
			break;

	// insert label into box
	memcpy(box->lenses[index], box_label, strlen((char*)box_label));
	if(index == box->lens_ptr)
		box->lens_ptr++;
}

uint8_t popLens(Box_t *box, uint8_t *label)
{
	// find index of lens
	uint8_t index;
	for(index=0; index<box->lens_ptr; index++)
		if(!memcmp(box->lenses[index], label, strlen((char*)label+1)))
			break;

	// disregard if not pressent
	if(index == box->lens_ptr)
		return 255;

	// move all positions backwards
	for(; index<box->lens_ptr; index++)
		memcpy(box->lenses[index], box->lenses[index+1], 16);

	// decrement ptr
	box->lens_ptr--;

	return index;
}

void printBoxes(Box_t *boxes)
{
	// print boxes
	for(uint16_t i=0; i<256; i++)
	{
		// abort if not filled
		if(boxes[i].lens_ptr == 0)
			continue;

		// print box
		{
			char answer[64];
			sprintf(answer, "Box %u: ", i);
			AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, answer, strlen(answer));
		}

		// print items
		for(uint8_t j=0; j<boxes[i].lens_ptr; j++)
		{
			char answer[64];
			sprintf(answer, "[%s]", boxes[i].lenses[j]);
			AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, answer, strlen(answer));
		}

		// newline
		{
			AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "\n", 1);
		}
	}
}

uint32_t calculateAnswer(Box_t *boxes)
{
	uint32_t sum = 0;
	for(uint16_t i=0; i<256; i++)
	{
		// abort if not filled
		if(boxes[i].lens_ptr == 0)
			continue;

		// print items
		for(uint8_t j=0; j<boxes[i].lens_ptr; j++)
		{
			uint32_t box = i + 1;
			uint32_t row = j + 1;
			uint32_t fcl = atoi(&boxes[i].lenses[j][strlen(boxes[i].lenses[j])-1]);

			sum += box * row * fcl;
		}
	}
	return sum;
}

AdventOfCode_return_t AoC_Day_15(AdventOfCode_config_t config)
{
	// implementation state (change to OK when the day is active)
	if(config == AOC_CONFIG_TEST)
		return AOC_RETURN_OK;

	{ // part 1
		// fetch file
		FIL file;
		uint8_t ret;
		if(config == AOC_CONFIG_DEBUG_VERBOSE)
			ret = f_open(&file, "day15t.txt", FA_READ);
		else ret = f_open(&file, "day15.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		uint8_t c[2];
		uint32_t sum = 0;
		uint8_t target_value = 0;
		while(f_gets((char*)c, 2, &file))
		{
			// end of command
			if(c[0] == ',' || c[0] == 0x00)
			{
				sum += target_value;
				target_value = 0;
			}

			// process command
			else
			{
				target_value += *c;
				target_value *= 17;
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

	{ // part 2
		// fetch file
		FIL file;
		uint8_t ret;
		if(config == AOC_CONFIG_DEBUG_VERBOSE)
			ret = f_open(&file, "day15t.txt", FA_READ);
		else ret = f_open(&file, "day15.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// create boxes
		Box_t *boxes = (Box_t*)sram_calloc(256, sizeof(Box_t));

		// process operations
		uint8_t c[2];
		uint8_t label[16];
		uint8_t label_ptr = 0;
		while(f_gets((char*)c, 2, &file))
		{
			// end of command
			if(c[0] == ',' || c[0] == 0x00)
			{
				// push assignment
				if(label[label_ptr - 2] == '=')
				{
					// get value
					char value[2] = {label[label_ptr - 1], 0x00};
					uint8_t focal_length = atoi(value);

					// clear process type
					label[label_ptr - 2] = ' ';
					label[label_ptr - 1] = 0x00;

					// get code hash
					uint8_t hash = getHash(label, strlen((char*)label) - 1);

					// insert hash into box
					pushLens(&boxes[hash], label, focal_length);
				}

				// subtraction operation
				else
				{
					// clear process type
					label[label_ptr - 1] = ' ';
					label[label_ptr] = 0x00;

					// get code hash
					uint8_t hash = getHash(label, strlen((char*)label) - 1);

					// pop lens from the box
					popLens(&boxes[hash], label);
				}

				// reset label
				memset(label, 0x00, 16);
				label_ptr = 0;
			}

			// process command
			else
			{
				// insert character
				label[label_ptr] = *c;
				label_ptr++;
			}
		}

		// debug print
		//printBoxes(boxes);

		// calculate output
		uint32_t result = calculateAnswer(boxes);

		// output answer
		char answer[64];
		sprintf(answer, "    Part 2 answer: %lu\n", result);
		AdventOfCode_print(config, answer, strlen(answer));

		// clean up
		f_close(&file);
		sram_clrmem();
	}

	// status code
	return AOC_RETURN_OK;
}
