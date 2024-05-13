#ifndef __SIMULATE_I2C_SLAVE_H__
#define __SIMULATE_I2C_SLAVE_H__

#include <stdint.h>

// �汾��Ϣ
#define SIMULATE_I2C_SLAVE_VERSION_MAJOR 1
#define SIMULATE_I2C_SLAVE_VERSION_MINOR 0
#define SIMULATE_I2C_SLAVE_VERSION_PATCH 0
#define SIMULATE_I2C_SLAVE_VERSION_STRING "1.0.0"

/*
 * I2C�ӻ�ģ���ͷ�ļ�
 *
 * �汾:     @SIMULATE_I2C_SLAVE_VERSION_STRING@
 * ����:     wujingsheng
 * ��������: 2024-04-28
 * ��������: 2024-04-28
 * ����:     ��ͷ�ļ�������ģ��I2C�ӻ������Ľӿں�������������Ӳ�������½���I2Cͨ�ŵ����ģ�⡣
 *
 * ע������:
 * - ��ģ��i2c�ӻ�����,���뿪��SCL��SDA���ŵĵ��жϹ���.(���жϱ�Ե�ж���Ҫ��ͨ�Ź������޸�)
 * - ��ģ��i2c�ӻ�����,����ѡ���Ƿ���Ҫ�ⲿ��ʱ�������Ƿ�ʱ,�ж��Ƿ�ָ�I2Cͨ��״̬��Idle.
 * 
 * �汾������˵��:
 * 1.0.0: ��ʼ�汾��ʵ���˻�����I2C�ӻ�ģ�⹦�ܡ�
 */


typedef enum 
{
    enSSI2c_Idle,           /*0:���߿���״̬*/
    enSSI2c_GetAddr,        /*1:��ȡ�豸��ַ�ź�*/
    enSSI2c_GetAddrACK,     /*2:�ж��������͵��豸��ַ�Ƿ�λ���豸��ַ*/
    enSSI2c_WaitAddrACK,    /*3:�ȴ��������ACK*/
    enSSI2c_RData,          /*4:�����������͵������ź�*/
    enSSI2c_RDataACK,       /*5:��ȡ�����źŵ�ACK�ͺ�*/
    enSSI2c_WaitRDataACK,   /*6:�ȴ��������ACK*/
    enSSI2c_TData,          /*7:�������ݸ�����*/
    enSSI2c_TDataACK,       /*8:�������ݸ�������ACK�ź�*/
    enSSI2c_WaitTDataACK,   /*9:�ȴ��������ACK*/
    enSSI2c_NotACK,         /*a:���������豸ACK�ź�*/
    enSSI2c_Stop,           /*b:�ȴ���ַ�ź�*/
    enSSI2c_ERROR,          /*c:��Ҫ��¼����,��ֹͨ��.�ָ���Idle״̬*/
    enSSI2c_NotDefine
}enumSimuI2cSlabeState;


typedef enum 
{
    enSSI2c_Write,       /*����д*/
    enSSI2c_Read,        /*������*/
    enSSRWI2c_NotDefine
}enumSimuI2cReadWriteState;

typedef struct 
{
    /*���ôӻ��豸�ĵ�ַ*/
    volatile uint8_t slaveAddress;
    /*�����Ķ�д��־λ, = 0 Ϊ����д = 1 Ϊ������*/
    volatile uint8_t ReadWriteflag;
    /*ģ��I2C�ӻ��Ľ���״̬*/
    volatile uint8_t state;
    /*ģ��I2C�ӻ��Ķ�дΪλ��*/
    volatile uint8_t BitIndex;
    /*��ȡBuff*/
    volatile uint8_t readbuff;
    /*����Buff*/
    volatile uint8_t sendbuff;
    /*ʱ�������.���ڶ�ʱ��������,��λ(ms)*/
    volatile uint16_t TimerMs;
    /*���ļ���������,<structSimulateI2c_SlaveCtrl.TimerMs>�������Ĵ���,ʹģ��I2C�ӻ��˳���д״̬.���ؿ���״̬*/
    volatile uint16_t MaxTimerMs;
    /*������������λ��*/
    volatile uint16_t dataIndex;
    /*���Ĵӻ��������ݳ���*/
    volatile uint16_t TMaxDataLen;
    /*���Ĵӻ��������ݳ���*/
    volatile uint16_t RMaxDataLen;
    /*�ӻ��������ݵĻ����ַ*/
    uint8_t * _arrTDataBuf;
    /*�ӻ������ݵĻ����ַ*/
    uint8_t * _arrRDataBuf;

    /*��ȡ�ӻ��������ݵĻص�������ַ
        * @param  **_Tdata ��Ҫ�������ݵ�����ָ���ָ��.�����޸������ָ��ֵ.��ָ����Ҫ���͵�����λ��.
        * @param  *len ��Ҫ�������ݵ����ݳ���.
    */
    void (* _Get_TdataCallback) (uint8_t **_Tdata, uint16_t * Len ,uint8_t RegAddrVal); /*������ص�����*/

    /*����ӻ��������ݵĻص�������ַ
        * @param  *_rdata �������ݵ�����ָ��.
        * @param  len �������ݵĳ���
        * @retval int err 
    */
    void (* _Set_RdataCallback) (uint8_t *_rData, uint16_t Len); /*������ص�����*/

    /*ģ��I2C����Ĵ���*/
    uint32_t ErrCode;       

    uint32_t HTimeoutRecovery;

} structSimulateI2c_SlaveCtrl;



