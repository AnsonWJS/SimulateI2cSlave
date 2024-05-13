#ifndef __SIMULATEI2C_S_H__
#define __SIMULATEI2C_S_H__
/*
 * I2C�ӻ�ģ���ͷ�ļ�
 *
 * �汾:     @version 0.0.0.1
 * ����:     wujingsheng
 * ��������: 2024-04-28
 * ��������: 2024-04-28
 * ����:     ��������I2C�ӻ�ģ�⹦��(���)����������ͷ�ļ�
 *
 * ע������:
 * - 
 * - 
 */

#include "SimuI2cSlaveDataList.h"
#include "SimulateI2cSlave.h"
#include "SimulateI2cSlave_bsp.h"
#include "SimuI2cSlaveErrCode.h"
#include "exIns.h"

/*�Ƿ�ʹ��������ʱ�ָ�����*/
#define defSSI2c_HeartbeatTimeoutRecovery  (1)     /*= 1 ʹ�ܹ��� = 0 ������*/
#define defdefSSI2c_HeartbeatTimeoutValue  3000    /*û���յ�����3��I2c ,��λ*/

// I2C�ӻ���ַ����
#define defI2C_SLAVE_ADDRESS 0x48   /*�ӻ��豸�ĵ�ַ*/

/*bitλ����*/
#define defSiumI2c_BitMAX    8      /*ͨ�ŵ�����bitλ����*/

/*�ӻ���ȡ���ݵ���󳤶�*/
#define defReadDataMaxlen    256    /*�ӻ���ȡ���ݵ���󳤶�*/



#endif

