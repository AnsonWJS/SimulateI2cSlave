/*
 * ģ��I2c�ӻ��Ĵ��������б�Ŀ��ƿ�ͷ�ļ�
 *
 * ����:     wujingsheng
 * ��������: 2024-04-30
 * ��������: 2024-04-30
 * ����:     ��ͷ�ļ�������ģ��I2C�ӻ��ļĴ�����ַ�������б���.
 *
 * ע������:
 * - 
 * - 
 */
#ifndef __SIMUICSLAVEDATALIST_H__
#define __SIMUICSLAVEDATALIST_H__

#include <stdint.h>
#include "GateCtrlProtocol.h"

typedef struct 
{
    uint32_t initFalg;  /*��ʼ��־ = 0 δ��ʼ = 1�Ѿ���ʼ��*/
    /*��Ӧ�Ĵ������ݵĳ���*/
    uint8_t RegDataLen[enSSI2c_ReaAddNotDefine];
    /*��Ӧ�Ĵ������ݵ��׵�ַָ��*/
    uint8_t * _RegData[enSSI2c_ReaAddNotDefine];
    /*��Ӧ�Ĵ������ݵ�����*/
    uint8_t Regaddr0[sizeof(structI2cRegAdd_Version) + 4];
    uint8_t Regaddr1[sizeof(structI2cRegAdd_Systime) + 4];
    uint8_t Regaddr2[sizeof(structI2cRegAdd_adjGateZeroMode) + 4];
    uint8_t Regaddr3[sizeof(structI2cRegAdd_LedCtrl) + 4];
} structSimulateI2c_DataList;



extern structSimulateI2c_DataList strSSI2cDataList;



/*
 * @brief  CRC16ѭ������У���㷨�� ����CRC-CCITT��Xmodem������ʽ��0x1021)
 * @param  const uint8_t*  ����ָ��
 * @param  size_t length   �������ݵ������׵�ַ
 * @retval uint16_t        У��ֵ 
 */
uint16_t calculateCrc16(const uint8_t* data, uint32_t length); 



/*
 * @brief  ��ȡcrc16��У����
 * @param  const uint8_t*  ����ָ��
 * @param  size_t length   �������ݵ������׵�ַ
 * @retval uint16_t        = 0У��ɹ� ����У��ʧ��  
 */
int outputCrc16(const uint8_t* data, uint32_t length);



/*
 * @brief  ��ʼ��ģ��I2C�ӻ��ļĴ������ݺ��б���ƽṹ��
 * @param  *pCtrl:ģ��I2C�ӻ����ƽṹ��
 * @retval int err 
 */
void SimuLateI2c_DataList_Init(structSimulateI2c_DataList * _strSimuI2cDataList);



/*
 * @brief  ����Ҫ�Ļظ�����������ָ�븳ֵ,
 * @param  uint8_t **_Tdata��Ҫ���͵����ݵ�ָ���ָ���ַ
 * @param  uint16_t * Len  ��Ҫ�ظ������ݳ���
 * @retval int err 
 */
__attribute__((section("ram_code"))) void SimuLateI2c_Get_TdataCallback_CB(uint8_t **_Tdata, uint16_t * Len ,uint8_t RegAddrVal);



/*
 * @brief  ���������͹��������ݸ�ֵ,
 * @param  uint8_t *_Rdata   �������ݵ������׵�ַ
 * @param  uint16_t * Len    ���������ݳ���
 * @retval int err 
 */
__attribute__((section("ram_code"))) void SimuLateI2c_Set_RdataCallback_CB(uint8_t *_Rdata, uint16_t Len);



/*
 * @brief  ��������������ɻص�����
 * @param  uint8_t *_Rdata   �������ݵ������׵�ַ
 * @param  uint16_t * Len    ���������ݳ���
 * @retval int err 
 */
__attribute__((section("ram_code"))) void SimuLateI2c_Tdata_SendOKCallback_CB(void);



/*
 * @brief  Ӧ�û�ȡ�Ĵ������ݽӿ�
 * @param  uint8_t RegVal    ��Ӧ��i2c�ĳ���ֵ
 * @param  uint8_t *_Rdata   �������ݵ������׵�ַ
 * @param  uint16_t * Len    ���������ݳ���
 * @retval int err 
 */
int User_get_SSI2cRegisterData(uint8_t RegVal, uint8_t *_Rdata, uint16_t  Len);



/*
 * @brief  Ӧ�û�ȡ�Ĵ������ݽӿ�
 * @param  uint8_t RegVal    ��Ӧ��i2c�ĳ���ֵ
 * @param  uint8_t *_Rdata   �������ݵ������׵�ַ
 * @param  uint16_t * Len    ���������ݳ���
 * @retval int err 
 */
int User_set_SSI2cRegisterData(uint8_t RegVal, uint8_t *_Wdata, uint16_t  Len);



#endif
