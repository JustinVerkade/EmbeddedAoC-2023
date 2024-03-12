/*
 * Day01.c
 *
 *  Created on: 9 aug. 2023
 *      Author: J.Verkade
 */

#include "AoC/Days/Day16.h"

typedef enum Dir_t Dir_t;
enum Dir_t
{
	UP=0,
	LEFT=1,
	DOWN=2,
	RIGHT=3,
	DERROR=4,
};

typedef struct Laser_t Laser_t;
struct Laser_t
{
	uint8_t x;
	uint8_t y;
	Dir_t dir;
};

typedef struct Queue_t Queue_t;
struct Queue_t
{
	Laser_t *lasers;
	uint8_t max;
	uint8_t ptr;
};

void printMap(char *map, uint8_t width, uint8_t depth)
{
	AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "Map[] = {\n", strlen("Map[] = \n{\n"));
	for(uint8_t i=0; i<depth; i++)
	{
		char tx_buffer[256];
		sprintf(tx_buffer, "    %s\n", &map[i * (width + 1)]);
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, tx_buffer, strlen(tx_buffer));
		HAL_Delay(10);
	}
	AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "}\n", strlen("}\n"));
}

void printQueue(Queue_t *queue)
{
	for(uint16_t i=0; i<queue->ptr; i++)
	{
		char tx_buffer[64];
		sprintf(tx_buffer, "Laser[%4u]: X:%u Y:%u\n", i, queue->lasers[i].x, queue->lasers[i].y);
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, tx_buffer, strlen(tx_buffer));
		HAL_Delay(100);
	}
}

uint8_t queuePush(Queue_t *queue, uint8_t x, uint8_t y, Dir_t dir)
{
	// check size
	if(queue->ptr == queue->max)
	{
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "Queue full!\n", strlen("Queue full!\n"));
		return 1;
	}

	// insert at the end
	queue->lasers[queue->ptr].x = x;
	queue->lasers[queue->ptr].y = y;
	queue->lasers[queue->ptr].dir = dir;
	queue->ptr++;

	return 0;
}

void queuePop(Queue_t *queue, uint8_t laser_id)
{
	// copy data from above taget to target id
	memcpy(&queue->lasers[laser_id], &queue->lasers[laser_id + 1], sizeof(Laser_t) * (queue->max - laser_id));
	queue->ptr--;
}

uint8_t laserStep(Laser_t *laser, uint8_t width, uint8_t depth)
{
	switch(laser->dir)
	{
	case UP:
		if(laser->y == 0)
			return 1;
		laser->y--;
		break;
	case DOWN:
		if(laser->y == depth - 1)
			return 1;
		laser->y++;
		break;
	case LEFT:
		if(laser->x == 0)
			return 1;
		laser->x--;
		break;
	case RIGHT:
		if(laser->x == width - 1)
			return 1;
		laser->x++;
		break;
	default:
		break;
	}
	return 0;
}

static uint8_t updateUp(Laser_t *laser, char condition)
{
	switch(condition)
	{
	case '.': // do nothing
		break;
	case '|': // do nothing
		break;
	case '/': // reflect right
		laser->dir = RIGHT;
		break;
	case '\\': // reflect left
		laser->dir = LEFT;
		break;
	case '-': // split
		laser->dir = RIGHT;
		return 1;
	default:
		break;
	}
	return 0;
}

static uint8_t updateDown(Laser_t *laser, char condition)
{
	switch(condition)
	{
	case '.': // do nothing
		break;
	case '|': // do nothing
		break;
	case '/': // reflect left
		laser->dir = LEFT;
		break;
	case '\\': // reflect right
		laser->dir = RIGHT;
		break;
	case '-': // split
		laser->dir = LEFT;
		return 1;
	default:
		break;
	}
	return 0;
}

static uint8_t updateLeft(Laser_t *laser, char condition)
{
	switch(condition)
	{
	case '.': // do nothing
		break;
	case '-': // do nothing
		break;
	case '/': // reflect down
		laser->dir = DOWN;
		break;
	case '\\': // reflect up
		laser->dir = UP;
		break;
	case '|': // split
		laser->dir = UP;
		return 1;
	default:
		break;
	}
	return 0;
}

