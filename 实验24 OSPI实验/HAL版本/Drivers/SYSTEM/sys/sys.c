/**
 * 修改说明
 * V1.0 20200312
 * 第一次发布
 * V1.0 20200401
 * 1, 将头文件包含路径改成相对路径,避免重复设置包含路径的麻烦
 * 2, 将函数改成带一个参数,用于支持W25Q256, 实现了支持所有25Q系列芯片
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"

/**
 * @brief       设置中断向量表偏移地址
 * @param       baseaddr: 基址
 * @param       offset: 偏移量
 * @retval      无
 */
void sys_nvic_set_vector_table(uint32_t baseaddr, uint32_t offset)
{
    /* 设置NVIC的向量表偏移寄存器,VTOR低9位保留,即[8:0]保留 */
    SCB->VTOR = baseaddr | (offset & (uint32_t)0xFFFFFE00);
}

/**
 * @brief       执行: WFI指令(执行完该指令进入低功耗状态, 等待中断唤醒)
 * @param       无
 * @retval      无
 */
void sys_wfi_set(void)
{
    __ASM volatile("wfi");
}

/**
 * @brief       关闭所有中断(但是不包括fault和NMI中断)
 * @param       无
 * @retval      无
 */
void sys_intx_disable(void)
{
    __ASM volatile("cpsid i");
}

/**
 * @brief       开启所有中断
 * @param       无
 * @retval      无
 */
void sys_intx_enable(void)
{
    __ASM volatile("cpsie i");
}

/**
 * @brief       设置栈顶地址
 * @note        左侧的红X, 属于MDK误报, 实际是没问题的
 * @param       addr: 栈顶地址
 * @retval      无
 */
void sys_msr_msp(uint32_t addr)
{
    __set_MSP(addr);    /* 设置栈顶地址 */
}

/**
 * @brief       使能STM32H7的L1-Cache, 同时开启D cache的强制透写
 * @param       无
 * @retval      无
 */
void sys_cache_enable(void)
{
    SCB_EnableICache();     /* 使能I-Cache,函数在core_cm7.h里面定义 */
    SCB_EnableDCache();     /* 使能D-Cache,函数在core_cm7.h里面定义 */
    SCB->CACR |= 1 << 2;    /* 强制D-Cache透写,如不开启透写,实际使用中可能遇到各种问题 */
}

