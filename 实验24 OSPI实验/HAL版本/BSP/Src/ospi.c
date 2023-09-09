#include "ospi.h"

OSPI_HandleTypeDef g_ospi_handle;    /* OSPI句柄 */

/**
 * @brief       等待状态标志
 * @param       flag : 需要等待的标志位
 * @param       sta  : 需要等待的状态
 * @param       wtime: 等待时间
 * @retval      0, 等待成功; 1, 等待失败.
 */
uint8_t ospi_wait_flag(uint32_t flag, uint8_t sta, uint32_t wtime)
{
    uint8_t flagsta = 0;

    while (wtime)
    {
        flagsta = (OCTOSPI1->SR & flag) ? 1 : 0; /* 获取状态标志 */

        if (flagsta == sta)break;

        wtime--;
    }

    if (wtime)return 0;
    else return 1;
}

/**
 * @brief       初始化OSPI接口
 * @param       无
 * @retval      0, 成功; 1, 失败.
 */
uint8_t ospi_init(void)
{
	OSPIM_CfgTypeDef sOspiManagerCfg = {0};
	uint32_t i;
	char *p;
	/*在某些场合，例如用作下载算法时，需要手动清零句柄等参数，否则会工作不正常*/	
/* 此处参考安富莱的代码，大家可以去学习安富莱的教程，非常专业 */
	p = (char *)&g_ospi_handle;
	for (i = 0; i < sizeof(g_ospi_handle); i++)
	{
		*p++ = 0;
	}
	HAL_OSPI_DeInit(&g_ospi_handle);           // 复位OSPI
	
    g_ospi_handle.Instance = OCTOSPI1;                                  /* OSPI */
    g_ospi_handle.Init.ClockPrescaler = 2;                             /* 时钟分频值，将OSPI内核时钟进行 2 分频得到OSPI通信驱动时钟
                                                                          所以此处应该为2，QSPI频率就为220/(1+1)=110MHZ
                                                                          稍微有点超频，可以正常就好，不行就只能降低频率 */
    g_ospi_handle.Init.FifoThreshold = 8;                              /* FIFO阈值为4个字节 */
    g_ospi_handle.Init.SampleShifting = HAL_OSPI_SAMPLE_SHIFTING_HALFCYCLE;;/* 采样移位半个周期(DDR模式下,必须设置为0) */
   // g_ospi_handle.Init.FlashSize = 25-1;                              /* SPI FLASH大小，BY25Q128大小为32M字节,2^25，所以取权值25-1=24 */
    g_ospi_handle.Init.ChipSelectHighTime = 1; 							/* 片选高电平时间为1个时钟(*1=27.3ns),即手册里面的tSHSL参数 */
    g_ospi_handle.Init.ClockMode = HAL_OSPI_CLOCK_MODE_3;               /* 模式3 */
   // g_ospi_handle.Init.FlashID = QSPI_FLASH_ID_1;                     /* 第一片flash */
   // g_ospi_handle.Init.DualFlash = HAL_OSPI_DUALQUAD_DISABLE;         /* 禁止双闪存模式 */
    g_ospi_handle.Init.DualQuad 		= HAL_OSPI_DUALQUAD_DISABLE;   // 禁止双OSPI模式
    g_ospi_handle.Init.MemoryType 		= HAL_OSPI_MEMTYPE_MICRON;     // 存储器模式，只有8线模式才会用到
	g_ospi_handle.Init.DeviceSize 			    = 23;					       	// flash大小，核心板采用是8M字节的W25Q64，这里设置为23，即2^23					         
	g_ospi_handle.Init.FreeRunningClock 		= HAL_OSPI_FREERUNCLK_DISABLE;  // 禁止自由时钟模式
	g_ospi_handle.Init.WrapSize				 	= HAL_OSPI_WRAP_NOT_SUPPORTED;  // 不使用 wrap-size
	g_ospi_handle.Init.DelayHoldQuarterCycle 	= HAL_OSPI_DHQC_DISABLE;        // 不使用数据保持功能
	g_ospi_handle.Init.ChipSelectBoundary 		= 0;							// 禁止片选边界功能
	g_ospi_handle.Init.ClkChipSelectHighTime	= 1;                            // 通信结束后 0 个CLK周期CS设置为高
	g_ospi_handle.Init.DelayBlockBypass 		= HAL_OSPI_DELAY_BLOCK_BYPASSED; // 延时模块旁路
	g_ospi_handle.Init.MaxTran           = 0;                             // 禁止通信管理功能
	g_ospi_handle.Init.Refresh           = 0;                             // 禁止刷新功能
	
	if(HAL_OSPI_Init(&g_ospi_handle) == HAL_OK) 
    {
        return 0;      /* QSPI初始化成功 */
    }
    else
    {
        return 1;
    }
	
	sOspiManagerCfg.ClkPort = 1;     // OSPI引脚分配管理器设置，使用 Port1 的 CLK
	sOspiManagerCfg.NCSPort = 1;     // OSPI引脚分配管理器设置，使用 Port1 的 NCS
	sOspiManagerCfg.IOLowPort = HAL_OSPIM_IOPORT_1_LOW;   // OSPI引脚分配管理器设置，使用 Port1 的低4位引脚，IO[3:0]
	
	HAL_OSPIM_Config(&g_ospi_handle, &sOspiManagerCfg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE);  // 初始化OSPI引脚分配管理器设置
	
}

