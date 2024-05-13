#ifndef __SIMULATE_I2C_SLAVE_H__
#define __SIMULATE_I2C_SLAVE_H__

#include <stdint.h>

// 版本信息
#define SIMULATE_I2C_SLAVE_VERSION_MAJOR 1
#define SIMULATE_I2C_SLAVE_VERSION_MINOR 0
#define SIMULATE_I2C_SLAVE_VERSION_PATCH 0
#define SIMULATE_I2C_SLAVE_VERSION_STRING "1.0.0"

/*
 * I2C从机模拟库头文件
 *
 * 版本:     @SIMULATE_I2C_SLAVE_VERSION_STRING@
 * 作者:     wujingsheng
 * 创建日期: 2024-04-28
 * 更新日期: 2024-04-28
 * 描述:     本头文件定义了模拟I2C从机操作的接口函数，用于在无硬件环境下进行I2C通信的软件模拟。
 *
 * 注意事项:
 * - 该模拟i2c从机程序,必须开启SCL和SDA引脚的的中断功能.(且中断边缘中断需要在通信过程中修改)
 * - 该模拟i2c从机程序,可以选择是否需要外部定时器计数是否超时,判断是否恢复I2C通信状态到Idle.
 * 
 * 版本兼容性说明:
 * 1.0.0: 初始版本，实现了基本的I2C从机模拟功能。
 */


typedef enum 
{
    enSSI2c_Idle,           /*0:总线空闲状态*/
    enSSI2c_GetAddr,        /*1:获取设备地址信号*/
    enSSI2c_GetAddrACK,     /*2:判断主机发送的设备地址是否位本设备地址*/
    enSSI2c_WaitAddrACK,    /*3:等待发送完成ACK*/
    enSSI2c_RData,          /*4:接收主机发送的数据信号*/
    enSSI2c_RDataACK,       /*5:读取数据信号的ACK型号*/
    enSSI2c_WaitRDataACK,   /*6:等待发送完成ACK*/
    enSSI2c_TData,          /*7:发送数据给主机*/
    enSSI2c_TDataACK,       /*8:发送数据给主机的ACK信号*/
    enSSI2c_WaitTDataACK,   /*9:等待发送完成ACK*/
    enSSI2c_NotACK,         /*a:不给主机设备ACK信号*/
    enSSI2c_Stop,           /*b:等待地址信号*/
    enSSI2c_ERROR,          /*c:主要记录错误,终止通信.恢复到Idle状态*/
    enSSI2c_NotDefine
}enumSimuI2cSlabeState;


typedef enum 
{
    enSSI2c_Write,       /*主机写*/
    enSSI2c_Read,        /*主机读*/
    enSSRWI2c_NotDefine
}enumSimuI2cReadWriteState;

typedef struct 
{
    /*设置从机设备的地址*/
    volatile uint8_t slaveAddress;
    /*主机的读写标志位, = 0 为主机写 = 1 为主机读*/
    volatile uint8_t ReadWriteflag;
    /*模拟I2C从机的接收状态*/
    volatile uint8_t state;
    /*模拟I2C从机的读写为位置*/
    volatile uint8_t BitIndex;
    /*读取Buff*/
    volatile uint8_t readbuff;
    /*发送Buff*/
    volatile uint8_t sendbuff;
    /*时间计数器.用于定时器计数器,单位(ms)*/
    volatile uint16_t TimerMs;
    /*最大的计数器次数,<structSimulateI2c_SlaveCtrl.TimerMs>超过最大的次数,使模拟I2C从机退出读写状态.返回空闲状态*/
    volatile uint16_t MaxTimerMs;
    /*数据数据引索位置*/
    volatile uint16_t dataIndex;
    /*最大的从机发送数据长度*/
    volatile uint16_t TMaxDataLen;
    /*最大的从机接收数据长度*/
    volatile uint16_t RMaxDataLen;
    /*从机发送数据的缓存地址*/
    uint8_t * _arrTDataBuf;
    /*从机读数据的缓存地址*/
    uint8_t * _arrRDataBuf;

    /*获取从机发送数据的回调函数地址
        * @param  **_Tdata 需要发送数据的数组指针的指针.可以修改数组的指针值.来指定需要发送的数据位置.
        * @param  *len 需要发送数据的数据长度.
    */
    void (* _Get_TdataCallback) (uint8_t **_Tdata, uint16_t * Len ,uint8_t RegAddrVal); /*数据针回调函数*/

    /*保存从机接收数据的回调函数地址
        * @param  *_rdata 接收数据的数组指针.
        * @param  len 接收数据的长度
        * @retval int err 
    */
    void (* _Set_RdataCallback) (uint8_t *_rData, uint16_t Len); /*数据针回调函数*/

    /*模拟I2C错误的代号*/
    uint32_t ErrCode;       

    uint32_t HTimeoutRecovery;

} structSimulateI2c_SlaveCtrl;



