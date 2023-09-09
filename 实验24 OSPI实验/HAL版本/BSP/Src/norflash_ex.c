/**
 ****************************************************************************************************
 * @file        norflash_ex.c
 * @author      
 * @version     V2.0
 * @date        2023-09-06
 * @brief       OSPI ��������
 *              ��ΪSTM32H7��֧��OSPI�ӿڶ�ʱд,��˸ô�������ʵ��QSPI FLASH������д��,ԭ���ǣ�
 *              qspi.c��norflash.c��norflash_ex.c��3���ִ���ȫ���洢��H7���ڲ�FLASH,��֤����
 *              QSPI FLASH��ʱ��,����Ҫ����QSPI FLASH�Ĵ���. �Ӷ�ʵ��QSPI FLASH����д��.
 *
 * @license     
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:
 *
 * �޸�˵��
 * V2.0 2023-09-06
 *
 ****************************************************************************************************
 */

#include "ospi.h"
#include "norflash.h"
#include "norflash_ex.h"


extern uint8_t g_norflash_addrw;    /* ��ʾ��ǰ��24bit/32bit����λ��, ��norflash.c���涨�� */

/**
 * @brief       QSPI�ӿڽ����ڴ�ӳ��ģʽ
 *   @note      ���øú���֮ǰ����Ѿ���ʼ����QSPI�ӿ�
 *              sys_qspi_enable_memmapmode or norflash_init
 * @param       ��
 * @retval      ��
 */
static void norflash_ex_enter_mmap(void)
{
    uint32_t tempreg = 0;

    /* BY/W25QXX дʹ�ܣ�0X06ָ� */
    while (OCTOSPI1->SR & (1 << 5)); /* �ȴ�BUSYλ���� */

	OCTOSPI1->IR = 0X00000006;
	OCTOSPI1->CCR = 0X00000100;      /* ����0X06ָ�BY/W25QXXдʹ�� */
	
    while ((OCTOSPI1->SR & (1 << 1)) == 0);  /* �ȴ�ָ������ */

    OCTOSPI1->FCR |= 1 << 1;

    if (ospi_wait_flag(1 << 5, 0, 0XFFFF) == 0) /* �ȴ�BUSY���� */
    {
        OCTOSPI1->IR = 0X000000EB;         /* INSTRUCTION[7:0]=0XEB,����0XEBָ�Fast Read QUAD I/O�� */
        tempreg |= 1 << 0;      /* IMODE[1:0]=1,���ߴ���ָ�� */
		tempreg |= 0 << 3;
		tempreg |= 0 << 4;
        tempreg |= 3 << 8;     /* ADDRESS[1:0]=3,���ߴ����ַ */ 
		tempreg |= 0 << 11;
        tempreg |= (uint32_t)g_norflash_addrw << 12;    /* ADSIZE[1:0]=2,24/32λ��ַ���� */
        tempreg |= 3 << 16;     /* ABMODE[1:0]=3,���ߴ��佻���ֽ� */
		tempreg |= 0 << 19;
        tempreg |= 0 << 20;     /* ABSIZE[1:0]=0,8λ�����ֽ�(M0~M7) */
       // tempreg |= 4 << 18;     /* DCYC[4:0]=4,4��dummy���� */
		OCTOSPI1->TCR = 6 << 0;   /* DCYC[4:0]=4,4��dummy���� */
        tempreg |= 3 << 24;     /* DMODE[1:0]=3,���ߴ������� */
		tempreg |= 0 << 27;
        //tempreg |= 3 << 26;     /* FMODE[1:0]=3,�ڴ�ӳ��ģʽ */
		OCTOSPI1->CR = 3 << 28;  /* FMODE[1:0]=3,�ڴ�ӳ��ģʽ */
		tempreg |= 0 << 29; 
		tempreg |= 0 << 31;
        OCTOSPI1->CCR = tempreg; /* ����CCR�Ĵ��� */
    }

    sys_intx_enable();          /* �����ж� */
}

/**
 * @brief       QSPI�ӿ��˳��ڴ�ӳ��ģʽ
 *   @note      ���øú���֮ǰ����Ѿ���ʼ����QSPI�ӿ�
 *              sys_qspi_enable_memmapmode or norflash_init
 * @param       ��
 * @retval      0, OK;  ����, �������
 */