/**
 * @brief       QSPI底层驱动,引脚配置，时钟使能
 * @param       hqspi:QSPI句柄
 * @note        此函数会被HAL_QSPI_Init()调用
 * @retval      0, 成功; 1, 失败.
 */
void HAL_OSPI_MspInit(OSPI_HandleTypeDef *hqspi)
{
    GPIO_InitTypeDef gpio_init_struct;
	
    __HAL_RCC_OCTOSPIM_CLK_ENABLE();    // 使能OSPI引脚分配管理器时钟
    __HAL_RCC_OSPI1_CLK_ENABLE();      /* 使能QSPI时钟 */
	
	__HAL_RCC_OSPI1_FORCE_RESET();		// 复位
	__HAL_RCC_OSPI1_RELEASE_RESET();	
	
    __HAL_RCC_GPIOB_CLK_ENABLE();     /* GPIOB时钟使能 */
    __HAL_RCC_GPIOD_CLK_ENABLE();     /* GPIOD时钟使能 */
    __HAL_RCC_GPIOE_CLK_ENABLE();     /* GPIOE时钟使能 */
/****引脚定义********************************************
      PB2     ------> OCTOSPIM_P1_CLK
      PB6     ------> OCTOSPIM_P1_NCS      
      PD11    ------> OCTOSPIM_P1_IO0
      PD12    ------> OCTOSPIM_P1_IO1
      PE2     ------> OCTOSPIM_P1_IO2     
      PD13    ------> OCTOSPIM_P1_IO3
*******************************************************/
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                     /* 复用 */
    gpio_init_struct.Pull = GPIO_NOPULL;                         /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;          /* 高速 */
	 
	gpio_init_struct.Pin = OSPI_BK1_NCS_GPIO_PIN;
    gpio_init_struct.Alternate = OSPI_BK1_NCS_GPIO_AF;              /* 复用为QSPI */
    HAL_GPIO_Init(OSPI_BK1_NCS_GPIO_PORT, &gpio_init_struct);    /* 初始化QSPI_BK1_NCS引脚 */

    gpio_init_struct.Pin = OSPI_BK1_CLK_GPIO_PIN;
    gpio_init_struct.Alternate = OSPI_BK1_CLK_GPIO_AF;               /* 复用为QSPI */
    HAL_GPIO_Init(OSPI_BK1_CLK_GPIO_PORT, &gpio_init_struct);    /* 初始化QSPI_BK1_CLK引脚 */
    
    gpio_init_struct.Pin = OSPI_BK1_IO0_GPIO_PIN;
	gpio_init_struct.Alternate = OSPI_BK1_IO0_GPIO_AF;               /* 复用为QSPI */
    HAL_GPIO_Init(OSPI_BK1_IO0_GPIO_PORT, &gpio_init_struct);    /* 初始化QSPI_BK1_IO0引脚 */
    
    gpio_init_struct.Pin = OSPI_BK1_IO1_GPIO_PIN;
	gpio_init_struct.Alternate = OSPI_BK1_IO1_GPIO_AF;               /* 复用为QSPI */
    HAL_GPIO_Init(OSPI_BK1_IO1_GPIO_PORT, &gpio_init_struct);    /* 初始化QSPI_BK1_IO1引脚 */
    
    gpio_init_struct.Pin = OSPI_BK1_IO2_GPIO_PIN;
	gpio_init_struct.Alternate = OSPI_BK1_IO2_GPIO_AF;               /* 复用为QSPI */
    HAL_GPIO_Init(OSPI_BK1_IO2_GPIO_PORT, &gpio_init_struct);    /* 初始化QSPI_BK1_IO2引脚 */
    
    gpio_init_struct.Pin = OSPI_BK1_IO3_GPIO_PIN;
	gpio_init_struct.Alternate = OSPI_BK1_IO3_GPIO_AF;               /* 复用为QSPI */
    HAL_GPIO_Init(OSPI_BK1_IO3_GPIO_PORT, &gpio_init_struct);    /* 初始化QSPI_BK1_IO3引脚 */
}

/**
 * @brief       OSPI发送命令
 * @param       cmd : 要发送的指令
 * @param       addr: 发送到的目的地址
 * @param       mode: 模式,详细位定义如下:
 *   @arg       mode[1:0]: 指令模式; 00,无指令;  01,单线传输指令; 10,双线传输指令; 11,四线传输指令.
 *   @arg       mode[3:2]: 地址模式; 00,无地址;  01,单线传输地址; 10,双线传输地址; 11,四线传输地址.
 *   @arg       mode[5:4]: 地址长度; 00,8位地址; 01,16位地址;     10,24位地址;     11,32位地址.
 *   @arg       mode[7:6]: 数据模式; 00,无数据;  01,单线传输数据; 10,双线传输数据; 11,四线传输数据.
 * @param       dmcycle: 空指令周期数
 * @retval      无
 */
