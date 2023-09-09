#ifndef __OSPI_H__
#define __OSPI_H__
#include "./SYSTEM/sys/sys.h"

/******************************************************************************************/
/* OSPI ��� ���� ���� */

#define OSPI_BK1_CLK_GPIO_PORT          GPIOB
#define OSPI_BK1_CLK_GPIO_PIN           GPIO_PIN_2
#define OSPI_BK1_CLK_GPIO_AF            GPIO_AF9_OCTOSPIM_P1
#define OSPI_BK1_CLK_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOB_CLK_ENABLE; }while(0)   /* PB��ʱ��ʹ�� */

#define OSPI_BK1_NCS_GPIO_PORT          GPIOB
#define OSPI_BK1_NCS_GPIO_PIN           GPIO_PIN_6
#define OSPI_BK1_NCS_GPIO_AF            GPIO_AF10_OCTOSPIM_P1
#define OSPI_BK1_NCS_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOB_CLK_ENABLE; }while(0)   /* PB��ʱ��ʹ�� */

#define OSPI_BK1_IO0_GPIO_PORT          GPIOD
#define OSPI_BK1_IO0_GPIO_PIN           GPIO_PIN_11
#define OSPI_BK1_IO0_GPIO_AF            GPIO_AF9_OCTOSPIM_P1
#define OSPI_BK1_IO0_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOD_CLK_ENABLE; }while(0)   /* PD��ʱ��ʹ�� */

#define OSPI_BK1_IO1_GPIO_PORT          GPIOD
#define OSPI_BK1_IO1_GPIO_PIN           GPIO_PIN_12
#define OSPI_BK1_IO1_GPIO_AF            GPIO_AF9_OCTOSPIM_P1
#define OSPI_BK1_IO1_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOD_CLK_ENABLE; }while(0)   /* PD��ʱ��ʹ�� */

#define OSPI_BK1_IO2_GPIO_PORT          GPIOE
#define OSPI_BK1_IO2_GPIO_PIN           GPIO_PIN_2
#define OSPI_BK1_IO2_GPIO_AF            GPIO_AF9_OCTOSPIM_P1
#define OSPI_BK1_IO2_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOE_CLK_ENABLE; }while(0)   /* PD��ʱ��ʹ�� */

#define OSPI_BK1_IO3_GPIO_PORT          GPIOD
#define OSPI_BK1_IO3_GPIO_PIN           GPIO_PIN_13
#define OSPI_BK1_IO3_GPIO_AF            GPIO_AF9_OCTOSPIM_P1
#define OSPI_BK1_IO3_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOD_CLK_ENABLE; }while(0)   /* PE��ʱ��ʹ�� */

/******************************************************************************************/

uint8_t ospi_wait_flag(uint32_t flag, uint8_t sta, uint32_t wtime); /* OSPI�ȴ�ĳ��״̬ */
uint8_t ospi_init(void);    /* ��ʼ��OSPI */
void ospi_send_cmd(uint8_t cmd, uint32_t addr, uint8_t mode, uint8_t dmcycle);  /* OSPI�������� */
uint8_t ospi_receive(uint8_t *buf, uint32_t datalen);   /* OSPI�������� */
uint8_t ospi_transmit(uint8_t *buf, uint32_t datalen);  /* OSPI�������� */

#endif