/**正点原子的注释
 * @brief       时钟设置函数
 * @param       plln: PLL1倍频系数(PLL倍频), 取值范围: 4~512.
 * @param       pllm: PLL1预分频系数(进PLL之前的分频), 取值范围: 2~63.
 * @param       pllp: PLL1的p分频系数(PLL之后的分频), 分频后作为系统时钟, 取值范围: 2~128.(且必须是2的倍数)
 * @param       pllq: PLL1的q分频系数(PLL之后的分频), 取值范围: 1~128.
 * @note
 *
 *              Fvco: VCO频率
 *              Fsys: 系统时钟频率, 也是PLL1的p分频输出时钟频率
 *              Fq:   PLL1的q分频输出时钟频率
 *              Fs:   PLL输入时钟频率, 可以是HSI, CSI, HSE等.
 *              Fvco = Fs * (plln / pllm);
 *              Fsys = Fvco / pllp = Fs * (plln / (pllm * pllp));
 *              Fq   = Fvco / pllq = Fs * (plln / (pllm * pllq));
 *
 *              外部晶振为25M的时候, 推荐值: plln = 112, pllm = 5, pllp = 2, pllq = 4.
 *              外部晶振为 8M的时候, 推荐值: plln = 240, pllm = 2, pllp = 2, pllq = 4.
 *              得到:Fvco = 8 * (240 / 2) = 960Mhz
 *                   Fsys = pll1_p_ck = 960 / 2 = 480Mhz
 *                   Fq   = pll1_q_ck = 960 / 4 = 240Mhz
 *
 *              H750默认需要配置的频率如下:
 *              CPU频率(rcc_c_ck) = sys_d1cpre_ck = 480Mhz
 *              rcc_aclk = rcc_hclk3 = 240Mhz
 *              AHB1/2/3/4(rcc_hclk1/2/3/4) = 240Mhz
 *              APB1/2/3/4(rcc_pclk1/2/3/4) = 120Mhz
 *              pll2_p_ck = (8 / 8) * 440 / 2) = 220Mhz
 *              pll2_r_ck = FMC时钟频率 = ((8 / 8) * 440 / 2) = 220Mhz
 *
 * @retval      错误代码: 0, 成功; 1, 错误;
 */
 /***反客科技的注释*****************************************************************************************/
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 280000000 (CPU Clock)
  *            HCLK(Hz)                       = 280000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 1 (AHB  Clock  280MHz)
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  140MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  140MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  140MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  140MHz)
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 5
  *            PLL_N                          = 112
  *            PLL_P                          = 2
  *            PLL_Q                          = 2
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
uint8_t sys_stm32_clock_init(uint32_t plln, uint32_t pllm, uint32_t pllp, uint32_t pllq)
{
    HAL_StatusTypeDef ret = HAL_OK;
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct= {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);                         /* 使能供电配置更新 */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);  /* VOS = 1, Scale1, 1.2V内核电压,FLASH访问可以得到最高性能 */
    while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY));     								/* 等待电压稳定 */
		__HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
    /* 使能HSE，并选择HSE作为PLL时钟源，配置PLL1，开启USB时钟 */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI48;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;						/* 打开 HSE */
    RCC_OscInitStruct.HSIState = RCC_HSI_OFF;						/* 关闭 HSI */
    RCC_OscInitStruct.CSIState = RCC_CSI_OFF;						/* 关闭 CSI */
    RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;				/* 打开 HSI48 */
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;				/* 打开 PLL */
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;/* 设置 PLL 时钟源为 HSE */
    RCC_OscInitStruct.PLL.PLLN = plln;
    RCC_OscInitStruct.PLL.PLLM = pllm;
    RCC_OscInitStruct.PLL.PLLP = pllp;
    RCC_OscInitStruct.PLL.PLLQ = pllq;
		RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    ret=HAL_RCC_OscConfig(&RCC_OscInitStruct);
    if(ret != HAL_OK)
    {
        return 1;
    }

    /*
     *  选择PLL的输出作为系统时钟
     *  配置RCC_CLOCKTYPE_SYSCLK系统时钟,280M
     *  配置RCC_CLOCKTYPE_HCLK 时钟,280Mhz,对应AHB1，AHB2，AHB3和AHB4总线
     *  配置RCC_CLOCKTYPE_PCLK1时钟,140Mhz,对应APB1总线
     *  配置RCC_CLOCKTYPE_PCLK2时钟,140Mhz,对应APB2总线
     *  配置RCC_CLOCKTYPE_D1PCLK1时钟,140Mhz,对应APB3总线
     *  配置RCC_CLOCKTYPE_D3PCLK1时钟,140Mhz,对应APB4总线
     */

    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK \
                                    | RCC_CLOCKTYPE_HCLK \
                                    | RCC_CLOCKTYPE_PCLK1 \
                                    | RCC_CLOCKTYPE_PCLK2 \
                                    | RCC_CLOCKTYPE_D1PCLK1 \
                                    | RCC_CLOCKTYPE_D3PCLK1);

    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2; 
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2; 
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;  
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2; 
    ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
    if(ret != HAL_OK)
    {
        return 1;
    }

    /*
     *  配置PLL2的R分频输出, 为220Mhz 
     *  配置FMC时钟源是PLL2R
     *  配置QSPI时钟源是PLL2R 改为系统主时钟HCLK 作为OSPI的内核时钟，速度为280M，再经过2分频得到140M驱动时钟
     *  配置串口1 和 串口6 的时钟源来自: PCLK2 = 120Mhz
     *  配置串口2 / 3 / 4 / 5 / 7 / 8 的时钟源来自: PCLK1 = 120Mhz
     *  USB工作需要48MHz的时钟,可以由PLL1Q,PLL3Q和HSI48提供,这里配置时钟源是HSI48
     */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_USART1
                                               | RCC_PERIPHCLK_USB  | RCC_PERIPHCLK_OSPI
											   | RCC_PERIPHCLK_FMC;
    PeriphClkInitStruct.PLL2.PLL2M = 25;
    PeriphClkInitStruct.PLL2.PLL2N = 266;
    PeriphClkInitStruct.PLL2.PLL2P = 2;
    PeriphClkInitStruct.PLL2.PLL2Q = 2;
    PeriphClkInitStruct.PLL2.PLL2R = 1;
    PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_0;
    PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOMEDIUM;
    PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
		
    PeriphClkInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_PLL2;
    PeriphClkInitStruct.OspiClockSelection = RCC_OSPICLKSOURCE_D1HCLK;
    PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
    PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
    PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
    ret = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
    if(ret != HAL_OK)
    {
        return 1;
    }

    sys_ospi_enable_memmapmode(0);          /* 使能QSPI内存映射模式, FLASH容量为普通类型 */

    HAL_PWREx_EnableUSBVoltageDetector();   /* 使能USB电压电平检测器 */
    __HAL_RCC_CSI_ENABLE() ;                /* 使能CSI时钟 */
    __HAL_RCC_SYSCFG_CLK_ENABLE() ;         /* 使能SYSCFG时钟 */
    HAL_EnableCompensationCell();           /* 使能IO补偿单元 */
    return 0;
}


