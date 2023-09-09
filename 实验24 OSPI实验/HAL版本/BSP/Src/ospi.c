#include "ospi.h"

OSPI_HandleTypeDef g_ospi_handle;    /* OSPI��� */

/**
 * @brief       �ȴ�״̬��־
 * @param       flag : ��Ҫ�ȴ��ı�־λ
 * @param       sta  : ��Ҫ�ȴ���״̬
 * @param       wtime: �ȴ�ʱ��
 * @retval      0, �ȴ��ɹ�; 1, �ȴ�ʧ��.
 */
uint8_t ospi_wait_flag(uint32_t flag, uint8_t sta, uint32_t wtime)
{
    uint8_t flagsta = 0;

    while (wtime)
    {
        flagsta = (OCTOSPI1->SR & flag) ? 1 : 0; /* ��ȡ״̬��־ */

        if (flagsta == sta)break;

        wtime--;
    }

    if (wtime)return 0;
    else return 1;
}

/**
 * @brief       ��ʼ��OSPI�ӿ�
 * @param       ��
 * @retval      0, �ɹ�; 1, ʧ��.
 */
uint8_t ospi_init(void)
{
	OSPIM_CfgTypeDef sOspiManagerCfg = {0};
	uint32_t i;
	char *p;
	/*��ĳЩ���ϣ��������������㷨ʱ����Ҫ�ֶ��������Ȳ���������Ṥ��������*/	
/* �˴��ο��������Ĵ��룬��ҿ���ȥѧϰ�������Ľ̳̣��ǳ�רҵ */
	p = (char *)&g_ospi_handle;
	for (i = 0; i < sizeof(g_ospi_handle); i++)
	{
		*p++ = 0;
	}
	HAL_OSPI_DeInit(&g_ospi_handle);           // ��λOSPI
	
    g_ospi_handle.Instance = OCTOSPI1;                                  /* OSPI */
    g_ospi_handle.Init.ClockPrescaler = 2;                             /* ʱ�ӷ�Ƶֵ����OSPI�ں�ʱ�ӽ��� 2 ��Ƶ�õ�OSPIͨ������ʱ��
                                                                          ���Դ˴�Ӧ��Ϊ2��QSPIƵ�ʾ�Ϊ220/(1+1)=110MHZ
                                                                          ��΢�е㳬Ƶ�����������ͺã����о�ֻ�ܽ���Ƶ�� */
    g_ospi_handle.Init.FifoThreshold = 8;                              /* FIFO��ֵΪ4���ֽ� */
    g_ospi_handle.Init.SampleShifting = HAL_OSPI_SAMPLE_SHIFTING_HALFCYCLE;;/* ������λ�������(DDRģʽ��,��������Ϊ0) */
   // g_ospi_handle.Init.FlashSize = 25-1;                              /* SPI FLASH��С��BY25Q128��СΪ32M�ֽ�,2^25������ȡȨֵ25-1=24 */
    g_ospi_handle.Init.ChipSelectHighTime = 1; 							/* Ƭѡ�ߵ�ƽʱ��Ϊ1��ʱ��(*1=27.3ns),���ֲ������tSHSL���� */
    g_ospi_handle.Init.ClockMode = HAL_OSPI_CLOCK_MODE_3;               /* ģʽ3 */
   // g_ospi_handle.Init.FlashID = QSPI_FLASH_ID_1;                     /* ��һƬflash */
   // g_ospi_handle.Init.DualFlash = HAL_OSPI_DUALQUAD_DISABLE;         /* ��ֹ˫����ģʽ */
    g_ospi_handle.Init.DualQuad 		= HAL_OSPI_DUALQUAD_DISABLE;   // ��ֹ˫OSPIģʽ
    g_ospi_handle.Init.MemoryType 		= HAL_OSPI_MEMTYPE_MICRON;     // �洢��ģʽ��ֻ��8��ģʽ�Ż��õ�
	g_ospi_handle.Init.DeviceSize 			    = 23;					       	// flash��С�����İ������8M�ֽڵ�W25Q64����������Ϊ23����2^23					         
	g_ospi_handle.Init.FreeRunningClock 		= HAL_OSPI_FREERUNCLK_DISABLE;  // ��ֹ����ʱ��ģʽ
	g_ospi_handle.Init.WrapSize				 	= HAL_OSPI_WRAP_NOT_SUPPORTED;  // ��ʹ�� wrap-size
	g_ospi_handle.Init.DelayHoldQuarterCycle 	= HAL_OSPI_DHQC_DISABLE;        // ��ʹ�����ݱ��ֹ���
	g_ospi_handle.Init.ChipSelectBoundary 		= 0;							// ��ֹƬѡ�߽繦��
	g_ospi_handle.Init.ClkChipSelectHighTime	= 1;                            // ͨ�Ž����� 0 ��CLK����CS����Ϊ��
	g_ospi_handle.Init.DelayBlockBypass 		= HAL_OSPI_DELAY_BLOCK_BYPASSED; // ��ʱģ����·
	g_ospi_handle.Init.MaxTran           = 0;                             // ��ֹͨ�Ź�����
	g_ospi_handle.Init.Refresh           = 0;                             // ��ֹˢ�¹���
	
	if(HAL_OSPI_Init(&g_ospi_handle) == HAL_OK) 
    {
        return 0;      /* QSPI��ʼ���ɹ� */
    }
    else
    {
        return 1;
    }
	
	sOspiManagerCfg.ClkPort = 1;     // OSPI���ŷ�����������ã�ʹ�� Port1 �� CLK
	sOspiManagerCfg.NCSPort = 1;     // OSPI���ŷ�����������ã�ʹ�� Port1 �� NCS
	sOspiManagerCfg.IOLowPort = HAL_OSPIM_IOPORT_1_LOW;   // OSPI���ŷ�����������ã�ʹ�� Port1 �ĵ�4λ���ţ�IO[3:0]
	
	HAL_OSPIM_Config(&g_ospi_handle, &sOspiManagerCfg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE);  // ��ʼ��OSPI���ŷ������������
	
}

