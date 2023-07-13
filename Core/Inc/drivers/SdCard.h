/*
 * SdCard.h
 *
 *  Created on: Jul 9, 2023
 *      Author: J.Verkade
 */

#ifndef INC_DRIVERS_SDCARD_H_
#define INC_DRIVERS_SDCARD_H_

#include "main.h"
#include "fatfs.h"

typedef struct SdCard_t SdCard_t;
struct SdCard_t
{
	// data
	FIL loaded_file;
	FATFS file_system;
	uint32_t bytes_written;
	uint8_t file_result;
};

// init
uint8_t SdCard_init(SdCard_t *sd);

// file system checking
uint8_t SdCard_dirExists(SdCard_t *sd, char *dir_name);
uint8_t SdCard_fileExists(SdCard_t *sd, char *file_name);

// file creation
uint8_t SdCard_createFile(SdCard_t *sd, char *file_name, uint8_t *data, uint32_t len);
uint8_t SdCard_destroyFile(SdCard_t *sd, char *file_name);

// file accesing and saving
uint8_t SdCard_open(SdCard_t *sd, char *file_name);
uint8_t SdCard_flush(SdCard_t *sd);
uint8_t SdCard_close(SdCard_t *sd, char *file_name);

// file writing
uint8_t SdCard_write(char *file_name, uint8_t *data, uint32_t *len);



uint8_t SdCard_readLine();



#endif /* INC_DRIVERS_SDCARD_H_ */
