#include "iot_errno.h"
#include "iot_flash.h"
#include "flash.h"

//#include "bk_flash.h"

static flash_protect_type_t  protect_flag;
unsigned int IoTFlashRead(unsigned int flashOffset, unsigned int size, unsigned char *ramData)
{
	bk_flash_read_bytes(flashOffset,ramData,size);
	return IOT_SUCCESS;
}

unsigned int IoTFlashWrite(unsigned int flashOffset, unsigned int size,
                           const unsigned char *ramData, unsigned char doErase)
{
	bk_flash_write_bytes(flashOffset,ramData,size);
	return IOT_SUCCESS;
}

unsigned int IoTFlashErase(unsigned int flashOffset, unsigned int size)
{
	UINT32 i;
	UINT32 start_sector, end_sector, param;
	GLOBAL_INT_DECLARATION();

	start_sector = flashOffset >> 12;
	end_sector = (flashOffset + size - 1) >> 12;

	GLOBAL_INT_DISABLE();
	for (i = start_sector; i <= end_sector; i ++) {
		param = i << 12;
		bk_flash_erase_sector(param);
	}
	GLOBAL_INT_RESTORE();

	return IOT_SUCCESS;
}

unsigned int IoTFlashInit(void)
{
	protect_flag = bk_flash_get_protect_type();
	return IOT_SUCCESS;
	bk_flash_set_protect_type(FLASH_PROTECT_NONE);
}

unsigned int IoTFlashDeinit(void)
{
	bk_flash_set_protect_type(protect_flag);
	return IOT_SUCCESS;
}