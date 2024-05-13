/**
 * @file SimulateI2cSlave.c
 * @brief ģ��I2C�ӻ�������ʵ��Դ�ļ���
 *
 * @version 1.0.0
 * @date 2024-04-28
 *
 * @copyright Copyright (c) 2024, wujingsheng
 * This source code is released under the MIT License.
 *
 * @author wujingsheng
 *
 * @brief ���ļ�ʵ����`simulate_i2c_slave.h`�������ĺ������ṩ������Ӳ��֧����ģ��I2C�ӻ���Ϊ��������
 * 
 * ע������:
 * - ��ģ��i2c�ӻ�����,���뿪��SCL��SDA���ŵĵ��жϹ���.(���жϱ�Ե�ж���Ҫ��ͨ�Ź������޸�)
 *      -SSI2c_SDAIO_IRQHandler()��Ҫ����SDAӲ�����ŵ��жϷ�������
 *      -SSI2c_SCLIO_IRQHandler()��Ҫ����SCLӲ�����ŵ��жϷ�������
 * - ��ģ��i2c�ӻ�����,����ѡ���Ƿ���Ҫ�ⲿ��ʱ�������Ƿ�ʱ,�ж��Ƿ�ָ�I2Cͨ��״̬��Idle.
 *      -SSI2c_Timer_IRQHandler()��Ҫ�ŵ���ʱ����,��ʱ�ж�.
 * �汾������˵��:
 * 1.0.0: ��ʼ�汾��ʵ���˻�����I2C�ӻ�ģ�⹦�ܡ�
 */

#include "SimulateI2c_S.h"


/*
 * @brief  ��ʼ��ģ��I2C�ӻ�
 * @param  *pCtrl:ģ��I2C�ӻ����ƽṹ��
 * @retval int err 
 */
int SimulateI2cSlaveStruct_Init(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl)
{
    int err = defErrOK;

    /*�����Ķ�д��־λ, = 0 Ϊ����д = 1 Ϊ������*/
    _strSSI2cCtrl->ReadWriteflag = enSSI2c_Write;
    /*ģ��I2C�ӻ��Ľ���״̬*/
    _strSSI2cCtrl->state = enSSI2c_Idle;
    /*ģ��I2C�ӻ��Ķ�дΪλ��*/
    _strSSI2cCtrl->BitIndex = 0;
    /*ʱ�������.���ڶ�ʱ��������,��λ(ms)*/
    _strSSI2cCtrl->TimerMs = 0;
    /*������������λ��*/
    _strSSI2cCtrl->dataIndex = 0;

    _strSSI2cCtrl->ErrCode = 0;

    _strSSI2cCtrl->readbuff = 0;

    _strSSI2cCtrl->sendbuff = 0;

    _strSSI2cCtrl->HTimeoutRecovery = 0;

    return err;
}


/*
 * @brief  ����ģ��I2C�ӻ��豸�ĵ�ַ
 * @param  *pCtrl:ģ��I2C�ӻ����ƽṹ��
 * @param  slaveAddress:�ӻ���ַ
 * @retval int err
 */
int Set_SimulateI2cSlave_address(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl, uint8_t slaveAddress)
{
    int err = defErrOK;
    
    /*�����Ķ�д��־λ, = 0 Ϊ����д = 1 Ϊ������*/
    _strSSI2cCtrl->slaveAddress = slaveAddress;

    return err;
}


 /*
 * @brief  ����ģ��I2C�ӻ��豸�Ķ�ȡ���ݵĻ�����,�ͻ�������С
 * @param  *pCtrl:ģ��I2C�ӻ����ƽṹ��
 * @param  _RDataAddress:   ��ȡ���ݵ�ַ
 * @param  RdataMaxLen:     ����ȡ����  
 * @retval int err
 */
