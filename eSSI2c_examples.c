/**
 * @file eSSI2c_examples.c
 * @brief 模拟I2C从机操作的适用例子
 *
 * @version 1.0.0
 * @date 2024-04-30
 *
 * @copyright Copyright (c) 2024, wujingsheng
 * This source code is released under the MIT License.
 *
 * @author wujingsheng
 *
 * @brief  软件模拟I2c的适用例子
 * 
 */

#include "eSSI2c_examples.h"

structSimulateI2c_SlaveCtrl strSSI2cCtrl;
uint8_t arri2cReadbuff[256];



/*
 * @brief  重新初始化模拟I2C从机
 * @param  void
 * @retval void 
 */
void SSI2c_Reset(void)
{
    /*先关闭中断*/
    DISENABLE_I2C_GIOP_TIME_IRQ();
    /*初始化模拟I2C从机的控制数据结构*/
    SimulateI2cSlaveStruct_Init(&strSSI2cCtrl);

    /*设置从机设备的设备地址*/
    Set_SimulateI2cSlave_address(&strSSI2cCtrl, defI2C_SLAVE_ADDRESS);

    /*设置从机接收数据的数组缓存地址和最大的接收长度*/
    Set_SimulateI2cSlave_RDataPram(&strSSI2cCtrl, arri2cReadbuff, 256);

    /*设置无SCL脉冲的超时,恢复状态*/
    strSSI2cCtrl.MaxTimerMs = defNotSCLMaxTimerMs;

    /*设置获取从机发送到主机的数据的回调函数*/
    strSSI2cCtrl._Get_TdataCallback = SimuLateI2c_Get_TdataCallback_CB;

    /*设置获取主机写入从机寄存器数据的回调函数*/
    strSSI2cCtrl._Set_RdataCallback = SimuLateI2c_Set_RdataCallback_CB;

    /*初始化SCL硬件的GIOP工作模式,和对应的外部中断模式*/
    SSI2c_SCLIO_IRQ_Init();

    /*初始化SDA硬件的GIOP工作模式,和对应的外部中断模式*/
    SSI2c_SDAIO_IRQ_Init();

    /*初始化用于无SCL脉冲超时计数的定时器,和对应的定时器中断*/
//    SSI2c_Timer_IRQ_Init();

    /*打开相关中断*/
    ENABLE_I2C_GIOP_TIME_IRQ();
}


/*
 * @brief  初始化模拟I2C从机
 * @param  void
 * @retval void 
 */
void SSI2c_Init(void)
{
    /*初始化模拟I2C从机的寄存器结构数据和寄存器里面的数据*/
    SimuLateI2c_DataList_Init(&strSSI2cDataList);

    /*初始化模拟I2C从机的控制数据结构*/
    SimulateI2cSlaveStruct_Init(&strSSI2cCtrl);

    /*设置从机设备的设备地址*/
    Set_SimulateI2cSlave_address(&strSSI2cCtrl, defI2C_SLAVE_ADDRESS);

    /*设置从机接收数据的数组缓存地址和最大的接收长度*/
    Set_SimulateI2cSlave_RDataPram(&strSSI2cCtrl, arri2cReadbuff, 256);

    /*设置无SCL脉冲的超时,恢复状态*/
    strSSI2cCtrl.MaxTimerMs = defNotSCLMaxTimerMs;

    /*设置获取从机发送到主机的数据的回调函数*/
    strSSI2cCtrl._Get_TdataCallback = SimuLateI2c_Get_TdataCallback_CB;

    /*设置获取主机写入从机寄存器数据的回调函数*/
    strSSI2cCtrl._Set_RdataCallback = SimuLateI2c_Set_RdataCallback_CB;

    /*初始化SCL硬件的GIOP工作模式,和对应的外部中断模式*/
    SSI2c_SCLIO_IRQ_Init();

    /*初始化SDA硬件的GIOP工作模式,和对应的外部中断模式*/
    SSI2c_SDAIO_IRQ_Init();

    /*初始化用于无SCL脉冲超时计数的定时器,和对应的定时器中断*/
//    SSI2c_Timer_IRQ_Init();

    /*打开相关中断*/
    ENABLE_I2C_GIOP_TIME_IRQ();

/*
 * 其他函数调用:
 * - 该模拟i2c从机程序,必须开启SCL和SDA引脚的的中断功能.(且中断边缘中断需要在通信过程中修改)
 *      -SSI2c_SDAIO_IRQHandler()需要放在SDA硬件引脚的中断服务函数中
 *      -SSI2c_SCLIO_IRQHandler()需要放在SCL硬件引脚的中断服务函数中
 * - 该模拟i2c从机程序,可以选择是否需要外部定时器计数是否超时,判断是否恢复I2C通信状态到Idle.
 *      -SSI2c_Timer_IRQHandler()需要放到定时器中,定时判断.
 * 
 * - 用户对于模式I2c寄存器的数据可以通过函数
 *      -User_set_SSI2cRegisterData()进行设置. 
 *      -User_get_SSI2cRegisterData()进行获取.
 *          注意:主机和从机对寄存器的操作没有加互斥锁.存在对寄存器操作的并发问题.
 * 
 * - 寄存器的内容可以参考 \SimuI2cSlaveDataList.h 和 SimuI2cSlaveDataList.C 的数据和初始化函数作修改
 *          注意:防止数组的越界赋值.
 * 
 * - 关闭I2CSlave的读写可以调用DISENABLE_I2C_GIOP_TIME_IRQ();通过关闭中断来实现禁止I2C Slave的读写.
 *        重新开启可以通过调用本函数SSI2c_Init();重新启动模拟I2C Slave服务.
 * 
 * 
 * 建议:
 *      -与主机有心跳机制.主机定时写入某个寄存器的值,确保软件I2c模块能正常通信. 
 *          使能心跳机制的步骤
 *              1.可通过"SimulateI2c_S.h"中的defSSI2c_HeartbeatTimeoutRecovery 设置是否使能
 *              2.通过Add_SSI2c_HTimeoutRecovery_cb()定时更新计数器
 *              3.strSSI2cCtrl->HTimeoutRecovery的值与defdefSSI2c_HeartbeatTimeoutValue作大小对比
 *                  if(strSSI2cCtrl->HTimeoutRecovery > defdefSSI2c_HeartbeatTimeoutValue)
 *                  {
 *                      //调用本函数再初始化,实现恢复.
 *                      SSI2c_Init();
 *                  }
 *          
 */ 
}

