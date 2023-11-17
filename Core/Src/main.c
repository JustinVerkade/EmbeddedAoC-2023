/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "usb_device.h"
#include "usb_host.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
#include "string.h"
#include "stm32f7xx_ll_fmc.h"
#include "math.h"

#include "Ffunctions/f_debug.h"
#include "System/System.h"

#include "AoC/AdventOfCode.h"
#include "AoC/AdventOfCode_Includes.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define USB_COMMAND_CHECK		('c') // check
#define USB_COMMAND_TEST		('t') // test
#define USB_COMMAND_RELEASE		('r') // release
#define USB_COMMAND_UPLOAD		('u') // upload
#define USB_COMMAND_LIST		('l') // list
#define USB_COMMAND_DELETE		('d') // delete
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

SD_HandleTypeDef hsd1;
DMA_HandleTypeDef hdma_sdmmc1_rx;
DMA_HandleTypeDef hdma_sdmmc1_tx;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim11;

UART_HandleTypeDef huart1;

SRAM_HandleTypeDef hsram1;

/* USER CODE BEGIN PV */

// system state management
System_t system_state;

uint8_t usb_rx_buffer[1024];
uint16_t usb_rx_length;
uint8_t usb_rx_ready;
uint8_t usb_connected;

AdventOfCode_t aoc;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SDMMC1_SD_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM11_Init(void);
static void MX_ADC1_Init(void);
static void MX_FMC_Init(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void checkImplementation()
{
	// fetch variable
	uint8_t *ptr;
	for(ptr=usb_rx_buffer; *ptr!=' '&&ptr<(usb_rx_buffer+1024); ptr++);
	ptr++;

	// no value given
	if(*ptr == '\0')
	{
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "No day number given!\n", strlen("No day number given!\n"));
		return;
	}

	// check value
	int32_t day = atoi((char*)ptr);
	if(day > 25 || day <= 0)
	{
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "Invalid day number given!\n", strlen("Invalid day number given!\n"));
		return;
	}

	// check implementation
	char buffer[1024];
	sprintf(buffer, "Checking day %ld\n", day);
	AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, buffer, strlen(buffer));

	// check day
	AdventOfCode_return_t ret = aoc.aoc_func_list[day - 1](AOC_CONFIG_TEST);
	if(ret == AOC_RETURN_OK)
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "\tState: Implemented\n", strlen("\tState: Implemented\n"));
	else AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "\tState: Not implemented\n", strlen("\tState: Not implemented\n"));

	// check debug file
	FILINFO fno;
	sprintf(buffer, "day%ldt.txt", day);
	if(f_stat(buffer, &fno) == FR_OK)
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "\tDebug: Present\n", strlen("\tDebug: Present\n"));
	else AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "\tDebug: Absent\n", strlen("\tDebug: Absent\n"));

	// check release file
	sprintf(buffer, "day%ld.txt", day);
	if(f_stat(buffer, &fno) == FR_OK)
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "\tRelease: Present\n", strlen("\tRelease: Present\n"));
	else AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "\tRelease: Absent\n", strlen("\tRelease: Absent\n"));
}

void runDebugImplementation()
{
	// fetch variable
	uint8_t *ptr;
	for(ptr=usb_rx_buffer; *ptr!=' '&&ptr<(usb_rx_buffer+1024); ptr++);
	ptr++;

	// no value given
	if(*ptr == '\0')
	{
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "No day number given!\n", strlen("No day number given!\n"));
		return;
	}

	// check value
	int32_t day = atoi((char*)ptr);
	if(day > 25 || day <= 0)
	{
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "Invalid day number given!\n", strlen("Invalid day number given!\n"));
		return;
	}

	// check implementation
	char buffer[1024];
	sprintf(buffer, "Executing debug day %ld\n", day);
	AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, buffer, strlen(buffer));

	// check implementationday
	AdventOfCode_return_t ret = aoc.aoc_func_list[day - 1](AOC_CONFIG_TEST);
	if(ret != AOC_RETURN_OK)
	{
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "\tState: Not implemented\n", strlen("\tState: Not implemented\n"));
		return;
	}

	// check debug file
	FILINFO fno;
	sprintf(buffer, "day%ldt.txt", day);
	if(f_stat(buffer, &fno) != FR_OK)
	{
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "\tDebug: Absent\n", strlen("\tDebug: Absent\n"));
		return;
	}

	// split line
	AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "Start:\n", strlen("Start:\n"));

	// execute debug mode day
	uint32_t start = HAL_GetTick();
	AdventOfCode_return_t dret = aoc.aoc_func_list[day - 1](AOC_CONFIG_DEBUG_VERBOSE);
	uint32_t end = HAL_GetTick();
	if(dret == AOC_RETURN_OK)
	{
		sprintf(buffer, "Succesfull: %ld ms\n", end - start);
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, buffer, strlen(buffer));
	}
	else
	{
		sprintf(buffer, "Failed: %ld ms\n", end - start);
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, buffer, strlen(buffer));
	}
}

