/**
 ****************************************************************************************************
 * @file        mpu.c
 * @author      
 * @version     V2.0
 * @date        2023-09-01
 * @brief       MPU内存保护 驱动代码
 * @license    
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:反客STM32H7B0VBT6核心板 （型号：FK7B0M2-VBT6）
 *
 * 修改说明
 * V2.0 20230901
 * 第一次发布
 *
 ****************************************************************************************************
 */

#ifndef __MPU_H
#define __MPU_H

#include "./SYSTEM/sys/sys.h"


uint8_t mpu_set_protection(uint32_t baseaddr, uint32_t size, uint32_t rnum, uint8_t de, uint8_t ap, uint8_t sen, uint8_t cen, uint8_t ben);
void mpu_memory_protection(void);


#endif
