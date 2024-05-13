/**
 * @file SimulateI2cSlave_bsp.c
 * @brief �弶֧�ְ�ʵ���ļ���Ϊģ��I2C�ӻ��ṩӲ������㹦�ܡ�
 *
 * @version 1.0.0
 * @date 2024-04-29
 *
 * @copyright Copyright (c) 2024, wujingsheng or
 * This source code is released under the MIT License.
 *
 * @author wujingsheng
 *
 * @brief ���ļ�ʵ����`SimulateI2cSlave_bsp.h`�������ĺ������ṩ��Ӳ����صĵͲ���ʣ�
 * ������������GPIO���ơ�ʱ�����á��жϴ���ȣ���֧��ģ��I2C�ӻ����������С�
 * 
 * 
 *  * ע������:
 *  - ��IO������������"
 * 
 * �汾������˵��:
 * 1.0.0: ��ʼ�汾��ʵ���˻�����I2C�ӻ�ģ�⹦�ܡ�
 */


#include "SimulateI2c_S.h"
/************************** SimulateI2cI2C0 ���Ŷ��� **********************/
/*I2C SDA*/
#define IOPort_I2C0SDA  GPIO_PORT_C
#define IOPin_I2C0SDA   GPIO_BIT_3
#define IO_I2C0SDA      1<<GPIO_BIT_3
#define GPIO_I2C0SDA    GPIO_PC3

/*I2C SCL*/
#define IOPort_I2C0SCL  GPIO_PORT_C
#define IOPin_I2C0SCL   GPIO_BIT_2
#define IO_I2C0SCL      1<<GPIO_BIT_2
#define GPIO_I2C0SCL    GPIO_PC2


volatile unsigned char exti4tpye = EXT_INT_TYPE_NEG;
volatile unsigned char exti5tpye = EXT_INT_TYPE_NEG;

/*
 * @brief  ģ��I2c�ӻ�������SCL��GPIO���ⲿ�жϳ�ʼ��
 * @param  *pCtrl:ģ��I2C�ӻ����ƽṹ��
 * @retval int err
 */
void SSI2c_SCLIO_IRQ_Init(void)
{
//    /*����GPIOΪCPU����*/
//    pmu_set_pin_to_CPU(GPIO_PORT_C, (1<<IOPin_I2C0SCL));
//    //IMPLEMENT
//    /*����Ϊ��ͨIO*/
//    system_set_port_mux(IOPort_I2C0SCL, IOPin_I2C0SCL, PORTC2_FUNC_C2);
//    /*������*/
//    system_set_port_pull(GPIO_I2C0SCL, false);
//    /*����ģʽ*/
//    gpio_set_dir(IOPort_I2C0SCL, IOPin_I2C0SCL, GPIO_DIR_IN);
//    /*�����ⲿ�ж�*/
//    ext_int_set_port_mux(EXTI_5,EXTI_5_PC2);

//		/*�����ж����ȼ�*/
//		NVIC_SetPriority(EXTI_IRQn ,0); /*��ֵԽС���ȼ�Խ��*/

//    /*����Ϊ�½��ش���*/
//    ext_int_set_type(EXTI_5, EXT_INT_TYPE_NEG);
//    ext_int_set_control(EXTI_5, defEXTI_DEB_CLK_DIV, defEXTI_DEB_CNT);
//    ext_int_enable(EXTI_5);

    system_set_port_mux(GPIO_PORT_C, GPIO_BIT_2, PORTC2_FUNC_C2);
    gpio_set_dir(GPIO_PORT_C, GPIO_BIT_2, GPIO_DIR_IN);
    system_set_port_pull(GPIO_PC2, false);
    ext_int_set_port_mux(EXTI_5,EXTI_5_PC2);
    ext_int_set_type(EXTI_5, EXT_INT_TYPE_POS);
		exti5tpye = EXT_INT_TYPE_POS;
    ext_int_set_control(EXTI_5, 24000000, 1);
    ext_int_enable(EXTI_5);
}


 /*
 * @brief  ģ��I2c�ӻ�������SCL��GPIO���ⲿ�жϳ�ʼ��
 * @param  void
 * @retval int err
 */