void runReleaseImplementation()
{
	// fetch variable
	uint8_t *ptr;
	for(ptr=usb_rx_buffer; *ptr!=' '&&ptr<(usb_rx_buffer+1024); ptr++);
	ptr++;

	// no value given
	if(*ptr == '\0')
	{
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "No day number given!\n", strlen("No day number given!\n"));
		return;
	}

	// check value
	int32_t day = atoi((char*)ptr);
	if(day > 25 || day <= 0)
	{
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "Invalid day number given!\n", strlen("Invalid day number given!\n"));
		return;
	}

	// check implementation
	char buffer[1024];
	sprintf(buffer, "Executing release day %ld\n", day);
	AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, buffer, strlen(buffer));

	// check implementationday
	AdventOfCode_return_t ret = aoc.aoc_func_list[day - 1](AOC_CONFIG_TEST);
	if(ret != AOC_RETURN_OK)
	{
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "\tState: Not implemented\n", strlen("\tState: Not implemented\n"));
		return;
	}

	// check debug file
	FILINFO fno;
	sprintf(buffer, "day%ld.txt", day);
	if(f_stat(buffer, &fno) != FR_OK)
	{
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "\tRelease: Absent\n", strlen("\tRelease: Absent\n"));
		return;
	}


	// split line
	AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "Start:\n", strlen("Start:\n"));

	// execute debug mode day
	uint32_t start = HAL_GetTick();
	AdventOfCode_return_t dret = aoc.aoc_func_list[day - 1](AOC_CONFIG_RELEASE_VERBOSE);
	uint32_t end = HAL_GetTick();
	if(dret == AOC_RETURN_OK)
	{
		sprintf(buffer, "Succesfull: %ld ms\n", end - start);
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, buffer, strlen(buffer));
	}
	else
	{
		sprintf(buffer, "Failed: %ld ms\n", end - start);
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, buffer, strlen(buffer));
	}
}

void fetchPuzzleInput()
{
	// fetch variable
	uint8_t *ptr;
	for(ptr=usb_rx_buffer; *ptr!=' '&&ptr<(usb_rx_buffer+1024); ptr++);
	ptr++;
	uint8_t *start = ptr;
	for(ptr=usb_rx_buffer; *ptr!='\n'&&ptr<(usb_rx_buffer+1024); ptr++);
	*ptr = 0x00;

	// no value given
	if(*start == '\0')
	{
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "No file name given!\n", strlen("No file name given!\n"));
		return;
	}

	// check implementation
	char buffer[1028];
	sprintf(buffer, "Creating %s\n", start);
	AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, buffer, strlen(buffer));

	// create file
	FIL file;
	uint8_t ret = f_open(&file, (char*)start, FA_CREATE_ALWAYS | FA_WRITE);
	if(ret != FR_OK)
	{
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "Failed to create file!\n", strlen("Failed to create file!\n"));
		return;
	}

	// fetch data
	AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "Uploade data:\n{\n", strlen("Uploade data:\n{\n"));

	usb_rx_ready = 0;
	uint8_t done = 0;
	uint32_t counter = 0;
	while(!done)
	{
		// wait for new data
		if(!usb_rx_ready)
			continue;

		// flicker lightn light
		if((counter++ % 2) == 0)
			HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);

		// check last character
		char last_character = usb_rx_buffer[usb_rx_length - 3];
		if(last_character == '`')
		{
			usb_rx_buffer[usb_rx_length - 3] = 0x00;
			done = 1;
		}

		// echo message
		sprintf(buffer, "\t%s", (char*)usb_rx_buffer);
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, buffer, strlen(buffer));

		// write to file
		f_write(&file, usb_rx_buffer, usb_rx_length, NULL);

		// clear flag
		usb_rx_ready = 0;
	}

	// close file
	f_close(&file);
	AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "}\nSaving input\n", strlen("}\nSaving input\n"));

	// turn on light
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
}

