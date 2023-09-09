/**
 ****************************************************************************************************
 * @file        norflash.h
 * @author      
 * @version     V2.0
 * @date        2023-09-05
 * @brief       NOR FLASH(25QXX) 驱动代码
 * @license     
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32H750开发板
 *
 * 修改说明
 * V2.0 20230905
 * 
 *
 ****************************************************************************************************
 */

#ifndef __norflash_H
#define __norflash_H

#include "./SYSTEM/sys/sys.h"


/* FLASH芯片列表 */
#define W25Q80      0XEF13          /* W25Q80   芯片ID */
#define W25Q16      0XEF14          /* W25Q16   芯片ID */
#define W25Q32      0XEF15          /* W25Q32   芯片ID */
#define W25Q64      0XEF16          /* W25Q64   芯片ID */
#define W25Q128     0XEF17          /* W25Q128  芯片ID */
#define W25Q256     0XEF18          /* W25Q256  芯片ID */
#define BY25Q64     0X6816          /* BY25Q64  芯片ID */
#define BY25Q128    0X6817          /* BY25Q128 芯片ID */
#define NM25Q64     0X5216          /* NM25Q64  芯片ID */
#define NM25Q128    0X5217          /* NM25Q128 芯片ID */

extern uint16_t norflash_TYPE;      /* 定义FLASH芯片型号 */
 
/* 指令表 */
#define FLASH_WriteEnable           0x06   		// 写使能
#define FLASH_WriteDisable          0x04 
#define FLASH_ReadStatusReg1        0x05 		// 读状态寄存器1
#define FLASH_ReadStatusReg2        0x35 
#define FLASH_ReadStatusReg3        0x15 
#define FLASH_WriteStatusReg1       0x01   // 读状态寄存器1的第0位（只读），Busy标志位，当正在擦除/写入数据/写命令时会被置1
#define FLASH_WriteStatusReg2       0x31 
#define FLASH_WriteStatusReg3       0x11 
#define FLASH_ReadData              0x03 
#define FLASH_FastReadData          0x0B 
#define FLASH_FastReadDual          0x3B 
#define FLASH_FastReadQuad          0xEB    // 1-4-4模式下(1线指令4线地址4线数据)，快速读取指令
#define FLASH_PageProgram           0x02 	// 读状态寄存器1的第1位（只读），WEL写使能标志位，该标志位为1时，代表可以进行写操作
#define FLASH_PageProgramQuad       0x32    // 1-1-4模式下(1线指令1线地址4线数据)，页编程指令，参考写入时间 0.4ms 

#define FLASH_BlockErase            0xD8 		// 块擦除，  64K字节，参考擦除时间 150ms
#define FLASH_SectorErase           0x20 		// 扇区擦除，4K字节， 参考擦除时间 45ms
#define FLASH_ChipErase             0xC7 		// 整片擦除，参考擦除时间 20S

#define FLASH_PowerDown             0xB9 
#define FLASH_ReleasePowerDown      0xAB 
#define FLASH_DeviceID              0xAB 
#define FLASH_ManufactDeviceID      0x90 
#define FLASH_JedecDeviceID         0x9F 		// JEDEC ID
#define FLASH_Enable4ByteAddr       0xB7
#define FLASH_Exit4ByteAddr         0xE9
#define FLASH_SetReadParam          0xC0 
#define FLASH_EnterQPIMode          0x38
#define FLASH_ExitQPIMode           0xFF

/* 静态函数 */
static void norflash_wait_busy(void);       /* 等待空闲 */
static void norflash_qe_enable(void);       /* 使能QE位 */
static void norflash_ospi_disable(void);    /* 退出QPI模式 */
static void norflash_write_page(uint8_t *pbuf, uint32_t addr, uint16_t datalen);    /* 写入page */
static void norflash_write_nocheck(uint8_t *pbuf, uint32_t addr, uint16_t datalen); /* 写flash,不带擦除 */

/* 普通函数 */
void norflash_init(void);                   /* 初始化25QXX */
uint32_t norflash_read_id(void);            /* 读取FLASH ID */
void norflash_write_enable(void);           /* 写使能 */
void norflash_write_disable(void);          /* 写保护 */
uint8_t norflash_read_sr(uint8_t regno);    /* 读取状态寄存器 */
void norflash_write_sr(uint8_t regno,uint8_t sr);   /* 写状态寄存器 */

void norflash_erase_chip(void);             /* 整片擦除 */
void norflash_erase_sector(uint32_t saddr); /* 扇区擦除 */
void norflash_read(uint8_t *pbuf, uint32_t addr, uint16_t datalen);     /* 读取flash */
void norflash_write(uint8_t *pbuf, uint32_t addr, uint16_t datalen);    /* 写入flash */

#endif















