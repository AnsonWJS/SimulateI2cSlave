/**
 * @file eSSI2c_examples.c
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

#include "eSSI2c_examples.h"

structSimulateI2c_SlaveCtrl strSSI2cCtrl;
uint8_t arri2cReadbuff[256];



/*
 * @brief  ���³�ʼ��ģ��I2C�ӻ�
 * @param  void
 * @retval void 
 */
void SSI2c_Reset(void)
{
    /*�ȹر��ж�*/
    DISENABLE_I2C_GIOP_TIME_IRQ();
    /*��ʼ��ģ��I2C�ӻ��Ŀ������ݽṹ*/
    SimulateI2cSlaveStruct_Init(&strSSI2cCtrl);

    /*���ôӻ��豸���豸��ַ*/
    Set_SimulateI2cSlave_address(&strSSI2cCtrl, defI2C_SLAVE_ADDRESS);

    /*���ôӻ��������ݵ����黺���ַ�����Ľ��ճ���*/
    Set_SimulateI2cSlave_RDataPram(&strSSI2cCtrl, arri2cReadbuff, 256);

    /*������SCL����ĳ�ʱ,�ָ�״̬*/
    strSSI2cCtrl.MaxTimerMs = defNotSCLMaxTimerMs;

    /*���û�ȡ�ӻ����͵����������ݵĻص�����*/
    strSSI2cCtrl._Get_TdataCallback = SimuLateI2c_Get_TdataCallback_CB;

    /*���û�ȡ����д��ӻ��Ĵ������ݵĻص�����*/
    strSSI2cCtrl._Set_RdataCallback = SimuLateI2c_Set_RdataCallback_CB;

    /*��ʼ��SCLӲ����GIOP����ģʽ,�Ͷ�Ӧ���ⲿ�ж�ģʽ*/
    SSI2c_SCLIO_IRQ_Init();

    /*��ʼ��SDAӲ����GIOP����ģʽ,�Ͷ�Ӧ���ⲿ�ж�ģʽ*/
    SSI2c_SDAIO_IRQ_Init();

    /*��ʼ��������SCL���峬ʱ�����Ķ�ʱ��,�Ͷ�Ӧ�Ķ�ʱ���ж�*/
//    SSI2c_Timer_IRQ_Init();

    /*������ж�*/
    ENABLE_I2C_GIOP_TIME_IRQ();
}


/*
 * @brief  ��ʼ��ģ��I2C�ӻ�
 * @param  void
 * @retval void 
 */
void SSI2c_Init(void)
{
    /*��ʼ��ģ��I2C�ӻ��ļĴ����ṹ���ݺͼĴ������������*/
    SimuLateI2c_DataList_Init(&strSSI2cDataList);

    /*��ʼ��ģ��I2C�ӻ��Ŀ������ݽṹ*/
    SimulateI2cSlaveStruct_Init(&strSSI2cCtrl);

    /*���ôӻ��豸���豸��ַ*/
    Set_SimulateI2cSlave_address(&strSSI2cCtrl, defI2C_SLAVE_ADDRESS);

    /*���ôӻ��������ݵ����黺���ַ�����Ľ��ճ���*/
    Set_SimulateI2cSlave_RDataPram(&strSSI2cCtrl, arri2cReadbuff, 256);

    /*������SCL����ĳ�ʱ,�ָ�״̬*/
    strSSI2cCtrl.MaxTimerMs = defNotSCLMaxTimerMs;

    /*���û�ȡ�ӻ����͵����������ݵĻص�����*/
    strSSI2cCtrl._Get_TdataCallback = SimuLateI2c_Get_TdataCallback_CB;

    /*���û�ȡ����д��ӻ��Ĵ������ݵĻص�����*/
    strSSI2cCtrl._Set_RdataCallback = SimuLateI2c_Set_RdataCallback_CB;

    /*��ʼ��SCLӲ����GIOP����ģʽ,�Ͷ�Ӧ���ⲿ�ж�ģʽ*/
    SSI2c_SCLIO_IRQ_Init();

    /*��ʼ��SDAӲ����GIOP����ģʽ,�Ͷ�Ӧ���ⲿ�ж�ģʽ*/
    SSI2c_SDAIO_IRQ_Init();

    /*��ʼ��������SCL���峬ʱ�����Ķ�ʱ��,�Ͷ�Ӧ�Ķ�ʱ���ж�*/
//    SSI2c_Timer_IRQ_Init();

    /*������ж�*/
    ENABLE_I2C_GIOP_TIME_IRQ();

/*
 * ������������:
 * - ��ģ��i2c�ӻ�����,���뿪��SCL��SDA���ŵĵ��жϹ���.(���жϱ�Ե�ж���Ҫ��ͨ�Ź������޸�)
 *      -SSI2c_SDAIO_IRQHandler()��Ҫ����SDAӲ�����ŵ��жϷ�������
 *      -SSI2c_SCLIO_IRQHandler()��Ҫ����SCLӲ�����ŵ��жϷ�������
 * - ��ģ��i2c�ӻ�����,����ѡ���Ƿ���Ҫ�ⲿ��ʱ�������Ƿ�ʱ,�ж��Ƿ�ָ�I2Cͨ��״̬��Idle.
 *      -SSI2c_Timer_IRQHandler()��Ҫ�ŵ���ʱ����,��ʱ�ж�.
 * 
 * - �û�����ģʽI2c�Ĵ��������ݿ���ͨ������
 *      -User_set_SSI2cRegisterData()��������. 
 *      -User_get_SSI2cRegisterData()���л�ȡ.
 *          ע��:�����ʹӻ��ԼĴ����Ĳ���û�мӻ�����.���ڶԼĴ��������Ĳ�������.
 * 
 * - �Ĵ��������ݿ��Բο� \SimuI2cSlaveDataList.h �� SimuI2cSlaveDataList.C �����ݺͳ�ʼ���������޸�
 *          ע��:��ֹ�����Խ�縳ֵ.
 * 
 * - �ر�I2CSlave�Ķ�д���Ե���DISENABLE_I2C_GIOP_TIME_IRQ();ͨ���ر��ж���ʵ�ֽ�ֹI2C Slave�Ķ�д.
 *        ���¿�������ͨ�����ñ�����SSI2c_Init();��������ģ��I2C Slave����.
 * 
 * 
 * ����:
 *      -����������������.������ʱд��ĳ���Ĵ�����ֵ,ȷ�����I2cģ��������ͨ��. 
 *          ʹ���������ƵĲ���
 *              1.��ͨ��"SimulateI2c_S.h"�е�defSSI2c_HeartbeatTimeoutRecovery �����Ƿ�ʹ��
 *              2.ͨ��Add_SSI2c_HTimeoutRecovery_cb()��ʱ���¼�����
 *              3.strSSI2cCtrl->HTimeoutRecovery��ֵ��defdefSSI2c_HeartbeatTimeoutValue����С�Ա�
 *                  if(strSSI2cCtrl->HTimeoutRecovery > defdefSSI2c_HeartbeatTimeoutValue)
 *                  {
 *                      //���ñ������ٳ�ʼ��,ʵ�ָֻ�.
 *                      SSI2c_Init();
 *                  }
 *          
 */ 
}

