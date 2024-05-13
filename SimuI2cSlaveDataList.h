/*
 * 模拟I2c从机寄存器数据列表的控制库头文件
 *
 * 作者:     wujingsheng
 * 创建日期: 2024-04-30
 * 更新日期: 2024-04-30
 * 描述:     本头文件定义了模拟I2C从机的寄存器地址的数据列表定义.
 *
 * 注意事项:
 * - 
 * - 
 */
#ifndef __SIMUICSLAVEDATALIST_H__
#define __SIMUICSLAVEDATALIST_H__

#include <stdint.h>
#include "GateCtrlProtocol.h"

typedef struct 
{
    uint32_t initFalg;  /*初始标志 = 0 未初始 = 1已经初始化*/
    /*对应寄存器数据的长短*/
    uint8_t RegDataLen[enSSI2c_ReaAddNotDefine];
    /*对应寄存器数据的首地址指针*/
    uint8_t * _RegData[enSSI2c_ReaAddNotDefine];
    /*对应寄存器数据的数组*/
    uint8_t Regaddr0[sizeof(structI2cRegAdd_Version) + 4];
    uint8_t Regaddr1[sizeof(structI2cRegAdd_Systime) + 4];
    uint8_t Regaddr2[sizeof(structI2cRegAdd_adjGateZeroMode) + 4];
    uint8_t Regaddr3[sizeof(structI2cRegAdd_LedCtrl) + 4];
} structSimulateI2c_DataList;



extern structSimulateI2c_DataList strSSI2cDataList;



/*
 * @brief  CRC16循环冗余校验算法的 采用CRC-CCITT（Xmodem）多项式（0x1021)
 * @param  const uint8_t*  数据指针
 * @param  size_t length   读到数据的数组首地址
 * @retval uint16_t        校验值 
 */
uint16_t calculateCrc16(const uint8_t* data, uint32_t length); 



/*
 * @brief  获取crc16的校验结果
 * @param  const uint8_t*  数据指针
 * @param  size_t length   读到数据的数组首地址
 * @retval uint16_t        = 0校验成功 否则校验失败  
 */
int outputCrc16(const uint8_t* data, uint32_t length);



/*
 * @brief  初始化模拟I2C从机的寄存器数据和列表控制结构体
 * @param  *pCtrl:模拟I2C从机控制结构体
 * @retval int err 
 */
void SimuLateI2c_DataList_Init(structSimulateI2c_DataList * _strSimuI2cDataList);



/*
 * @brief  把需要的回复主机的数据指针赋值,
 * @param  uint8_t **_Tdata需要发送的数据的指针的指针地址
 * @param  uint16_t * Len  需要回复的数据长度
 * @retval int err 
 */
__attribute__((section("ram_code"))) void SimuLateI2c_Get_TdataCallback_CB(uint8_t **_Tdata, uint16_t * Len ,uint8_t RegAddrVal);



/*
 * @brief  把主机发送过来的数据赋值,
 * @param  uint8_t *_Rdata   读到数据的数组首地址
 * @param  uint16_t * Len    读到的数据长度
 * @retval int err 
 */
__attribute__((section("ram_code"))) void SimuLateI2c_Set_RdataCallback_CB(uint8_t *_Rdata, uint16_t Len);



/*
 * @brief  主机接收数据完成回调函数
 * @param  uint8_t *_Rdata   读到数据的数组首地址
 * @param  uint16_t * Len    读到的数据长度
 * @retval int err 
 */
__attribute__((section("ram_code"))) void SimuLateI2c_Tdata_SendOKCallback_CB(void);



/*
 * @brief  应用获取寄存器数据接口
 * @param  uint8_t RegVal    对应的i2c寄出器值
 * @param  uint8_t *_Rdata   读到数据的数组首地址
 * @param  uint16_t * Len    读到的数据长度
 * @retval int err 
 */
int User_get_SSI2cRegisterData(uint8_t RegVal, uint8_t *_Rdata, uint16_t  Len);



/*
 * @brief  应用获取寄存器数据接口
 * @param  uint8_t RegVal    对应的i2c寄出器值
 * @param  uint8_t *_Rdata   读到数据的数组首地址
 * @param  uint16_t * Len    读到的数据长度
 * @retval int err 
 */
int User_set_SSI2cRegisterData(uint8_t RegVal, uint8_t *_Wdata, uint16_t  Len);



#endif
