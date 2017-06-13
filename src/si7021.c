/** @file si7021.h
 *  @brief Define Silicon Image Si7021 constants
 *
 *  Source: https://www.silabs.com/documents/public/data-sheets/Si7021-A20.pdf
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>		//Needed for I2C port
#include <fcntl.h>		//Needed for I2C port
#include <sys/ioctl.h>		//Needed for I2C port
#include <linux/i2c-dev.h>	//Needed for I2C port

#include "si7021.h"

#define	Si7021_BUS	"/dev/i2c-1"

int si7021_read_temperature(float *temperature)
{
    int file_i2c;
    int length;
    unsigned char buffer[60] = { 0 };

    //----- OPEN THE I2C BUS -----
    if ((file_i2c = open(Si7021_BUS, O_RDWR)) < 0) {
	return FAIL_TO_OPEN;
    }

    int addr = Si7021_ADDRESS;
    if (ioctl(file_i2c, I2C_SLAVE, addr) < 0) {
	return FAIL_TO_SELECT;
    }
    //----- WRITE measure command  -----
    buffer[0] = Si7021_TEMP_HOLD;
    length = 1;
    if (write(file_i2c, buffer, length) != length) {
	/* ERROR HANDLING: i2c transaction failed */
	return FAIL_TO_WRITE;
    }
    //----- READ RESULT -----
    length = 4;			// expect 2 data bytes + crc but reading more does not hurt
    int il = read(file_i2c, buffer, length);
    if (il != length)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
    {
	//ERROR HANDLING: i2c transaction failed
	printf("Failed to read from the i2c bus., il = %d\n", il);
    }

    // convert the reading to degrees celcius using the conversion from the data sheet.

    long in = (buffer[0] << 8) | buffer[1];
    float c = ((175.2 * (double) in) / 65536.0) - 46.85;
    *temperature = c;
    return 0;
}