void showDirectory()
{
	// opening log
	AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "Uploaded files:\n{\n", strlen("Uploaded files:\n{\n"));
	char buffer[64];

	DIR dir;
	FILINFO fno;
	uint8_t counter = 0;
	f_opendir(&dir, "/");
	do
	{
	    f_readdir(&dir, &fno);
	    if(fno.fname[0] != 0 && counter++ > 1)
	    {
	    	sprintf(buffer, "\t%s\n", fno.fname);
	    	AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, buffer, strlen(buffer));
	    }
	} while(fno.fname[0] != 0);
	f_closedir(&dir);

	// closing log
	AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "}\n", strlen("}\n"));
}

void deleteFile()
{
	// fetch variable
	uint8_t *ptr;
	for(ptr=usb_rx_buffer; *ptr!=' '&&ptr<(usb_rx_buffer+1024); ptr++);
	ptr++;
	uint8_t *start = ptr;
	for(ptr=usb_rx_buffer; *ptr!='\n'&&ptr<(usb_rx_buffer+1024); ptr++);
	*ptr = 0x00;

	// no value given
	if(*start == '\0')
	{
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "No file name given!\n", strlen("No file name given!\n"));
		return;
	}

	// check implementation
	char buffer[1028];
	sprintf(buffer, "Deleting %s\n", start);
	AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, buffer, strlen(buffer));

	// delete file
	uint8_t ret = f_unlink((char*)start);
	if(ret != FR_OK)
	{
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "Failed to delete file!\n", strlen("Failed to delete file!\n"));
		return;
	}

	// deleted file
	AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "Deleted file\n", strlen("Deleted file\n"));
}