void SSI2c_SDAIO_IRQ_Init(void)
{
//    /*����GPIOΪCPU����*/
//    pmu_set_pin_to_CPU(GPIO_PORT_C, (1<<IOPin_I2C0SDA));
//    //IMPLEMENT
//    /*����Ϊ��ͨIO*/
//    system_set_port_mux(IOPort_I2C0SDA, IOPin_I2C0SDA, PORTC3_FUNC_C3);
//    /*������*/
//    system_set_port_pull(GPIO_I2C0SDA, false);
//    /*����ģʽ*/
//    gpio_set_dir(IOPort_I2C0SDA, IOPin_I2C0SDA, GPIO_DIR_IN);

//    /*�����ⲿ�ж�*/
//    ext_int_set_port_mux(EXTI_4,EXTI_4_PC3);

//		/*�����ж����ȼ�*/
//		NVIC_SetPriority(EXTI_IRQn ,0); /*��ֵԽС���ȼ�Խ��*/

//    /*����Ϊ�����ش���*/
//    ext_int_set_type(EXTI_4, EXT_INT_TYPE_POS);
//    ext_int_set_control(EXTI_4, defEXTI_DEB_CLK_DIV, defEXTI_DEB_CNT);
//    ext_int_enable(EXTI_4);

    system_set_port_mux(GPIO_PORT_C, GPIO_BIT_3, PORTC3_FUNC_C3);
    gpio_set_dir(GPIO_PORT_C, GPIO_BIT_3, GPIO_DIR_IN);
    system_set_port_pull( GPIO_PC3, false);
    ext_int_set_port_mux(EXTI_4,EXTI_4_PC3);
    ext_int_set_type(EXTI_4,  EXT_INT_TYPE_NEG);
		exti4tpye = EXT_INT_TYPE_NEG;
    ext_int_set_control(EXTI_4, 24000000, 1);
    ext_int_enable(EXTI_4);

}



 /*
 * @brief  ģ��I2c�ӻ������Ķ�ʱ����ʼ��
 * @param  void
 * @retval int err
 */
void SSI2c_Timer_IRQ_Init(void)
{
//  timer_init(TIMER1,defI2cTime1Value,TIMER_PERIODIC);
//  timer_run(TIMER1);										/*��ʼ��ʱ��1����*/
//	NVIC_EnableIRQ(TIMER1_IRQn);          /*�����ж�*/
}



 /*
 * @brief  ʹ������ж�
 * @param  void
 * @retval void
 */
void ENABLE_I2C_GIOP_TIME_IRQ(void)
{
//    timer_run(TIMER1);										/*��ʼ��ʱ��1����*/
    NVIC_SetPriority(EXTI_IRQn, 0);
    NVIC_EnableIRQ(EXTI_IRQn);
//    NVIC_EnableIRQ(TIMER1_IRQn);
}



 /*
 * @brief  ȡ������ж�
 * @param  void
 * @retval void
 */
void DISENABLE_I2C_GIOP_TIME_IRQ(void)
{
//    timer_stop(TIMER1);										/*ֹͣ��ʱ��1����*/
    NVIC_DisableIRQ(EXTI_IRQn);
//    NVIC_DisableIRQ(TIMER1_IRQn);
}



 /*
 * @brief  ����SDAIOΪ�������ж�
 * @param  void
 * @retval void
 */
__attribute__((section("ram_code"))) void Set_SDAIO_IRQ_RisingEdge(void)
{
//	volatile struct ext_int_t *const ext_int_reg_my = (struct ext_int_t *)EXTI_BASE;
//	volatile uint32_t ext_int_type_my = 0;
//	uint8_t offset, index;

//	index = (EXTI_5 / 16);
//	offset = (EXTI_5 % 16)<<1;
//	ext_int_type_my &= (~(EXT_INT_TYPE_MSK<<offset));
//	ext_int_type_my |= (EXT_INT_TYPE_POS << offset);
//	ext_int_reg_my->ext_int_type[index] = ext_int_type_my;

	ext_int_set_type(EXTI_4, EXT_INT_TYPE_POS);
}


 /*
 * @brief  ����SDAIOΪ�½����ж�
 * @param  void
 * @retval void
 */
