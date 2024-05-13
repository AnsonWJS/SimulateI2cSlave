/*
 * բ��LED����BLE����Э���ͷ�ļ�
 *
 * �汾:     @SIMULATE_I2C_SLAVE_BSP_VERSION_STRING@
 * ����:     wujingsheng
 * ��������: 2024-04-30
 * ��������: 2024-04-30
 * ����:     ��ͷ�ļ�������ģ��I2C�ӻ��ļĴ�����ַ�Ͷ�Ӧ�����ݶ���.
 *
 * ע������:
 * - 
 * - 
 */
#ifndef __GATECTRLPROTOCOL_H__
#define __GATECTRLPROTOCOL_H__

#include <stdint.h>


/*CRC16����ʽ*/
#define CRC16_POLYNOMIAL 0x1021
/*��ʼ��CRCֵ*/
#define CRC16_INIT 0xFFFF


/*crc16У��,����*/
#if 0
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
#endif


/*�Ĵ�����ַ����*/
typedef enum 
{
    enI2cRegAdd_Version,            /*(0x00)�汾��,ֻ��*/
    enI2cRegAdd_SysParm,           /*(0x01)ϵͳ����ʱ��,������ֵ�Ĳ�����У�����,ֻ��*/
    enI2cRegAdd_adjGateZeroMode,   /*(0x02)բ���ŵ���ģʽ,�ɶ�������*/
    enI2cRegAdd_LedCtrl,           /*(0x03)LED����,�ɶ�������*/
    enSSI2c_ReaAddNotDefine
}enumSimuI2cReaAdd;




#pragma pack(1)
/*
	I2c�Ĵ�����ַ : enI2cRegAdd_Version,(0x00)�汾��,ֻ��
	������:
	
*/
typedef struct
{
    uint8_t version[4];
    uint16_t crc16value;   /*CRC16��ֵ*/
}structI2cRegAdd_Version;
#pragma pack()



#pragma pack(1)
/*
	I2c�Ĵ�����ַ : enI2cRegAdd_SysParm, (0x01)ϵͳ����ʱ��,������ֵ�Ĳ�����У�����,ֻ��
	����:
	
*/
typedef struct
{
    uint32_t systime;       /*��λ:S*/
    uint32_t crc16valueErr; /*�ӻ����յ�I2cУ�������ܴ���*/
    uint16_t crc16value;    /*CRC16��ֵ*/
}structI2cRegAdd_Systime;
#pragma pack()



#pragma pack(1)
/*
	I2c�Ĵ�����ַ :  enI2cRegAdd_adjGateZeroMode, (0x02)բ���ŵ���ģʽ,�ɶ�������
	����:
	
*/
/*
*    -MCUϵͳ��ʼĬ��ֵΪ
*       adjGateZeroMode = 0
*/
typedef struct
{
    uint8_t adjGateZeroMode;     /*= 0 ���ڵ���ģʽ,  = 1 �ڵ���ģʽ*/
    uint16_t crc16value;         /*CRC16��ֵ*/
}structI2cRegAdd_adjGateZeroMode;
#pragma pack()



#pragma pack(1)
/*
	I2c�Ĵ�����ַ :enI2cRegAdd_LedCtrl,    (0x03)LED����,�ɶ�������
	����:
	
*/
/*
    *    -MCUϵͳ��ʼĬ��ֵΪ
    *    mode = 0   
    *    RLedvalue = 0
    *    GLedvalue = 0
    *    BLedvalue = 0
    *    OnTime = 0
    *    OffTime = 0
		*    CyclesTimes =0
		*    uint8_t 
*/
typedef struct
{
    uint8_t mode;        /*=0 ����ģʽ, =1 APP����ģʽ*/
    uint8_t RLedvalue;   /*0-255 ����ת��Ϊ 10K��PWM����, ��ֵΪ���Ƶ�ռ��*/
    uint8_t GLedvalue;   /*0-255 ����ת��Ϊ 10K��PWM����, ��ֵΪ���Ƶ�ռ��*/
    uint8_t BLedvalue;   /*0-255 ����ת��Ϊ 10K��PWM����, ��ֵΪ���Ƶ�ռ��*/
    uint8_t OnTime;      /*0-255 ���Ƶ�ʱ�� ��λΪ100ms*/
    uint8_t OffTime;     /*0-255 ���ʱ�� ��λΪ100ms*/
		uint8_t CyclesTimes; /*���ڴ���. 0 = ���. = 255 ������*/
    uint16_t crc16value; /*CRC16��ֵ*/
}structI2cRegAdd_LedCtrl;
#pragma pack()

#endif
