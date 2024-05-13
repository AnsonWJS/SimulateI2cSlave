/**
 * @file SimulateI2cSlave.c
 * @brief 模拟I2C从机操作的实现源文件。
 *
 * @version 1.0.0
 * @date 2024-04-28
 *
 * @copyright Copyright (c) 2024, wujingsheng
 * This source code is released under the MIT License.
 *
 * @author wujingsheng
 *
 * @brief 本文件实现了`simulate_i2c_slave.h`中声明的函数，提供了在无硬件支持下模拟I2C从机行为的能力。
 * 
 * 注意事项:
 * - 该模拟i2c从机程序,必须开启SCL和SDA引脚的的中断功能.(且中断边缘中断需要在通信过程中修改)
 *      -SSI2c_SDAIO_IRQHandler()需要放在SDA硬件引脚的中断服务函数中
 *      -SSI2c_SCLIO_IRQHandler()需要放在SCL硬件引脚的中断服务函数中
 * - 该模拟i2c从机程序,可以选择是否需要外部定时器计数是否超时,判断是否恢复I2C通信状态到Idle.
 *      -SSI2c_Timer_IRQHandler()需要放到定时器中,定时判断.
 * 版本兼容性说明:
 * 1.0.0: 初始版本，实现了基本的I2C从机模拟功能。
 */

#include "SimulateI2c_S.h"


/*
 * @brief  初始化模拟I2C从机
 * @param  *pCtrl:模拟I2C从机控制结构体
 * @retval int err 
 */
int SimulateI2cSlaveStruct_Init(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl)
{
    int err = defErrOK;

    /*主机的读写标志位, = 0 为主机写 = 1 为主机读*/
    _strSSI2cCtrl->ReadWriteflag = enSSI2c_Write;
    /*模拟I2C从机的接收状态*/
    _strSSI2cCtrl->state = enSSI2c_Idle;
    /*模拟I2C从机的读写为位置*/
    _strSSI2cCtrl->BitIndex = 0;
    /*时间计数器.用于定时器计数器,单位(ms)*/
    _strSSI2cCtrl->TimerMs = 0;
    /*数据数据引索位置*/
    _strSSI2cCtrl->dataIndex = 0;

    _strSSI2cCtrl->ErrCode = 0;

    _strSSI2cCtrl->readbuff = 0;

    _strSSI2cCtrl->sendbuff = 0;

    _strSSI2cCtrl->HTimeoutRecovery = 0;

    return err;
}


/*
 * @brief  设置模拟I2C从机设备的地址
 * @param  *pCtrl:模拟I2C从机控制结构体
 * @param  slaveAddress:从机地址
 * @retval int err
 */
int Set_SimulateI2cSlave_address(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl, uint8_t slaveAddress)
{
    int err = defErrOK;
    
    /*主机的读写标志位, = 0 为主机写 = 1 为主机读*/
    _strSSI2cCtrl->slaveAddress = slaveAddress;

    return err;
}


 /*
 * @brief  设置模拟I2C从机设备的读取数据的缓冲区,和缓冲区大小
 * @param  *pCtrl:模拟I2C从机控制结构体
 * @param  _RDataAddress:   读取数据地址
 * @param  RdataMaxLen:     最大读取长度  
 * @retval int err
 */
int Set_SimulateI2cSlave_RDataPram(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl, uint8_t *_RDataAddress, uint16_t RdataMaxLen)
{
    int err = defErrOK;
    
    /*主机的读写标志位, = 0 为主机写 = 1 为主机读*/
    _strSSI2cCtrl->_arrRDataBuf = _RDataAddress;

    _strSSI2cCtrl->RMaxDataLen = RdataMaxLen;

    return err;
}



 /*
 * @brief  设置模拟I2C从机设备的发送数据的缓冲区,和可发送数据大小
 * @param  *pCtrl:模拟I2C从机控制结构体
 * @param  _TDataAddress:   发送数据地址
 * @param  TdataMaxLen:     最大读取长度  T
 * @retval int err
 */
