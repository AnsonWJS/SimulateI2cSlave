模拟I2c从机功能带代码, 经过测试:
	I2c的SCL的速率为10K测试条件下:
	通信成功:578416次.(间隔100ms).读写0次失败. 
	数据校验:288116,CRC16校验错误次数1104次

一.源文件说明:
/**************库的代码**************/
<exIns.h>
	功能说明:
		把对应单片机需要添加的工程头文件,添加到这个文件中. (需要根据你的业务需求修改其中代码)
<SimuI2cSlaveDataList.c>和<SimuI2cSlaveDataList.c>	
	功能说明:
		1.I2c从机读数据通信完毕后.把对应获取的数据赋值到相关的数据数组中.
		2.I2c从机写入主机数据的.地址和数据长短的赋值.
			注:与你的I2c从机寄存器数据又密切关系. 如例子中的<GateCtrlProtocol.h>
				如果添加了这两个文件. 你的接收和发送的数据就可以自动赋值到相对应的寄存器值中.
<SimuI2cSlaveErrCode.h>需要根据你的业务需求修改其中代码
	功能说明:
		该库支持错误代码记录.方便调试和排查问题.可以通过structSimulateI2c_SlaveCtrl结构中的ErrCode获取错误代码值
<SimulateI2c_S.h>
	功能说明:
		I2c从机的配置预定义
<SimuLateI2cSlave_bsp.c>和<SimuLateI2cSlave_bsp.h>
	功能说明:
		板级支持板.根据你使用相关的单片机.更改硬件配置. (必须修改修改其中代码)
<SimulateI2cSlave.c>和<SimulateI2cSlave.h>
	功能说明:
		模拟I2c的核心代码. 主要是接收和写入I2c时序逻辑.
/************例子代码**************/
<eSSI2c_examples.c>和<eSSI2c_examples.和>
功能说明:
	使用例子, 和相关数据结构的初始化.
<GateCtrlProtocol.h>
二.功能说明:
	例子中的I2c从机的寄存器数据的变量定义. 和数据结构的定义. 

三.功能使用说明:
	例子使用的是FR8018HA的蓝牙SOC单片机为主控. 硬件不支持I2c从机. 所以自己编写了模拟I2c从机通信.
	        1.本库的代码.都运行在硬件中断中. 所以初始化后无需再添加其他函数. 
		2.不过可以与主机设备添加接收心跳超时后,重新初始化模拟I2c从机的机制. 增加系统的健壮性
		3.数据读写是通过访问structSimulateI2c_DataList数据结构来获取对应I2c寄存器的数据.作读写操作. (详细请查看地址)
		
四.移植的修改说明:
	<exIns.h>:添加对应单片机的头文件.
	<SimuI2cSlaveDataList.c>和<SimuI2cSlaveDataList.c>.根据你工程业务的需要.学习例子修改你.I2c从机设备的数据结构.和数据内容.以及读写的限制条件的相关代码
	<SimulateI2c_S.h>	:I2c从机的相关配置
	<SimuLateI2cSlave_bsp.c>和<SimuLateI2cSlave_bsp.h> .工程对应的硬件GPIO和外部中断的设置.

	如果你不需要数据自动获取到对应空间的功能. 你可以通过修改下面两个回调函数来实现自己的逻辑.同时也不需要(<SimuI2cSlaveDataList.c>和<SimuI2cSlaveDataList.c>)里面数据结构的初始化与数据操作.编写自己的需求.
		(*_strSSI2cCtrl->_Get_TdataCallback)
		(*_strSSI2cCtrl->_Set_RdataCallback)


五.注意事项:
	1.相关中断的优先等级的设置. 按你工程的需要设置. 
	2.模拟I2c的速率不能太快.
