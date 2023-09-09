/**
 ****************************************************************************************************
 * @file        delay.h
 * @author      
 * @version     V2.0
 * @date        2023-09-01
 * @brief       使用SysTick的普通计数模式对延迟进行管理(支持ucosii)
 *              提供delay_init初始化函数， delay_us和delay_ms等延时函数
 * @license     
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:反客科技 STM32H7BOVBT6开发板  FK7B0M2-VBT6
 *
 * 修改说明
 * V2.0 20230901
 ****************************************************************************************************
 */
 
#ifndef __DELAY_H
#define __DELAY_H

#include "./SYSTEM/sys/sys.h"


void delay_init(uint16_t sysclk); /* 初始化延迟函数 */
void delay_ms(uint16_t nms);      /* 延时nms */
void delay_us(uint32_t nus);      /* 延时nus */

#endif