int Set_SimulateI2cSlave_RDataPram(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl, uint8_t *_RDataAddress, uint16_t RdataMaxLen)
{
    int err = defErrOK;
    
    /*�����Ķ�д��־λ, = 0 Ϊ����д = 1 Ϊ������*/
    _strSSI2cCtrl->_arrRDataBuf = _RDataAddress;

    _strSSI2cCtrl->RMaxDataLen = RdataMaxLen;

    return err;
}



 /*
 * @brief  ����ģ��I2C�ӻ��豸�ķ������ݵĻ�����,�Ϳɷ������ݴ�С
 * @param  *pCtrl:ģ��I2C�ӻ����ƽṹ��
 * @param  _TDataAddress:   �������ݵ�ַ
 * @param  TdataMaxLen:     ����ȡ����  T
 * @retval int err
 */
__attribute__((section("ram_code"))) int Set_SimulateI2cSlave_TDataPram(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl, uint8_t * _TDataAddress, uint16_t TdataMaxLen)
{
    int err = defErrOK;

    /*���ͻ�������ֵ*/
    _strSSI2cCtrl->_arrTDataBuf = _TDataAddress;

    _strSSI2cCtrl->TMaxDataLen = TdataMaxLen;

    return err;
}



 /*
 * @brief  ��ʼģ��I2c�ӻ�����
 * @param  *pCtrl:ģ��I2C�ӻ����ƽṹ��
 * @retval int err
 */
int Start_SimulateI2cSlave_CB(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl)
{
    int err = defErrOK;

    _strSSI2cCtrl->BitIndex = 0;
    _strSSI2cCtrl->dataIndex = 0;

    _strSSI2cCtrl->state = enSSI2c_Idle;
    _strSSI2cCtrl->ReadWriteflag = enSSI2c_Write;
    _strSSI2cCtrl->TimerMs = 0;

    /*����SDA�ж�ģʽλ�½����ж�.�����ж��Ƿ�����ʼλ*/
    Set_SDAIO_IRQ_DescendingEdge();
    /*����SCL�ж�ģʽλ�������ж�.�������ж��ж�ȡ����д��bitλ����*/
    Set_SCLIO_IRQ_RisingEdge();

    ENABLE_I2C_GIOP_TIME_IRQ();

    return err;
}



 /*
 * @brief  ֹͣģ��I2c�ӻ�����
 * @param  *pCtrl:ģ��I2C�ӻ����ƽṹ��
 * @retval int err
 */
int Stop_SimulateI2cSlave_CB(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl)
{
    int err = defErrOK;

    _strSSI2cCtrl->BitIndex = 0;
    _strSSI2cCtrl->dataIndex = 0;

    _strSSI2cCtrl->state = enSSI2c_Idle;
    _strSSI2cCtrl->ReadWriteflag = enSSI2c_Write;
    _strSSI2cCtrl->TimerMs = 0;

    DISENABLE_I2C_GIOP_TIME_IRQ();

    return err;
}



 /*
 * @brief  ģ��I2c�ӻ�������SDA�����жϻص�����
 * @param  *pCtrl:ģ��I2C�ӻ����ƽṹ��
 * @retval int err
 */