__attribute__((section("ram_code"))) void Set_SDAIO_IRQ_DescendingEdge(void)
{
//	volatile struct ext_int_t *const ext_int_reg_my = (struct ext_int_t *)EXTI_BASE;
//	volatile uint32_t ext_int_type_my = 0;
//	uint8_t offset, index;

//	index = (EXTI_5 / 16);
//	offset = (EXTI_5 % 16)<<1;
//	ext_int_type_my &= (~(EXT_INT_TYPE_MSK<<offset));
//	ext_int_type_my |= (EXT_INT_TYPE_NEG << offset);
//	ext_int_reg_my->ext_int_type[index] = ext_int_type_my;

	ext_int_set_type(EXTI_4, EXT_INT_TYPE_NEG);
}



 /*
 * @brief  ����SCLIOΪ�������ж�
 * @param  void
 * @retval void
 */
__attribute__((section("ram_code"))) void Set_SCLIO_IRQ_RisingEdge(void)
{
//	volatile struct ext_int_t *const ext_int_reg_my = (struct ext_int_t *)EXTI_BASE;
//	volatile uint32_t ext_int_type_my = 0;
//	uint8_t offset, index;

//	index = (EXTI_4 / 16);
//	offset = (EXTI_4 % 16)<<1;
//	ext_int_type_my &= (~(EXT_INT_TYPE_MSK<<offset));
//	ext_int_type_my |= (EXT_INT_TYPE_POS << offset);
//	ext_int_reg_my->ext_int_type[index] = ext_int_type_my;

	ext_int_set_type(EXTI_5, EXT_INT_TYPE_POS);
}



 /*
 * @brief  ����SCLIOΪ�½����ж�
 * @param  void
 * @retval void
 */
__attribute__((section("ram_code"))) void Set_SCLIO_IRQ_DescendingEdge(void)
{
//	volatile struct ext_int_t *const ext_int_reg_my = (struct ext_int_t *)EXTI_BASE;
//	volatile uint32_t ext_int_type_my = 0;
//	uint8_t offset, index;

//	index = (EXTI_4 / 16);
//	offset = (EXTI_4 % 16)<<1;
//	ext_int_type_my &= (~(EXT_INT_TYPE_MSK<<offset));
//	ext_int_type_my |= (EXT_INT_TYPE_NEG << offset);
//	ext_int_reg_my->ext_int_type[index] = ext_int_type_my;

    ext_int_set_type(EXTI_5, EXT_INT_TYPE_NEG);
}



 /*
 * @brief  ����SDAIO�����ƽ
 * @param  void
 * @retval void
 */
__attribute__((section("ram_code"))) void Set_SDAIO_Level(uint8_t IOlevel)
{
    if(IOlevel == enSSI2c_IOLow_Level) /*��Ҫ����͵�ƽ*/
    {
      gpio_set_dir(IOPort_I2C0SDA, IOPin_I2C0SDA, GPIO_DIR_OUT);
	    gpio_portc_write(gpio_portc_read() & ~(BIT(IOPin_I2C0SDA)));
    }
    else/*�ָ���״̬.Ϊ����״̬,��ͬ�ڸߵ�ƽ���*/
    {
      gpio_set_dir(IOPort_I2C0SDA, IOPin_I2C0SDA, GPIO_DIR_IN);
			gpio_portc_write(gpio_portc_read() & BIT(IOPin_I2C0SDA));
    }
}



 /*
 * @brief  ��ȡSCLIO��ƽ״̬
 * @param  void
 * @retval enumSimuI2cIO_Level
 */
__attribute__((section("ram_code"))) uint8_t get_SCLIO_leven(void)
{
    uint8_t SCLRead = gpio_portc_read();
    /*(SDARead>>2)&0x01 ��ȡPC2��IO��ƽ*/
    return ((SCLRead>>2)&0x01);
}



 /*
 * @brief  ��ȡSDAIO��ƽ״̬
 * @param  void
 * @retval enumSimuI2cIO_Level
 */
__attribute__((section("ram_code"))) uint8_t get_SDAIO_leven(void)
{
    uint8_t SDARead = gpio_portc_read();
    /*(SDARead>>3)&0x01 ��ȡPC3��IO��ƽ*/
    return ((SDARead>>3)&0x01);
}

