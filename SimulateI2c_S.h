#ifndef __SIMULATEI2C_S_H__
#define __SIMULATEI2C_S_H__
/*
 * I2C从机模拟库头文件
 *
 * 版本:     @version 0.0.0.1
 * 作者:     wujingsheng
 * 创建日期: 2024-04-28
 * 更新日期: 2024-04-28
 * 描述:     用于配置I2C从机模拟功能(如果)。和添加相关头文件
 *
 * 注意事项:
 * - 
 * - 
 */

#include "SimuI2cSlaveDataList.h"
#include "SimulateI2cSlave.h"
#include "SimulateI2cSlave_bsp.h"
#include "SimuI2cSlaveErrCode.h"
#include "exIns.h"

/*是否使能心跳超时恢复功能*/
#define defSSI2c_HeartbeatTimeoutRecovery  (1)     /*= 1 使能功能 = 0 不适用*/
#define defdefSSI2c_HeartbeatTimeoutValue  3000    /*没有收到主机3的I2c ,置位*/

// I2C从机地址定义
#define defI2C_SLAVE_ADDRESS 0x48   /*从机设备的地址*/

/*bit位个数*/
#define defSiumI2c_BitMAX    8      /*通信的最大的bit位个数*/

/*从机读取数据的最大长度*/
#define defReadDataMaxlen    256    /*从机读取数据的最大长度*/



#endif

