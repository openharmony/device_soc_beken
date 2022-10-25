#include "iot_errno.h"
#include "iot_i2c.h"

#include <stdbool.h>	//extra

#include <driver/i2c.h>

struct i2c_ctrl {
	unsigned int inited;
	unsigned int baudrate;
};

#define I2C_NUM		2

static struct i2c_ctrl i2c_ctrls[I2C_NUM] = {
	{0, I2C_BAUD_RATE_400KHZ},
	{0, I2C_BAUD_RATE_400KHZ},
};

unsigned int IoTI2cWrite(unsigned int id, unsigned short deviceAddr, const unsigned char *data, unsigned int dataLen)
{
	
	if (id > I2C_NUM)
		return IOT_FAILURE;

	if ((deviceAddr == 0) || (data == NULL) || (dataLen == 0))
		return IOT_FAILURE;

	if (!i2c_ctrls[id].inited)
		return IOT_FAILURE;

	//bk_i2c_master_write
	return IOT_SUCCESS;
}

unsigned int IoTI2cRead(unsigned int id, unsigned short deviceAddr, unsigned char *data, unsigned int dataLen)
{
	//bk_i2c_master_read
	return IOT_FAILURE;
}

unsigned int IoTI2cInit(unsigned int id, unsigned int baudrate)
{
	return IOT_FAILURE;
}

unsigned int IoTI2cDeinit(unsigned int id)
{
	return IOT_FAILURE;
}

unsigned int IoTI2cSetBaudrate(unsigned int id, unsigned int baudrate)
{
	return IOT_FAILURE;
}
