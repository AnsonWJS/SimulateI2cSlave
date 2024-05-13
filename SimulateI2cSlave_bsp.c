/**
 * @file SimulateI2cSlave_bsp.c
 * @brief 板级支持包实现文件，为模拟I2C从机提供硬件抽象层功能。
 *
 * @version 1.0.0
 * @date 2024-04-29
 *
 * @copyright Copyright (c) 2024, wujingsheng or
 * This source code is released under the MIT License.
 *
 * @author wujingsheng
 *
 * @brief 本文件实现了`SimulateI2cSlave_bsp.h`中声明的函数，提供了硬件相关的低层访问，
 * 包括但不限于GPIO控制、时钟配置、中断处理等，以支撑模拟I2C从机的正常运行。
 * 
 * 
 *  * 注意事项:
 *  - 该IO配置适用用于"
 * 
 * 版本兼容性说明:
 * 1.0.0: 初始版本，实现了基本的I2C从机模拟功能。
 */


#include "SimulateI2c_S.h"
/************************** SimulateI2cI2C0 引脚定义 **********************/
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
 * @brief  模拟I2c从机工作的SCL的GPIO和外部中断初始化
 * @param  *pCtrl:模拟I2C从机控制结构体
 * @retval int err
 */
void SSI2c_SCLIO_IRQ_Init(void)
{
//    /*设置GPIO为CPU控制*/
//    pmu_set_pin_to_CPU(GPIO_PORT_C, (1<<IOPin_I2C0SCL));
//    //IMPLEMENT
//    /*设置为普通IO*/
//    system_set_port_mux(IOPort_I2C0SCL, IOPin_I2C0SCL, PORTC2_FUNC_C2);
//    /*无上拉*/
//    system_set_port_pull(GPIO_I2C0SCL, false);
//    /*输入模式*/
//    gpio_set_dir(IOPort_I2C0SCL, IOPin_I2C0SCL, GPIO_DIR_IN);
//    /*设置外部中断*/
//    ext_int_set_port_mux(EXTI_5,EXTI_5_PC2);

//		/*设置中断优先级*/
//		NVIC_SetPriority(EXTI_IRQn ,0); /*数值越小优先级越高*/

//    /*设置为下降沿触发*/
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
 * @brief  模拟I2c从机工作的SCL的GPIO和外部中断初始化
 * @param  void
 * @retval int err
 */
void SSI2c_SDAIO_IRQ_Init(void)
{
//    /*设置GPIO为CPU控制*/
//    pmu_set_pin_to_CPU(GPIO_PORT_C, (1<<IOPin_I2C0SDA));
//    //IMPLEMENT
//    /*设置为普通IO*/
//    system_set_port_mux(IOPort_I2C0SDA, IOPin_I2C0SDA, PORTC3_FUNC_C3);
//    /*无上拉*/
//    system_set_port_pull(GPIO_I2C0SDA, false);
//    /*输入模式*/
//    gpio_set_dir(IOPort_I2C0SDA, IOPin_I2C0SDA, GPIO_DIR_IN);

//    /*设置外部中断*/
//    ext_int_set_port_mux(EXTI_4,EXTI_4_PC3);

//		/*设置中断优先级*/
//		NVIC_SetPriority(EXTI_IRQn ,0); /*数值越小优先级越高*/

//    /*设置为上升沿触发*/
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
 * @brief  模拟I2c从机工作的定时器初始化
 * @param  void
 * @retval int err
 */
void SSI2c_Timer_IRQ_Init(void)
{
//  timer_init(TIMER1,defI2cTime1Value,TIMER_PERIODIC);
//  timer_run(TIMER1);										/*开始定时器1计数*/
//	NVIC_EnableIRQ(TIMER1_IRQn);          /*开启中断*/
}



 /*
 * @brief  使能相关中断
 * @param  void
 * @retval void
 */
void ENABLE_I2C_GIOP_TIME_IRQ(void)
{
//    timer_run(TIMER1);										/*开始定时器1计数*/
    NVIC_SetPriority(EXTI_IRQn, 0);
    NVIC_EnableIRQ(EXTI_IRQn);
//    NVIC_EnableIRQ(TIMER1_IRQn);
}



 /*
 * @brief  取消相关中断
 * @param  void
 * @retval void
 */
void DISENABLE_I2C_GIOP_TIME_IRQ(void)
{
//    timer_stop(TIMER1);										/*停止定时器1计数*/
    NVIC_DisableIRQ(EXTI_IRQn);
//    NVIC_DisableIRQ(TIMER1_IRQn);
}



 /*
 * @brief  设置SDAIO为上升沿中断
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
 * @brief  设置SDAIO为下降沿中断
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
 * @brief  设置SCLIO为上升沿中断
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
 * @brief  设置SCLIO为下降沿中断
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
 * @brief  设置SDAIO输出电平
 * @param  void
 * @retval void
 */
__attribute__((section("ram_code"))) void Set_SDAIO_Level(uint8_t IOlevel)
{
    if(IOlevel == enSSI2c_IOLow_Level) /*需要输出低电平*/
    {
      gpio_set_dir(IOPort_I2C0SDA, IOPin_I2C0SDA, GPIO_DIR_OUT);
	    gpio_portc_write(gpio_portc_read() & ~(BIT(IOPin_I2C0SDA)));
    }
    else/*恢复读状态.为输入状态,等同于高电平输出*/
    {
      gpio_set_dir(IOPort_I2C0SDA, IOPin_I2C0SDA, GPIO_DIR_IN);
			gpio_portc_write(gpio_portc_read() & BIT(IOPin_I2C0SDA));
    }
}



 /*
 * @brief  获取SCLIO电平状态
 * @param  void
 * @retval enumSimuI2cIO_Level
 */
__attribute__((section("ram_code"))) uint8_t get_SCLIO_leven(void)
{
    uint8_t SCLRead = gpio_portc_read();
    /*(SDARead>>2)&0x01 读取PC2的IO电平*/
    return ((SCLRead>>2)&0x01);
}



 /*
 * @brief  获取SDAIO电平状态
 * @param  void
 * @retval enumSimuI2cIO_Level
 */
__attribute__((section("ram_code"))) uint8_t get_SDAIO_leven(void)
{
    uint8_t SDARead = gpio_portc_read();
    /*(SDARead>>3)&0x01 读取PC3的IO电平*/
    return ((SDARead>>3)&0x01);
}

