/*
 * AdventOfCode.c
 *
 *  Created on: 9 aug. 2023
 *      Author: J.Verkade
 */

#include "Aoc/AdventOfCode.h"
#include "usbd_cdc_if.h"

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
		AdventOfCode_return_t(*aoc_day_25)(AdventOfCode_config_t))
{
	aoc->aoc_func_list[0] = aoc_day_01;
	aoc->aoc_func_list[1] = aoc_day_02;
	aoc->aoc_func_list[2] = aoc_day_03;
	aoc->aoc_func_list[3] = aoc_day_04;
	aoc->aoc_func_list[4] = aoc_day_05;
	aoc->aoc_func_list[5] = aoc_day_06;
	aoc->aoc_func_list[6] = aoc_day_07;
	aoc->aoc_func_list[7] = aoc_day_08;
	aoc->aoc_func_list[8] = aoc_day_09;
	aoc->aoc_func_list[9] = aoc_day_10;
	aoc->aoc_func_list[10] = aoc_day_11;
	aoc->aoc_func_list[11] = aoc_day_12;
	aoc->aoc_func_list[12] = aoc_day_13;
	aoc->aoc_func_list[13] = aoc_day_14;
	aoc->aoc_func_list[14] = aoc_day_15;
	aoc->aoc_func_list[15] = aoc_day_16;
	aoc->aoc_func_list[16] = aoc_day_17;
	aoc->aoc_func_list[17] = aoc_day_18;
	aoc->aoc_func_list[18] = aoc_day_19;
	aoc->aoc_func_list[19] = aoc_day_20;
	aoc->aoc_func_list[20] = aoc_day_21;
	aoc->aoc_func_list[21] = aoc_day_22;
	aoc->aoc_func_list[22] = aoc_day_23;
	aoc->aoc_func_list[23] = aoc_day_24;
	aoc->aoc_func_list[24] = aoc_day_25;
}

extern USBD_HandleTypeDef hUsbDeviceHS;
void AdventOfCode_checkImplementedDays(AdventOfCode_t *aoc)
{
	char *text = "\nChecking AoC days:\n";
	AdventOfCode_print(text, strlen(text));

	for(int day=0; day<25; day++)
	{
		AdventOfCode_return_t ret = aoc->aoc_func_list[day](AOC_CONFIG_TEST);
		char buffer[64];
		if(ret == AOC_RETURN_OK)
			sprintf(buffer, "-> Day %2d: %s\n", day + 1, "FINISHED");
		else
			sprintf(buffer, "-> Day %2d: %s\n", day + 1, "NOT FINISHED");
		AdventOfCode_print(buffer, strlen(buffer));
	}

	char *line = "________________________________________________________________________________\n";
	AdventOfCode_print(line, strlen(line));
}

void AdventOfCode_executeImplementations(AdventOfCode_t *aoc)
{
	char *text = "\nExecuting AoC days:\n";
	AdventOfCode_print(text, strlen(text));

	int days_completed = 0;
	char print_buffer[64];
	for(int day=0; day<25; day++)
	{

		AdventOfCode_return_t ret = aoc->aoc_func_list[day](AOC_CONFIG_TEST);

		if(ret == AOC_RETURN_NOK)
		{
			sprintf(print_buffer, "-> Day %2d: %s\n", day + 1, "NOT IMPLEMENTED");
			AdventOfCode_print(print_buffer, strlen(print_buffer));
			continue;
		}

		// header
		sprintf(print_buffer, "-> Day %2d: ", day + 1);
		AdventOfCode_print(print_buffer, strlen(print_buffer));

		// exceute debug mode
		uint32_t debug_start_time = HAL_GetTick();
		aoc->aoc_func_list[day](AOC_CONFIG_DEBUG);
		uint32_t debug_end_time = HAL_GetTick();
		sprintf(print_buffer, "Debug: [ %7lu ms]", debug_end_time - debug_start_time);
		AdventOfCode_print(print_buffer, strlen(print_buffer));

		// execute release mode
		uint32_t release_start_time = HAL_GetTick();
		aoc->aoc_func_list[day](AOC_CONFIG_RELEASE);
		uint32_t release_end_time = HAL_GetTick();
		sprintf(print_buffer, " Release: [ %7lu ms]\n", release_end_time - release_start_time);
		AdventOfCode_print(print_buffer, strlen(print_buffer));

		// increment days
		days_completed++;
	}

	char *line = "________________________________________________________________________________\n";
	AdventOfCode_print(line, strlen(line));

	if(days_completed == 25)
	{
		AdventOfCode_print("\n                              Mery Christmas\n", strlen("\n                              Mery Christmas\n"));
		char *line = "________________________________________________________________________________\n";
		AdventOfCode_print(line, strlen(line));
	}
}

void AdventOfCode_print(char *text, int len)
{
	while(((USBD_CDC_HandleTypeDef*)hUsbDeviceHS.pClassData)->TxState != 0);
	CDC_Transmit_HS((uint8_t*)text, len);
}



