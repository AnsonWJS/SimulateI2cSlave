/**
 * @file SimuI2cSlaveDataList.c
 * @brief 模拟I2c从机寄存器数据列表的控制
 *
 * @version 1.0.0
 * @date 2024-04-30
 *
 * @copyright Copyright (c) 2024, wujingsheng
 * This source code is released under the MIT License.
 *
 * @author wujingsheng
 *
 * @brief 本文件实现了模拟I2c从机寄存器数据列表的控制. 用于与主机进行通信时.数据的赋值与发送.
 * 
 * 
 *  * 注意事项:
 *  该函数没有对数据作互锁.所以在从机和主机通信对某个寄存器操作时.会存在读写的冲突.
 */

#include "SimulateI2c_S.h"

structSimulateI2c_DataList strSSI2cDataList;
extern const unsigned char ArrOldCodeVers[8];


/*
 * @brief  CRC16循环冗余校验算法的 采用CRC-CCITT（Xmodem）多项式（0x1021)
 * @param  const uint8_t*  数据指针
 * @param  size_t length   读到数据的数组首地址
 * @retval uint16_t        校验值 
 */
uint16_t calculateCrc16(const uint8_t* _data, uint32_t length) 
{
    uint16_t crc = CRC16_INIT;
    for (uint32_t i = 0; i < length; ++i) 
    {
        crc ^= (uint16_t)_data[i] << 8;
        for (int j = 0; j < 8; ++j) 
        {
            if (crc & 0x8000) 
            {
                crc = (crc << 1) ^ CRC16_POLYNOMIAL;
            } 
            else 
            {
                crc <<= 1;
            }
        }
    }
    return crc;
}

/*
 * @brief  获取crc16的校验结果
 * @param  const uint8_t*  数据指针
 * @param  size_t length   读到数据的数组首地址
 * @retval uint16_t        = 0校验成功 否则校验失败  
 */
int outputCrc16(const uint8_t* _data, uint32_t length)
{
    int err = defErrOK;
    uint16_t crcvalue = 0xFFFF;
    if(length >= 3)
    {
        crcvalue = calculateCrc16(_data , (length - 2));
        if(crcvalue != *((unsigned short *)(_data + length - 2)))
        {
            err = -2;
        }
    }
    else
    {
        err = -1;
    }
    return err;
}



/*
 * @brief  初始化模拟I2C从机的寄存器数据
 * @param  *pCtrl:模拟I2C从机控制结构体
 * @retval int err 
 */
void SimuLateI2c_RegData_Init(structSimulateI2c_DataList * _strSimuI2cDataList)
{    
	structI2cRegAdd_Version * _strVer;
	structI2cRegAdd_Systime * _strSystime;
	structI2cRegAdd_adjGateZeroMode *_strAdj;
	structI2cRegAdd_LedCtrl * _strLedCtrl;

	if (_strSimuI2cDataList != NULL)
	{   
			if(_strSimuI2cDataList->initFalg != 1)
			{
					_strVer = (structI2cRegAdd_Version *)&_strSimuI2cDataList->Regaddr0[0];
					_strVer->version[0] = ArrOldCodeVers[0];
					_strVer->version[1] = ArrOldCodeVers[1];
					_strVer->version[2] = ArrOldCodeVers[2];
					_strVer->version[3] = ArrOldCodeVers[3];
					_strVer->crc16value = calculateCrc16(&_strSimuI2cDataList->Regaddr0[0] , (sizeof(structI2cRegAdd_Version) - 2));

					_strSystime = (structI2cRegAdd_Systime *)&_strSimuI2cDataList->Regaddr1[0];
					_strSystime->systime = 0;
					_strSystime->crc16valueErr = 0;
					_strSystime->crc16value = calculateCrc16(&_strSimuI2cDataList->Regaddr1[0] , (sizeof(structI2cRegAdd_Systime) - 2));

					_strAdj = (structI2cRegAdd_adjGateZeroMode *)&_strSimuI2cDataList->Regaddr2[0];
					_strAdj->adjGateZeroMode = 0;
					_strAdj->crc16value = calculateCrc16(&_strSimuI2cDataList->Regaddr2[0] ,(sizeof(structI2cRegAdd_adjGateZeroMode) - 2));

					_strLedCtrl = (structI2cRegAdd_LedCtrl *)&_strSimuI2cDataList->Regaddr3[0];
					_strLedCtrl->mode = 0;   
					_strLedCtrl->RLedvalue = 0;
					_strLedCtrl->GLedvalue = 0;
					_strLedCtrl->BLedvalue = 0;
					_strLedCtrl->OnTime = 0;
					_strLedCtrl->OffTime = 0;
					_strLedCtrl->CyclesTimes = 0;  
					_strLedCtrl->crc16value = calculateCrc16(&_strSimuI2cDataList->Regaddr3[0] , (sizeof(structI2cRegAdd_LedCtrl) - 2));

				 _strSimuI2cDataList->initFalg = 1;
			}
	}
}