void ospi_send_cmd(uint8_t cmd, uint32_t addr, uint8_t mode, uint8_t dmcycle)
{
    OSPI_RegularCmdTypeDef ospi_command_handle;
    ospi_command_handle.OperationType		= HAL_OSPI_OPTYPE_COMMON_CFG;    // 通用配置
    ospi_command_handle.Instruction			= cmd;                           /* 指令 */
    ospi_command_handle.Address 			= addr;                          /* 地址 */
    ospi_command_handle.DummyCycles 		= dmcycle;                       /* 设置空指令周期数 */
	ospi_command_handle.FlashId     		= HAL_OSPI_FLASH_ID_1;           // flash ID
	
    if(((mode >> 0) & 0x03) == 0)
    ospi_command_handle.InstructionMode = HAL_OSPI_INSTRUCTION_NONE;        /* 指令模式 */
    else if(((mode >> 0) & 0x03) == 1)
    ospi_command_handle.InstructionMode = HAL_OSPI_INSTRUCTION_1_LINE;      /* 指令模式 */
    else if(((mode >> 0) & 0x03) == 2)
    ospi_command_handle.InstructionMode = HAL_OSPI_INSTRUCTION_2_LINES;     /* 指令模式 */
    else if(((mode >> 0) & 0x03) == 3)
    ospi_command_handle.InstructionMode = HAL_OSPI_INSTRUCTION_4_LINES;     /* 指令模式 */

    if(((mode >> 2) & 0x03) == 0)
    ospi_command_handle.AddressMode = HAL_OSPI_ADDRESS_NONE;                /* 地址模式 */
    else if(((mode >> 2) & 0x03) == 1)
    ospi_command_handle.AddressMode = HAL_OSPI_ADDRESS_1_LINE;              /* 地址模式 */
    else if(((mode >> 2) & 0x03) == 2)
    ospi_command_handle.AddressMode = HAL_OSPI_ADDRESS_2_LINES;             /* 地址模式 */
    else if(((mode >> 2) & 0x03) == 3)
    ospi_command_handle.AddressMode = HAL_OSPI_ADDRESS_4_LINES;             /* 地址模式 */

    if(((mode >> 4)&0x03) == 0)
    ospi_command_handle.AddressSize = HAL_OSPI_ADDRESS_8_BITS;              /* 地址长度 */
    else if(((mode >> 4) & 0x03) == 1)
    ospi_command_handle.AddressSize = HAL_OSPI_ADDRESS_16_BITS;             /* 地址长度 */
    else if(((mode >> 4) & 0x03) == 2)
    ospi_command_handle.AddressSize = HAL_OSPI_ADDRESS_24_BITS;             /* 地址长度 */
    else if(((mode >> 4) & 0x03) == 3)
    ospi_command_handle.AddressSize = HAL_OSPI_ADDRESS_32_BITS;             /* 地址长度 */

    if(((mode >> 6) & 0x03) == 0)
    ospi_command_handle.DataMode=HAL_OSPI_DATA_NONE;                        /* 数据模式 */
    else if(((mode >> 6) & 0x03) == 1)
    ospi_command_handle.DataMode = HAL_OSPI_DATA_1_LINE;                    /* 数据模式 */
    else if(((mode >> 6) & 0x03) == 2)
    ospi_command_handle.DataMode = HAL_OSPI_DATA_2_LINES;                   /* 数据模式 */
    else if(((mode >> 6) & 0x03) == 3)
    ospi_command_handle.DataMode = HAL_OSPI_DATA_4_LINES;                   /* 数据模式 */

    ospi_command_handle.SIOOMode = HAL_OSPI_SIOO_INST_EVERY_CMD;                /* 每次都发送指令 */
    ospi_command_handle.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;     /* 无交替字节 */
    ospi_command_handle.DataDtrMode = HAL_OSPI_DATA_DTR_DISABLE;                /* 禁止数据DTR模式 */
    ospi_command_handle.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;  // 禁止指令DTR模式

    HAL_OSPI_Command(&g_ospi_handle, &ospi_command_handle, HAL_OSPI_TIMEOUT_DEFAULT_VALUE);  // 发送指令
	
}

/**
 * @brief       OSPI接收指定长度的数据
 * @param       buf     : 接收数据缓冲区首地址
 * @param       datalen : 要传输的数据长度
 * @retval      0, 成功; 其他, 错误代码.
 */
uint8_t ospi_receive(uint8_t *buf, uint32_t datalen)
{
    g_ospi_handle.Instance->DLR = datalen - 1;   /* 配置数据长度 */
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
 * @brief       QSPI发送指定长度的数据
 * @param       buf     : 发送数据缓冲区首地址
 * @param       datalen : 要传输的数据长度
 * @retval      0, 成功; 其他, 错误代码.
 */
uint8_t ospi_transmit(uint8_t *buf, uint32_t datalen)
{
    g_ospi_handle.Instance->DLR = datalen - 1; /* 配置数据长度 */
    if (HAL_OSPI_Transmit(&g_ospi_handle, buf, 5000) == HAL_OK)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
