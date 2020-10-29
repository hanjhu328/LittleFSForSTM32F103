#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "w25qxx.h"
#include "stdio.h"
#include "lfs.h"
 
 
lfs_t lfs; 
lfs_file_t file;




uint8_t sendbuf[256] = "W25Q128 data write test for littlefs.littlefs is error\r\n";
uint8_t readbuf[256] = {0};


// configuration of the filesystem is provided by this struct
const struct lfs_config cfg = {
    // block device operations
    .read  = W25Qxx_readLittlefs,
    .prog  = W25Qxx_writeLittlefs,
    .erase = W25Qxx_eraseLittlefs,
    .sync  = W25Qxx_syncLittlefs,

    // block device configuration
    .read_size = 256,
    .prog_size = 256,
    .block_size = W25Q128_ERASE_GRAN,
    .block_count = W25Q128_NUM_GRAN,
    .cache_size = 512,
    .lookahead_size = 512,
    .block_cycles = 500,
};


int main(void)
{
	int err = -1;
	
	
	
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	LED_Init();		  		//初始化与LED连接的硬件接口
	KEY_Init();				//按键初始化		 	 	
	W25QXX_Init();			//W25QXX初始化
    
	
	
	

	while(W25QXX_ReadID()!=W25Q128)								//检测不到W25Q128
	{
		printf("W25Q128 Check Failed!\r\n");
		delay_ms(500);
		printf("Please Check!        \r\n");
		delay_ms(500);
		LED0=!LED0;//DS0闪烁
	}
	printf("W25Q128 Ready!\r\n");  

    err =  lfs_mount(&lfs, &cfg);
	
	if(err )
	{
		lfs_format(&lfs, &cfg);
        lfs_mount(&lfs, &cfg);
	}
	
	
	while(1)
	{
		LED0=!LED0;

		
		uint32_t boot_count = 0;
		lfs_file_open(&lfs, &file, "boot_count", LFS_O_RDWR | LFS_O_CREAT);
		lfs_file_read(&lfs, &file, &boot_count, sizeof(boot_count));

		
		// update boot count
		boot_count += 1;
		lfs_file_rewind(&lfs, &file);  // seek the file to begin
		lfs_file_write(&lfs, &file, &boot_count, sizeof(boot_count));

		// remember the storage is not updated until the file is closed successfully
		lfs_file_close(&lfs, &file);

//		// release any resources we were using
//		lfs_unmount(&lfs);

		// print the boot count
		printf("boot_count: %d\n", boot_count);

		delay_ms(20000);
	}
}


