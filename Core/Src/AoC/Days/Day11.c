/*
 * Day01.c
 *
 *  Created on: 9 aug. 2023
 *      Author: J.Verkade
 */

#include "AoC/Days/Day11.h"

typedef struct Galaxy_t Galaxy_t;
struct Galaxy_t
{
	int32_t id;
	int32_t x;
	int32_t y;
	int32_t status;
};

typedef struct Roadmap_t Roadmap_t;
struct Roadmap_t
{
	uint32_t current;
	uint8_t stars_visited[512];
	uint32_t total_dist;
};

void printSpace(char *space, uint32_t width, uint32_t depth)
{
	for(uint32_t line=0; line<depth; line++)
	{
		uint32_t index = line * (width + 1);
		char tx_buffer[256];
		sprintf(tx_buffer, "%s\n", &space[index]);
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, tx_buffer, strlen(tx_buffer));
		HAL_Delay(10);
	}
}

uint32_t distance(Galaxy_t *galaxy0, Galaxy_t* galaxy1)
{
	uint32_t offset_x = abs(galaxy0->x - galaxy1->x);
	uint32_t offset_y = abs(galaxy0->y - galaxy1->y);
	return offset_x + offset_y;
}

AdventOfCode_return_t AoC_Day_11(AdventOfCode_config_t config)
{
	// implementation state (change to OK when the day is active)
	if(config == AOC_CONFIG_TEST)
		return AOC_RETURN_OK;

	{ // part 1
		// fetch file
		FIL file;
		uint8_t ret;
		if(config == AOC_CONFIG_DEBUG_VERBOSE)
			ret = f_open(&file, "day11t.txt", FA_READ);
		else ret = f_open(&file, "day11.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// get file length
		int32_t file_size = f_size(&file);

		// extract data
		char *space = (char*)sram_malloc(file_size + 1);

		// insert data
		f_read(&file, space, file_size + 1, NULL);

		// close file
		f_close(&file);

		// get space dimensions and insert line ends
		char *token = strtok(space, "\n");
		uint32_t space_width = strlen(token);
		uint32_t counter = 1;
		while(strtok(NULL, "\n"))
			counter++;

		// count galaxies
		uint32_t total_galaxies = 0;
		for(uint32_t x=0; x<space_width; x++)
			for(uint32_t y=0; y<space_width; y++)
				if(space[y * (space_width + 1) + x] == '#')
					total_galaxies++;

		// create galaxy entites
		uint32_t galaxy_ptr = 0;
		Galaxy_t *galaxies = (Galaxy_t*)sram_calloc(total_galaxies, sizeof(Galaxy_t));

		// process X coordinates
		uint32_t current_x = 0;
		for(uint32_t x=0; x<space_width; x++)
		{
			uint32_t is_empty = 1;
			for(uint32_t y=0; y<space_width; y++)
			{
				uint32_t index = y * (space_width + 1) + x;
				if(space[index] == '#')
				{
					is_empty = 0;
					galaxies[galaxy_ptr].id = index;
					galaxies[galaxy_ptr].x = current_x;
					galaxy_ptr++;
				}
			}
			current_x += 1 + is_empty;
		}

		// process Y coordinates
		uint32_t current_y = 0;
		for(uint32_t y=0; y<space_width; y++)
		{
			uint32_t is_empty = 1;
			for(uint32_t x=0; x<space_width; x++)
			{
				uint32_t index = y * (space_width + 1) + x;
				if(space[index] == '#')
				{
					// locate galaxy id
					galaxy_ptr = 0;
					for(; galaxy_ptr<total_galaxies; galaxy_ptr++)
						if(galaxies[galaxy_ptr].id == index)
							break;

					is_empty = 0;
					galaxies[galaxy_ptr].y = current_y;
					galaxy_ptr++;
				}
			}
			current_y += 1 + is_empty;
		}

		// process pairs
		uint64_t total_distance = 0;
		for(uint32_t i=0; i<total_galaxies; i++)
		{
			for(uint32_t j=i+1; j<total_galaxies; j++)
			{
				Galaxy_t *galaxy0 = &galaxies[i];
				Galaxy_t *galaxy1 = &galaxies[j];
				uint32_t dist = distance(galaxy0, galaxy1);
				total_distance += dist;
			}
		}

		// output answer
		char answer[64];
		sprintf(answer, "    Part 1 answer: %llu\n", total_distance);
		AdventOfCode_print(config, answer, strlen(answer));

		// clean up
		sram_clrmem();
	}

	{ // part 2
		// fetch file
		FIL file;
		uint8_t ret;
		uint32_t multiplier;
		if(config == AOC_CONFIG_DEBUG_VERBOSE)
		{
			ret = f_open(&file, "day11t.txt", FA_READ);
			multiplier = 10;
		}
		else
		{
			ret = f_open(&file, "day11.txt", FA_READ);
			multiplier = 1000000;
		}
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// get file length
		int32_t file_size = f_size(&file);

		// extract data
		char *space = (char*)sram_malloc(file_size + 1);

		// insert data
		f_read(&file, space, file_size + 1, NULL);

		// close file
		f_close(&file);

		// get space dimensions and insert line ends
		char *token = strtok(space, "\n");
		uint32_t space_width = strlen(token);
		uint32_t counter = 1;
		while(strtok(NULL, "\n"))
			counter++;

		// count galaxies
		uint32_t total_galaxies = 0;
		for(uint32_t x=0; x<space_width; x++)
			for(uint32_t y=0; y<space_width; y++)
				if(space[y * (space_width + 1) + x] == '#')
					total_galaxies++;

		// create galaxy entites
		uint32_t galaxy_ptr = 0;
		Galaxy_t *galaxies = (Galaxy_t*)sram_calloc(total_galaxies, sizeof(Galaxy_t));

		// process X coordinates
		uint32_t current_x = 0;
		for(uint32_t x=0; x<space_width; x++)
		{
			uint32_t is_empty = 1;
			for(uint32_t y=0; y<space_width; y++)
			{
				uint32_t index = y * (space_width + 1) + x;
				if(space[index] == '#')
				{
					is_empty = 0;
					galaxies[galaxy_ptr].id = index;
					galaxies[galaxy_ptr].x = current_x;
					galaxy_ptr++;
				}
			}
			current_x += 1 + is_empty * (multiplier - 1);
		}

		// process Y coordinates
		uint32_t current_y = 0;
		for(uint32_t y=0; y<space_width; y++)
		{
			uint32_t is_empty = 1;
			for(uint32_t x=0; x<space_width; x++)
			{
				uint32_t index = y * (space_width + 1) + x;
				if(space[index] == '#')
				{
					// locate galaxy id
					galaxy_ptr = 0;
					for(; galaxy_ptr<total_galaxies; galaxy_ptr++)
						if(galaxies[galaxy_ptr].id == index)
							break;

					is_empty = 0;
					galaxies[galaxy_ptr].y = current_y;
					galaxy_ptr++;
				}
			}
			current_y += 1 + is_empty * (multiplier - 1);
		}

		// process pairs
		uint64_t total_distance = 0;
		for(uint32_t i=0; i<total_galaxies; i++)
		{
			for(uint32_t j=i+1; j<total_galaxies; j++)
			{
				Galaxy_t *galaxy0 = &galaxies[i];
				Galaxy_t *galaxy1 = &galaxies[j];
				uint32_t dist = distance(galaxy0, galaxy1);
				total_distance += dist;
			}
		}

		// output answer
		char answer[64];
		sprintf(answer, "    Part 2 answer: %llu\n", total_distance);
		AdventOfCode_print(config, answer, strlen(answer));

		// clean up
		sram_clrmem();
	}

	// status code
	return AOC_RETURN_OK;
}
