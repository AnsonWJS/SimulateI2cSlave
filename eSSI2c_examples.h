#ifndef __ESSI2C_EXAMPLES_H__
#define __ESSI2C_EXAMPLES_H__

/**
 * @file eSSI2c_examples.h
 * @brief ģ��I2C�ӻ���������������
 *
 * @version 1.0.0
 * @date 2024-04-30
 *
 * @copyright Copyright (c) 2024, wujingsheng
 * This source code is released under the MIT License.
 *
 * @author wujingsheng
 *
 * @brief  ���ģ��I2c����������
 * 
 */
#include "SimulateI2c_S.h"

extern structSimulateI2c_SlaveCtrl strSSI2cCtrl;


/*
 * @brief  ���³�ʼ��ģ��I2C�ӻ�
 * @param  void
 * @retval void 
 */
void SSI2c_Reset (void);

/*
 * @brief  ��ʼ��ģ��I2C�ӻ�
 * @param  void
 * @retval void 
 */
void SSI2c_Init(void);



#endif

