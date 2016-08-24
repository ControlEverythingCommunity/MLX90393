# Distributed with a free-will license.
# Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
# MLX90393
# This code is designed to work with the MLX90393_I2CS I2C Mini Module available from ControlEverything.com.
# https://www.controleverything.com/products

from OmegaExpansion import onionI2C
import time

# Get I2C bus
i2c = onionI2C.OnionI2C()

# MLX90393 address, 0x0C(12)
# Select write register command, 0x60(96)
# AH = 0x00, AL = 0x5C, GAIN_SEL = 5, Address register (0x00 << 2)
config = [0x00, 0x5C, 0x00]
i2c.writeBytes(0x0C, 0x60, config)

# Read data back, 1 byte
# Status byte
data = i2c.readBytes(0x0C, 0x00, 1)

# MLX90393 address, 0x0C(12)
# Select write register command, 0x60(96)
# AH = 0x02, AL = 0xB4, RES for magnetic measurement = 0, Address register (0x02 << 2)
config = [0x02, 0xB4, 0x08]
i2c.writeBytes(0x0C, 0x60, config)

# Read data back, 1 byte
# Status byte
data = i2c.readBytes(0x0C, 0x00, 1)

# MLX90393 address, 0x0C(12)
# Start single meaurement mode, X, Y, Z-Axis enabled
data = [0x3E]
i2c.write(0x0C, data)

# Read data back, 1 byte
# Status byte
data = i2c.readBytes(0x0C, 0x00, 1)

time.sleep(0.5)

# MLX90393 address, 0x0C(12)
# Read data back from 0x4E(78), 7 bytes
# Status, xMag msb, xMag lsb, yMag msb, yMag lsb, zMag msb, zMag lsb
data = i2c.readBytes(0x0C, 0x4E, 7)

# Convert the data
xMag = data[1] * 256 + data[2]
if xMag > 32767 :
	xMag -= 65536

yMag = data[3] * 256 + data[4]
if yMag > 32767 :
	yMag -= 65536

zMag = data[5] * 256 + data[6]
if zMag > 32767 :
	zMag -= 65536

# Output data to screen
print "Magnetic Field in X-Axis : %d" %xMag
print "Magnetic Field in Y-Axis : %d" %yMag
print "Magnetic Field in Z-Axis : %d" %zMag
