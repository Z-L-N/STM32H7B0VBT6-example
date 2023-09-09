/**
 ****************************************************************************************************
 * @file        malloc.c
 * @author      
 * @version     V2.0
 * @date        2023-09-05
 * @brief       �ڴ���� ����
 * @license     
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:
 * �޸�˵��
 * V1.0 20230905
 *
 ****************************************************************************************************
 */


#ifndef __MALLOC_H
#define __MALLOC_H

#include "./SYSTEM/sys/sys.h"

/* ��������ڴ�� */
#define AXI_SRAM123  0       /* AXI SRAM1/2/3�ڴ��,SRAM1=256Kb, SRAM2=384Kb,SRAM3=384Kb,��1024Kb*/
#define AHB_SRAM12   1		 /* AHB SRAM1/2�ڴ��,SRAM1=64Kb, SRAM2=64Kb��128Kb*/
#define SDR_SRAM     2       /* SDR SRAM�ڴ��,��32Kb */
#define SRAM_DTCM    3       /* DTCM�ڴ��,DTCM��128Kb,�˲����ڴ��CPU��MDMA(ͨ��AHBS)���Է���!!!! */
#define SRAM_ITCM    4       /* ITCM�ڴ��,DTCM��64Kb,�˲����ڴ��CPU��MDMA(ͨ��AHBS)���Է���!!!! */

#define SRAMBANK    5       /* ����֧�ֵ�SRAM����. */

/* �����ڴ���������,������SDRAM��ʱ�򣬱���ʹ��uint32_t���ͣ�������Զ����uint16_t���Խ�ʡ�ڴ�ռ�� */
#define MT_TYPE     uint16_t


/* �����ڴ棬�ڴ������ռ�õ�ȫ���ռ��С���㹫ʽ���£�
 * size=MEM1_MAX_SIZE+(MEM1_MAX_SIZE/MEM1_BLOCK_SIZE)*sizeof(MT_TYPE)
 * ��SRAMINΪ����size=474*1024+(474*1024/64)*2=500544��489KB

 * ��֪���ڴ�����(size)������ڴ�صļ��㹫ʽ���£�
 * MEM1_MAX_SIZE=(MEM1_BLOCK_SIZE*size)/(MEM1_BLOCK_SIZE+sizeof(MT_TYPE))
 * ��SRAM12Ϊ��,MEM1_MAX_SIZE=(64*256)/(64+2)=248.24KB��248KB
 */
 
/* mem1�ڴ�����趨.mem1��H7�ڲ���AXI�ڴ�. */
#define MEM1_BLOCK_SIZE         64                              /* �ڴ���СΪ64�ֽ� */
#define MEM1_MAX_SIZE           512*1024                        /* �������ڴ� 512K,H7��AXI�ڴ��ܹ�1024Kb */
#define MEM1_ALLOC_TABLE_SIZE   MEM1_MAX_SIZE/MEM1_BLOCK_SIZE   /* �ڴ���С */

/* mem2�ڴ�����趨.mem2��H7�ڲ���AHB�ڴ� */
#define MEM2_BLOCK_SIZE         64                              /* �ڴ���СΪ64�ֽ� */
#define MEM2_MAX_SIZE           124 *1024                       /* �������ڴ�124K,H7��AHB SRAM1/2��128KB */
#define MEM2_ALLOC_TABLE_SIZE   MEM2_MAX_SIZE/MEM2_BLOCK_SIZE   /* �ڴ���С */

/* mem3�ڴ�����趨.mem3��H7�ڲ���SDR�ڴ� */
#define MEM3_BLOCK_SIZE         64                              /* �ڴ���СΪ64�ֽ� */
#define MEM3_MAX_SIZE           24 *1024                        /* �������ڴ�32K,H7��SDR SRAM��32KB */
#define MEM3_ALLOC_TABLE_SIZE   MEM3_MAX_SIZE/MEM3_BLOCK_SIZE   /* �ڴ���С */

/* mem4�ڴ�����趨.mem4��H7�ڲ���DTCM�ڴ�,�˲����ڴ��CPU��MDMA���Է���!!!!!! */
#define MEM4_BLOCK_SIZE         64                              /* �ڴ���СΪ64�ֽ� */
#define MEM4_MAX_SIZE           124 *1024                       /* �������ڴ�124K,H7��DTCM��128KB */
#define MEM4_ALLOC_TABLE_SIZE   MEM4_MAX_SIZE/MEM4_BLOCK_SIZE   /* �ڴ���С */

/* mem5�ڴ�����趨.mem5��H7�ڲ���ITCM�ڴ�,�˲����ڴ��CPU��MDMA���Է���!!!!!! */
#define MEM5_BLOCK_SIZE         64                              /* �ڴ���СΪ64�ֽ� */
#define MEM5_MAX_SIZE           62 *1024                        /* �������ڴ�62K,H7��ITCM��64KB */
#define MEM5_ALLOC_TABLE_SIZE   MEM5_MAX_SIZE/MEM5_BLOCK_SIZE   /* �ڴ���С */


/* ���û�ж���NULL, ����NULL */
#ifndef NULL
#define NULL 0
#endif



/* �ڴ��������� */
struct _m_mallco_dev
{
    void (*init)(uint8_t);          /* ��ʼ�� */
    uint16_t (*perused)(uint8_t);   /* �ڴ�ʹ���� */
    uint8_t *membase[SRAMBANK];     /* �ڴ�� ����SRAMBANK��������ڴ� */
    MT_TYPE *memmap[SRAMBANK];      /* �ڴ����״̬�� */
    uint8_t  memrdy[SRAMBANK];      /* �ڴ�����Ƿ���� */
};

extern struct _m_mallco_dev mallco_dev; /* ��mallco.c���涨�� */


/* �û����ú��� */
void my_mem_init(uint8_t memx);                          /* �ڴ�����ʼ������(��/�ڲ�����) */
uint16_t my_mem_perused(uint8_t memx) ;                  /* ����ڴ�ʹ����(��/�ڲ�����) */
void my_mem_set(void *s, uint8_t c, uint32_t count);     /* �ڴ����ú��� */
void my_mem_copy(void *des, void *src, uint32_t n);      /* �ڴ濽������ */

void myfree(uint8_t memx, void *ptr);                    /* �ڴ��ͷ�(�ⲿ����) */
void *mymalloc(uint8_t memx, uint32_t size);             /* �ڴ����(�ⲿ����) */
void *myrealloc(uint8_t memx, void *ptr, uint32_t size); /* ���·����ڴ�(�ⲿ����) */

#endif













