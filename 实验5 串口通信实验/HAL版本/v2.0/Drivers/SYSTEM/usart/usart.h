/**
 ****************************************************************************************************
 * @file        usart.c
 * @author      
 * @version     V3.0
 * @date        2023-09-03
 * @brief       ���ڳ�ʼ������(һ���Ǵ���1)��֧��printf
 * @license     
 ****************************************************************************************************
 * @attention
 *
 * �޸�˵��
 * V3.0 20230901
 *
 ****************************************************************************************************
 */

#ifndef _USART_H
#define _USART_H

#include "stdio.h"
#include "./SYSTEM/sys/sys.h"

/*******************************************************************************************************/
/* ���� �� ���� ���� 
 * Ĭ�������USART1��.
 * ע��: ͨ���޸���12���궨��,����֧��USART1~UART7����һ������.
 */
/*************************************USART1 ���ú궨��**************************************************************/
#define USART1_TX_GPIO_PORT              GPIOA
#define USART1_TX_GPIO_PIN               GPIO_PIN_9
#define USART1_TX_GPIO_AF                GPIO_AF7_USART1
#define USART1_TX_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* ��������ʱ��ʹ�� */

#define USART1_RX_GPIO_PORT              GPIOA
#define USART1_RX_GPIO_PIN               GPIO_PIN_10
#define USART1_RX_GPIO_AF                GPIO_AF7_USART1
#define USART1_RX_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* ��������ʱ��ʹ�� */

#define USART1_UX                        USART1
#define USART1_UX_IRQn                   USART1_IRQn
#define USART1_UX_IRQHandler             USART1_IRQHandler
#define USART1_UX_CLK_ENABLE()           do{ __HAL_RCC_USART1_CLK_ENABLE(); }while(0)  /* USART1 ʱ��ʹ�� */

/*******************************************************************************************************/
/*************************************USART2 ���ú궨��**************************************************************/
#define USART2_GPIO_PORT                 GPIOA
#define USART2_TX_GPIO_PIN               GPIO_PIN_2
#define USART2_TX_GPIO_AF                GPIO_AF7_USART2
#define USART2_TX_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* ��������ʱ��ʹ�� */

#define USART2_RX_GPIO_PIN               GPIO_PIN_3
#define USART2_RX_GPIO_AF                GPIO_AF7_USART2
#define USART2_RX_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* ��������ʱ��ʹ�� */

#define USART2_UX                        USART2
#define USART2_UX_IRQn                   USART2_IRQn
#define USART2_UX_IRQHandler             USART2_IRQHandler
#define USART2_UX_CLK_ENABLE()           do{ __HAL_RCC_USART2_CLK_ENABLE(); }while(0)  /* USART2 ʱ��ʹ�� */

/*******************************************************************************************************/
/*************************************USART3 ���ú궨��**************************************************************/
#define USART3_GPIO_PORT                 GPIOB
#define USART3_TX_GPIO_PIN               GPIO_PIN_10
#define USART3_TX_GPIO_AF                GPIO_AF7_USART3
#define USART3_TX_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* ��������ʱ��ʹ�� */

#define USART3_RX_GPIO_PIN               GPIO_PIN_11
#define USART3_RX_GPIO_AF                GPIO_AF7_USART3
#define USART3_RX_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* ��������ʱ��ʹ�� */

#define USART3_UX                        USART3
#define USART3_UX_IRQn                   USART3_IRQn
#define USART3_UX_IRQHandler             USART3_IRQHandler
#define USART3_UX_CLK_ENABLE()           do{ __HAL_RCC_USART3_CLK_ENABLE(); }while(0)  /* USART2 ʱ��ʹ�� */

/*******************************************************************************************************/


#define USART1_REC_LEN   200                     /* �����������ֽ��� 200 */
#define USART1_EN_RX     1                       /* ʹ�ܣ�1��/��ֹ��0������1���� */
#define RXBUFFERSIZE    1                       /* �����С */

extern UART_HandleTypeDef g_uart1_handle;       /* UART��� */

extern uint8_t  g_usart1_rx_buf[USART1_REC_LEN];  /* ���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� */
extern uint16_t g_usart1_rx_sta;                 /* ����״̬��� */
extern uint8_t g_rx1_buffer[RXBUFFERSIZE];       /* HAL��USART����Buffer */


void USART1_Init(uint32_t baudrate);             /* ���ڳ�ʼ������ */

#endif







