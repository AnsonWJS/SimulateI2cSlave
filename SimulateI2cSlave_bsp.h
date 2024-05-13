#ifndef SIMULATE_I2C_SLAVE_BSP_H
#define SIMULATE_I2C_SLAVE_BSP_H

#include <stdint.h>
#include "exIns.h"

// 版本信息
#define SIMULATE_I2C_SLAVE_BSP_VERSION_MAJOR 1
#define SIMULATE_I2C_SLAVE_BSP_VERSION_MINOR 0
#define SIMULATE_I2C_SLAVE_BSP_VERSION_PATCH 0
#define SIMULATE_I2C_SLAVE_BSP_VERSION_STRING "1.0.0"

/*
 * I2C从机模拟,板机控制的库头文件
 *
 * 版本:     @SIMULATE_I2C_SLAVE_BSP_VERSION_STRING@
 * 作者:     wujingsheng
 * 创建日期: 2024-04-28
 * 更新日期: 2024-04-28
 * 描述:     本头文件定义了模拟I2C从机操需要修改硬件外设功能或者获取的硬件外设状态的功能接口。
 *
 * 注意事项:
 * - Low
 * - 
 */

/*外部中断配置参数*/
#define defEXTI_DEB_CLK_DIV     48000000/48 /*外部脉冲延时中断系统时钟次数*/
#define defEXTI_DEB_CNT         1 /*不知道什么意思, 看<FR801xH Specification>数据手册第19章节也没看明白*/

/*定时器中断配置参数*/
#define defI2cTime1Value           1000/*1ms中断*/

typedef enum 
{
    enSSI2c_IOLow_Level,       /*总线IO等级低电平*/
    enSSI2c_IOHigh_Level,      /*总线IO等级高电平*/
}enumSimuI2cIO_Level;


 /*
 * @brief  模拟I2c从机工作的SCL的GPIO和外部中断初始化
 * @param  *pCtrl:模拟I2C从机控制结构体
 * @retval int err
 */
void SSI2c_SCLIO_IRQ_Init(void);


 /*
 * @brief  模拟I2c从机工作的SCL的GPIO和外部中断初始化
 * @param  void
 * @retval int err
 */
void SSI2c_SDAIO_IRQ_Init(void);



 /*

 * @brief  模拟I2c从机工作的定时器初始化
 * @param  void
 * @retval int err
 */
void SSI2c_Timer_IRQ_Init(void);



 /*
 * @brief  使能相关中断
 * @param  void
 * @retval void
 */
void ENABLE_I2C_GIOP_TIME_IRQ(void);



 /*
 * @brief  取消相关中断
 * @param  void
 * @retval void
 */
void DISENABLE_I2C_GIOP_TIME_IRQ(void);



 /*
 * @brief  设置SDAIO为上升沿中断
 * @param  void
 * @retval void
 */
__attribute__((section("ram_code"))) void Set_SDAIO_IRQ_RisingEdge(void);


 /*
 * @brief  设置SDAIO为下降沿中断
 * @param  void
 * @retval void
 */
__attribute__((section("ram_code"))) void Set_SDAIO_IRQ_DescendingEdge(void);



 /*
 * @brief  设置SCLIO为上升沿中断
 * @param  void
 * @retval void
 */
__attribute__((section("ram_code"))) void Set_SCLIO_IRQ_RisingEdge(void);



 /*
 * @brief  设置SCLIO为下降沿中断s
 * @param  void
 * @retval void
 */
__attribute__((section("ram_code"))) void Set_SCLIO_IRQ_DescendingEdge(void);



 /*
 * @brief  设置SDAIO输出电平
 * @param  void
 * @retval void
 */
__attribute__((section("ram_code"))) void Set_SDAIO_Level(uint8_t IOlevel);



 /*
 * @brief  获取SCLIO电平状态
 * @param  void
 * @retval enumSimuI2cIO_Level
 */
__attribute__((section("ram_code"))) uint8_t get_SCLIO_leven(void);



 /*
 * @brief  获取SDAIO电平状态
 * @param  void
 * @retval enumSimuI2cIO_Level
 */
__attribute__((section("ram_code"))) uint8_t get_SDAIO_leven(void);



#endif 