/*
 * @brief  初始化模拟I2C从机的寄存器数据和列表控制结构体
 * @param  *pCtrl:模拟I2C从机控制结构体
 * @retval int err 
 */
void SimuLateI2c_DataList_Init(structSimulateI2c_DataList * _strSimuI2cDataList)
{
    if (_strSimuI2cDataList != NULL)
    {
        _strSimuI2cDataList->RegDataLen[enI2cRegAdd_Version] = sizeof(structI2cRegAdd_Version);
        _strSimuI2cDataList->RegDataLen[enI2cRegAdd_SysParm] = sizeof(structI2cRegAdd_Systime);
        _strSimuI2cDataList->RegDataLen[enI2cRegAdd_adjGateZeroMode] = sizeof(structI2cRegAdd_adjGateZeroMode);
        _strSimuI2cDataList->RegDataLen[enI2cRegAdd_LedCtrl] = sizeof(structI2cRegAdd_LedCtrl);

        _strSimuI2cDataList->_RegData[enI2cRegAdd_Version] = _strSimuI2cDataList->Regaddr0;
        _strSimuI2cDataList->_RegData[enI2cRegAdd_SysParm] = _strSimuI2cDataList->Regaddr1;
        _strSimuI2cDataList->_RegData[enI2cRegAdd_adjGateZeroMode] = _strSimuI2cDataList->Regaddr2;
        _strSimuI2cDataList->_RegData[enI2cRegAdd_LedCtrl] = _strSimuI2cDataList->Regaddr3;

        SimuLateI2c_RegData_Init(_strSimuI2cDataList);
    }
}



/*
 * @brief  把需要的回复主机的数据指针赋值,
 * @param  uint8_t **_Tdata需要发送的数据的指针的指针地址
 * @param  uint16_t * Len  需要回复的数据长度
 * @retval int err 
 */
__attribute__((section("ram_code"))) void SimuLateI2c_Get_TdataCallback_CB(uint8_t **_Tdata, uint16_t * Len ,uint8_t RegAddrVal)
{
    switch (RegAddrVal)
    {
        case enI2cRegAdd_Version:
            {
                * Len = sizeof(structI2cRegAdd_Version);
                *_Tdata = strSSI2cDataList.Regaddr0;
            } 
            break;
        case enI2cRegAdd_SysParm:
            {
                * Len = sizeof(structI2cRegAdd_Systime);
                *_Tdata = strSSI2cDataList.Regaddr1;
            }
            break;
        case enI2cRegAdd_adjGateZeroMode:
            {
                * Len = sizeof(structI2cRegAdd_adjGateZeroMode);
                *_Tdata = strSSI2cDataList.Regaddr2;                
            }
            break;
        case enI2cRegAdd_LedCtrl:
            {
                * Len = sizeof(structI2cRegAdd_LedCtrl);
                *_Tdata = strSSI2cDataList.Regaddr3;                
            }
            break;
        default:
            {
                * Len = 0;
                *_Tdata = NULL;
            }
            break;
    }
}



/*
 * @brief  把主机发送过来的数据赋值,
 * @param  uint8_t *_Rdata   读到数据的数组首地址
 * @param  uint16_t * Len    读到的数据长度
 * @retval int err 
 */