__attribute__((section("ram_code"))) int Set_SimulateI2cSlave_TDataPram(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl, uint8_t * _TDataAddress, uint16_t TdataMaxLen)
{
    int err = defErrOK;

    /*发送缓存区赋值*/
    _strSSI2cCtrl->_arrTDataBuf = _TDataAddress;

    _strSSI2cCtrl->TMaxDataLen = TdataMaxLen;

    return err;
}



 /*
 * @brief  开始模拟I2c从机工作
 * @param  *pCtrl:模拟I2C从机控制结构体
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

    /*设置SDA中断模式位下降沿中断.用于判断是否有起始位*/
    Set_SDAIO_IRQ_DescendingEdge();
    /*设置SCL中断模式位上升沿中断.用于在中断中读取或者写入bit位数据*/
    Set_SCLIO_IRQ_RisingEdge();

    ENABLE_I2C_GIOP_TIME_IRQ();

    return err;
}



 /*
 * @brief  停止模拟I2c从机工作
 * @param  *pCtrl:模拟I2C从机控制结构体
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
 * @brief  模拟I2c从机工作的SDA引脚中断回调函数
 * @param  *pCtrl:模拟I2C从机控制结构体
 * @retval int err
 */
__attribute__((section("ram_code"))) void SSI2c_SDAIO_IRQHandler(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl)
{
    if(get_SCLIO_leven() == enSSI2c_IOHigh_Level)/*SDA引脚改变状态,在SCL引脚为高电平时,说明是主机发送开始位或者发送停止位*/
    {
        if(_strSSI2cCtrl->state == enSSI2c_Idle)/*如果在空闲状态,为起始位*/
        {
            _strSSI2cCtrl->BitIndex = 0;
            _strSSI2cCtrl->dataIndex = 0;
            /*切换SDA中断模式位上升沿中断.用于判断是否有停止位*/
            Set_SDAIO_IRQ_RisingEdge();
            /*设置SCL中断模式位上升沿中断.用于在中断中读取或者写入bit位数据*/
            Set_SCLIO_IRQ_RisingEdge();
            _strSSI2cCtrl->readbuff = 0;
            _strSSI2cCtrl->state = enSSI2c_GetAddr;
        }
        else/*非空闲状态.为结束位*/
        {
            Set_SDAIO_Level(enSSI2c_IOHigh_Level);
            if(_strSSI2cCtrl->ReadWriteflag == enSSI2c_Write)/*判断是否主机写操作*/
            {
                if(_strSSI2cCtrl->dataIndex == 1)/*如果主机写入的数据只有一个字节.则把发送的数据组地址和数组长度赋值*/
                {
                    (*_strSSI2cCtrl->_Get_TdataCallback)(&(_strSSI2cCtrl->_arrTDataBuf), (uint16_t *)&_strSSI2cCtrl->TMaxDataLen , *(_strSSI2cCtrl->_arrRDataBuf));
                }
                else
                {
										/*_strSSI2cCtrl->dataIndex - 1 是为了减掉 寄存器地址长度*/
                    (*_strSSI2cCtrl->_Set_RdataCallback)(_strSSI2cCtrl->_arrRDataBuf,(uint16_t) (_strSSI2cCtrl->dataIndex - 1));
                }
                if(_strSSI2cCtrl->state != enSSI2c_Stop)/*状态不等于等待停止位,证明写入的时序正常.*/
                {
                    _strSSI2cCtrl->HTimeoutRecovery = 0;
                }
            }
            else
            {
                /*主机读取完成,清除发送数据组指针地址*/
                _strSSI2cCtrl->_arrTDataBuf = NULL; 
            }
            _strSSI2cCtrl->state = enSSI2c_Idle;
            /*切换SDA中断模式位下降沿中断.用于判断是否有起始位*/
            Set_SDAIO_IRQ_DescendingEdge();
        }
    }
}


 /*
 * @brief  模拟I2c从机工作的SDA引脚中断回调函数
 * @param  *pCtrl:模拟I2C从机控制结构体
 * @retval int err
 */
