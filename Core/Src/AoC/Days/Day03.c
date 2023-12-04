/*
 * Day01.c
 *
 *  Created on: 9 aug. 2023
 *      Author: J.Verkade
 */

#include "AoC/Days/Day03.h"

int32_t getIndex(int32_t maze_width, int32_t column, int32_t row)
{
	return row * (maze_width + 1) + column;
}

AdventOfCode_return_t AoC_Day_03(AdventOfCode_config_t config)
{
	// implementation state (change to OK when the day is active)
	if(config == AOC_CONFIG_TEST)
		return AOC_RETURN_OK;

	{ // part 1
		// fetch file
		FIL file;
		uint8_t ret;
		if(config == AOC_CONFIG_DEBUG_VERBOSE)
			ret = f_open(&file, "day3t.txt", FA_READ);
		else ret = f_open(&file, "day3.txt", FA_READ);
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

		// tokenize data
		strtok(maze, "\n");
		int32_t maze_dept = 1;
		while(strtok(NULL, "\n"))
			maze_dept++;
		int32_t maze_width = file_size / maze_dept - 1;

		// process data
		int32_t sum_of_values = 0;
		int32_t maze_ptr = 0;
		while(maze_ptr < file_size)
		{
			// find next character
			for(; (maze[maze_ptr] < '0' || maze[maze_ptr] > '9') && maze_ptr < file_size; maze_ptr++);

			// check alignement with symbol
			int32_t start_ptr = maze_ptr;
			uint8_t is_aligned_flag = 0;
			while(maze[maze_ptr] >= '0' && maze[maze_ptr] <= '9' && maze_ptr < file_size)
			{
				// get row and column
				int32_t current_column = maze_ptr % (maze_width + 1);
				int32_t current_row = maze_ptr / (maze_width + 1);

				// check up
				if(current_row > 0)
				{
					int32_t index = getIndex(maze_width, current_column, current_row - 1);
					if(maze[index] != '.' && (maze[index] < '0' || maze[index] > '9'))
					{
						is_aligned_flag = 1;
						break;
					}
				}

				// right up
				if(current_row > 0 && current_column < maze_width-1)
				{
					int32_t index = getIndex(maze_width, current_column + 1, current_row - 1);
					if(maze[index] != '.' && maze[index] != '\n' && (maze[index] < '0' || maze[index] > '9'))
					{
						is_aligned_flag = 2;
						break;
					}
				}

				// right
				if(current_column < maze_width-1)
				{
					int32_t index = getIndex(maze_width, current_column + 1, current_row);
					if(maze[index] != '.' && maze[index] != '\n' && (maze[index] < '0' || maze[index] > '9'))
					{
						is_aligned_flag = 3;
						break;
					}
				}

				// right down
				if(current_row < maze_dept-1 && current_column < maze_width-1)
				{
					int32_t index = getIndex(maze_width, current_column + 1, current_row + 1);
					if(maze[index] != '.' && maze[index] != '\n' && (maze[index] < '0' || maze[index] > '9'))
					{
						is_aligned_flag = 4;
						break;
					}
				}

				// down
				if(current_row < maze_dept-1)
				{
					int32_t index = getIndex(maze_width, current_column, current_row + 1);
					if(maze[index] != '.' && (maze[index] < '0' || maze[index] > '9'))
					{
						is_aligned_flag = 5;
						break;
					}
				}

				// down left
				if(current_row < maze_dept-1 && current_column > 0)
				{
					int32_t index = getIndex(maze_width, current_column - 1, current_row + 1);
					if(maze[index] != '.' && (maze[index] < '0' || maze[index] > '9'))
					{
						is_aligned_flag = 6;
						break;
					}
				}

				// left
				if(current_column > 0)
				{
					int32_t index = getIndex(maze_width, current_column - 1, current_row);
					if(maze[index] != '.' && (maze[index] < '0' || maze[index] > '9'))
					{
						is_aligned_flag = 7;
						break;
					}
				}

				// left up
				if(current_row > 0 && current_column > 0)
				{
					int32_t index = getIndex(maze_width, current_column - 1, current_row - 1);
					if(maze[index] != '.' && (maze[index] < '0' || maze[index] > '9'))
					{
						is_aligned_flag = 8;
						break;
					}
				}
				maze_ptr++;
			}

			// move to end of character
			for(; maze[maze_ptr] >= '0' && maze[maze_ptr] <= '9' && maze_ptr < file_size; maze_ptr++);

			// if not aligned get numeric value
			if(is_aligned_flag)
			{
				char value[64];
				memset(value, 0x00, 64);
				memcpy(value, &maze[start_ptr], maze_ptr - start_ptr);
				sum_of_values += atol(value);
			}
		}

		// output answer
		char answer[32];
		sprintf(answer, "    Part 1 answer: %ld\n", sum_of_values);
		AdventOfCode_print(config, answer, strlen(answer));

		// clean up
		sram_clrmem();
	}

	{ // part 2
		// fetch file
		FIL file;
		uint8_t ret;
		if(config == AOC_CONFIG_DEBUG_VERBOSE)
			ret = f_open(&file, "day3t.txt", FA_READ);
		else ret = f_open(&file, "day3.txt", FA_READ);
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

		// tokenize data
		strtok(maze, "\n");
		int32_t maze_dept = 1;
		while(strtok(NULL, "\n"))
			maze_dept++;
		int32_t maze_width = file_size / maze_dept - 1;

		// process data
		int32_t sum_of_values = 0;
		int32_t maze_ptr = 0;
		while(maze_ptr < file_size)
		{
			// find next gear
			for(; maze[maze_ptr] != '*' && maze_ptr < file_size; maze_ptr++);

			// check alignement with symbol
			uint8_t is_aligned_counter = 0;
			int32_t gear_ratio = 1;

			// get row and column
			int32_t current_column = maze_ptr % (maze_width + 1);
			int32_t current_row = maze_ptr / (maze_width + 1);

			// check up
			if(current_row > 0)
			{
				int32_t index = getIndex(maze_width, current_column, current_row - 1);
				if(maze[index] >= '0' && maze[index] <= '9')
				{
					is_aligned_counter++;

					// get beginning and end of value
					int32_t start_ptr = index;
					for(; start_ptr >= 0 && maze[start_ptr] >= '0' && maze[start_ptr] <= '9'; start_ptr--);
					int32_t end_ptr = index;
					for(; maze[end_ptr] >= '0' && maze[end_ptr] <= '9'; end_ptr++);

					// get value
					char value_str[32];
					memset(value_str, 0x00, 32);
					memcpy(value_str, &maze[start_ptr + 1], end_ptr - start_ptr - 1);
					int32_t value = atoi(value_str);
					gear_ratio *= value;

					// clear value
					memset(&maze[start_ptr + 1], '.', end_ptr - start_ptr - 1);
				}
			}

			// right up
			if(current_row > 0 && current_column < maze_width-1)
			{
				int32_t index = getIndex(maze_width, current_column + 1, current_row - 1);
				if(maze[index] >= '0' && maze[index] <= '9')
				{
					is_aligned_counter++;

					// get beginning and end of value
					int32_t start_ptr = index;
					for(; start_ptr >= 0 && maze[start_ptr] >= '0' && maze[start_ptr] <= '9'; start_ptr--);
					int32_t end_ptr = index;
					for(; maze[end_ptr] >= '0' && maze[end_ptr] <= '9'; end_ptr++);

					// get value
					char value_str[32];
					memset(value_str, 0x00, 32);
					memcpy(value_str, &maze[start_ptr + 1], end_ptr - start_ptr - 1);
					int32_t value = atoi(value_str);
					gear_ratio *= value;

					// clear value
					memset(&maze[start_ptr + 1], '.', end_ptr - start_ptr - 1);
				}
			}

			// right
			if(current_column < maze_width-1)
			{
				int32_t index = getIndex(maze_width, current_column + 1, current_row);
				if(maze[index] >= '0' && maze[index] <= '9')
				{
					is_aligned_counter++;

					// get beginning and end of value
					int32_t start_ptr = index;
					for(; start_ptr >= 0 && maze[start_ptr] >= '0' && maze[start_ptr] <= '9'; start_ptr--);
					int32_t end_ptr = index;
					for(; maze[end_ptr] >= '0' && maze[end_ptr] <= '9'; end_ptr++);

					// get value
					char value_str[32];
					memset(value_str, 0x00, 32);
					memcpy(value_str, &maze[start_ptr + 1], end_ptr - start_ptr - 1);
					int32_t value = atoi(value_str);
					gear_ratio *= value;

					// clear value
					memset(&maze[start_ptr + 1], '.', end_ptr - start_ptr - 1);
				}
			}

			// right down
			if(current_row < maze_dept-1 && current_column < maze_width-1)
			{
				int32_t index = getIndex(maze_width, current_column + 1, current_row + 1);
				if(maze[index] >= '0' && maze[index] <= '9')
				{
					is_aligned_counter++;

					// get beginning and end of value
					int32_t start_ptr = index;
					for(; start_ptr >= 0 && maze[start_ptr] >= '0' && maze[start_ptr] <= '9'; start_ptr--);
					int32_t end_ptr = index;
					for(; maze[end_ptr] >= '0' && maze[end_ptr] <= '9'; end_ptr++);

					// get value
					char value_str[32];
					memset(value_str, 0x00, 32);
					memcpy(value_str, &maze[start_ptr + 1], end_ptr - start_ptr - 1);
					int32_t value = atoi(value_str);
					gear_ratio *= value;

					// clear value
					memset(&maze[start_ptr + 1], '.', end_ptr - start_ptr - 1);
				}
			}

			// down
			if(current_row < maze_dept-1)
			{
				int32_t index = getIndex(maze_width, current_column, current_row + 1);
				if(maze[index] >= '0' && maze[index] <= '9')
				{
					is_aligned_counter++;

					// get beginning and end of value
					int32_t start_ptr = index;
					for(; start_ptr >= 0 && maze[start_ptr] >= '0' && maze[start_ptr] <= '9'; start_ptr--);
					int32_t end_ptr = index;
					for(; maze[end_ptr] >= '0' && maze[end_ptr] <= '9'; end_ptr++);

					// get value
					char value_str[32];
					memset(value_str, 0x00, 32);
					memcpy(value_str, &maze[start_ptr + 1], end_ptr - start_ptr - 1);
					int32_t value = atoi(value_str);
					gear_ratio *= value;

					// clear value
					memset(&maze[start_ptr + 1], '.', end_ptr - start_ptr - 1);
				}
			}

			// down left
			if(current_row < maze_dept-1 && current_column > 0)
			{
				int32_t index = getIndex(maze_width, current_column - 1, current_row + 1);
				if(maze[index] >= '0' && maze[index] <= '9')
				{
					is_aligned_counter++;

					// get beginning and end of value
					int32_t start_ptr = index;
					for(; start_ptr >= 0 && maze[start_ptr] >= '0' && maze[start_ptr] <= '9'; start_ptr--);
					int32_t end_ptr = index;
					for(; maze[end_ptr] >= '0' && maze[end_ptr] <= '9'; end_ptr++);

					// get value
					char value_str[32];
					memset(value_str, 0x00, 32);
					memcpy(value_str, &maze[start_ptr + 1], end_ptr - start_ptr - 1);
					int32_t value = atoi(value_str);
					gear_ratio *= value;

					// clear value
					memset(&maze[start_ptr + 1], '.', end_ptr - start_ptr - 1);
				}
			}

			// left
			if(current_column > 0)
			{
				int32_t index = getIndex(maze_width, current_column - 1, current_row);
				if(maze[index] >= '0' && maze[index] <= '9')
				{
					is_aligned_counter++;

					// get beginning and end of value
					int32_t start_ptr = index;
					for(; start_ptr >= 0 && maze[start_ptr] >= '0' && maze[start_ptr] <= '9'; start_ptr--);
					int32_t end_ptr = index;
					for(; maze[end_ptr] >= '0' && maze[end_ptr] <= '9'; end_ptr++);

					// get value
					char value_str[32];
					memset(value_str, 0x00, 32);
					memcpy(value_str, &maze[start_ptr + 1], end_ptr - start_ptr - 1);
					int32_t value = atoi(value_str);
					gear_ratio *= value;

					// clear value
					memset(&maze[start_ptr + 1], '.', end_ptr - start_ptr - 1);
				}
			}

			// left up
			if(current_row > 0 && current_column > 0)
			{
				int32_t index = getIndex(maze_width, current_column - 1, current_row - 1);
				if(maze[index] >= '0' && maze[index] <= '9')
				{
					is_aligned_counter++;

					// get beginning and end of value
					int32_t start_ptr = index;
					for(; start_ptr >= 0 && maze[start_ptr] >= '0' && maze[start_ptr] <= '9'; start_ptr--);
					int32_t end_ptr = index;
					for(; maze[end_ptr] >= '0' && maze[end_ptr] <= '9'; end_ptr++);

					// get value
					char value_str[32];
					memset(value_str, 0x00, 32);
					memcpy(value_str, &maze[start_ptr + 1], end_ptr - start_ptr - 1);
					int32_t value = atoi(value_str);
					gear_ratio *= value;

					// clear value
					memset(&maze[start_ptr + 1], '.', end_ptr - start_ptr - 1);
				}
			}

			// move to end of character
			maze_ptr++;

			// if alignment count is 2
			if(is_aligned_counter == 2)
				sum_of_values += gear_ratio;
		}

		// output answer
		char answer[32];
		sprintf(answer, "    Part 2 answer: %ld\n", sum_of_values);
		AdventOfCode_print(config, answer, strlen(answer));

		// clean up
		sram_clrmem();
	}

	// status code
	return AOC_RETURN_OK;
}
