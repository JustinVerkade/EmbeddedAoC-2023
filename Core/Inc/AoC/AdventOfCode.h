/*
 * AdventOfCode.h
 *
 *  Created on: 9 aug. 2023
 *      Author: J.Verkade
 */

#ifndef SRC_AOC_ADVENTOFCODE_H_
#define SRC_AOC_ADVENTOFCODE_H_

#include "fatfs.h"
#include "stdint.h"
#include "string.h"
#include "main.h"
#include "stdio.h"

typedef enum AdventOfCode_config_t AdventOfCode_config_t;
enum AdventOfCode_config_t
{
	AOC_CONFIG_TEST,
	AOC_CONFIG_DEBUG,
	AOC_CONFIG_DEBUG_VERBOSE,
	AOC_CONFIG_RELEASE,
	AOC_CONFIG_RELEASE_VERBOSE,
};

typedef enum AdventOfCode_return_t AdventOfCode_return_t;
enum AdventOfCode_return_t
{
	AOC_RETURN_OK,
	AOC_RETURN_NOK,
};

typedef struct AdventOfCode_t AdventOfCode_t;
struct AdventOfCode_t
{
	AdventOfCode_return_t(*aoc_func_list[25])(AdventOfCode_config_t);
};


void AdventOfCode_init(AdventOfCode_t *aoc,
		AdventOfCode_return_t(*aoc_day_01)(AdventOfCode_config_t),
		AdventOfCode_return_t(*aoc_day_02)(AdventOfCode_config_t),
		AdventOfCode_return_t(*aoc_day_03)(AdventOfCode_config_t),
		AdventOfCode_return_t(*aoc_day_04)(AdventOfCode_config_t),
		AdventOfCode_return_t(*aoc_day_05)(AdventOfCode_config_t),
		AdventOfCode_return_t(*aoc_day_06)(AdventOfCode_config_t),
		AdventOfCode_return_t(*aoc_day_07)(AdventOfCode_config_t),
		AdventOfCode_return_t(*aoc_day_08)(AdventOfCode_config_t),
		AdventOfCode_return_t(*aoc_day_09)(AdventOfCode_config_t),
		AdventOfCode_return_t(*aoc_day_10)(AdventOfCode_config_t),
		AdventOfCode_return_t(*aoc_day_11)(AdventOfCode_config_t),
		AdventOfCode_return_t(*aoc_day_12)(AdventOfCode_config_t),
		AdventOfCode_return_t(*aoc_day_13)(AdventOfCode_config_t),
		AdventOfCode_return_t(*aoc_day_14)(AdventOfCode_config_t),
		AdventOfCode_return_t(*aoc_day_15)(AdventOfCode_config_t),
		AdventOfCode_return_t(*aoc_day_16)(AdventOfCode_config_t),
		AdventOfCode_return_t(*aoc_day_17)(AdventOfCode_config_t),
		AdventOfCode_return_t(*aoc_day_18)(AdventOfCode_config_t),
		AdventOfCode_return_t(*aoc_day_19)(AdventOfCode_config_t),
		AdventOfCode_return_t(*aoc_day_20)(AdventOfCode_config_t),
		AdventOfCode_return_t(*aoc_day_21)(AdventOfCode_config_t),
		AdventOfCode_return_t(*aoc_day_22)(AdventOfCode_config_t),
		AdventOfCode_return_t(*aoc_day_23)(AdventOfCode_config_t),
		AdventOfCode_return_t(*aoc_day_24)(AdventOfCode_config_t),
		AdventOfCode_return_t(*aoc_day_25)(AdventOfCode_config_t));

void AdventOfCode_checkImplementedDays(AdventOfCode_t *aoc);
void AdventOfCode_executeImplementations(AdventOfCode_t *aoc);
void AdventOfCode_print(AdventOfCode_config_t config, char *text, int len);

#endif /* SRC_AOC_ADVENTOFCODE_H_ */
