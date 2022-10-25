#include <stdio.h>
#include <string.h>

#include "iot_errno.h"
#include "iot_gpio.h"
#include "iot_flash.h"
#include "iot_i2c.h"
#include "iot_pwm.h"
#include "iot_uart.h"
#include "iot_watchdog.h"
#include "lowpower.h"
#include "reset.h"

#include "iot_test.h"
#include "typedef.h"
#include "target_util_pub.h"
#include "securec.h"

#define GPIO15 15
#define TEST_PIN GPIO15

#define CHECK_RESULT(ret) do {if ((ret) != IOT_SUCCESS) { printf("test failed %s:%d\n", __FILE__, __LINE__); return ret; }} while(0)

static void gpio_cb(char *arg)
{
	printf("gpio_cb : got %d\n", *arg);
}

int iot_test_gpio(void)
{
	unsigned int ret;
	unsigned int id;
	IotGpioValue value;
	IotGpioDir dir;

	printf("iot_test_gpio begin...\r\n");
	
	id = TEST_PIN;
	
//****************** test output ****************//
	ret = IoTGpioInit(id);
	CHECK_RESULT(ret);

	ret = IoTGpioSetDir(id, IOT_GPIO_DIR_OUT);
	CHECK_RESULT(ret);

	ret = IoTGpioGetDir(id, &dir);
	CHECK_RESULT(ret);
	if (dir != IOT_GPIO_DIR_OUT) {
		printf("set IOT_GPIO_DIR_OUT, but got %d\n", dir);
		return -1;
	}
	
	ret = IoTGpioSetOutputVal(id, IOT_GPIO_VALUE1);
	CHECK_RESULT(ret);

	ret = IoTGpioGetOutputVal(id, &value);
	CHECK_RESULT(ret);
	if (value != IOT_GPIO_VALUE1) {
		printf("set IOT_GPIO_VALUE1, but got %d\n", value);
		return -1;
	}

	ret = IoTGpioSetOutputVal(id, IOT_GPIO_VALUE0);
	CHECK_RESULT(ret);

	ret = IoTGpioGetOutputVal(id, &value);
	CHECK_RESULT(ret);
	if (value != IOT_GPIO_VALUE0) {
		printf("set IOT_GPIO_VALUE0, but got %d\n", value);
		return -1;
	}
	
//****************** test input & interrupt ****************//
	ret = IoTGpioSetDir(id, IOT_GPIO_DIR_IN);
	CHECK_RESULT(ret);

	ret = IoTGpioGetInputVal(id, &value);
	CHECK_RESULT(ret);

	ret = IoTGpioRegisterIsrFunc(id, IOT_INT_TYPE_EDGE, IOT_GPIO_EDGE_RISE_LEVEL_HIGH, gpio_cb, (char *)&id);
	CHECK_RESULT(ret);

	ret = IoTGpioSetIsrMask(id, 1);
	CHECK_RESULT(ret);
	
	ret = IoTGpioSetIsrMask(id, 0);
	CHECK_RESULT(ret);

	ret = IoTGpioUnregisterIsrFunc(id);
	CHECK_RESULT(ret);

	ret = IoTGpioDeinit(id);
	CHECK_RESULT(ret);
	
	printf("iot_test_gpio ok.\r\n");
	return 0;	
}

