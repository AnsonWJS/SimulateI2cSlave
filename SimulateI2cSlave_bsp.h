#ifndef SIMULATE_I2C_SLAVE_BSP_H
#define SIMULATE_I2C_SLAVE_BSP_H

#include <stdint.h>
#include "exIns.h"

// �汾��Ϣ
#define SIMULATE_I2C_SLAVE_BSP_VERSION_MAJOR 1
#define SIMULATE_I2C_SLAVE_BSP_VERSION_MINOR 0
#define SIMULATE_I2C_SLAVE_BSP_VERSION_PATCH 0
#define SIMULATE_I2C_SLAVE_BSP_VERSION_STRING "1.0.0"

/*
 * I2C�ӻ�ģ��,������ƵĿ�ͷ�ļ�
 *
 * �汾:     @SIMULATE_I2C_SLAVE_BSP_VERSION_STRING@
 * ����:     wujingsheng
 * ��������: 2024-04-28
 * ��������: 2024-04-28
 * ����:     ��ͷ�ļ�������ģ��I2C�ӻ�����Ҫ�޸�Ӳ�����蹦�ܻ��߻�ȡ��Ӳ������״̬�Ĺ��ܽӿڡ�
 *
 * ע������:
 * - Low
 * - 
 */

/*�ⲿ�ж����ò���*/
#define defEXTI_DEB_CLK_DIV     48000000/48 /*�ⲿ������ʱ�ж�ϵͳʱ�Ӵ���*/
#define defEXTI_DEB_CNT         1 /*��֪��ʲô��˼, ��<FR801xH Specification>�����ֲ��19�½�Ҳû������*/

/*��ʱ���ж����ò���*/
#define defI2cTime1Value           1000/*1ms�ж�*/

typedef enum 
{
    enSSI2c_IOLow_Level,       /*����IO�ȼ��͵�ƽ*/
    enSSI2c_IOHigh_Level,      /*����IO�ȼ��ߵ�ƽ*/
}enumSimuI2cIO_Level;


 /*
 * @brief  ģ��I2c�ӻ�������SCL��GPIO���ⲿ�жϳ�ʼ��
 * @param  *pCtrl:ģ��I2C�ӻ����ƽṹ��
 * @retval int err
 */
void SSI2c_SCLIO_IRQ_Init(void);


 /*
 * @brief  ģ��I2c�ӻ�������SCL��GPIO���ⲿ�жϳ�ʼ��
 * @param  void
 * @retval int err
 */
void SSI2c_SDAIO_IRQ_Init(void);



 /*

 * @brief  ģ��I2c�ӻ������Ķ�ʱ����ʼ��
 * @param  void
 * @retval int err
 */
void SSI2c_Timer_IRQ_Init(void);



 /*
 * @brief  ʹ������ж�
 * @param  void
 * @retval void
 */
void ENABLE_I2C_GIOP_TIME_IRQ(void);



 /*
 * @brief  ȡ������ж�
 * @param  void
 * @retval void
 */
void DISENABLE_I2C_GIOP_TIME_IRQ(void);



 /*
 * @brief  ����SDAIOΪ�������ж�
 * @param  void
 * @retval void
 */
__attribute__((section("ram_code"))) void Set_SDAIO_IRQ_RisingEdge(void);


 /*
 * @brief  ����SDAIOΪ�½����ж�
 * @param  void
 * @retval void
 */
__attribute__((section("ram_code"))) void Set_SDAIO_IRQ_DescendingEdge(void);



 /*
 * @brief  ����SCLIOΪ�������ж�
 * @param  void
 * @retval void
 */
__attribute__((section("ram_code"))) void Set_SCLIO_IRQ_RisingEdge(void);



 /*
 * @brief  ����SCLIOΪ�½����ж�s
 * @param  void
 * @retval void
 */
__attribute__((section("ram_code"))) void Set_SCLIO_IRQ_DescendingEdge(void);



 /*
 * @brief  ����SDAIO�����ƽ
 * @param  void
 * @retval void
 */
__attribute__((section("ram_code"))) void Set_SDAIO_Level(uint8_t IOlevel);



 /*
 * @brief  ��ȡSCLIO��ƽ״̬
 * @param  void
 * @retval enumSimuI2cIO_Level
 */
__attribute__((section("ram_code"))) uint8_t get_SCLIO_leven(void);



 /*
 * @brief  ��ȡSDAIO��ƽ״̬
 * @param  void
 * @retval enumSimuI2cIO_Level
 */
__attribute__((section("ram_code"))) uint8_t get_SDAIO_leven(void);



#endif 