__attribute__((section("ram_code"))) void SSI2c_SDAIO_IRQHandler(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl)
{
    if(get_SCLIO_leven() == enSSI2c_IOHigh_Level)/*SDA���Ÿı�״̬,��SCL����Ϊ�ߵ�ƽʱ,˵�����������Ϳ�ʼλ���߷���ֹͣλ*/
    {
        if(_strSSI2cCtrl->state == enSSI2c_Idle)/*����ڿ���״̬,Ϊ��ʼλ*/
        {
            _strSSI2cCtrl->BitIndex = 0;
            _strSSI2cCtrl->dataIndex = 0;
            /*�л�SDA�ж�ģʽλ�������ж�.�����ж��Ƿ���ֹͣλ*/
            Set_SDAIO_IRQ_RisingEdge();
            /*����SCL�ж�ģʽλ�������ж�.�������ж��ж�ȡ����д��bitλ����*/
            Set_SCLIO_IRQ_RisingEdge();
            _strSSI2cCtrl->readbuff = 0;
            _strSSI2cCtrl->state = enSSI2c_GetAddr;
        }
        else/*�ǿ���״̬.Ϊ����λ*/
        {
            Set_SDAIO_Level(enSSI2c_IOHigh_Level);
            if(_strSSI2cCtrl->ReadWriteflag == enSSI2c_Write)/*�ж��Ƿ�����д����*/
            {
                if(_strSSI2cCtrl->dataIndex == 1)/*�������д�������ֻ��һ���ֽ�.��ѷ��͵��������ַ�����鳤�ȸ�ֵ*/
                {
                    (*_strSSI2cCtrl->_Get_TdataCallback)(&(_strSSI2cCtrl->_arrTDataBuf), (uint16_t *)&_strSSI2cCtrl->TMaxDataLen , *(_strSSI2cCtrl->_arrRDataBuf));
                }
                else
                {
										/*_strSSI2cCtrl->dataIndex - 1 ��Ϊ�˼��� �Ĵ�����ַ����*/
                    (*_strSSI2cCtrl->_Set_RdataCallback)(_strSSI2cCtrl->_arrRDataBuf,(uint16_t) (_strSSI2cCtrl->dataIndex - 1));
                }
                if(_strSSI2cCtrl->state != enSSI2c_Stop)/*״̬�����ڵȴ�ֹͣλ,֤��д���ʱ������.*/
                {
                    _strSSI2cCtrl->HTimeoutRecovery = 0;
                }
            }
            else
            {
                /*������ȡ���,�������������ָ���ַ*/
                _strSSI2cCtrl->_arrTDataBuf = NULL; 
            }
            _strSSI2cCtrl->state = enSSI2c_Idle;
            /*�л�SDA�ж�ģʽλ�½����ж�.�����ж��Ƿ�����ʼλ*/
            Set_SDAIO_IRQ_DescendingEdge();
        }
    }
}


 /*
 * @brief  ģ��I2c�ӻ�������SDA�����жϻص�����
 * @param  *pCtrl:ģ��I2C�ӻ����ƽṹ��
 * @retval int err
 */