static uint8_t norflash_ex_exit_mmap(void)
{
    uint8_t res = 0;

    sys_intx_disable();         /* �ر��ж� */
    SCB_InvalidateICache();     /* ���I CACHE */
    SCB_InvalidateDCache();     /* ���D CACHE */
    OCTOSPI1->CR &= ~(1 << 0);   /* �ر� QSPI �ӿ� */
    OCTOSPI1->CR |= 1 << 1;      /* �˳�MEMMAPEDģʽ */
    res = ospi_wait_flag(1 << 5, 0, 0XFFFF);    /* �ȴ�BUSY���� */

    if (res == 0)
    {
        OCTOSPI1->CCR = 0;       /* CCR�Ĵ������� */
        OCTOSPI1->CR |= 1 << 0;  /* ʹ�� QSPI �ӿ� */
    }

    return res;
}

/**
 * @brief       �� OSPI FLASHд������
 *   @note      ��ָ����ַ��ʼд��ָ�����ȵ�����
 *              �ú�������������!
 * @param       pbuf    : ���ݴ洢��
 * @param       addr    : ��ʼд��ĵ�ַ(���32bit)
 * @param       datalen : Ҫд����ֽ���(���65535)
 * @retval      0, OK;  ����, �������
 */
uint8_t norflash_ex_write(uint8_t *pbuf, uint32_t addr, uint16_t datalen)
{
    uint8_t res = 0;
    res = norflash_ex_exit_mmap();  /* �˳��ڴ�ӳ��ģʽ */

    if (res == 0)
    {
        norflash_write(pbuf, addr, datalen);
    }

    norflash_ex_enter_mmap();       /* �����ڴ�ӳ��ģʽ */
    return res;
}

/**
 * @brief       �� OSPI FLASH ��ȡ����
 *   @note      ��ָ����ַ��ʼ��ȡָ�����ȵ����ݣ����봦���ڴ�ӳ��ģʽ�£��ſ���ִ�У�
 *
 * @param       pbuf    : ���ݴ洢��
 * @param       addr    : ��ʼ��ȡ�ĵ�ַ(���32bit)
 * @param       datalen : Ҫ��ȡ���ֽ���(���65535)
 * @retval      0, OK;  ����, �������
 */
void norflash_ex_read(uint8_t *pbuf, uint32_t addr, uint16_t datalen)
{
    uint16_t i = 0;
    addr += 0X90000000;     /* ʹ���ڴ�ӳ��ģʽ��ȡ��QSPI�Ļ�ַ��0X90000000����������Ҫ���ϻ�ַ */
    sys_intx_disable();     /* �ر��ж� */

    for (i = 0; i < datalen; i++)
    {
        pbuf[i] = *(volatile uint8_t *)(addr + i);
    }

    sys_intx_enable();      /* �����ж� */
}

/**
 * @brief       ��ȡOSPI FLASH��ID
 * @param       ��
 * @retval      NOR FLASH ID
 */
uint32_t norflash_ex_read_id(void)
{
    uint8_t res = 0;
    uint16_t id = 0; 
    res = norflash_ex_exit_mmap();  /* �˳��ڴ�ӳ��ģʽ */

    if (res == 0)
    {
        id = norflash_read_id();
    }

    norflash_ex_enter_mmap();       /* �����ڴ�ӳ��ģʽ */
    return id;
}

/**
 * @brief       ����QSPI FLASH��ĳ������
 *   @note      ע��,������������ַ,�����ֽڵ�ַ!!
 *              ����һ������������ʱ��:150ms
 *
 * @param       saddr: ������ַ
 * @retval      ��
 */
void norflash_ex_erase_sector(uint32_t addr)
{
    uint8_t res = 0;
    res = norflash_ex_exit_mmap();  /* �˳��ڴ�ӳ��ģʽ */

    if (res == 0)
    {
        norflash_erase_sector(addr);
    }

    norflash_ex_enter_mmap();       /* �����ڴ�ӳ��ģʽ */
}

/**
 * @brief       ����QSPI FLASH����оƬ
 *   @note      �ȴ�ʱ�䳬��...
 *
 * @param       ��
 * @retval      ��
 */
void norflash_ex_erase_chip(void)
{
    uint8_t res = 0;
    res = norflash_ex_exit_mmap();  /* �˳��ڴ�ӳ��ģʽ */

    if (res == 0)
    {
        norflash_erase_chip();
    }

    norflash_ex_enter_mmap();       /* �����ڴ�ӳ��ģʽ */
}














