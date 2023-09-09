/**
 ****************************************************************************************************
 * @file        norflash.h
 * @author      
 * @version     V2.0
 * @date        2023-09-05
 * @brief       NOR FLASH(25QXX) ��������
 * @license     
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32H750������
 *
 * �޸�˵��
 * V2.0 20230905
 * 
 *
 ****************************************************************************************************
 */

#ifndef __norflash_H
#define __norflash_H

#include "./SYSTEM/sys/sys.h"


/* FLASHоƬ�б� */
#define W25Q80      0XEF13          /* W25Q80   оƬID */
#define W25Q16      0XEF14          /* W25Q16   оƬID */
#define W25Q32      0XEF15          /* W25Q32   оƬID */
#define W25Q64      0XEF16          /* W25Q64   оƬID */
#define W25Q128     0XEF17          /* W25Q128  оƬID */
#define W25Q256     0XEF18          /* W25Q256  оƬID */
#define BY25Q64     0X6816          /* BY25Q64  оƬID */
#define BY25Q128    0X6817          /* BY25Q128 оƬID */
#define NM25Q64     0X5216          /* NM25Q64  оƬID */
#define NM25Q128    0X5217          /* NM25Q128 оƬID */

extern uint16_t norflash_TYPE;      /* ����FLASHоƬ�ͺ� */
 
/* ָ��� */
#define FLASH_WriteEnable           0x06   		// дʹ��
#define FLASH_WriteDisable          0x04 
#define FLASH_ReadStatusReg1        0x05 		// ��״̬�Ĵ���1
#define FLASH_ReadStatusReg2        0x35 
#define FLASH_ReadStatusReg3        0x15 
#define FLASH_WriteStatusReg1       0x01   // ��״̬�Ĵ���1�ĵ�0λ��ֻ������Busy��־λ�������ڲ���/д������/д����ʱ�ᱻ��1
#define FLASH_WriteStatusReg2       0x31 
#define FLASH_WriteStatusReg3       0x11 
#define FLASH_ReadData              0x03 
#define FLASH_FastReadData          0x0B 
#define FLASH_FastReadDual          0x3B 
#define FLASH_FastReadQuad          0xEB    // 1-4-4ģʽ��(1��ָ��4�ߵ�ַ4������)�����ٶ�ȡָ��
#define FLASH_PageProgram           0x02 	// ��״̬�Ĵ���1�ĵ�1λ��ֻ������WELдʹ�ܱ�־λ���ñ�־λΪ1ʱ��������Խ���д����
#define FLASH_PageProgramQuad       0x32    // 1-1-4ģʽ��(1��ָ��1�ߵ�ַ4������)��ҳ���ָ��ο�д��ʱ�� 0.4ms 

#define FLASH_BlockErase            0xD8 		// �������  64K�ֽڣ��ο�����ʱ�� 150ms
#define FLASH_SectorErase           0x20 		// ����������4K�ֽڣ� �ο�����ʱ�� 45ms
#define FLASH_ChipErase             0xC7 		// ��Ƭ�������ο�����ʱ�� 20S

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

/* ��̬���� */
static void norflash_wait_busy(void);       /* �ȴ����� */
static void norflash_qe_enable(void);       /* ʹ��QEλ */
static void norflash_ospi_disable(void);    /* �˳�QPIģʽ */
static void norflash_write_page(uint8_t *pbuf, uint32_t addr, uint16_t datalen);    /* д��page */
static void norflash_write_nocheck(uint8_t *pbuf, uint32_t addr, uint16_t datalen); /* дflash,�������� */

/* ��ͨ���� */
void norflash_init(void);                   /* ��ʼ��25QXX */
uint32_t norflash_read_id(void);            /* ��ȡFLASH ID */
void norflash_write_enable(void);           /* дʹ�� */
void norflash_write_disable(void);          /* д���� */
uint8_t norflash_read_sr(uint8_t regno);    /* ��ȡ״̬�Ĵ��� */
void norflash_write_sr(uint8_t regno,uint8_t sr);   /* д״̬�Ĵ��� */

void norflash_erase_chip(void);             /* ��Ƭ���� */
void norflash_erase_sector(uint32_t saddr); /* �������� */
void norflash_read(uint8_t *pbuf, uint32_t addr, uint16_t datalen);     /* ��ȡflash */
void norflash_write(uint8_t *pbuf, uint32_t addr, uint16_t datalen);    /* д��flash */

#endif