__attribute__((section("ram_code"))) void SSI2c_SCLIO_IRQHandler(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl)
{
    switch(_strSSI2cCtrl->state)
    {
        case enSSI2c_Idle:      /*总线空闲状态*/
            {
                /*SCL上升沿中断*/
                /*切换SDA中断模式位下降沿中断.用于判断是否有起始位.判断下一次I2C通信*/
                Set_SDAIO_IRQ_DescendingEdge();
                _strSSI2cCtrl->ErrCode |= defSSI2cErr_NotCheckStartBit;
            }
            break;
        case enSSI2c_GetAddr:   /*获取设备地址信号*/
            {
                if(get_SCLIO_leven() == enSSI2c_IOHigh_Level)/*SCL上升沿中断, 接收SDA信号*/
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
                        /*设置SCL中断模式位下降沿中断.用于在中断中中发送应答信号*/
                        Set_SCLIO_IRQ_DescendingEdge();
                        if(_strSSI2cCtrl->slaveAddress != (_strSSI2cCtrl->readbuff & 0xFE))/*非本机地址*/
                        {
                            _strSSI2cCtrl->ErrCode |= defSSI2cErr_DrivedAddress;
                            _strSSI2cCtrl->state = enSSI2c_NotACK;
                        }
                        else/*本机地址*/
                        {
                            /*写应答信号*/
                            _strSSI2cCtrl->state = enSSI2c_GetAddrACK;
                            if((_strSSI2cCtrl->readbuff & 0x01) == 0)/*主机写操作*/
                            {
                                _strSSI2cCtrl->ReadWriteflag = enSSI2c_Write;
                            }
                            else/*主机读操作*/
                            {
                                if(_strSSI2cCtrl->_arrTDataBuf != NULL)/*有发送的数据*/
                                {
                                    _strSSI2cCtrl->sendbuff = *(_strSSI2cCtrl->_arrTDataBuf);
                                    _strSSI2cCtrl->ReadWriteflag = enSSI2c_Read;
                                }
                                else/*没有获取到相关数据*/
                                {
                                    _strSSI2cCtrl->state = enSSI2c_NotACK;
                                }
                            }
                        }
                    }
                }
                else
                {
                    /*设置SCL中断模式位下降沿中断.用于在中断中中发送应答信号或者不应答*/
                    Set_SCLIO_IRQ_DescendingEdge();
                    _strSSI2cCtrl->ErrCode |= defSSI2cErr_SCLIrqMode;
                    _strSSI2cCtrl->state = enSSI2c_ERROR;
                }
            } 
            break;
        case enSSI2c_GetAddrACK:/*判断主机发送的设备地址是否位本设备地址*/
            {
                if(get_SCLIO_leven() == enSSI2c_IOLow_Level)/*SCL下降沿中断*/
                {
                    /*SDA总线输出ACK低电平信号*/
                    Set_SDAIO_Level(enSSI2c_IOLow_Level);
                    _strSSI2cCtrl->state = enSSI2c_WaitAddrACK;
                    /*设置SCL中断模式位下降沿中断.用于在中断中,取消从机SDAIO的ACK型号,释放SDA总线*/
                    Set_SCLIO_IRQ_DescendingEdge();
                }
                else
                {
                    /*设置SCL中断模式位下降沿中断.用于在中断中中发送应答信号或者不应答*/
                    Set_SCLIO_IRQ_DescendingEdge();
                    _strSSI2cCtrl->ErrCode |= defSSI2cErr_SCLIrqMode;
                    _strSSI2cCtrl->state = enSSI2c_ERROR;
                }
            }
            break;
        case enSSI2c_WaitAddrACK:
            {
                if(get_SCLIO_leven() == enSSI2c_IOLow_Level)/*SCL下降沿中断*/
                {
                    /*应答接收释放SDA总线*/
                    Set_SDAIO_Level(enSSI2c_IOHigh_Level);
                    _strSSI2cCtrl->dataIndex = 0;
                    _strSSI2cCtrl->BitIndex = 0;
                    if(_strSSI2cCtrl->ReadWriteflag == enSSI2c_Write)/*主机写数据*/
                    {
                        /*设置SCL中断模式位上升沿中断.用于在中断中读取下一个byte的Bit位数据*/
                        Set_SCLIO_IRQ_RisingEdge();
                        _strSSI2cCtrl->state = enSSI2c_RData;
                        _strSSI2cCtrl->readbuff = 0;
                    }
                    else/*主机读数据*/
                    {
                        /*发送第一个字节的Hbit位*/
                        if((_strSSI2cCtrl->sendbuff & 0x80) == 0)
                        {
                            Set_SDAIO_Level(enSSI2c_IOLow_Level);
                        }
                        else
                        {
                            Set_SDAIO_Level(enSSI2c_IOHigh_Level);
                        }
                        /*设置SCL中断模式位下降沿中断.用于在中断写入bit位数据*/
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
        case enSSI2c_RData:     /*接收主机发送的数据信号*/
            {
                if(get_SCLIO_leven() == enSSI2c_IOHigh_Level)/*SCL上升沿中断,接收SDAbit位数据*/
                {
                    _strSSI2cCtrl->readbuff = _strSSI2cCtrl->readbuff << 1;
                    _strSSI2cCtrl->BitIndex++;
                    if(get_SDAIO_leven() == enSSI2c_IOHigh_Level)
                    {
                        _strSSI2cCtrl->readbuff |= 0x01;
                    }
                    if(_strSSI2cCtrl->BitIndex >= defSiumI2c_BitMAX)/*接收完一个字节*/
                    {                  
                        /*设置SCL中断模式位下降沿中断.用于在中断中中发送应答信号或者不应答*/
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
                    /*设置SCL中断模式位下降沿中断.用于在中断中中发送应答信号或者不应答*/
                    Set_SCLIO_IRQ_DescendingEdge();
                    _strSSI2cCtrl->ErrCode |= defSSI2cErr_SCLIrqMode;
                    _strSSI2cCtrl->state = enSSI2c_ERROR;
                }
            } 
            break;
        case enSSI2c_RDataACK:  /*读取数据信号的ACK型号*/
            {
                if(get_SCLIO_leven() == enSSI2c_IOLow_Level)/*SCL下降沿中断*/
                {
                    /*SDA总线输出ACK低电平信号*/
                    Set_SDAIO_Level(enSSI2c_IOLow_Level);
                    /*设置SCL中断模式位下降沿中断.用于在中断中,取消从机SDAIO的ACK型号,释放SDA总线*/
                    Set_SCLIO_IRQ_DescendingEdge();
                    _strSSI2cCtrl->state = enSSI2c_WaitRDataACK;
                    /*下一个接收字节的接收位置偏移量*/
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
                if(get_SCLIO_leven() == enSSI2c_IOLow_Level)/*SCL下降沿中断*/
                {
                    /*应答接收释放SDA总线*/
                    Set_SDAIO_Level(enSSI2c_IOHigh_Level);
                    _strSSI2cCtrl->BitIndex = 0;
                    /*设置SCL中断模式位上升沿模式中断,准备读取下一个byte的数据*/
                    Set_SCLIO_IRQ_RisingEdge();
                    _strSSI2cCtrl->state = enSSI2c_RData;
                    /*下一个接收字节的接收位置偏移量*/
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
        case enSSI2c_TData:     /*发送数据给主机*/
            {
                if(get_SCLIO_leven() == enSSI2c_IOLow_Level)/*SCL下沿中断,主机接收SDAbit位数据*/
                {
                    if(_strSSI2cCtrl->BitIndex < defSiumI2c_BitMAX)/*未发送完8bit数据*/
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
                        if(_strSSI2cCtrl->BitIndex == defSiumI2c_BitMAX)/*发送完8bit数据*/
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
        case enSSI2c_TDataACK:  /*发送数据给主机的ACK信号*/
            {
                if(get_SCLIO_leven() == enSSI2c_IOLow_Level)/*SCL下降沿中断*/
                {
                    /*下一个接收字节的接收位置偏移量*/
                    _strSSI2cCtrl->dataIndex++;
                    if(_strSSI2cCtrl->dataIndex < _strSSI2cCtrl->TMaxDataLen)
                    {
                        /*SDA总线输出ACK低电平信号*/
                        Set_SDAIO_Level(enSSI2c_IOLow_Level);
                        /*设置SCL中断模式位上下降沿,等待ACK接收*/
                        Set_SCLIO_IRQ_DescendingEdge();
                        _strSSI2cCtrl->state = enSSI2c_WaitTDataACK;
                        _strSSI2cCtrl->sendbuff = *(_strSSI2cCtrl->_arrTDataBuf + _strSSI2cCtrl->dataIndex);
                    }
                    else
                    {
                        /*SDA总线输出高电平,不给应答信号*/
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
                if(get_SCLIO_leven() == enSSI2c_IOLow_Level)/*SCL下降沿中断*/
                {
                    /*发送下一个byte的Hbit位*/
                    if((_strSSI2cCtrl->sendbuff & 0x80) == 0)
                    {
                        Set_SDAIO_Level(enSSI2c_IOLow_Level);
                    }
                    else
                    {
                        Set_SDAIO_Level(enSSI2c_IOHigh_Level);
                    }
                    /*设置SCL中断模式位下降沿中断.用于在中断写入bit位数据*/
                    Set_SCLIO_IRQ_DescendingEdge();
                    _strSSI2cCtrl->state = enSSI2c_TData;
                    _strSSI2cCtrl->BitIndex = 1;                       
                }
                else
                {
                    /*设置SCL中断模式位下降沿中断*/
                    Set_SCLIO_IRQ_DescendingEdge();
                    _strSSI2cCtrl->ErrCode |= defSSI2cErr_SCLIrqMode;
                    _strSSI2cCtrl->state = enSSI2c_ERROR;
                }
            }
            break;
        case enSSI2c_NotACK:    /*不给主机设备ACK信号*/
            {
                if(get_SCLIO_leven() == enSSI2c_IOLow_Level)/*SCL下降沿中断*/
                {
                    Set_SDAIO_Level(enSSI2c_IOHigh_Level);
                    _strSSI2cCtrl->state = enSSI2c_Stop;
                }
                else
                {                
                    /*设置SDA中断模式位下降沿中断.用于判断是否有起始位*/
                    Set_SCLIO_IRQ_DescendingEdge();
                }
            }
            break;
        case enSSI2c_Stop:      /*等待地址信号*/
            {
                if(get_SCLIO_leven() == enSSI2c_IOLow_Level)/*SCL下降沿中断*/
                {
                    Set_SDAIO_Level(enSSI2c_IOHigh_Level);
                }
                else
                {                
                    /*设置SDA中断模式位下降沿中断.用于判断是否有起始位*/
										Set_SCLIO_IRQ_RisingEdge();
                }
            }
            break;
        case enSSI2c_ERROR:     /*发送时序错误*/
            {
                if(get_SCLIO_leven() == enSSI2c_IOLow_Level)/*SCL下降沿中断*/
                {
                    Set_SDAIO_Level(enSSI2c_IOHigh_Level);
                    _strSSI2cCtrl->state = enSSI2c_Stop;
                }
                else
                {                
                    /*设置SDA中断模式位下降沿中断.用于判断是否有起始位*/
                    Set_SCLIO_IRQ_DescendingEdge();
                }
            }
            break;
        default: 
            _strSSI2cCtrl->state = enSSI2c_ERROR;
            break;
    }

    /*如果不在等待Stop信号,就清空时间计数器*/
    if (_strSSI2cCtrl->state != enSSI2c_Stop)
    {
        _strSSI2cCtrl->TimerMs = 0;
    }
}



 /*
 * @brief  模拟I2c从机工作的定时器中断回调函数
 * @param  *pCtrl:模拟I2C从机控制结构体
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
 * @brief  模拟I2c与主机心跳超时的时钟Add的回调函数
 * @param  *pCtrl:模拟I2C从机控制结构体
 * @retval int err
 */
void Add_SSI2c_HTimeoutRecovery_cb(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl , uint32_t Time1Value)
{
    if(defSSI2c_HeartbeatTimeoutRecovery)
    {
        _strSSI2cCtrl->HTimeoutRecovery += Time1Value;
    }
}

