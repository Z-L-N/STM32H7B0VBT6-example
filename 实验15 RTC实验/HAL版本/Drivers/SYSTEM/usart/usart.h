/**
 ****************************************************************************************************
 * @file        usart.c
 * @author      
 * @version     V2.0
 * @date        2023-09-01
 * @brief       串口初始化代码(一般是串口1)，支持printf
 * @license     
 ****************************************************************************************************
 * @attention
 *
 * 修改说明
 * V2.0 20230901
 *
 ****************************************************************************************************
 */

#ifndef _USART_H
#define _USART_H

#include "stdio.h"
#include "./SYSTEM/sys/sys.h"

/*******************************************************************************************************/
/* 引脚 和 串口 定义 
 * 默认是针对USART1的.
 * 注意: 通过修改这12个宏定义,可以支持USART1~UART7任意一个串口.
 */

#define USART1_TX_GPIO_PORT              GPIOA
#define USART1_TX_GPIO_PIN               GPIO_PIN_9
#define USART1_TX_GPIO_AF                GPIO_AF7_USART1
#define USART1_TX_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* 发送引脚时钟使能 */

#define USART1_RX_GPIO_PORT              GPIOA
#define USART1_RX_GPIO_PIN               GPIO_PIN_10
#define USART1_RX_GPIO_AF                GPIO_AF7_USART1
#define USART1_RX_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* 接收引脚时钟使能 */

#define USART1_UX                        USART1
#define USART1_UX_IRQn                   USART1_IRQn
#define USART1_UX_IRQHandler             USART1_IRQHandler
#define USART1_UX_CLK_ENABLE()           do{ __HAL_RCC_USART1_CLK_ENABLE(); }while(0)  /* USART1 时钟使能 */

/*******************************************************************************************************/

#define USART1_REC_LEN   200                     /* 定义最大接收字节数 200 */
#define USART1_EN_RX     1                       /* 使能（1）/禁止（0）串口1接收 */
#define RXBUFFERSIZE    1                       /* 缓存大小 */

extern UART_HandleTypeDef g_uart1_handle;       /* UART句柄 */

extern uint8_t  g_usart1_rx_buf[USART1_REC_LEN];  /* 接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 */
extern uint16_t g_usart1_rx_sta;                 /* 接收状态标记 */
extern uint8_t g_rx1_buffer[RXBUFFERSIZE];       /* HAL库USART接收Buffer */


void USART1_Init(uint32_t baudrate);             /* 串口初始化函数 */

#endif







