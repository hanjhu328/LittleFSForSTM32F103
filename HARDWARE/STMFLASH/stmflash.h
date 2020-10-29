#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "sys.h"  
#include "lfs.h"



//////////////////////////////////////////////////////////////////////////////////////////////////////
//用户根据自己的需要设置
#define STM32_FLASH_SIZE 512 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1              //使能FLASH写入(0，不是能;1，使能)
//////////////////////////////////////////////////////////////////////////////////////////////////////


#define STM32Flash_ERASE_GRAN              2048
#define STM32Flash_NUM_GRAN                256    //总的Page数量


//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
//FLASH解锁键值

//使用一半空间作为系统文件资源
#define STM32_FLASH_FLLESYS_START_BASE   (0x08000000 + STM32Flash_ERASE_GRAN*128)

uint8_t STMFLASH_ReadHalfWord(u32 faddr);		  //读出半字  
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//指定地址开始写入指定长度的数据
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);						//指定地址开始读取指定长度数据
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//从指定地址开始写入指定长度的数据
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//从指定地址开始读出指定长度的数据

//测试写入
void Test_Write(u32 WriteAddr,u16 WriteData);

//lfs interface
int stm32flash_readLittlefs(const struct lfs_config *c, lfs_block_t block,
				lfs_off_t off, void *buffer, lfs_size_t size);

int stm32flash_writeLittlefs(const struct lfs_config *c, lfs_block_t block,
				lfs_off_t off,void *buffer, lfs_size_t size);

int stm32flash_eraseLittlefs(const struct lfs_config *c, lfs_block_t block);


int stm32flash_syncLittlefs(const struct lfs_config *c );


#endif

