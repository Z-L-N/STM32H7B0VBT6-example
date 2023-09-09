/**
 ****************************************************************************************************
 * @file        norflash_ex.c
 * @author      
 * @version     V2.0
 * @date        2023-09-06
 * @brief       OSPI ��������
 *              ��ΪSTM32H7��֧��OSPI�ӿڶ�ʱд,��˸ô�������ʵ��QSPI FLASH������д��,ԭ���ǣ�
 *              qspi.c��norflash.c��norflash_ex.c��3���ִ���ȫ���洢��H7���ڲ�FLASH,��֤����
 *              QSPI FLASH��ʱ��,����Ҫ����QSPI FLASH�Ĵ���. �Ӷ�ʵ��QSPI FLASH����д��.
 *
 * @license     
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:
 *
 * �޸�˵��
 * V2.0 2023-09-06
 *
 ****************************************************************************************************
 */
 
#ifndef __NORFLASH_EX_H
#define __NORFLASH_EX_H

#include "./SYSTEM/sys/sys.h"


void norflash_ex_erase_chip(void);              /* NOR FLASH ȫƬ���� */
uint32_t norflash_ex_read_id(void);             /* NOR FLASH��ȡID */
void norflash_ex_erase_sector(uint32_t addr);   /* NOR FLASH �������� */
uint8_t norflash_ex_write(uint8_t *pbuf, uint32_t addr, uint16_t datalen);  /* NOR FLASHд������ */
void norflash_ex_read(uint8_t *pbuf, uint32_t addr, uint16_t datalen);      /* NOR FLASH��ȡ���� */

#endif















