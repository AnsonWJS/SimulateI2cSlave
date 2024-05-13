#ifndef __ESSI2C_EXAMPLES_H__
#define __ESSI2C_EXAMPLES_H__

/**
 * @file eSSI2c_examples.h
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
#include "SimulateI2c_S.h"

extern structSimulateI2c_SlaveCtrl strSSI2cCtrl;


/*
 * @brief  重新初始化模拟I2C从机
 * @param  void
 * @retval void 
 */
void SSI2c_Reset (void);

/*
 * @brief  初始化模拟I2C从机
 * @param  void
 * @retval void 
 */
void SSI2c_Init(void);



#endif