/*
 * @brief  初始化模拟I2C从机
 * @param  *pCtrl:模拟I2C从机控制结构体
 * @retval int err 
 */
int SimulateI2cSlaveStruct_Init(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl);



/*
 * @brief  设置模拟I2C从机设备的地址
 * @param  *pCtrl:模拟I2C从机控制结构体
 * @param  slaveAddress:从机地址
 * @retval int err
 */
int Set_SimulateI2cSlave_address(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl, uint8_t slaveAddress);



 /*
 * @brief  设置模拟I2C从机设备的读取数据的缓冲区,和缓冲区大小
 * @param  *pCtrl:模拟I2C从机控制结构体
 * @param  _RDataAddress:   读取数据地址
 * @param  RdataMaxLen:     最大读取长度  
 * @retval int err
 */
int Set_SimulateI2cSlave_RDataPram(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl, uint8_t *_RDataAddress, uint16_t RdataMaxLen);



 /*
 * @brief  设置模拟I2C从机设备的发送数据的缓冲区,和可发送数据大小
 * @param  *pCtrl:模拟I2C从机控制结构体
 * @param  _TDataAddress:   发送数据地址
 * @param  TdataMaxLen:     最大读取长度  T
 * @retval int err
 */
int Set_SimulateI2cSlave_TDataPram(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl, uint8_t *_TDataAddress, uint16_t TdataMaxLen);



 /*
 * @brief  开始模拟I2c从机工作
 * @param  *pCtrl:模拟I2C从机控制结构体
 * @retval int err
 */
int Start_SimulateI2cSlave_CB(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl);



 /*
 * @brief  停止模拟I2c从机工作
 * @param  *pCtrl:模拟I2C从机控制结构体
 * @retval int err
 */
int Stop_SimulateI2cSlave_CB(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl);



 /*
 * @brief  模拟I2c从机工作的SDA引脚中断回调函数
 * @param  *pCtrl:模拟I2C从机控制结构体
 * @retval int err
 */
__attribute__((section("ram_code"))) void SSI2c_SDAIO_IRQHandler(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl);



 /*
 * @brief  模拟I2c从机工作的SDA引脚中断回调函数
 * @param  *pCtrl:模拟I2C从机控制结构体
 * @retval int err
 */
__attribute__((section("ram_code"))) void SSI2c_SCLIO_IRQHandler(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl);



 /*
 * @brief  模拟I2c从机工作的定时器中断回调函数
 * @param  *pCtrl:模拟I2C从机控制结构体
 * @retval int err
 */
__attribute__((section("ram_code"))) void SSI2c_Timer_IRQHandler(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl);



 /*
 * @brief  模拟I2c与主机心跳超时的时钟Add的回调函数
 * @param  *pCtrl:模拟I2C从机控制结构体
 * @retval int err
 */
void Add_SSI2c_HTimeoutRecovery_cb(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl , uint32_t Time1Value);



#endif // __SIMULATE_I2C_SLAVE_H__T