static uint8_t updateRight(Laser_t *laser, char condition)
{
	switch(condition)
	{
	case '.': // do nothing
		break;
	case '-': // do nothing
		break;
	case '/': // reflect up
		laser->dir = UP;
		break;
	case '\\': // reflect up
		laser->dir = DOWN;
		break;
	case '|': // split
		laser->dir = DOWN;
		return 1;
	default:
		break;
	}
	return 0;
}

uint8_t laserUpdate(Laser_t *laser, char condition)
{
	switch(laser->dir)
	{
		case UP: return updateUp(laser, condition);
		case DOWN: return updateDown(laser, condition);
		case LEFT: return updateLeft(laser, condition);
		case RIGHT: return updateRight(laser, condition);
		default: break;
	}
	return 0;
}

uint8_t pushEdge(Queue_t *queue, uint16_t edge, uint8_t width, uint8_t depth)
{
	// get position and direction
	uint8_t position_x = 0;
	uint8_t position_y = 0;
	Dir_t direction = DERROR;
	if(edge < width)
	{
		position_x = edge;
		position_y = 0;
		direction = DOWN;
	}
	else if(edge < width + depth)
	{
		position_x = width - 1;
		position_y = edge - width;
		direction = LEFT;
	}
	else if(edge < width + depth + width)
	{
		position_x = width - (edge - width - depth) - 1;
		position_y = depth - 1;
		direction = UP;
	}
	else
	{
		position_x = 0;
		position_y = depth - (edge - (2 * width + depth)) - 1;
		direction = RIGHT;
	}

	// insert start condition
	if(queuePush(queue, position_x, position_y, direction))
		return 1;
	return 0;
}

