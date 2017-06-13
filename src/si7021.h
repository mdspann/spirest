/** @file si7021.h
 *  @brief Define Silicon Image Si7021 constants
 *
 *  Source: https://www.silabs.com/documents/public/data-sheets/Si7021-A20.pdf
 */

#ifndef _SI7021_H
#define _SI7021_H

// i2c slave address for Si7021

#define Si7021_ADDRESS		0x40

// Operation codes for Si7021

#define Si7021_RH_HOLD  	0xE5
#define Si7021_RH_NOHOLD	0xF5
#define Si7021_TEMP_HOLD	0xE3
#define Si7021_TEMP_NOHOLD	0xE3
#define Si7021_TEMP_STORED	0xE0
#define Si7021_RESET		0xFE

// error codes

#define	FAIL_TO_OPEN	-1
#define	FAIL_TO_SELECT	-2
#define	FAIL_TO_WRITE	-3

extern int si7021_read_temperature(float *temperature);
#endif
