#include "osapi.h"
#include "user_interface.h"

#if ((SPI_FLASH_SIZE_MAP == 0) || (SPI_FLASH_SIZE_MAP == 1))
        #error "The flash map is not supported"
#elif (SPI_FLASH_SIZE_MAP == 2)
        #define SYSTEM_PARTITION_OTA_SIZE                       0x6A000
        #define SYSTEM_PARTITION_OTA_2_ADDR                     0x81000
        #define SYSTEM_PARTITION_RF_CAL_ADDR                    0xfb000
        #define SYSTEM_PARTITION_PHY_DATA_ADDR                  0xfc000
        #define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR          0xfd000
#elif (SPI_FLASH_SIZE_MAP == 3)
        #define SYSTEM_PARTITION_OTA_SIZE                       0x6A000
        #define SYSTEM_PARTITION_OTA_2_ADDR                     0x81000
        #define SYSTEM_PARTITION_RF_CAL_ADDR                    0x1fb000
        #define SYSTEM_PARTITION_PHY_DATA_ADDR                  0x1fc000
        #define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR          0x1fd000
#elif (SPI_FLASH_SIZE_MAP == 4)
        #define SYSTEM_PARTITION_OTA_SIZE                       0x6A000
        #define SYSTEM_PARTITION_OTA_2_ADDR                     0x81000
        #define SYSTEM_PARTITION_RF_CAL_ADDR                    0x3fb000
        #define SYSTEM_PARTITION_PHY_DATA_ADDR                  0x3fc000
        #define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR          0x3fd000
#elif (SPI_FLASH_SIZE_MAP == 5)
        #define SYSTEM_PARTITION_OTA_SIZE                       0x6A000
        #define SYSTEM_PARTITION_OTA_2_ADDR                     0x101000
        #define SYSTEM_PARTITION_RF_CAL_ADDR                    0x1fb000
        #define SYSTEM_PARTITION_PHY_DATA_ADDR                  0x1fc000
        #define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR          0x1fd000
#elif (SPI_FLASH_SIZE_MAP == 6)
        #define SYSTEM_PARTITION_OTA_SIZE                       0x06A000
        #define SYSTEM_PARTITION_OTA_2_ADDR                     0x101000
        #define SYSTEM_PARTITION_RF_CAL_ADDR                    0x3fb000
        #define SYSTEM_PARTITION_PHY_DATA_ADDR                  0x3fc000
        #define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR          0x3fd000
#else
        #error "The flash map is not supported"
#endif

/*
 *     Non FOTA partition table
 *     */

#define EAGLE_FLASH_BIN_ADDR				(SYSTEM_PARTITION_CUSTOMER_BEGIN + 1)
#define EAGLE_IROM0TEXT_BIN_ADDR			(SYSTEM_PARTITION_CUSTOMER_BEGIN + 2)

static const partition_item_t partition_table[] = {
    { EAGLE_FLASH_BIN_ADDR, 	0x00000, 0x10000},
    { EAGLE_IROM0TEXT_BIN_ADDR, 0x10000, 0x60000},
    { SYSTEM_PARTITION_RF_CAL, SYSTEM_PARTITION_RF_CAL_ADDR, 0x1000},
    { SYSTEM_PARTITION_PHY_DATA, SYSTEM_PARTITION_PHY_DATA_ADDR, 0x1000},
    { SYSTEM_PARTITION_SYSTEM_PARAMETER,SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR, 0x3000},
};

void ICACHE_FLASH_ATTR user_pre_init(void) {
	if(!system_partition_table_regist(partition_table, sizeof(partition_table)/sizeof(partition_table[0]),SPI_FLASH_SIZE_MAP)) {
		os_printf("system_partition_table_regist fail\r\n");
		for(;;) { }
	} else {
		os_printf("system_partition_table_regist success\r\n");
	}
}


