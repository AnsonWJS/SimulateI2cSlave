#ifndef __SimuI2cSlaveErrCode_H__
#define __SimuI2cSlaveErrCode_H__

#define  defSSI2cErr_I2cCtrlStrNull    0x00000001   /*结构地址为空*/ 
#define  defSSI2cErr_Timeout           0x00000002   /*通信超时*/
#define  defSSI2cErr_NotCheckStartBit  0x00000004   /*未能检测到起始位*/
#define  defSSI2cErr_DrivedAddress     0x00000008   /*设备地址错误*/
#define  defSSI2cErr_SCLIrqMode        0x00000010   /*SCL中断模式错误*/

#define  defSSI2cErr_RDataLen          0x00000020   /*主机写入长度超出数组范围*/
#define  defSSI2cErr_TDataLen          0x00000040   /*主机读取的长度超出数组范围*/
#define  defSSI2cErr_HaetTimerOut      0x00000080   /*心跳超时*/

#define  defSSI2cErr_CodeMax           8

#endif 
