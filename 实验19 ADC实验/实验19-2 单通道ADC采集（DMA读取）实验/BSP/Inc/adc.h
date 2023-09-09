/**
 ****************************************************************************************************
 * @file        adc.h
 * @author      
 * @version     V2.0
 * @date        2023-09-03
 * @brief       ADC 驱动代码
 * @license     
 ****************************************************************************************************
 * @attention
 *
 * 实验平台: 
 * 修改说明
 * V2.0 20230903
 *
 ****************************************************************************************************
 */

#ifndef __ADC_H
#define __ADC_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* ADC及引脚 定义 */

#define ADC_ADCX_CHY_GPIO_PORT              GPIOA
#define ADC_ADCX_CHY_GPIO_PIN               GPIO_PIN_5
#define ADC_ADCX_CHY_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define ADC_ADCX                            ADC1 
#define ADC_ADCX_CHY                        ADC_CHANNEL_19                                /* 通道Y,  0 <= Y <= 19 */ 
#define ADC_ADCX_CHY_CLK_ENABLE()           do{ __HAL_RCC_ADC12_CLK_ENABLE(); }while(0)   /* ADC1 & ADC2 时钟使能 */

/******************************************************************************************/

void adc_init(void);                                           /* ADC初始化函数 */
uint32_t adc_get_result(uint32_t ch);                          /* 获得某个通道值  */
uint32_t adc_get_result_average(uint32_t ch, uint8_t times);   /* 得到某个通道给定次数采样的平均值 */


#endif 