int iot_test_flash(void)
{
#define FLASH_TEST_SIZE (4096 * 2)
	unsigned int start_addr,size,i;
	static unsigned char buf[FLASH_TEST_SIZE];
	//1. flash erase
	start_addr = 0x1F0000;
	size = FLASH_TEST_SIZE;
	memset(buf,0,FLASH_TEST_SIZE);
	IoTFlashErase(start_addr,size);
	IoTFlashRead(start_addr,size,buf);
	for(i=0;i<FLASH_TEST_SIZE;i++)
	{
		if(buf[i] != 0xff)
			break;
	}
	if(i == FLASH_TEST_SIZE)
		printf("flash Erase success!\r\n");
	else
		printf("flash Erase fail!!!\r\n");
		
	//2. flash write
	memset(buf,0x5a,FLASH_TEST_SIZE);
	IoTFlashWrite(start_addr,size,buf,0);
	memset(buf,0,FLASH_TEST_SIZE);

	IoTFlashRead(start_addr,size,buf);
	for(i=0;i<FLASH_TEST_SIZE;i++)
	{
		if(buf[i] != 0x5a)
			break;
	}
	if(i == FLASH_TEST_SIZE)
		printf("flash write success 11!\r\n");
	else
		printf("flash write fail!!!\r\n");

	//3. flash erase & write
	memset(buf,0xa5,FLASH_TEST_SIZE);
	IoTFlashWrite(start_addr,size,buf,1);
	memset(buf,0,FLASH_TEST_SIZE);

	IoTFlashRead(start_addr,size,buf);
	for(i=0;i<FLASH_TEST_SIZE;i++)
	{
		if(buf[i] != 0xa5)
			break;
	}
	if(i == FLASH_TEST_SIZE)
		printf("flash write success 22!\r\n");
	else
		printf("flash write fail!!!\r\n");
	return 0;
}

int iot_test_i2c(void)
{
	return 113;
}

#define TEST_PWM_PORT 3

int iot_test_pwm(void)
{
	unsigned int ret;
	unsigned int port;

	printf("iot_test_pwm begin...\r\n");

	port = TEST_PWM_PORT;

	ret = IoTPwmInit(port);
	CHECK_RESULT(ret);

	ret = IoTPwmStart(port, 30, 1000);
	CHECK_RESULT(ret);

	delay_ms(500);

	ret = IoTPwmStop(port);
	CHECK_RESULT(ret);


	ret = IoTPwmDeinit(port);
	CHECK_RESULT(ret);

	printf("iot_test_pwm ok.\r\n");
	return 0;
}

#define TEST_UART_PORT 1

int iot_test_uart(void)
{
#define BUFFER_LEN 32

	unsigned int ret;
	unsigned int port;
	IotUartAttribute param;
	unsigned char buffer[BUFFER_LEN];

	printf("iot_test_uart begin...\r\n");

	param.baudRate = 115200;
	param.dataBits = IOT_UART_DATA_BIT_8;
	param.stopBits = IOT_UART_STOP_BIT_1;
	param.parity = IOT_UART_PARITY_NONE;
	param.rxBlock = IOT_UART_BLOCK_STATE_NONE_BLOCK;
	param.txBlock = IOT_UART_BLOCK_STATE_NONE_BLOCK;

	port = TEST_UART_PORT;
	ret = IoTUartInit(port, &param);
	CHECK_RESULT(ret);

	printf("read from serial...\n");
	delay_ms(500);

	ret = IoTUartRead(port, buffer, BUFFER_LEN - 1);
	buffer[BUFFER_LEN - 1] = '\0';
	printf("IoTUartRead : ret=%d\n", ret);

	memcpy_s(buffer, BUFFER_LEN, "send some string\n", 17 + 1);
	ret = IoTUartWrite(port, buffer, 17 + 1);
	printf("IoTUartWrite : ret=%d\n", ret);
	
	printf("iot_test_uart ok.\r\n");
	return 0;
}

int iot_test_watchdog(void)
{
	int i;
	
	printf("iot_test_watchdog begin...\r\n");

	IoTWatchDogEnable();
	for (i = 0; i < 5; i++) {
		printf("kick wdg %d\n", i);
		IoTWatchDogKick();
		delay_ms(500);
	}
	//delay_ms(5000);
	IoTWatchDogDisable();
	
	printf("iot_test_watchdog ok.\r\n");
	return 0;
}

int iot_test_lowpower(void)
{
	return 117;
}

int iot_test_reset(void)
{
	printf("iot_test_reset begin...\r\n");

	delay_ms(2000);
	printf("reboot device\r\n");
	delay_ms(200);
	RebootDevice(0);
	
	printf("iot_test_reset ok.\r\n");
	return 0;
}