__attribute__((section("ram_code"))) void SimuLateI2c_Set_RdataCallback_CB(uint8_t *_Rdata, uint16_t  Len)
{
    uint32_t dataindex;
    uint8_t * _arraddr = NULL;
		uint8_t * _rdataaddr = NULL;
    switch (*(_Rdata))
    {
        case enI2cRegAdd_Version:
            {
                if(Len <= sizeof(structI2cRegAdd_Version))
                {
									 /*该命令,只读*/
                   _arraddr = NULL;
                }
            } 
            break;
        case enI2cRegAdd_SysParm:
            {
                if(Len <= sizeof(structI2cRegAdd_Systime))
                {
									 /*该命令,只读*/
                   _arraddr = NULL;
                }
            }
            break;
        case enI2cRegAdd_adjGateZeroMode:
            {
                if(Len <= sizeof(structI2cRegAdd_adjGateZeroMode))
                {
                    _arraddr = strSSI2cDataList.Regaddr2;
                }               
            }
            break;
        case enI2cRegAdd_LedCtrl:
            {
                if(Len <= sizeof(structI2cRegAdd_LedCtrl))
                {
                    _arraddr = strSSI2cDataList.Regaddr3;
                }               
            }
            break;
        default:
            {
                _arraddr = NULL;
            }
            break;
    }

    if(_arraddr != NULL)
    {
				_rdataaddr = _Rdata + 1;
				
        for(dataindex = 0; dataindex < Len; dataindex++)
        {
						/*_Rdata + dataindex + 1把地址指针加1 ,使数据指针指向寄存器数据的首地址*/
            *(_arraddr + dataindex) = *(_rdataaddr + dataindex);
        }
    }
}



/*
 * @brief  应用获取寄存器数据接口
 * @param  uint8_t RegVal    对应的i2c寄出器值
 * @param  uint8_t *_Rdata   读到数据的数组首地址
 * @param  uint16_t * Len    读到的数据长度
 * @retval int err 
 */
int User_get_SSI2cRegisterData(uint8_t RegVal, uint8_t *_Rdata, uint16_t Len)
{
    int err;
    uint16_t dataindex;
    uint8_t * _arraddr = NULL;
    switch (RegVal)
    {
        case enI2cRegAdd_Version:
            {
                if((Len) == sizeof(structI2cRegAdd_Version))
                {
                    _arraddr = strSSI2cDataList.Regaddr0;
                }
            } 
            break;
        case enI2cRegAdd_SysParm:
            {
                if((Len) == sizeof(structI2cRegAdd_Systime))
                {
                    _arraddr = strSSI2cDataList.Regaddr0;
                }
            }
            break;
        case enI2cRegAdd_adjGateZeroMode:
            {
                if((Len) == sizeof(structI2cRegAdd_adjGateZeroMode))
                {
                    _arraddr = strSSI2cDataList.Regaddr2;
                }               
            }
            break;
        case enI2cRegAdd_LedCtrl:
            {
                if((Len) == sizeof(structI2cRegAdd_LedCtrl))
                {
                    _arraddr = strSSI2cDataList.Regaddr3;
                }               
            }
            break;
        default:
            {
                _arraddr = NULL;
            }
            break;
    }

    if(_arraddr != NULL)
    {
        for(dataindex = 0; dataindex < (Len); dataindex++)
        {
            *(_Rdata + dataindex) = *(_arraddr + dataindex);
        }
        err = defErrOK;
    }
    else
    {
        err = -1;
    }

    return err;
}



/*
 * @brief  应用获取寄存器数据接口
 * @param  uint8_t RegVal    对应的i2c寄出器值
 * @param  uint8_t *_Rdata   读到数据的数组首地址
 * @param  uint16_t * Len    读到的数据长度
 * @retval int err 
 */
int User_set_SSI2cRegisterData(uint8_t RegVal, uint8_t *_Wdata, uint16_t Len)
{
    int err;
    uint16_t dataindex;
    uint8_t * _arraddr = NULL;
    switch (RegVal)
    {
        case enI2cRegAdd_Version:
            {
                if((Len) == sizeof(structI2cRegAdd_Version))
                {
                    _arraddr = strSSI2cDataList.Regaddr0;
                }
            } 
            break;
        case enI2cRegAdd_SysParm:
            {
                if((Len) == sizeof(structI2cRegAdd_Systime))
                {
                    _arraddr = strSSI2cDataList.Regaddr0;
                }
            }
            break;
        case enI2cRegAdd_adjGateZeroMode:
            {
							/*主机配置不能修改*/
							_arraddr = NULL;
            }
            break;
        case enI2cRegAdd_LedCtrl:
            {
							/*主机配置不能修改*/
							_arraddr = NULL;             
            }
            break;
        default:
            {
                _arraddr = NULL;
            }
            break;
    }

    if(_arraddr != NULL)
    {
        for(dataindex = 0; dataindex < (Len); dataindex++)
        {
            *(_arraddr + dataindex) = *(_Wdata + dataindex);
        }
        err = defErrOK;
    }
    else
    {
        err = -1;
    }

    return err;
}