/**
 * @brief       QSPI�ײ�����,�������ã�ʱ��ʹ��
 * @param       hqspi:QSPI���
 * @note        �˺����ᱻHAL_QSPI_Init()����
 * @retval      0, �ɹ�; 1, ʧ��.
 */
void HAL_OSPI_MspInit(OSPI_HandleTypeDef *hqspi)
{
    GPIO_InitTypeDef gpio_init_struct;
	
    __HAL_RCC_OCTOSPIM_CLK_ENABLE();    // ʹ��OSPI���ŷ��������ʱ��
    __HAL_RCC_OSPI1_CLK_ENABLE();      /* ʹ��QSPIʱ�� */
	
	__HAL_RCC_OSPI1_FORCE_RESET();		// ��λ
	__HAL_RCC_OSPI1_RELEASE_RESET();	
	
    __HAL_RCC_GPIOB_CLK_ENABLE();     /* GPIOBʱ��ʹ�� */
    __HAL_RCC_GPIOD_CLK_ENABLE();     /* GPIODʱ��ʹ�� */
    __HAL_RCC_GPIOE_CLK_ENABLE();     /* GPIOEʱ��ʹ�� */
/****���Ŷ���********************************************
      PB2     ------> OCTOSPIM_P1_CLK
      PB6     ------> OCTOSPIM_P1_NCS      
      PD11    ------> OCTOSPIM_P1_IO0
      PD12    ------> OCTOSPIM_P1_IO1
      PE2     ------> OCTOSPIM_P1_IO2     
      PD13    ------> OCTOSPIM_P1_IO3
*******************************************************/
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                     /* ���� */
    gpio_init_struct.Pull = GPIO_NOPULL;                         /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;          /* ���� */
	 
	gpio_init_struct.Pin = OSPI_BK1_NCS_GPIO_PIN;
    gpio_init_struct.Alternate = OSPI_BK1_NCS_GPIO_AF;              /* ����ΪQSPI */
    HAL_GPIO_Init(OSPI_BK1_NCS_GPIO_PORT, &gpio_init_struct);    /* ��ʼ��QSPI_BK1_NCS���� */

    gpio_init_struct.Pin = OSPI_BK1_CLK_GPIO_PIN;
    gpio_init_struct.Alternate = OSPI_BK1_CLK_GPIO_AF;               /* ����ΪQSPI */
    HAL_GPIO_Init(OSPI_BK1_CLK_GPIO_PORT, &gpio_init_struct);    /* ��ʼ��QSPI_BK1_CLK���� */
    
    gpio_init_struct.Pin = OSPI_BK1_IO0_GPIO_PIN;
	gpio_init_struct.Alternate = OSPI_BK1_IO0_GPIO_AF;               /* ����ΪQSPI */
    HAL_GPIO_Init(OSPI_BK1_IO0_GPIO_PORT, &gpio_init_struct);    /* ��ʼ��QSPI_BK1_IO0���� */
    
    gpio_init_struct.Pin = OSPI_BK1_IO1_GPIO_PIN;
	gpio_init_struct.Alternate = OSPI_BK1_IO1_GPIO_AF;               /* ����ΪQSPI */
    HAL_GPIO_Init(OSPI_BK1_IO1_GPIO_PORT, &gpio_init_struct);    /* ��ʼ��QSPI_BK1_IO1���� */
    
    gpio_init_struct.Pin = OSPI_BK1_IO2_GPIO_PIN;
	gpio_init_struct.Alternate = OSPI_BK1_IO2_GPIO_AF;               /* ����ΪQSPI */
    HAL_GPIO_Init(OSPI_BK1_IO2_GPIO_PORT, &gpio_init_struct);    /* ��ʼ��QSPI_BK1_IO2���� */
    
    gpio_init_struct.Pin = OSPI_BK1_IO3_GPIO_PIN;
	gpio_init_struct.Alternate = OSPI_BK1_IO3_GPIO_AF;               /* ����ΪQSPI */
    HAL_GPIO_Init(OSPI_BK1_IO3_GPIO_PORT, &gpio_init_struct);    /* ��ʼ��QSPI_BK1_IO3���� */
}

