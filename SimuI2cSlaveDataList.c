/**
 * @file SimuI2cSlaveDataList.c
 * @brief ģ��I2c�ӻ��Ĵ��������б�Ŀ���
 *
 * @version 1.0.0
 * @date 2024-04-30
 *
 * @copyright Copyright (c) 2024, wujingsheng
 * This source code is released under the MIT License.
 *
 * @author wujingsheng
 *
 * @brief ���ļ�ʵ����ģ��I2c�ӻ��Ĵ��������б�Ŀ���. ��������������ͨ��ʱ.���ݵĸ�ֵ�뷢��.
 * 
 * 
 *  * ע������:
 *  �ú���û�ж�����������.�����ڴӻ�������ͨ�Ŷ�ĳ���Ĵ�������ʱ.����ڶ�д�ĳ�ͻ.
 */

#include "SimulateI2c_S.h"

structSimulateI2c_DataList strSSI2cDataList;
extern const unsigned char ArrOldCodeVers[8];


/*
 * @brief  CRC16ѭ������У���㷨�� ����CRC-CCITT��Xmodem������ʽ��0x1021)
 * @param  const uint8_t*  ����ָ��
 * @param  size_t length   �������ݵ������׵�ַ
 * @retval uint16_t        У��ֵ 
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
 * @brief  ��ȡcrc16��У����
 * @param  const uint8_t*  ����ָ��
 * @param  size_t length   �������ݵ������׵�ַ
 * @retval uint16_t        = 0У��ɹ� ����У��ʧ��  
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
 * @brief  ��ʼ��ģ��I2C�ӻ��ļĴ�������
 * @param  *pCtrl:ģ��I2C�ӻ����ƽṹ��
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
 * @brief  ��ʼ��ģ��I2C�ӻ��ļĴ������ݺ��б���ƽṹ��
 * @param  *pCtrl:ģ��I2C�ӻ����ƽṹ��
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
 * @brief  ����Ҫ�Ļظ�����������ָ�븳ֵ,
 * @param  uint8_t **_Tdata��Ҫ���͵����ݵ�ָ���ָ���ַ
 * @param  uint16_t * Len  ��Ҫ�ظ������ݳ���
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
 * @brief  ���������͹��������ݸ�ֵ,
 * @param  uint8_t *_Rdata   �������ݵ������׵�ַ
 * @param  uint16_t * Len    ���������ݳ���
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
									 /*������,ֻ��*/
                   _arraddr = NULL;
                }
            } 
            break;
        case enI2cRegAdd_SysParm:
            {
                if(Len <= sizeof(structI2cRegAdd_Systime))
                {
									 /*������,ֻ��*/
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
						/*_Rdata + dataindex + 1�ѵ�ַָ���1 ,ʹ����ָ��ָ��Ĵ������ݵ��׵�ַ*/
            *(_arraddr + dataindex) = *(_rdataaddr + dataindex);
        }
    }
}



/*
 * @brief  Ӧ�û�ȡ�Ĵ������ݽӿ�
 * @param  uint8_t RegVal    ��Ӧ��i2c�ĳ���ֵ
 * @param  uint8_t *_Rdata   �������ݵ������׵�ַ
 * @param  uint16_t * Len    ���������ݳ���
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
 * @brief  Ӧ�û�ȡ�Ĵ������ݽӿ�
 * @param  uint8_t RegVal    ��Ӧ��i2c�ĳ���ֵ
 * @param  uint8_t *_Rdata   �������ݵ������׵�ַ
 * @param  uint16_t * Len    ���������ݳ���
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
							/*�������ò����޸�*/
							_arraddr = NULL;
            }
            break;
        case enI2cRegAdd_LedCtrl:
            {
							/*�������ò����޸�*/
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


