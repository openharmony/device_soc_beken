// Copyright (C) 2022 Beken Corporation
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "iot_errno.h"
#include "iot_uart.h"

#include <driver/uart.h>
#include <os/os.h>

//SOC_UART_ID_NUM_PER_UNIT
#define BK_UART_MAX 2

struct uart_ctrl {
	unsigned int inited;
};

static struct uart_ctrl uart_ctrls[BK_UART_MAX] = {
	[ 0 ... (BK_UART_MAX-1) ] = {0},
};

static int convert_uart_config(uart_config_t *dst, const IotUartAttribute *src)
{
	dst->baud_rate = src->baudRate;
	
	if (src->dataBits == IOT_UART_DATA_BIT_5)
		dst->data_bits = UART_DATA_5_BITS;
	else if (src->dataBits == IOT_UART_DATA_BIT_6)
		dst->data_bits = UART_DATA_6_BITS;
	else if (src->dataBits == IOT_UART_DATA_BIT_7)
		dst->data_bits = UART_DATA_7_BITS;
	else if (src->dataBits == IOT_UART_DATA_BIT_8)
		dst->data_bits = UART_DATA_8_BITS;
	else
		return -1;
	
	dst->parity = src->parity ;
	
	if (src->stopBits == IOT_UART_STOP_BIT_1)
		dst->stop_bits = UART_STOP_BITS_1;
	else if (src->stopBits == IOT_UART_STOP_BIT_2)
		dst->stop_bits = UART_STOP_BITS_2;
	else
		return -1;
	
	dst->flow_ctrl = UART_FLOWCTRL_DISABLE;
	dst->src_clk = UART_SCLK_XTAL_26M;

	return 0;
}

unsigned int IoTUartInit(unsigned int id, const IotUartAttribute *param)
{
	unsigned int ret;
	uart_config_t config;
	
	if (id >= BK_UART_MAX)
		return IOT_FAILURE;

	if (uart_ctrls[id].inited)
		return IOT_FAILURE;
	
	ret = convert_uart_config(&config, param);
	if (ret)
		return IOT_FAILURE;

	ret = bk_uart_init(id, &config);
	if (ret)
		return IOT_FAILURE;

	uart_ctrls[id].inited = 1;
	return IOT_SUCCESS;
}

unsigned int IoTUartDeinit(unsigned int id)
{
	if (id >= BK_UART_MAX)
		return IOT_FAILURE;

	bk_uart_deinit(id);
	uart_ctrls[id].inited = 0;	
	return IOT_SUCCESS;
}

int IoTUartRead(unsigned int id, unsigned char *data, unsigned int dataLen)
{
	unsigned int ret;
	
	if (id >= BK_UART_MAX)
		return IOT_FAILURE;
	if ((data == NULL) || (dataLen == 0))
		return IOT_FAILURE;

	if (!uart_ctrls[id].inited)
		return IOT_FAILURE;

	ret = bk_uart_read_bytes(id, data, dataLen, BEKEN_NO_WAIT);
	if (ret)
		return IOT_FAILURE;

	return dataLen;
}

int IoTUartWrite(unsigned int id, const unsigned char *data, unsigned int dataLen)
{
	unsigned int ret;

	if (id >= BK_UART_MAX)
		return IOT_FAILURE;
	if ((data == NULL) || (dataLen == 0))
		return IOT_FAILURE;

	if (!uart_ctrls[id].inited)
		return IOT_FAILURE;

	ret = bk_uart_write_bytes(id, data, dataLen);
	if (ret)
		return IOT_FAILURE;

	return dataLen;	
}

unsigned int IoTUartSetFlowCtrl(unsigned int id, IotFlowCtrl flowCtrl)
{
	return IOT_FAILURE;	//TODO
}

