/*
 * Day01.c
 *
 *  Created on: 9 aug. 2023
 *      Author: J.Verkade
 */

#include "AoC/Days/Day10.h"

typedef struct Position_t Position_t;
struct Position_t
{
	uint32_t x;
	uint32_t y;
	uint32_t offset;
};

typedef enum Direction_t Direction_t;
enum Direction_t
{
	UP=0,
	DOWN=1,
	LEFT=2,
	RIGHT=3,
	DERROR=4
};

char *direction_string[] = {"UP", "DOWN", "LEFT", "RIGHT"};

char replacementChar(char c)
{
	switch(c)
	{
	case '|': return 'i';
	case '-': return '_';
	case 'L': return 'l';
	case 'J': return 'j';
	case '7': return '?';
	case 'F': return 'r';
	case 'S': return 'X';
	}
	return '!';
}

uint32_t maze_width;
uint32_t maze_depth;
char getMazePosition(char *maze, Position_t position)
{
	uint32_t index = position.y * (maze_width + 1) + position.x;
	return maze[index];
}

void setMazePosition(char *maze, Position_t position, char c)
{
	uint32_t index = position.y * (maze_width + 1) + position.x;
	maze[index] = c;
}

void printMaze(char *maze)
{
	for(uint32_t line=0; line<maze_depth; line++)
	{
		uint32_t index = line * (maze_width + 1);
		char tx_buffer[256];
		sprintf(tx_buffer, "%s\n", &maze[index]);
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, tx_buffer, strlen(tx_buffer));
		HAL_Delay(10);
	}
}

uint32_t big_maze_width;
uint32_t big_maze_depth;
void printBigMaze(char *maze)
{
	for(uint32_t line=0; line<big_maze_depth; line++)
	{
		uint32_t index = line * (big_maze_width + 1);
		char tx_buffer[512];
		sprintf(tx_buffer, "%s\n", &maze[index]);
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, tx_buffer, strlen(tx_buffer));
		HAL_Delay(10);
	}
}

char getBigMazePosition(char *maze, Position_t position)
{
	uint32_t index = position.y * (big_maze_width + 1) + position.x;
	return maze[index];
}

void setBigMazePosition(char *maze, Position_t position, char c)
{
	uint32_t index = position.y * (big_maze_width + 1) + position.x;
	maze[index] = c;
}

Direction_t getValidDirection(char *maze, Position_t position)
{
	if(position.y > 0)
	{
		Position_t new_pos = {position.x, position.y-1, 1};
		char chr = getMazePosition(maze, new_pos);
		if(chr == '|' || chr == '7' || chr == 'F')
			return UP;
	}
	if(position.y+1 < maze_depth)
	{
		Position_t new_pos = {position.x, position.y+1, 1};
		char chr = getMazePosition(maze, new_pos);
		if(chr == '|' || chr == 'L' || chr == 'J')
			return DOWN;
	}
	if(position.x > 0)
	{
		Position_t new_pos = {position.x-1, position.y, 1};
		char chr = getMazePosition(maze, new_pos);
		if(chr == '|' || chr == 'L' || chr == 'J')
			return LEFT;
	}
	if(position.x + 1 < maze_width)
	{
		Position_t new_pos = {position.x+1, position.y, 1};
		char chr = getMazePosition(maze, new_pos);
		if(chr == '|' || chr == 'L' || chr == 'J')
			return RIGHT;
	}
	return DERROR;
}

Direction_t nextDirection(char c, Direction_t direction)
{
	switch(direction)
	{
	case UP:
		switch(c)
		{
		case '|': return UP;
		case 'F': return RIGHT;
		case '7': return LEFT;
		}
		break;
	case DOWN:
		switch(c)
		{
		case '|': return DOWN;
		case 'L': return RIGHT;
		case 'J': return LEFT;
		}
		break;
	case LEFT:
		switch(c)
		{
		case '-': return LEFT;
		case 'L': return UP;
		case 'F': return DOWN;
		}
		break;
	case RIGHT:
		switch(c)
		{
		case '-': return RIGHT;
		case 'J': return UP;
		case '7': return DOWN;
		}
		break;
	default:
		break;
	}
	return DERROR;
}