/*
 * @brief  ��ʼ��ģ��I2C�ӻ�
 * @param  *pCtrl:ģ��I2C�ӻ����ƽṹ��
 * @retval int err 
 */
int SimulateI2cSlaveStruct_Init(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl);



/*
 * @brief  ����ģ��I2C�ӻ��豸�ĵ�ַ
 * @param  *pCtrl:ģ��I2C�ӻ����ƽṹ��
 * @param  slaveAddress:�ӻ���ַ
 * @retval int err
 */
int Set_SimulateI2cSlave_address(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl, uint8_t slaveAddress);



 /*
 * @brief  ����ģ��I2C�ӻ��豸�Ķ�ȡ���ݵĻ�����,�ͻ�������С
 * @param  *pCtrl:ģ��I2C�ӻ����ƽṹ��
 * @param  _RDataAddress:   ��ȡ���ݵ�ַ
 * @param  RdataMaxLen:     ����ȡ����  
 * @retval int err
 */
int Set_SimulateI2cSlave_RDataPram(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl, uint8_t *_RDataAddress, uint16_t RdataMaxLen);



 /*
 * @brief  ����ģ��I2C�ӻ��豸�ķ������ݵĻ�����,�Ϳɷ������ݴ�С
 * @param  *pCtrl:ģ��I2C�ӻ����ƽṹ��
 * @param  _TDataAddress:   �������ݵ�ַ
 * @param  TdataMaxLen:     ����ȡ����  T
 * @retval int err
 */
int Set_SimulateI2cSlave_TDataPram(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl, uint8_t *_TDataAddress, uint16_t TdataMaxLen);



 /*
 * @brief  ��ʼģ��I2c�ӻ�����
 * @param  *pCtrl:ģ��I2C�ӻ����ƽṹ��
 * @retval int err
 */
int Start_SimulateI2cSlave_CB(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl);



 /*
 * @brief  ֹͣģ��I2c�ӻ�����
 * @param  *pCtrl:ģ��I2C�ӻ����ƽṹ��
 * @retval int err
 */
int Stop_SimulateI2cSlave_CB(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl);



 /*
 * @brief  ģ��I2c�ӻ�������SDA�����жϻص�����
 * @param  *pCtrl:ģ��I2C�ӻ����ƽṹ��
 * @retval int err
 */
__attribute__((section("ram_code"))) void SSI2c_SDAIO_IRQHandler(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl);



 /*
 * @brief  ģ��I2c�ӻ�������SDA�����жϻص�����
 * @param  *pCtrl:ģ��I2C�ӻ����ƽṹ��
 * @retval int err
 */
__attribute__((section("ram_code"))) void SSI2c_SCLIO_IRQHandler(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl);



 /*
 * @brief  ģ��I2c�ӻ������Ķ�ʱ���жϻص�����
 * @param  *pCtrl:ģ��I2C�ӻ����ƽṹ��
 * @retval int err
 */
__attribute__((section("ram_code"))) void SSI2c_Timer_IRQHandler(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl);



 /*
 * @brief  ģ��I2c������������ʱ��ʱ��Add�Ļص�����
 * @param  *pCtrl:ģ��I2C�ӻ����ƽṹ��
 * @retval int err
 */
void Add_SSI2c_HTimeoutRecovery_cb(structSimulateI2c_SlaveCtrl * _strSSI2cCtrl , uint32_t Time1Value);



#endif // __SIMULATE_I2C_SLAVE_H__T