#ifdef  USE_FULL_ASSERT

/**
 * @brief       当编译提示出错的时候此函数用来报告错误的文件和所在行
 * @param       file：指向源文件
 *              line：指向在文件中的行数
 * @retval      无
 */
void assert_failed(uint8_t* file, uint32_t line)
{ 
    while (1)
    {
    }
}
#endif

/**
 * @brief       OSPI进入内存映射模式（执行OSPI代码必备前提）
 *   @note      必须根据所使用OSPI FLASH的容量设置正确的ftype值!
 * @param       ftype: flash类型
 *   @arg           0, 普通FLASH, 容量在128Mbit及以内的
 *   @arg           1, 大容量FLASH, 容量在256Mbit及以上的.
 * @retval      无
 */
void sys_ospi_enable_memmapmode(uint8_t ftype)
{
    uint32_t tempreg = 0; 
    GPIO_InitTypeDef ospi_gpio;
	OSPI_HandleTypeDef hospi1;
	OSPIM_CfgTypeDef sOspiManagerCfg = {0};
	OSPI_RegularCmdTypeDef     sCommand;         // QSPI传输配置
    OSPI_MemoryMappedTypeDef   sMemMappedCfg;    // 内存映射访问参数
	
	uint32_t i;
	char *p;
	
	__HAL_RCC_OCTOSPIM_CLK_ENABLE();    // 使能OSPI引脚分配管理器时钟
    __HAL_RCC_OSPI1_CLK_ENABLE();       /* QSPI时钟使能 */
	
	__HAL_RCC_OSPI1_FORCE_RESET();		// 复位
	__HAL_RCC_OSPI1_RELEASE_RESET();	
	
	__HAL_RCC_GPIOB_CLK_ENABLE();                            /* 使能PORTB时钟 */
    __HAL_RCC_GPIOD_CLK_ENABLE();                            /* 使能PORTD时钟 */
    __HAL_RCC_GPIOE_CLK_ENABLE();                            /* 使能PORTE时钟 */
	
	ospi_gpio.Pin = GPIO_PIN_6;                              /* PB6 AF10 */
    ospi_gpio.Mode = GPIO_MODE_AF_PP;
    ospi_gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    ospi_gpio.Pull = GPIO_PULLUP;
    ospi_gpio.Alternate = GPIO_AF10_OCTOSPIM_P1;
    HAL_GPIO_Init(GPIOB, &ospi_gpio);

    ospi_gpio.Pin = GPIO_PIN_2;                              /* PB2 AF9 */
    ospi_gpio.Alternate = GPIO_AF9_OCTOSPIM_P1;
    HAL_GPIO_Init(GPIOB, &ospi_gpio);

    ospi_gpio.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13; /* PD11,12,13 AF9 */
    ospi_gpio.Alternate = GPIO_AF9_OCTOSPIM_P1;
    HAL_GPIO_Init(GPIOD, &ospi_gpio);

    ospi_gpio.Pin = GPIO_PIN_2;                              /* PE2 AF9 */
    ospi_gpio.Alternate = GPIO_AF9_OCTOSPIM_P1;
    HAL_GPIO_Init(GPIOE, &ospi_gpio);

	/*在某些场合，例如用作下载算法时，需要手动清零句柄等参数，否则会工作不正常*/	
/* 此处参考安富莱的代码，大家可以去学习安富莱的教程，非常专业 */
	p = (char *)&hospi1;
	for (i = 0; i < sizeof(hospi1); i++)
	{
		*p++ = 0;
	}	
	
	HAL_OSPI_DeInit(&hospi1);           // 复位OSPI

	hospi1.Instance 							= OCTOSPI1;								// OSPI外设

	hospi1.Init.ClockPrescaler 			= 2;                                      // 时钟分频值，将OSPI内核时钟进行 2 分频得到OSPI通信驱动时钟
	hospi1.Init.FifoThreshold 				= 8;                                     	// FIFO阈值
	hospi1.Init.DualQuad 					= HAL_OSPI_DUALQUAD_DISABLE;              // 禁止双OSPI模式
	hospi1.Init.MemoryType 					= HAL_OSPI_MEMTYPE_MICRON;                // 存储器模式，只有8线模式才会用到
	hospi1.Init.DeviceSize 					= 23;					                        // flash大小，核心板采用是8M字节的W25Q64，这里设置为23，即2^23					         
	hospi1.Init.ChipSelectHighTime 		= 1;                                      // 片选保持高电平的时间
	hospi1.Init.FreeRunningClock 			= HAL_OSPI_FREERUNCLK_DISABLE;            // 禁止自由时钟模式
	hospi1.Init.ClockMode 					= HAL_OSPI_CLOCK_MODE_3;                  // 模式3
	hospi1.Init.WrapSize						= HAL_OSPI_WRAP_NOT_SUPPORTED;            // 不使用 wrap-size
	hospi1.Init.SampleShifting 			= HAL_OSPI_SAMPLE_SHIFTING_HALFCYCLE;     // 半个CLK周期之后进行采样
	hospi1.Init.DelayHoldQuarterCycle	= HAL_OSPI_DHQC_DISABLE;                  // 不使用数据保持功能
	hospi1.Init.ChipSelectBoundary 		= 0;											      // 禁止片选边界功能
	hospi1.Init.ClkChipSelectHighTime	= 0;                                      // 通信结束后 0 个CLK周期CS设置为高
	hospi1.Init.DelayBlockBypass 			= HAL_OSPI_DELAY_BLOCK_BYPASSED;          // 延时模块旁路
   hospi1.Init.MaxTran                 = 0;                                      // 禁止通信管理功能
   hospi1.Init.Refresh                 = 0;                                      // 禁止刷新功能

	HAL_OSPI_Init(&hospi1);    // 初始化 OSPI1 设置
	
	sOspiManagerCfg.ClkPort = 1;     // OSPI引脚分配管理器设置，使用 Port1 的 CLK
	sOspiManagerCfg.NCSPort = 1;     // OSPI引脚分配管理器设置，使用 Port1 的 NCS
	sOspiManagerCfg.IOLowPort = HAL_OSPIM_IOPORT_1_LOW;   // OSPI引脚分配管理器设置，使用 Port1 的低4位引脚，IO[3:0]

	HAL_OSPIM_Config(&hospi1, &sOspiManagerCfg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE);  // 初始化OSPI引脚分配管理器设置

	sCommand.OperationType           = HAL_OSPI_OPTYPE_COMMON_CFG;             // 通用配置
   sCommand.FlashId                 = HAL_OSPI_FLASH_ID_1;                    // flash ID

   sCommand.Instruction             = 0xEB;             // 1-4-4模式下(1线指令4线地址4线数据)，快速读取指令
   sCommand.InstructionMode         = HAL_OSPI_INSTRUCTION_1_LINE;            // 1线指令模式
   sCommand.InstructionSize         = HAL_OSPI_INSTRUCTION_8_BITS;            // 指令长度8位
   sCommand.InstructionDtrMode      = HAL_OSPI_INSTRUCTION_DTR_DISABLE;       // 禁止指令DTR模式

   sCommand.AddressMode             = HAL_OSPI_ADDRESS_4_LINES;               // 4线地址模式
   sCommand.AddressSize             = HAL_OSPI_ADDRESS_24_BITS;               // 地址长度24位
   sCommand.AddressDtrMode          = HAL_OSPI_ADDRESS_DTR_DISABLE;           // 禁止地址DTR模式

   sCommand.AlternateBytesMode      = HAL_OSPI_ALTERNATE_BYTES_NONE;          // 无交替字节    
   sCommand.AlternateBytesDtrMode   = HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;   // 禁止替字节DTR模式 

   sCommand.DataMode                = HAL_OSPI_DATA_4_LINES;                  // 4线数据模式
   sCommand.DataDtrMode             = HAL_OSPI_DATA_DTR_DISABLE;              // 禁止数据DTR模式 

   sCommand.DummyCycles             = 6;                                      // 空周期个数
   sCommand.DQSMode                 = HAL_OSPI_DQS_DISABLE;                   // 不使用DQS 
   sCommand.SIOOMode                = HAL_OSPI_SIOO_INST_EVERY_CMD;           // 每次传输数据都发送指令   

	HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE);

	sMemMappedCfg.TimeOutActivation  = HAL_OSPI_TIMEOUT_COUNTER_DISABLE;          // 禁用超时计数器, nCS 保持激活状态
	sMemMappedCfg.TimeOutPeriod      = 0;                                         // 超时判断周期
   // 开启内存映射模式
	HAL_OSPI_MemoryMapped(&hospi1,  &sMemMappedCfg);	// 进行配置
	
