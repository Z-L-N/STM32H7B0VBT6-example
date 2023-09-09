/**
 ****************************************************************************************************
 * @file        malloc.c
 * @author      
 * @version     V2.0
 * @date        2023-09-05
 * @brief       内存管理 驱动
 * @license     
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:
 * 修改说明
 * V1.0 20230905
 *
 ****************************************************************************************************
 */


#ifndef __MALLOC_H
#define __MALLOC_H

#include "./SYSTEM/sys/sys.h"

/* 定义五个内存池 */
#define AXI_SRAM123  0       /* AXI SRAM1/2/3内存池,SRAM1=256Kb, SRAM2=384Kb,SRAM3=384Kb,共1024Kb*/
#define AHB_SRAM12   1		 /* AHB SRAM1/2内存池,SRAM1=64Kb, SRAM2=64Kb共128Kb*/
#define SDR_SRAM     2       /* SDR SRAM内存池,共32Kb */
#define SRAM_DTCM    3       /* DTCM内存池,DTCM共128Kb,此部分内存仅CPU和MDMA(通过AHBS)可以访问!!!! */
#define SRAM_ITCM    4       /* ITCM内存池,DTCM共64Kb,此部分内存仅CPU和MDMA(通过AHBS)可以访问!!!! */

#define SRAMBANK    5       /* 定义支持的SRAM块数. */

/* 定义内存管理表类型,当外扩SDRAM的时候，必须使用uint32_t类型，否则可以定义成uint16_t，以节省内存占用 */
#define MT_TYPE     uint16_t


/* 单块内存，内存管理所占用的全部空间大小计算公式如下：
 * size=MEM1_MAX_SIZE+(MEM1_MAX_SIZE/MEM1_BLOCK_SIZE)*sizeof(MT_TYPE)
 * 以SRAMIN为例，size=474*1024+(474*1024/64)*2=500544≈489KB

 * 已知总内存容量(size)，最大内存池的计算公式如下：
 * MEM1_MAX_SIZE=(MEM1_BLOCK_SIZE*size)/(MEM1_BLOCK_SIZE+sizeof(MT_TYPE))
 * 以SRAM12为例,MEM1_MAX_SIZE=(64*256)/(64+2)=248.24KB≈248KB
 */
 
/* mem1内存参数设定.mem1是H7内部的AXI内存. */
#define MEM1_BLOCK_SIZE         64                              /* 内存块大小为64字节 */
#define MEM1_MAX_SIZE           512*1024                        /* 最大管理内存 512K,H7的AXI内存总共1024Kb */
#define MEM1_ALLOC_TABLE_SIZE   MEM1_MAX_SIZE/MEM1_BLOCK_SIZE   /* 内存表大小 */

/* mem2内存参数设定.mem2是H7内部的AHB内存 */
#define MEM2_BLOCK_SIZE         64                              /* 内存块大小为64字节 */
#define MEM2_MAX_SIZE           124 *1024                       /* 最大管理内存124K,H7的AHB SRAM1/2共128KB */
#define MEM2_ALLOC_TABLE_SIZE   MEM2_MAX_SIZE/MEM2_BLOCK_SIZE   /* 内存表大小 */

/* mem3内存参数设定.mem3是H7内部的SDR内存 */
#define MEM3_BLOCK_SIZE         64                              /* 内存块大小为64字节 */
#define MEM3_MAX_SIZE           24 *1024                        /* 最大管理内存32K,H7的SDR SRAM共32KB */
#define MEM3_ALLOC_TABLE_SIZE   MEM3_MAX_SIZE/MEM3_BLOCK_SIZE   /* 内存表大小 */

/* mem4内存参数设定.mem4是H7内部的DTCM内存,此部分内存仅CPU和MDMA可以访问!!!!!! */
#define MEM4_BLOCK_SIZE         64                              /* 内存块大小为64字节 */
#define MEM4_MAX_SIZE           124 *1024                       /* 最大管理内存124K,H7的DTCM共128KB */
#define MEM4_ALLOC_TABLE_SIZE   MEM4_MAX_SIZE/MEM4_BLOCK_SIZE   /* 内存表大小 */

/* mem5内存参数设定.mem5是H7内部的ITCM内存,此部分内存仅CPU和MDMA可以访问!!!!!! */
#define MEM5_BLOCK_SIZE         64                              /* 内存块大小为64字节 */
#define MEM5_MAX_SIZE           62 *1024                        /* 最大管理内存62K,H7的ITCM共64KB */
#define MEM5_ALLOC_TABLE_SIZE   MEM5_MAX_SIZE/MEM5_BLOCK_SIZE   /* 内存表大小 */


/* 如果没有定义NULL, 定义NULL */
#ifndef NULL
#define NULL 0
#endif



/* 内存管理控制器 */
struct _m_mallco_dev
{
    void (*init)(uint8_t);          /* 初始化 */
    uint16_t (*perused)(uint8_t);   /* 内存使用率 */
    uint8_t *membase[SRAMBANK];     /* 内存池 管理SRAMBANK个区域的内存 */
    MT_TYPE *memmap[SRAMBANK];      /* 内存管理状态表 */
    uint8_t  memrdy[SRAMBANK];      /* 内存管理是否就绪 */
};

extern struct _m_mallco_dev mallco_dev; /* 在mallco.c里面定义 */


/* 用户调用函数 */
void my_mem_init(uint8_t memx);                          /* 内存管理初始化函数(外/内部调用) */
uint16_t my_mem_perused(uint8_t memx) ;                  /* 获得内存使用率(外/内部调用) */
void my_mem_set(void *s, uint8_t c, uint32_t count);     /* 内存设置函数 */
void my_mem_copy(void *des, void *src, uint32_t n);      /* 内存拷贝函数 */

void myfree(uint8_t memx, void *ptr);                    /* 内存释放(外部调用) */
void *mymalloc(uint8_t memx, uint32_t size);             /* 内存分配(外部调用) */
void *myrealloc(uint8_t memx, void *ptr, uint32_t size); /* 重新分配内存(外部调用) */

#endif