__attribute__((section("ram_code"))) void SSI2c_SCLIO_IRQHandler(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl)
{
    switch(_strSSI2cCtrl->state)
    {
        case enSSI2c_Idle:      /*���߿���״̬*/
            {
                /*SCL�������ж�*/
                /*�л�SDA�ж�ģʽλ�½����ж�.�����ж��Ƿ�����ʼλ.�ж���һ��I2Cͨ��*/
                Set_SDAIO_IRQ_DescendingEdge();
                _strSSI2cCtrl->ErrCode |= defSSI2cErr_NotCheckStartBit;
            }
            break;
        case enSSI2c_GetAddr:   /*��ȡ�豸��ַ�ź�*/
            {
                if(get_SCLIO_leven() == enSSI2c_IOHigh_Level)/*SCL�������ж�, ����SDA�ź�*/
                {
                    _strSSI2cCtrl->readbuff = _strSSI2cCtrl->readbuff << 1;
                    _strSSI2cCtrl->BitIndex++;
                    if(get_SDAIO_leven() == enSSI2c_IOHigh_Level)
                    {
                        _strSSI2cCtrl->readbuff |= 0x01;
                    }
                    if(_strSSI2cCtrl->BitIndex >= defSiumI2c_BitMAX)
                    {
												_strSSI2cCtrl->BitIndex = 0;
                        /*����SCL�ж�ģʽλ�½����ж�.�������ж����з���Ӧ���ź�*/
                        Set_SCLIO_IRQ_DescendingEdge();
                        if(_strSSI2cCtrl->slaveAddress != (_strSSI2cCtrl->readbuff & 0xFE))/*�Ǳ�����ַ*/
                        {
                            _strSSI2cCtrl->ErrCode |= defSSI2cErr_DrivedAddress;
                            _strSSI2cCtrl->state = enSSI2c_NotACK;
                        }
                        else/*������ַ*/
                        {
                            /*дӦ���ź�*/
                            _strSSI2cCtrl->state = enSSI2c_GetAddrACK;
                            if((_strSSI2cCtrl->readbuff & 0x01) == 0)/*����д����*/
                            {
                                _strSSI2cCtrl->ReadWriteflag = enSSI2c_Write;
                            }
                            else/*����������*/
                            {
                                if(_strSSI2cCtrl->_arrTDataBuf != NULL)/*�з��͵�����*/
                                {
                                    _strSSI2cCtrl->sendbuff = *(_strSSI2cCtrl->_arrTDataBuf);
                                    _strSSI2cCtrl->ReadWriteflag = enSSI2c_Read;
                                }
                                else/*û�л�ȡ���������*/
                                {
                                    _strSSI2cCtrl->state = enSSI2c_NotACK;
                                }
                            }
                        }
                    }
                }
                else
                {
                    /*����SCL�ж�ģʽλ�½����ж�.�������ж����з���Ӧ���źŻ��߲�Ӧ��*/
                    Set_SCLIO_IRQ_DescendingEdge();
                    _strSSI2cCtrl->ErrCode |= defSSI2cErr_SCLIrqMode;
                    _strSSI2cCtrl->state = enSSI2c_ERROR;
                }
            } 
            break;
        case enSSI2c_GetAddrACK:/*�ж��������͵��豸��ַ�Ƿ�λ���豸��ַ*/
            {
                if(get_SCLIO_leven() == enSSI2c_IOLow_Level)/*SCL�½����ж�*/
                {
                    /*SDA�������ACK�͵�ƽ�ź�*/
                    Set_SDAIO_Level(enSSI2c_IOLow_Level);
                    _strSSI2cCtrl->state = enSSI2c_WaitAddrACK;
                    /*����SCL�ж�ģʽλ�½����ж�.�������ж���,ȡ���ӻ�SDAIO��ACK�ͺ�,�ͷ�SDA����*/
                    Set_SCLIO_IRQ_DescendingEdge();
                }
                else
                {
                    /*����SCL�ж�ģʽλ�½����ж�.�������ж����з���Ӧ���źŻ��߲�Ӧ��*/
                    Set_SCLIO_IRQ_DescendingEdge();
                    _strSSI2cCtrl->ErrCode |= defSSI2cErr_SCLIrqMode;
                    _strSSI2cCtrl->state = enSSI2c_ERROR;
                }
            }
            break;
        case enSSI2c_WaitAddrACK:
            {
                if(get_SCLIO_leven() == enSSI2c_IOLow_Level)/*SCL�½����ж�*/
                {
                    /*Ӧ������ͷ�SDA����*/
                    Set_SDAIO_Level(enSSI2c_IOHigh_Level);
                    _strSSI2cCtrl->dataIndex = 0;
                    _strSSI2cCtrl->BitIndex = 0;
                    if(_strSSI2cCtrl->ReadWriteflag == enSSI2c_Write)/*����д����*/
                    {
                        /*����SCL�ж�ģʽλ�������ж�.�������ж��ж�ȡ��һ��byte��Bitλ����*/
                        Set_SCLIO_IRQ_RisingEdge();
                        _strSSI2cCtrl->state = enSSI2c_RData;
                        _strSSI2cCtrl->readbuff = 0;
                    }
                    else/*����������*/
                    {
                        /*���͵�һ���ֽڵ�Hbitλ*/
                        if((_strSSI2cCtrl->sendbuff & 0x80) == 0)
                        {
                            Set_SDAIO_Level(enSSI2c_IOLow_Level);
                        }
                        else
                        {
                            Set_SDAIO_Level(enSSI2c_IOHigh_Level);
                        }
                        /*����SCL�ж�ģʽλ�½����ж�.�������ж�д��bitλ����*/
                        Set_SCLIO_IRQ_DescendingEdge();
                        _strSSI2cCtrl->state = enSSI2c_TData;
                        _strSSI2cCtrl->BitIndex++;
                    }
                }
                else
                {
                    Set_SCLIO_IRQ_DescendingEdge();
                    _strSSI2cCtrl->ErrCode |= defSSI2cErr_SCLIrqMode;
                    _strSSI2cCtrl->state = enSSI2c_ERROR;
                }
            }
            break;
        case enSSI2c_RData:     /*�����������͵������ź�*/
            {
                if(get_SCLIO_leven() == enSSI2c_IOHigh_Level)/*SCL�������ж�,����SDAbitλ����*/
                {
                    _strSSI2cCtrl->readbuff = _strSSI2cCtrl->readbuff << 1;
                    _strSSI2cCtrl->BitIndex++;
                    if(get_SDAIO_leven() == enSSI2c_IOHigh_Level)
                    {
                        _strSSI2cCtrl->readbuff |= 0x01;
                    }
                    if(_strSSI2cCtrl->BitIndex >= defSiumI2c_BitMAX)/*������һ���ֽ�*/
                    {                  
                        /*����SCL�ж�ģʽλ�½����ж�.�������ж����з���Ӧ���źŻ��߲�Ӧ��*/
                        Set_SCLIO_IRQ_DescendingEdge();
                        if(_strSSI2cCtrl->dataIndex < _strSSI2cCtrl->RMaxDataLen)
                        {
                            _strSSI2cCtrl->state = enSSI2c_RDataACK;
                            *(_strSSI2cCtrl->_arrRDataBuf + _strSSI2cCtrl->dataIndex) = _strSSI2cCtrl->readbuff;
                        }
                        else
                        {
                            _strSSI2cCtrl->ErrCode |= defSSI2cErr_RDataLen;
                            _strSSI2cCtrl->state = enSSI2c_ERROR;
                        }  
                    }
                }
                else
                {
                    /*����SCL�ж�ģʽλ�½����ж�.�������ж����з���Ӧ���źŻ��߲�Ӧ��*/
                    Set_SCLIO_IRQ_DescendingEdge();
                    _strSSI2cCtrl->ErrCode |= defSSI2cErr_SCLIrqMode;
                    _strSSI2cCtrl->state = enSSI2c_ERROR;
                }
            } 
            break;
        case enSSI2c_RDataACK:  /*��ȡ�����źŵ�ACK�ͺ�*/
            {
                if(get_SCLIO_leven() == enSSI2c_IOLow_Level)/*SCL�½����ж�*/
                {
                    /*SDA�������ACK�͵�ƽ�ź�*/
                    Set_SDAIO_Level(enSSI2c_IOLow_Level);
                    /*����SCL�ж�ģʽλ�½����ж�.�������ж���,ȡ���ӻ�SDAIO��ACK�ͺ�,�ͷ�SDA����*/
                    Set_SCLIO_IRQ_DescendingEdge();
                    _strSSI2cCtrl->state = enSSI2c_WaitRDataACK;
                    /*��һ�������ֽڵĽ���λ��ƫ����*/
                }
                else
                {
                    Set_SCLIO_IRQ_DescendingEdge();
                    _strSSI2cCtrl->ErrCode |= defSSI2cErr_SCLIrqMode;
                    _strSSI2cCtrl->state = enSSI2c_ERROR;
                }
            }
            break;
        case enSSI2c_WaitRDataACK:
            {
                if(get_SCLIO_leven() == enSSI2c_IOLow_Level)/*SCL�½����ж�*/
                {
                    /*Ӧ������ͷ�SDA����*/
                    Set_SDAIO_Level(enSSI2c_IOHigh_Level);
                    _strSSI2cCtrl->BitIndex = 0;
                    /*����SCL�ж�ģʽλ������ģʽ�ж�,׼����ȡ��һ��byte������*/
                    Set_SCLIO_IRQ_RisingEdge();
                    _strSSI2cCtrl->state = enSSI2c_RData;
                    /*��һ�������ֽڵĽ���λ��ƫ����*/
                    _strSSI2cCtrl->dataIndex++;
                }
                else
                {
                    Set_SCLIO_IRQ_DescendingEdge();
                    _strSSI2cCtrl->ErrCode |= defSSI2cErr_SCLIrqMode;
                    _strSSI2cCtrl->state = enSSI2c_ERROR;
                }
            }
            break;
        case enSSI2c_TData:     /*�������ݸ�����*/
            {
                if(get_SCLIO_leven() == enSSI2c_IOLow_Level)/*SCL�����ж�,��������SDAbitλ����*/
                {
                    if(_strSSI2cCtrl->BitIndex < defSiumI2c_BitMAX)/*δ������8bit����*/
                    {
                        if(((_strSSI2cCtrl->sendbuff << _strSSI2cCtrl->BitIndex) & 0x80) == 0)
                        {
                            Set_SDAIO_Level(enSSI2c_IOLow_Level);
                        }
                        else
                        {
                            Set_SDAIO_Level(enSSI2c_IOHigh_Level);
                        }
                        _strSSI2cCtrl->BitIndex++;
                        if(_strSSI2cCtrl->BitIndex == defSiumI2c_BitMAX)/*������8bit����*/
                        {
                            _strSSI2cCtrl->state = enSSI2c_TDataACK;
                        }
                    }
                }
                else
                {
                    _strSSI2cCtrl->ErrCode |= defSSI2cErr_SCLIrqMode;
                    _strSSI2cCtrl->state = enSSI2c_ERROR;
                }
            } 
            break;
        case enSSI2c_TDataACK:  /*�������ݸ�������ACK�ź�*/
            {
                if(get_SCLIO_leven() == enSSI2c_IOLow_Level)/*SCL�½����ж�*/
                {
                    /*��һ�������ֽڵĽ���λ��ƫ����*/
                    _strSSI2cCtrl->dataIndex++;
                    if(_strSSI2cCtrl->dataIndex < _strSSI2cCtrl->TMaxDataLen)
                    {
                        /*SDA�������ACK�͵�ƽ�ź�*/
                        Set_SDAIO_Level(enSSI2c_IOLow_Level);
                        /*����SCL�ж�ģʽλ���½���,�ȴ�ACK����*/
                        Set_SCLIO_IRQ_DescendingEdge();
                        _strSSI2cCtrl->state = enSSI2c_WaitTDataACK;
                        _strSSI2cCtrl->sendbuff = *(_strSSI2cCtrl->_arrTDataBuf + _strSSI2cCtrl->dataIndex);
                    }
                    else
                    {
                        /*SDA��������ߵ�ƽ,����Ӧ���ź�*/
                        Set_SDAIO_Level(enSSI2c_IOHigh_Level);
                        _strSSI2cCtrl->state = enSSI2c_NotACK;
                    }                        
                }
                else
                {
                    _strSSI2cCtrl->ErrCode |= defSSI2cErr_SCLIrqMode;
                    _strSSI2cCtrl->state = enSSI2c_ERROR;
                }
            }
            break;
        case enSSI2c_WaitTDataACK:
            {
                if(get_SCLIO_leven() == enSSI2c_IOLow_Level)/*SCL�½����ж�*/
                {
                    /*������һ��byte��Hbitλ*/
                    if((_strSSI2cCtrl->sendbuff & 0x80) == 0)
                    {
                        Set_SDAIO_Level(enSSI2c_IOLow_Level);
                    }
                    else
                    {
                        Set_SDAIO_Level(enSSI2c_IOHigh_Level);
                    }
                    /*����SCL�ж�ģʽλ�½����ж�.�������ж�д��bitλ����*/
                    Set_SCLIO_IRQ_DescendingEdge();
                    _strSSI2cCtrl->state = enSSI2c_TData;
                    _strSSI2cCtrl->BitIndex = 1;                       
                }
                else
                {
                    /*����SCL�ж�ģʽλ�½����ж�*/
                    Set_SCLIO_IRQ_DescendingEdge();
                    _strSSI2cCtrl->ErrCode |= defSSI2cErr_SCLIrqMode;
                    _strSSI2cCtrl->state = enSSI2c_ERROR;
                }
            }
            break;
        case enSSI2c_NotACK:    /*���������豸ACK�ź�*/
            {
                if(get_SCLIO_leven() == enSSI2c_IOLow_Level)/*SCL�½����ж�*/
                {
                    Set_SDAIO_Level(enSSI2c_IOHigh_Level);
                    _strSSI2cCtrl->state = enSSI2c_Stop;
                }
                else
                {                
                    /*����SDA�ж�ģʽλ�½����ж�.�����ж��Ƿ�����ʼλ*/
                    Set_SCLIO_IRQ_DescendingEdge();
                }
            }
            break;
        case enSSI2c_Stop:      /*�ȴ���ַ�ź�*/
            {
                if(get_SCLIO_leven() == enSSI2c_IOLow_Level)/*SCL�½����ж�*/
                {
                    Set_SDAIO_Level(enSSI2c_IOHigh_Level);
                }
                else
                {                
                    /*����SDA�ж�ģʽλ�½����ж�.�����ж��Ƿ�����ʼλ*/
										Set_SCLIO_IRQ_RisingEdge();
                }
            }
            break;
        case enSSI2c_ERROR:     /*����ʱ�����*/
            {
                if(get_SCLIO_leven() == enSSI2c_IOLow_Level)/*SCL�½����ж�*/
                {
                    Set_SDAIO_Level(enSSI2c_IOHigh_Level);
                    _strSSI2cCtrl->state = enSSI2c_Stop;
                }
                else
                {                
                    /*����SDA�ж�ģʽλ�½����ж�.�����ж��Ƿ�����ʼλ*/
                    Set_SCLIO_IRQ_DescendingEdge();
                }
            }
            break;
        default: 
            _strSSI2cCtrl->state = enSSI2c_ERROR;
            break;
    }

    /*������ڵȴ�Stop�ź�,�����ʱ�������*/
    if (_strSSI2cCtrl->state != enSSI2c_Stop)
    {
        _strSSI2cCtrl->TimerMs = 0;
    }
}



 /*
 * @brief  ģ��I2c�ӻ������Ķ�ʱ���жϻص�����
 * @param  *pCtrl:ģ��I2C�ӻ����ƽṹ��
 * @retval int err
 */
__attribute__((section("ram_code"))) void SSI2c_Timer_IRQHandler(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl)
{
    if(_strSSI2cCtrl->state != enSSI2c_Idle)
    {
        if(_strSSI2cCtrl->TimerMs < _strSSI2cCtrl->MaxTimerMs)
        {
            _strSSI2cCtrl->TimerMs++;
        }
        else
        {
            Set_SDAIO_Level(enSSI2c_IOHigh_Level);
            _strSSI2cCtrl->state = enSSI2c_Idle;
            _strSSI2cCtrl->TimerMs = 0;
						_strSSI2cCtrl->ErrCode |= defSSI2cErr_HaetTimerOut;
        }
    }        
}


 /*
 * @brief  ģ��I2c������������ʱ��ʱ��Add�Ļص�����
 * @param  *pCtrl:ģ��I2C�ӻ����ƽṹ��
 * @retval int err
 */
void Add_SSI2c_HTimeoutRecovery_cb(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl , uint32_t Time1Value)
{
    if(defSSI2c_HeartbeatTimeoutRecovery)
    {
        _strSSI2cCtrl->HTimeoutRecovery += Time1Value;
    }
}

