// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// MLX30393
// This code is designed to work with the MLX30393_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void main() 
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, MLX30393 I2C address is 0x0C(12)
	ioctl(file, I2C_SLAVE, 0x0C);

	// Write register command, AH = 0x00, AL = 0x5C, Hall plate spinning rate = DEFAULT, GAIN_SEL = 5(0x60, 0x00, 0x5C, 0x00)
	// Address register, (0x00 << 2)
	char config[4] = {0};
	config[0] = 0x60;
	config[1] = 0x00;
	config[2] = 0x5C;
	config[3] = 0x00;
	write(file, config, 4);

	// Read 1 Byte of data
	// status byte
	char data[7] = {0};
	read(file, data, 1);

	// Write register command, AH = 0x02, AL = 0xB4, RES for magnetic measurement = 0(0x60, 0x02, 0xB4, 0x08)
	// Address register, (0x02 << 2)
	config[0] = 0x60;
	config[1] = 0x02;
	config[2] = 0xB4;
	config[3] = 0x08;
	write(file, config, 4);

	// Read 1 Byte of data
	// status byte
	read(file, data, 1);

	// Start single meaurement mode, X, Y, Z-Axis enabled(0x3E)
	config[0] = 0x3E;
	write(file, config, 1);

	// Read 1 byte
	// status
	read(file, data, 1);
	sleep(1);

	// Send read measurement command, X, Y, Z-Axis enabled(0x4E)
	config[0] = 0x4E;
	write(file, config, 1);

	// Read 7 bytes of data
	// status, xMag msb, xMag lsb, yMag msb, yMag lsb, zMag msb, zMag lsb
	if(read(file, data, 7) != 7)
	{
		printf("Erorr : Input/output Erorr \n");
	}
	else
	{
		// Convert the data
		int xMag = (data[1]* 256 + data[2]);
		if(xMag > 32767)
		{
			xMag -= 65536;
		}

		int yMag = (data[3] * 256 + data[4]);
		if(yMag > 32767)
		{
			yMag -= 65536;
		}

		int zMag = (data[5] * 256 + data[6]);
		if(zMag > 32767)
		{
			zMag -= 65536;
		}

		// Output data to screen
		printf("Magnetic Field in X-Axis : %d \n", xMag);
		printf("Magnetic Field in Y-Axis : %d \n", yMag);
		printf("Magnetic Field in Z-Axis : %d \n", zMag);
	}
}