/**
 * @brief       OSPI��������
 * @param       cmd : Ҫ���͵�ָ��
 * @param       addr: ���͵���Ŀ�ĵ�ַ
 * @param       mode: ģʽ,��ϸλ��������:
 *   @arg       mode[1:0]: ָ��ģʽ; 00,��ָ��;  01,���ߴ���ָ��; 10,˫�ߴ���ָ��; 11,���ߴ���ָ��.
 *   @arg       mode[3:2]: ��ַģʽ; 00,�޵�ַ;  01,���ߴ����ַ; 10,˫�ߴ����ַ; 11,���ߴ����ַ.
 *   @arg       mode[5:4]: ��ַ����; 00,8λ��ַ; 01,16λ��ַ;     10,24λ��ַ;     11,32λ��ַ.
 *   @arg       mode[7:6]: ����ģʽ; 00,������;  01,���ߴ�������; 10,˫�ߴ�������; 11,���ߴ�������.
 * @param       dmcycle: ��ָ��������
 * @retval      ��
 */
void ospi_send_cmd(uint8_t cmd, uint32_t addr, uint8_t mode, uint8_t dmcycle)
{
    OSPI_RegularCmdTypeDef ospi_command_handle;
    ospi_command_handle.OperationType		= HAL_OSPI_OPTYPE_COMMON_CFG;    // ͨ������
    ospi_command_handle.Instruction			= cmd;                           /* ָ�� */
    ospi_command_handle.Address 			= addr;                          /* ��ַ */
    ospi_command_handle.DummyCycles 		= dmcycle;                       /* ���ÿ�ָ�������� */
	ospi_command_handle.FlashId     		= HAL_OSPI_FLASH_ID_1;           // flash ID
	
    if(((mode >> 0) & 0x03) == 0)
    ospi_command_handle.InstructionMode = HAL_OSPI_INSTRUCTION_NONE;        /* ָ��ģʽ */
    else if(((mode >> 0) & 0x03) == 1)
    ospi_command_handle.InstructionMode = HAL_OSPI_INSTRUCTION_1_LINE;      /* ָ��ģʽ */
    else if(((mode >> 0) & 0x03) == 2)
    ospi_command_handle.InstructionMode = HAL_OSPI_INSTRUCTION_2_LINES;     /* ָ��ģʽ */
    else if(((mode >> 0) & 0x03) == 3)
    ospi_command_handle.InstructionMode = HAL_OSPI_INSTRUCTION_4_LINES;     /* ָ��ģʽ */

    if(((mode >> 2) & 0x03) == 0)
    ospi_command_handle.AddressMode = HAL_OSPI_ADDRESS_NONE;                /* ��ַģʽ */
    else if(((mode >> 2) & 0x03) == 1)
    ospi_command_handle.AddressMode = HAL_OSPI_ADDRESS_1_LINE;              /* ��ַģʽ */
    else if(((mode >> 2) & 0x03) == 2)
    ospi_command_handle.AddressMode = HAL_OSPI_ADDRESS_2_LINES;             /* ��ַģʽ */
    else if(((mode >> 2) & 0x03) == 3)
    ospi_command_handle.AddressMode = HAL_OSPI_ADDRESS_4_LINES;             /* ��ַģʽ */

    if(((mode >> 4)&0x03) == 0)
    ospi_command_handle.AddressSize = HAL_OSPI_ADDRESS_8_BITS;              /* ��ַ���� */
    else if(((mode >> 4) & 0x03) == 1)
    ospi_command_handle.AddressSize = HAL_OSPI_ADDRESS_16_BITS;             /* ��ַ���� */
    else if(((mode >> 4) & 0x03) == 2)
    ospi_command_handle.AddressSize = HAL_OSPI_ADDRESS_24_BITS;             /* ��ַ���� */
    else if(((mode >> 4) & 0x03) == 3)
    ospi_command_handle.AddressSize = HAL_OSPI_ADDRESS_32_BITS;             /* ��ַ���� */

    if(((mode >> 6) & 0x03) == 0)
    ospi_command_handle.DataMode=HAL_OSPI_DATA_NONE;                        /* ����ģʽ */
    else if(((mode >> 6) & 0x03) == 1)
    ospi_command_handle.DataMode = HAL_OSPI_DATA_1_LINE;                    /* ����ģʽ */
    else if(((mode >> 6) & 0x03) == 2)
    ospi_command_handle.DataMode = HAL_OSPI_DATA_2_LINES;                   /* ����ģʽ */
    else if(((mode >> 6) & 0x03) == 3)
    ospi_command_handle.DataMode = HAL_OSPI_DATA_4_LINES;                   /* ����ģʽ */

    ospi_command_handle.SIOOMode = HAL_OSPI_SIOO_INST_EVERY_CMD;                /* ÿ�ζ�����ָ�� */
    ospi_command_handle.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;     /* �޽����ֽ� */
    ospi_command_handle.DataDtrMode = HAL_OSPI_DATA_DTR_DISABLE;                /* ��ֹ����DTRģʽ */
    ospi_command_handle.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;  // ��ָֹ��DTRģʽ

    HAL_OSPI_Command(&g_ospi_handle, &ospi_command_handle, HAL_OSPI_TIMEOUT_DEFAULT_VALUE);  // ����ָ��
	
}

/**
 * @brief       OSPI����ָ�����ȵ�����
 * @param       buf     : �������ݻ������׵�ַ
 * @param       datalen : Ҫ��������ݳ���
 * @retval      0, �ɹ�; ����, �������.
 */
uint8_t ospi_receive(uint8_t *buf, uint32_t datalen)
{
    g_ospi_handle.Instance->DLR = datalen - 1;   /* �������ݳ��� */
    if (HAL_OSPI_Receive(&g_ospi_handle, buf, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) == HAL_OK) 
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/**
 * @brief       QSPI����ָ�����ȵ�����
 * @param       buf     : �������ݻ������׵�ַ
 * @param       datalen : Ҫ��������ݳ���
 * @retval      0, �ɹ�; ����, �������.
 */
uint8_t ospi_transmit(uint8_t *buf, uint32_t datalen)
{
    g_ospi_handle.Instance->DLR = datalen - 1; /* �������ݳ��� */
    if (HAL_OSPI_Transmit(&g_ospi_handle, buf, 5000) == HAL_OK)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