AdventOfCode_return_t AoC_Day_10(AdventOfCode_config_t config)
{
	// implementation state (change to OK when the day is active)
	if(config == AOC_CONFIG_TEST)
		return AOC_RETURN_OK;

	{ // part 1
		// fetch file
		FIL file;
		uint8_t ret;
		if(config == AOC_CONFIG_DEBUG_VERBOSE)
			ret = f_open(&file, "day10t.txt", FA_READ);
		else ret = f_open(&file, "day10.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// get file length
		int32_t file_size = f_size(&file);

		// extract data
		char *maze = (char*)sram_malloc(file_size + 1);

		// insert data
		f_read(&file, maze, file_size + 1, NULL);

		// close file
		f_close(&file);

		// get maze dimensions and insert line ends
		char *token = strtok(maze, "\n");
		maze_width = strlen(token);
		uint32_t counter = 1;
		while(strtok(NULL, "\n"))
			counter++;
		maze_depth = counter;

		// locate start position
		Position_t start = {0, 0, 0};
		for(start.y=0; start.y<maze_depth; start.y++)
		{
			uint8_t located = 0;
			for(start.x=0; start.x<maze_width; start.x++)
			{
				if(getMazePosition(maze, start) == 'S')
				{
					located = 1;
					break;
				}
			}
			if(located)
				break;
		}

		// process track
		Position_t position = start;
		Direction_t direction = getValidDirection(maze, position);

		// do first movement
		uint32_t pipe_length = 0;
		char pipe_type;
		switch(direction)
		{
		case UP:
			position.y--;
			pipe_type = getMazePosition(maze, position);
			direction = nextDirection(pipe_type, direction);
			break;
		case DOWN:
			position.y++;
			pipe_type = getMazePosition(maze, position);
			direction = nextDirection(pipe_type, direction);
			break;
		case LEFT:
			position.x--;
			pipe_type = getMazePosition(maze, position);
			direction = nextDirection(pipe_type, direction);
			break;
		case RIGHT:
			position.x++;
			pipe_type = getMazePosition(maze, position);
			direction = nextDirection(pipe_type, direction);
			break;
		default:
			break;
		}

		// insert next pipe
		pipe_length++;
		char track_token = '\0';
		while(track_token != 'S')
		{
			// process movement
			char pipe_type;
			switch(direction)
			{
			case UP:
				position.y--;
				pipe_type = getMazePosition(maze, position);
				direction = nextDirection(pipe_type, direction);
				break;
			case DOWN:
				position.y++;
				pipe_type = getMazePosition(maze, position);
				direction = nextDirection(pipe_type, direction);
				break;
			case LEFT:
				position.x--;
				pipe_type = getMazePosition(maze, position);
				direction = nextDirection(pipe_type, direction);
				break;
			case RIGHT:
				position.x++;
				pipe_type = getMazePosition(maze, position);
				direction = nextDirection(pipe_type, direction);
				break;
			default:
				break;
			}

			// insert next pipe
			pipe_length++;

			// update pype type token
			track_token = pipe_type;
		}

		// answer
		uint32_t answer_value =  pipe_length / 2;

		// output answer
		char answer[64];
		sprintf(answer, "    Part 1 answer: %lu\n", answer_value);
		AdventOfCode_print(config, answer, strlen(answer));

		// clean up
		sram_clrmem();
	}

	{ // part 2
		// fetch file
		FIL file;
		uint8_t ret;
		if(config == AOC_CONFIG_DEBUG_VERBOSE)
			ret = f_open(&file, "day10t.txt", FA_READ);
		else ret = f_open(&file, "day10.txt", FA_READ);
		if(ret != FR_OK)
		{
			AdventOfCode_print(config, "    Cannot find file!\n", strlen("\tCannot find file!\n"));
			return AOC_RETURN_NOK;
		}

		// get file length
		int32_t file_size = f_size(&file);

		// extract data
		char *maze = (char*)sram_malloc(file_size + 1);

		// insert data
		f_read(&file, maze, file_size + 1, NULL);

		// close file
		f_close(&file);

		// get maze dimensions and insert line ends
		char *token = strtok(maze, "\n");
		maze_width = strlen(token);
		uint32_t counter = 1;
		while(strtok(NULL, "\n"))
			counter++;
		maze_depth = counter;

		// locate start position
		Position_t start = {0, 0, 0};
		for(start.y=0; start.y<maze_depth; start.y++)
		{
			uint8_t located = 0;
			for(start.x=0; start.x<maze_width; start.x++)
			{
				if(getMazePosition(maze, start) == 'S')
				{
					located = 1;
					break;
				}
			}
			if(located)
				break;
		}

		// process track
		Position_t position = start;
		Direction_t direction = getValidDirection(maze, position);

		// do first movement
		uint32_t pipe_length = 0;
		char pipe_type;
		switch(direction)
		{
		case UP:
			position.y--;
			pipe_type = getMazePosition(maze, position);
			direction = nextDirection(pipe_type, direction);
			break;
		case DOWN:
			position.y++;
			pipe_type = getMazePosition(maze, position);
			direction = nextDirection(pipe_type, direction);
			break;
		case LEFT:
			position.x--;
			pipe_type = getMazePosition(maze, position);
			direction = nextDirection(pipe_type, direction);
			break;
		case RIGHT:
			position.x++;
			pipe_type = getMazePosition(maze, position);
			direction = nextDirection(pipe_type, direction);
			break;
		default:
			break;
		}

		// clear pipe position with a #
		setMazePosition(maze, position, replacementChar(pipe_type));
		pipe_length++;
		char track_token = '\0';
		while(track_token != 'S')
		{
			// process movement
			char pipe_type;
			switch(direction)
			{
			case UP:
				position.y--;
				pipe_type = getMazePosition(maze, position);
				direction = nextDirection(pipe_type, direction);
				break;
			case DOWN:
				position.y++;
				pipe_type = getMazePosition(maze, position);
				direction = nextDirection(pipe_type, direction);
				break;
			case LEFT:
				position.x--;
				pipe_type = getMazePosition(maze, position);
				direction = nextDirection(pipe_type, direction);
				break;
			case RIGHT:
				position.x++;
				pipe_type = getMazePosition(maze, position);
				direction = nextDirection(pipe_type, direction);
				break;
			default:
				break;
			}

			// insert next pipe
			pipe_length++;

			// fill maze with pathing
			setMazePosition(maze, position, replacementChar(pipe_type));

			// update pype type token
			track_token = pipe_type;
		}

		// create oversized maze
		big_maze_width = maze_width * 3;
		big_maze_depth = maze_depth * 3;
		char *big_maze = (char*)sram_calloc((big_maze_width + 1) * big_maze_depth, 1);

		// fill big maze with dots
		for(uint32_t line=0; line<big_maze_depth; line++)
		{
			uint32_t index = line * (big_maze_width + 1);
			memset(&big_maze[index], '.', big_maze_width);
		}

		// fill big maze with characters
		for(uint32_t position_x=0; position_x<maze_width; position_x++)
		{
			for(uint32_t position_y=0; position_y<maze_depth; position_y++)
			{
				// abort if character is not part of real pipe
				char chr = getMazePosition(maze, (Position_t){position_x, position_y, 0});
				if(chr != 'X' && chr != 'i' && chr != 'r' && chr != '?' && chr != 'l' && chr != 'j' && chr != '_')
					continue;

				// get center index of maze part
				uint32_t big_pos_x = 1 + position_x * 3;
				uint32_t big_pos_y = 1 + position_y * 3;
				uint32_t index = big_pos_y * (big_maze_width + 1) + big_pos_x;
				if(chr == 'X')
				{
					// draw figure
					big_maze[index] = '#';
					big_maze[index+1] = '#';
					big_maze[index-1] = '#';
					big_maze[index+(big_maze_width + 1)] = '#';
					big_maze[index-(big_maze_width + 1)] = '#';
				}
				else if(chr == 'i')
				{
					// draw figure
					big_maze[index] = '#';
					big_maze[index+(big_maze_width + 1)] = '#';
					big_maze[index-(big_maze_width + 1)] = '#';
				}
				else if(chr == 'r')
				{
					// draw figure
					big_maze[index] = '#';
					big_maze[index+1] = '#';
					big_maze[index+(big_maze_width + 1)] = '#';
				}
				else if(chr == '?')
				{
					// draw figure
					big_maze[index] = '#';
					big_maze[index-1] = '#';
					big_maze[index+(big_maze_width + 1)] = '#';
				}
				else if(chr == 'l')
				{
					// draw figure
					big_maze[index] = '#';
					big_maze[index+1] = '#';
					big_maze[index-(big_maze_width + 1)] = '#';
				}
				else if(chr == 'j')
				{
					// draw figure
					big_maze[index] = '#';
					big_maze[index-1] = '#';
					big_maze[index-(big_maze_width + 1)] = '#';
				}
				else if(chr == '_')
				{
					// draw figure
					big_maze[index] = '#';
					big_maze[index+1] = '#';
					big_maze[index-1] = '#';
				}
			}
		}

		// flood fill
		Position_t queue[4000];
		uint32_t write_ptr = 0;
		uint32_t read_ptr = 0;

		// add ptr
		setBigMazePosition(big_maze, (Position_t){0, 0, 0}, '~');
		queue[write_ptr] = (Position_t){0, 0, 0};
		write_ptr++;

		while(write_ptr != read_ptr)
		{
			Position_t position = queue[read_ptr];
			read_ptr = (read_ptr + 1) % 4000;

			// check positions
			if(position.y > 0)
			{
				Position_t new_pos = {position.x, position.y-1, 1};
				char chr = getBigMazePosition(big_maze, new_pos);
				if(chr != '#' && chr !='~')
				{
					// push position to queue
					setBigMazePosition(big_maze, new_pos, '~');
					queue[write_ptr] = new_pos;
					write_ptr = (write_ptr + 1) % 4000;
				}
			}
			if(position.y+1 < big_maze_depth)
			{
				Position_t new_pos = {position.x, position.y+1, 1};
				char chr = getBigMazePosition(big_maze, new_pos);
				if(chr != '#' && chr !='~')
				{
					// push position to queue
					setBigMazePosition(big_maze, new_pos, '~');
					queue[write_ptr] = new_pos;
					write_ptr = (write_ptr + 1) % 4000;
				}
			}
			if(position.x > 0)
			{
				Position_t new_pos = {position.x-1, position.y, 1};
				char chr = getBigMazePosition(big_maze, new_pos);
				if(chr != '#' && chr !='~')
				{
					// push position to queue
					setBigMazePosition(big_maze, new_pos, '~');
					queue[write_ptr] = new_pos;
					write_ptr = (write_ptr + 1) % 4000;
				}
			}
			if(position.x + 1 < big_maze_width)
			{
				Position_t new_pos = {position.x+1, position.y, 1};
				char chr = getBigMazePosition(big_maze, new_pos);
				if(chr != '#' && chr !='~')
				{
					// push position to queue
					setBigMazePosition(big_maze, new_pos, '~');
					queue[write_ptr] = new_pos;
					write_ptr = (write_ptr + 1) % 4000;
				}
			}
		}

		// count all fills
		uint32_t empty_counter = 0;
		for(uint32_t position_x=0; position_x<maze_width; position_x++)
			for(uint32_t position_y=0; position_y<maze_depth; position_y++)
				if(getBigMazePosition(big_maze, (Position_t){position_x * 3 + 1, position_y * 3 + 1, 0}) == '.')
					empty_counter++;

		// output answer
		char answer[64];
		sprintf(answer, "    Part 2 answer: %lu\n", empty_counter);
		AdventOfCode_print(config, answer, strlen(answer));

		// clean up
		sram_clrmem();
	}

	// status code
	return AOC_RETURN_OK;
}