AdventOfCode_return_t AoC_Day_16(AdventOfCode_config_t config)
{
	// implementation state (change to OK when the day is active)
	if(config == AOC_CONFIG_TEST)
		return AOC_RETURN_OK;

	{ // part 1
		// fetch file
		FIL file;
		uint8_t ret;
		if(config == AOC_CONFIG_DEBUG_VERBOSE)
			ret = f_open(&file, "day16t.txt", FA_READ);
		else ret = f_open(&file, "day16.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// get file length
		int32_t file_size = f_size(&file);

		// extract data
		char *map = (char*)sram_malloc(file_size + 1);
		uint8_t *heatmap = (uint8_t*)sram_calloc(file_size + 1, 1);

		// insert data
		f_read(&file, map, file_size + 1, NULL);

		// close file
		f_close(&file);

		// get map dimensions and insert line ends
		char *token = strtok(map, "\n");
		uint8_t width = strlen(token);
		uint8_t depth = 1;
		while(strtok(NULL, "\n"))
			depth++;

		// create queue
		Queue_t queue;
		queue.lasers = (Laser_t*)sram_calloc(128, sizeof(Laser_t));
		queue.max = 128;
		queue.ptr = 0;

		// insert start condition
		if(queuePush(&queue, 0, 0, RIGHT))
			return AOC_RETURN_NOK;

		// get direction
		laserUpdate(&queue.lasers[0], map[0]);
		heatmap[0] |= (1<<(uint8_t)queue.lasers[0].dir);

		// iterate lines
		while(queue.ptr)
		{
			// iterate over current lasers
			for(uint8_t laser_id=0; laser_id<queue.ptr; laser_id++)
			{
				// take step
				Laser_t *laser = &queue.lasers[laser_id];
				uint8_t ret = laserStep(laser, width, depth);

				// increment current tile
				uint16_t index = laser->y * (width + 1) + laser->x;

				// pop laser if finished
				if(ret)
				{
					queuePop(&queue, laser_id);
					laser_id--;
					continue;
				}

				// process next direction
				char condition = map[index];
				uint8_t split = laserUpdate(laser, condition);
				if(heatmap[index] & (1<<(uint8_t)laser->dir))
				{
					queuePop(&queue, laser_id);
					laser_id--;
					continue;
				}

				// set current position in the
				// heatmap to a visited block
				heatmap[index] |= (1<<(uint8_t)laser->dir);

				// add new laser if it has split
				if(split)
				{
					Dir_t direction = (Dir_t)(((uint8_t)laser->dir + 2) % 4);
					if(queuePush(&queue, laser->x, laser->y, direction))
						return AOC_RETURN_NOK;
				}
			}
		}

		// count visited
		uint16_t counter = 0;
		for(uint8_t x=0; x<width; x++)
			for(uint8_t y=0; y<depth; y++)
				if(heatmap[y * (width + 1) + x])
					counter++;

		// output answer
		char answer[64];
		sprintf(answer, "    Part 1 answer: %u\n", counter);
		AdventOfCode_print(config, answer, strlen(answer));

		// clean up
		sram_clrmem();
	}

	{ // part 2
		// fetch file
		FIL file;
		uint8_t ret;
		if(config == AOC_CONFIG_DEBUG_VERBOSE)
			ret = f_open(&file, "day16t.txt", FA_READ);
		else ret = f_open(&file, "day16.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// get file length
		int32_t file_size = f_size(&file);

		// extract data
		char *map = (char*)sram_malloc(file_size + 1);
		uint8_t *heatmap = (uint8_t*)sram_calloc(file_size + 1, 1);

		// insert data
		f_read(&file, map, file_size + 1, NULL);

		// close file
		f_close(&file);

		// get map dimensions and insert line ends
		char *token = strtok(map, "\n");
		uint8_t width = strlen(token);
		uint8_t depth = 1;
		while(strtok(NULL, "\n"))
			depth++;

		// create queue
		Queue_t queue;
		queue.lasers = (Laser_t*)sram_calloc(128, sizeof(Laser_t));
		queue.max = 128;
		queue.ptr = 0;

		// insert start condition
		if(queuePush(&queue, 0, 0, RIGHT))
			return AOC_RETURN_NOK;

		// get direction
		laserUpdate(&queue.lasers[0], map[0]);
		heatmap[0] |= (1<<(uint8_t)queue.lasers[0].dir);

		// calculate best case for each edge case
		uint16_t best_energized_state = 0;
		for(uint16_t edge=0; edge<width * 2 + depth * 2; edge++)
		{
			// push edge
			if(pushEdge(&queue, edge, width, depth))
				return AOC_RETURN_NOK;

			// get direction
			uint16_t index = queue.lasers[0].y * (width + 1) + queue.lasers[0].x;
			laserUpdate(&queue.lasers[0], map[index]);
			heatmap[0] |= (1<<(uint8_t)queue.lasers[0].dir);

			// iterate lines
			while(queue.ptr)
			{
				// iterate over current lasers
				for(uint8_t laser_id=0; laser_id<queue.ptr; laser_id++)
				{
					// take step
					Laser_t *laser = &queue.lasers[laser_id];
					uint8_t ret = laserStep(laser, width, depth);

					// increment current tile
					uint16_t index = laser->y * (width + 1) + laser->x;

					// pop laser if finished
					if(ret)
					{
						queuePop(&queue, laser_id);
						laser_id--;
						continue;
					}

					// process next direction
					char condition = map[index];
					uint8_t split = laserUpdate(laser, condition);
					if(heatmap[index] & (1<<(uint8_t)laser->dir))
					{
						queuePop(&queue, laser_id);
						laser_id--;
						continue;
					}

					// set current position in the
					// heatmap to a visited block
					heatmap[index] |= (1<<(uint8_t)laser->dir);

					// add new laser if it has split
					if(split)
					{
						Dir_t direction = (Dir_t)(((uint8_t)laser->dir + 2) % 4);
						if(queuePush(&queue, laser->x, laser->y, direction))
							return AOC_RETURN_NOK;
					}
				}
			}

			// count visited
			uint16_t counter = 0;
			for(uint8_t x=0; x<width; x++)
				for(uint8_t y=0; y<depth; y++)
					if(heatmap[y * (width + 1) + x])
						counter++;

			// check if it is better than previous
			if(best_energized_state < counter)
				best_energized_state = counter;

			// reset heatmap
			memset(heatmap, 0x00, file_size + 1);
		}

		// output answer
		char answer[64];
		sprintf(answer, "    Part 2 answer: %u\n", best_energized_state);
		AdventOfCode_print(config, answer, strlen(answer));

		// clean up
		sram_clrmem();
	}

	// status code
	return AOC_RETURN_OK;
}
