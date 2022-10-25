#include "iot_errno.h"
#include "iot_gpio.h"

#include <driver/gpio.h>
//#include "bk_gpio.h"

#define GPIO_PIN_INIT    1
#define GPIO_PIN_UNINIT  0
#define GPIONUM SOC_GPIO_NUM

static unsigned char g_gpioMap[GPIONUM] = {0};
static unsigned char g_gpioInitNum = 0;

#define GPIO_CHECK(gpio) do { if ((gpio) >= GPIONUM) return IOT_FAILURE; } while(0)

unsigned int IoTGpioInit(unsigned int id)
{
    GPIO_CHECK(id);

    if (g_gpioInitNum == 0) {
        //(void)hi_gpio_init();
    }

    if (g_gpioMap[id] == GPIO_PIN_INIT) {
        return IOT_FAILURE;
    } else {
        g_gpioMap[id] = GPIO_PIN_INIT;
        g_gpioInitNum++;
    }

    return IOT_SUCCESS;
}

unsigned int IoTGpioDeinit(unsigned int id)
{
    GPIO_CHECK(id);

    if (g_gpioMap[id] == GPIO_PIN_INIT) {
        g_gpioInitNum--;
        g_gpioMap[id] = GPIO_PIN_UNINIT;
    } else {
        return IOT_FAILURE;
    }

    if (g_gpioInitNum == 0) {
        //return hi_gpio_deinit();
        return IOT_SUCCESS;
    } else {
        return IOT_SUCCESS;
    }
}

unsigned int IoTGpioSetDir(unsigned int id, IotGpioDir dir)
{
	unsigned int ret;
	const gpio_config_t gpio_cfg_in = { GPIO_INPUT_ENABLE, GPIO_PULL_UP_EN,GPIO_SECOND_FUNC_DISABLE};
	const gpio_config_t gpio_cfg_out = { GPIO_OUTPUT_ENABLE, GPIO_PULL_DISABLE,GPIO_SECOND_FUNC_DISABLE};
    GPIO_CHECK(id);

	switch (dir) {
		case IOT_GPIO_DIR_IN : 
			gpio_dev_unmap(id);
			ret = IOT_SUCCESS;
			bk_gpio_set_config(id, &gpio_cfg_in);
			break;
			
		case IOT_GPIO_DIR_OUT : 
			gpio_dev_unmap(id);
			bk_gpio_set_config(id, &gpio_cfg_out);
			ret = IOT_SUCCESS;
			break;
			
		default : 
			ret = IOT_FAILURE;
			break;
	}
	return ret;
}

unsigned int IoTGpioGetDir(unsigned int id, IotGpioDir *dir)
{
	unsigned int value = 0;

    GPIO_CHECK(id);

	if (!dir)
		return IOT_FAILURE;

	if(BK_ERR_GPIO_NOT_INPUT_MODE == bk_gpio_get_input(id))
		*dir = IOT_GPIO_DIR_OUT ;
	else
		*dir = IOT_GPIO_DIR_IN;
	return IOT_SUCCESS;
}

unsigned int IoTGpioSetOutputVal(unsigned int id, IotGpioValue val)
{
    GPIO_CHECK(id);
	if(val == IOT_GPIO_VALUE1)
		bk_gpio_set_output_high(id);
	else
		bk_gpio_set_output_low(id);
	return IOT_SUCCESS;
}

unsigned int IoTGpioGetOutputVal(unsigned int id, IotGpioValue *val)
{
	unsigned int value = 0;
	
    GPIO_CHECK(id);

	if (!val)
		return IOT_FAILURE;

	//fixme
	//gpio_get_output(id, &value);	//new
	*val = value ? IOT_GPIO_VALUE1 : IOT_GPIO_VALUE0;
	return IOT_SUCCESS;
}

unsigned int IoTGpioGetInputVal(unsigned int id, IotGpioValue *val)
{
	unsigned int value;
	
    GPIO_CHECK(id);

	if (!val)
		return IOT_FAILURE;

	value = bk_gpio_get_input(id);
	if(BK_ERR_GPIO_NOT_INPUT_MODE == value)
		return IOT_FAILURE;

	*val = value ? IOT_GPIO_VALUE1 : IOT_GPIO_VALUE0;
	return IOT_SUCCESS;
}

struct gpio_isr_adapter {
	GpioIsrCallbackFunc func;
	char * arg;
};

static struct gpio_isr_adapter gpio_isr_adapters[GPIONUM] = {
	[ 0 ... (GPIONUM - 1) ] = {NULL, NULL}
};

static void gpio_default_handler(unsigned char gpio_id)
{
	if (gpio_id >= GPIONUM)
		return;
	
	if (gpio_isr_adapters[gpio_id].func)
		gpio_isr_adapters[gpio_id].func(gpio_isr_adapters[gpio_id].arg);
}

static unsigned int combine_isr_mode(IotGpioIntType intType, IotGpioIntPolarity intPolarity)
{
	unsigned int mode;
	
	if (intType == IOT_INT_TYPE_LEVEL) {
		mode = intPolarity ? 1 : 0;
	} else {
		mode = intPolarity ? 2 : 3;
	}

	return mode;
}

unsigned int IoTGpioRegisterIsrFunc(unsigned int id, IotGpioIntType intType, IotGpioIntPolarity intPolarity,
                                    GpioIsrCallbackFunc func, char *arg)
{
	unsigned int mode;

    GPIO_CHECK(id);

	mode = combine_isr_mode(intType, intPolarity);
	gpio_isr_adapters[id].func = func;
	gpio_isr_adapters[id].arg = arg;
	//gpio_int_enable(id, mode, gpio_default_handler);
	bk_gpio_set_interrupt_type(id, mode);
	bk_gpio_register_isr(id, gpio_default_handler);
	bk_gpio_enable_interrupt(id);
	return IOT_SUCCESS;
}

unsigned int IoTGpioUnregisterIsrFunc(unsigned int id)
{
    GPIO_CHECK(id);

	//gpio_int_disable(id);
	bk_gpio_disable_interrupt(id);
	return IOT_SUCCESS;
}

unsigned int IoTGpioSetIsrMask(unsigned int id, unsigned char mask)
{
    GPIO_CHECK(id);

	if (mask)
		bk_gpio_disable_interrupt(id);
	else
		bk_gpio_enable_interrupt(id);
	return IOT_SUCCESS;
}

unsigned int IoTGpioSetIsrMode(unsigned int id, IotGpioIntType intType, IotGpioIntPolarity intPolarity)
{
    GPIO_CHECK(id);

	if (gpio_isr_adapters[id].func)
		return IoTGpioRegisterIsrFunc(id, intType, intPolarity, gpio_isr_adapters[id].func, gpio_isr_adapters[id].arg);
	else
		return IOT_FAILURE;
}

