#ifndef __LED_H
#define __LED_H

/*------------------------------------------ LED���ú� ----------------------------------*/

#define LED0_PIN            			 GPIO_PIN_13        				 	// LED1 ����      
#define LED0_PORT           			 GPIOC                 			 	// LED1 GPIO�˿�     
#define __HAL_RCC_LED0_CLK_ENABLE    __HAL_RCC_GPIOC_CLK_ENABLE() 	// LED1 GPIO�˿�ʱ��
 
/*----------------------------------------- LED���ƺ� ----------------------------------*/
						
#define LED0_ON 	  	HAL_GPIO_WritePin(LED0_PORT, LED0_PIN, GPIO_PIN_RESET)		// ����͵�ƽ������LED1	
#define LED0_OFF 	  	HAL_GPIO_WritePin(LED0_PORT, LED0_PIN, GPIO_PIN_SET)			// ����ߵ�ƽ���ر�LED1	
#define LED0_Toggle	  HAL_GPIO_TogglePin(LED0_PORT,LED0_PIN);								    // ��תIO��״̬
			
/*---------------------------------------- �������� ------------------------------------*/

void LED_Init(void);

#endif //__LED_H