void USBD_HandleCommands()
{
	// check if message is valid
	if(!usb_rx_ready)
		return;

	// turn on light
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);

	// echo message
	//	AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, (char*)usb_rx_buffer, strlen((char*)usb_rx_buffer));

	// process command
	switch(usb_rx_buffer[0])
	{
	case USB_COMMAND_CHECK:
		checkImplementation();
		break;

	case USB_COMMAND_TEST:
		runDebugImplementation();
		break;

	case USB_COMMAND_RELEASE:
		runReleaseImplementation();
		break;

	case USB_COMMAND_UPLOAD:
		fetchPuzzleInput();
		break;

	case USB_COMMAND_LIST:
		showDirectory();
		break;

	case USB_COMMAND_DELETE:
		deleteFile();
		break;

	default:
		AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, "Unkown command!\n", strlen("Unkown command!\n"));
		break;
	}

	// print new cursor
	AdventOfCode_print(AOC_CONFIG_DEBUG_VERBOSE, ">", 1);

	// clear receive flag
	usb_rx_ready = 0;

	// turn off led
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SDMMC1_SD_Init();
  MX_USB_DEVICE_Init();
  MX_USB_HOST_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_TIM11_Init();
  MX_ADC1_Init();
  MX_FMC_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */

	// wait for usb connection
	while(!usb_connected)
	HAL_Delay(500);

	// system startup
	System_init(&system_state);

	// system init terminal log
	System_initLog(&system_state);
	System_selfTestLog(&system_state);

	// init AOC
	AdventOfCode_init(&aoc,
			AoC_Day_01,	AoC_Day_02,	AoC_Day_03,	AoC_Day_04,	AoC_Day_05,
			AoC_Day_06,	AoC_Day_07,	AoC_Day_08,	AoC_Day_09,	AoC_Day_10,
			AoC_Day_11,	AoC_Day_12,	AoC_Day_13,	AoC_Day_14,	AoC_Day_15,
			AoC_Day_16,	AoC_Day_17,	AoC_Day_18,	AoC_Day_19,	AoC_Day_20,
			AoC_Day_21,	AoC_Day_22,	AoC_Day_23,	AoC_Day_24,	AoC_Day_25);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
		USBD_HandleCommands();

    /* USER CODE END WHILE */
    MX_USB_HOST_Process();

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief SDMMC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDMMC1_SD_Init(void)
{

  /* USER CODE BEGIN SDMMC1_Init 0 */

  /* USER CODE END SDMMC1_Init 0 */

  /* USER CODE BEGIN SDMMC1_Init 1 */

  /* USER CODE END SDMMC1_Init 1 */
  hsd1.Instance = SDMMC1;
  hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
  hsd1.Init.ClockBypass = SDMMC_CLOCK_BYPASS_DISABLE;
  hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd1.Init.BusWide = SDMMC_BUS_WIDE_1B;
  hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_ENABLE;
  hsd1.Init.ClockDiv = 4;
  /* USER CODE BEGIN SDMMC1_Init 2 */
	HAL_SD_Init(&hsd1);
  /* USER CODE END SDMMC1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM11 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM11_Init(void)
{

  /* USER CODE BEGIN TIM11_Init 0 */

  /* USER CODE END TIM11_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM11_Init 1 */

  /* USER CODE END TIM11_Init 1 */
  htim11.Instance = TIM11;
  htim11.Init.Prescaler = 0;
  htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim11.Init.Period = 65535;
  htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim11, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM11_Init 2 */

  /* USER CODE END TIM11_Init 2 */
  HAL_TIM_MspPostInit(&htim11);

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
  /* DMA2_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);

}

/* FMC initialization function */
static void MX_FMC_Init(void)
{

  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_NORSRAM_TimingTypeDef Timing = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram1.Instance = FMC_NORSRAM_DEVICE;
  hsram1.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FMC_NORSRAM_BANK1;
  hsram1.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1.Init.MemoryType = FMC_MEMORY_TYPE_PSRAM;
  hsram1.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram1.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
  hsram1.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
  hsram1.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
  hsram1.Init.ExtendedMode = FMC_EXTENDED_MODE_DISABLE;
  hsram1.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
  hsram1.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
  hsram1.Init.WriteFifo = FMC_WRITE_FIFO_ENABLE;
  hsram1.Init.PageSize = FMC_PAGE_SIZE_NONE;
  /* Timing */
  Timing.AddressSetupTime = 5;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 8;
  Timing.BusTurnAroundDuration = 1;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */

  /* USER CODE END FMC_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(WIFI_ENABLE_GPIO_Port, WIFI_ENABLE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_HOST_VBUF_GPIO_Port, USB_HOST_VBUF_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, LED2_Pin|LED1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : WIFI_ENABLE_Pin */
  GPIO_InitStruct.Pin = WIFI_ENABLE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(WIFI_ENABLE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PB5 PB4 PB12 PB13
                           PB1 PB0 PB10 PB11 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_1|GPIO_PIN_0|GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PG14 PG13 PG15 PG12
                           PG11 PG10 PG8 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_13|GPIO_PIN_15|GPIO_PIN_12
                          |GPIO_PIN_11|GPIO_PIN_10|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : SDMMC_SW_Pin PA10 PA9 PA6
                           PA5 PA3 PA7 */
  GPIO_InitStruct.Pin = SDMMC_SW_Pin|GPIO_PIN_10|GPIO_PIN_9|GPIO_PIN_6
                          |GPIO_PIN_5|GPIO_PIN_3|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PD6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PI3 PI1 PI0 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_1|GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_HOST_VBUF_Pin */
  GPIO_InitStruct.Pin = USB_HOST_VBUF_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_HOST_VBUF_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PC7 PC5 */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : LED3_Pin */
  GPIO_InitStruct.Pin = LED3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED2_Pin LED1_Pin */
  GPIO_InitStruct.Pin = LED2_Pin|LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : PF11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();

	// reset pins
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);

	// blink error code
	while (1)
	{
		switch(system_state.error)
		{
		case SYS_ERROR_SRAM_FAIL:
			HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
			//	HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
			//	HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
			break;

		case SYS_ERROR_SDIO_FAIL:
			//	HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
			HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
			//	HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
			break;

		case SYS_ERROR_WIFI_FAIL:
			HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
			HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
			//	HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
			break;

		default:
			break;
		}

		// loop delay
		for(volatile uint32_t t=0; t<2000000; t++);
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
