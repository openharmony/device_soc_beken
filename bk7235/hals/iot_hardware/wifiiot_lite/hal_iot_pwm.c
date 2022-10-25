#include "iot_errno.h"
#include "iot_pwm.h"

#include <driver/pwm.h>

#define BK_PWM_MAX 6

unsigned int IoTPwmInit(unsigned int port)
{
	if(port >= BK_PWM_MAX)
		return IOT_FAILURE;
	
	return IOT_SUCCESS;
}

unsigned int IoTPwmDeinit(unsigned int port)
{
	if(port >= BK_PWM_MAX)
		return IOT_FAILURE;

	return IOT_SUCCESS;
}

unsigned int IoTPwmStart(unsigned int port, unsigned short duty, unsigned int freq)
{
	unsigned int ret;
	pwm_init_config_t config;
	
	if(port >= BK_PWM_MAX || (freq == 0))
		return IOT_FAILURE;

	config.period_cycle = 1000000 / freq;
	config.duty_cycle = duty * config.period_cycle / 100;
	ret = bk_pwm_init(port, &config);
	if (ret)
		return IOT_FAILURE;
	
	ret = bk_pwm_start(port);
	if (ret)
		return IOT_FAILURE;
	
	return IOT_SUCCESS;
}

unsigned int IoTPwmStop(unsigned int port)
{
	if(port >= BK_PWM_MAX)
		return IOT_FAILURE;

	bk_pwm_stop(port);
	return IOT_SUCCESS;
}