//	/* QSPI设置，参考QSPI实验的QSPI_Init函数 */
//	RCC->AHB3RSTR |= 1 << 14;       /* 复位QSPI */
//    RCC->AHB3RSTR &= ~(1 << 14);    /* 停止复位QSPI */
//	
//	while (OCTOSPI1->SR & (1 << 5)); /* 等待BUSY位清零 */
//	
//	/* QSPI时钟源已经在sys_stm32_clock_init()函数中设置 */
//    OCTOSPI1->CR = 0X01000310;       /* 设置CR寄存器, 这些值怎么来的，请参考QSPI实验/看H750参考手册寄存器描述分析 */
//    OCTOSPI1->DCR1 = 0X00180201;      /* 设置DCR寄存器(FLASH容量32M(最大容量设置为32M, 默认用16M的), tSHSL = 3个时钟) */
//    OCTOSPI1->CR |= 1 << 0;          /* 使能QSPI */
}

//寄存器版本
void sys_ospi_enable_memmapmode1(uint8_t ftype)
{
    uint32_t tempreg = 0; 
    GPIO_InitTypeDef ospi_gpio;

	uint32_t i;
	char *p;
	
	__HAL_RCC_OCTOSPIM_CLK_ENABLE();    // 使能OSPI引脚分配管理器时钟
    __HAL_RCC_OSPI1_CLK_ENABLE();       /* QSPI时钟使能 */
	
	__HAL_RCC_OSPI1_FORCE_RESET();		// 复位
	__HAL_RCC_OSPI1_RELEASE_RESET();	
	
	__HAL_RCC_GPIOB_CLK_ENABLE();                            /* 使能PORTB时钟 */
    __HAL_RCC_GPIOD_CLK_ENABLE();                            /* 使能PORTD时钟 */
    __HAL_RCC_GPIOE_CLK_ENABLE();                            /* 使能PORTE时钟 */
	
	ospi_gpio.Pin = GPIO_PIN_6;                              /* PB6 AF10 */
    ospi_gpio.Mode = GPIO_MODE_AF_PP;
    ospi_gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    ospi_gpio.Pull = GPIO_PULLUP;
    ospi_gpio.Alternate = GPIO_AF10_OCTOSPIM_P1;
    HAL_GPIO_Init(GPIOB, &ospi_gpio);

    ospi_gpio.Pin = GPIO_PIN_2;                              /* PB2 AF9 */
    ospi_gpio.Alternate = GPIO_AF9_OCTOSPIM_P1;
    HAL_GPIO_Init(GPIOB, &ospi_gpio);

    ospi_gpio.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13; /* PD11,12,13 AF9 */
    ospi_gpio.Alternate = GPIO_AF9_OCTOSPIM_P1;
    HAL_GPIO_Init(GPIOD, &ospi_gpio);

    ospi_gpio.Pin = GPIO_PIN_2;                              /* PE2 AF9 */
    ospi_gpio.Alternate = GPIO_AF9_OCTOSPIM_P1;
    HAL_GPIO_Init(GPIOE, &ospi_gpio);
	
//	/* QSPI设置，参考QSPI实验的QSPI_Init函数 */
	RCC->AHB3RSTR |= 1 << 14;       /* 复位QSPI */
    RCC->AHB3RSTR &= ~(1 << 14);    /* 停止复位QSPI */
//	
	while (OCTOSPI1->SR & (1 << 5)); /* 等待BUSY位清零 */
	
//	
//	/* QSPI时钟源已经在sys_stm32_clock_init()函数中设置 */
//    OCTOSPI1->CR = 0X01000310;       /* 设置CR寄存器, 这些值怎么来的，请参考QSPI实验/看H750参考手册寄存器描述分析 */
//    OCTOSPI1->DCR1 = 0X00180201;      /* 设置DCR寄存器(FLASH容量32M(最大容量设置为32M, 默认用16M的), tSHSL = 3个时钟) */
//    OCTOSPI1->CR |= 1 << 0;          /* 使能QSPI */
}


