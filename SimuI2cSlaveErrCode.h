#ifndef __SimuI2cSlaveErrCode_H__
#define __SimuI2cSlaveErrCode_H__

#define  defSSI2cErr_I2cCtrlStrNull    0x00000001   /*�ṹ��ַΪ��*/ 
#define  defSSI2cErr_Timeout           0x00000002   /*ͨ�ų�ʱ*/
#define  defSSI2cErr_NotCheckStartBit  0x00000004   /*δ�ܼ�⵽��ʼλ*/
#define  defSSI2cErr_DrivedAddress     0x00000008   /*�豸��ַ����*/
#define  defSSI2cErr_SCLIrqMode        0x00000010   /*SCL�ж�ģʽ����*/

#define  defSSI2cErr_RDataLen          0x00000020   /*����д�볤�ȳ������鷶Χ*/
#define  defSSI2cErr_TDataLen          0x00000040   /*������ȡ�ĳ��ȳ������鷶Χ*/
#define  defSSI2cErr_HaetTimerOut      0x00000080   /*������ʱ*/

#define  defSSI2cErr_CodeMax           8

#endif 
