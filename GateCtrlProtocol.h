/*
 * 闸机LED控制BLE控制协议库头文件
 *
 * 版本:     @SIMULATE_I2C_SLAVE_BSP_VERSION_STRING@
 * 作者:     wujingsheng
 * 创建日期: 2024-04-30
 * 更新日期: 2024-04-30
 * 描述:     本头文件定义了模拟I2C从机的寄存器地址和对应的数据定义.
 *
 * 注意事项:
 * - 
 * - 
 */
#ifndef __GATECTRLPROTOCOL_H__
#define __GATECTRLPROTOCOL_H__

#include <stdint.h>


/*CRC16多项式*/
#define CRC16_POLYNOMIAL 0x1021
/*初始化CRC值*/
#define CRC16_INIT 0xFFFF


/*crc16校验,函数*/
#if 0
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
#endif


/*寄存器地址定义*/
typedef enum 
{
    enI2cRegAdd_Version,            /*(0x00)版本号,只读*/
    enI2cRegAdd_SysParm,           /*(0x01)系统运行时间,与设置值的参数的校验情况,只读*/
    enI2cRegAdd_adjGateZeroMode,   /*(0x02)闸机门调零模式,可读可配置*/
    enI2cRegAdd_LedCtrl,           /*(0x03)LED控制,可读可配置*/
    enSSI2c_ReaAddNotDefine
}enumSimuI2cReaAdd;




#pragma pack(1)
/*
	I2c寄存器地址 : enI2cRegAdd_Version,(0x00)版本号,只读
	例子中:
	
*/
typedef struct
{
    uint8_t version[4];
    uint16_t crc16value;   /*CRC16的值*/
}structI2cRegAdd_Version;
#pragma pack()



#pragma pack(1)
/*
	I2c寄存器地址 : enI2cRegAdd_SysParm, (0x01)系统运行时间,与设置值的参数的校验情况,只读
	例子:
	
*/
typedef struct
{
    uint32_t systime;       /*单位:S*/
    uint32_t crc16valueErr; /*从机接收到I2c校验错误的总次数*/
    uint16_t crc16value;    /*CRC16的值*/
}structI2cRegAdd_Systime;
#pragma pack()



#pragma pack(1)
/*
	I2c寄存器地址 :  enI2cRegAdd_adjGateZeroMode, (0x02)闸机门调零模式,可读可配置
	例子:
	
*/
/*
*    -MCU系统初始默认值为
*       adjGateZeroMode = 0
*/
typedef struct
{
    uint8_t adjGateZeroMode;     /*= 0 不在调零模式,  = 1 在调零模式*/
    uint16_t crc16value;         /*CRC16的值*/
}structI2cRegAdd_adjGateZeroMode;
#pragma pack()



#pragma pack(1)
/*
	I2c寄存器地址 :enI2cRegAdd_LedCtrl,    (0x03)LED控制,可读可配置
	例子:
	
*/
/*
    *    -MCU系统初始默认值为
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
    uint8_t mode;        /*=0 工厂模式, =1 APP控制模式*/
    uint8_t RLedvalue;   /*0-255 最终转换为 10K的PWM调制, 该值为亮灯的占比*/
    uint8_t GLedvalue;   /*0-255 最终转换为 10K的PWM调制, 该值为亮灯的占比*/
    uint8_t BLedvalue;   /*0-255 最终转换为 10K的PWM调制, 该值为亮灯的占比*/
    uint8_t OnTime;      /*0-255 亮灯的时间 单位为100ms*/
    uint8_t OffTime;     /*0-255 灭的时间 单位为100ms*/
		uint8_t CyclesTimes; /*周期次数. 0 = 灭掉. = 255 不计数*/
    uint16_t crc16value; /*CRC16的值*/
}structI2cRegAdd_LedCtrl;
#pragma pack()

#endif
