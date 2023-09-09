/**
 ****************************************************************************************************
 * @file        delay.h
 * @author      
 * @version     V2.0
 * @date        2023-09-01
 * @brief       ʹ��SysTick����ͨ����ģʽ���ӳٽ��й���(֧��ucosii)
 *              �ṩdelay_init��ʼ�������� delay_us��delay_ms����ʱ����
 * @license     
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:���ͿƼ� STM32H7BOVBT6������  FK7B0M2-VBT6
 *
 * �޸�˵��
 * V2.0 20230901
 ****************************************************************************************************
 */
 
#ifndef __DELAY_H
#define __DELAY_H

#include "./SYSTEM/sys/sys.h"


void delay_init(uint16_t sysclk); /* ��ʼ���ӳٺ��� */
void delay_ms(uint16_t nms);      /* ��ʱnms */
void delay_us(uint32_t nus);      /* ��ʱnus */

#endif

