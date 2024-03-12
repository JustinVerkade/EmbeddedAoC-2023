/*
 * Day01.c
 *
 *  Created on: 9 aug. 2023
 *      Author: J.Verkade
 */

#include "AoC/Days/Day17.h"

#define N 	(Direction_t){ 0,-1}
#define W	(Direction_t){-1, 0}
#define S	(Direction_t){ 0, 1}
#define E	(Direction_t){ 1, 0}

typedef struct Direction_t Direction_t;
struct Direction_t
{
	int8_t x;
	int8_t y;
};

typedef struct Position_t Position_t;
struct Position_t
{
	int16_t x;
	int16_t y;
	uint32_t value;
};

uint8_t Position_isValid(Position_t *position, uint8_t width, uint8_t depth)
{
	if(position->x < 0 && position->x >= width)
		return 0;
	if(position->y < 0 && position->y >= depth)
		return 0;
	return 1;
}

uint8_t Position_validDirection(Position_t *position, Direction_t dir, uint8_t width, uint8_t depth)
{
	Position_t new_position = *position;
	new_position.x += dir.x;
	new_position.y += dir.y;
	return Position_isValid(&new_position, width, depth);
}

void Blocks_print(char *blocks, uint8_t width, uint8_t depth)
{
	char tx_buffer[256];
	sprintf(tx_buffer, "char blocks[%u][%u] = {\n", depth, width);
	AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, tx_buffer, strlen(tx_buffer));
	for(uint8_t i=0; i<depth; i++)
	{
		sprintf(tx_buffer, "    %s\n", &blocks[i * (width + 1)]);
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, tx_buffer, strlen(tx_buffer));
		HAL_Delay(10);
	}
	AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "}\n", 2);
}

typedef struct Queue_t Queue_t;
struct Queue_t
{
	Position_t *positions;
	uint32_t max;
	uint32_t ptr;
};

uint8_t Queue_push(Queue_t *queue, int16_t x, int16_t y, uint32_t value)
{
	// quit if full
	if(queue->ptr == queue->max)
		return 1;

	queue->positions[queue->ptr].x = x;
	queue->positions[queue->ptr].y = y;
	queue->positions[queue->ptr].y = value;
	queue->ptr++;

	return 0;
}

Position_t Queue_pop(Queue_t *queue)
{
	Position_t pop = queue->positions[0];
	memcpy(&queue->positions[0], &queue->positions[1], (queue->ptr - 1) * sizeof(Position_t));
	queue->ptr--;
	return pop;
}

AdventOfCode_return_t AoC_Day_17(AdventOfCode_config_t config)
{
	// implementation state (change to OK when the day is active)
	if(config == AOC_CONFIG_TEST)
		return AOC_RETURN_OK;

	{ // part 1
		// fetch file
		FIL file;
		uint8_t ret;
		if(config == AOC_CONFIG_DEBUG_VERBOSE)
			ret = f_open(&file, "day17t.txt", FA_READ);
		else ret = f_open(&file, "day17.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// get file length
		int32_t file_size = f_size(&file);

		// extract data
		char *blocks = (char*)sram_malloc(file_size + 1);
		char *path = (char*)sram_calloc(file_size + 1, 1);

		// insert data
		f_read(&file, blocks, file_size + 1, NULL);
		memcpy(path, blocks, file_size + 1);

		// close file
		f_close(&file);

		// get map dimensions and insert line ends
		char *token = strtok(blocks, "\n");
		uint8_t width = strlen(token);
		uint8_t depth = 1;
		while(strtok(NULL, "\n"))
			depth++;

		// test blocks
		Blocks_print(blocks, width, depth);

		// create queue
		Queue_t queue;
		queue.positions = (Position_t*)sram_calloc(1024, sizeof(Position_t));
		queue.max = 1024;
		queue.ptr = 0;

		// insert starting position
		Queue_push(&queue, 0, 0, 0);
		path[0] = 0;

		// BFS
		while(queue.ptr)
		{
			// pop position
			Position_t position = Queue_pop(&queue);
			(void)position;
		}

//		// output answer
//		char answer[64];
//		sprintf(answer, "    Part 1 answer: %u\n", counter);
//		AdventOfCode_print(config, answer, strlen(answer));

		// clean up
		sram_clrmem();
	}

	// status code
	return AOC_RETURN_OK;
}
