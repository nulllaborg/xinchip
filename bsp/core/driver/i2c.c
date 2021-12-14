#include "i2c.h"

i2cHandler_callback_t i2cHandler_Callback = (i2cHandler_callback_t)0;

/* ---------------------------------------------------------------------------------------------------
- ��������: i2c_statuscheck
- ��������: ״̬���
- �������: �ڼ�bit; ��Ӧbit�ϵ�ֵ
- ��������: 
----------------------------------------------------------------------------------------------------*/
void i2c_statuscheck(uint8_t nbit, uint8_t bitval)
{
	uint8_t	val;
	for ( ; ; )	{
		readReg32(I2C_STATUS, val);
		if(bitval == ((val >> nbit) & 0x01)) break;
	}	
}


//IIC_MASTER_START
/* ---------------------------------------------------------------------------------------------------
- ��������: i2c_master_init
- ��������: ��ʼ��i2c Masterģʽ
- �������: i2c������ַ
- ��������: 
----------------------------------------------------------------------------------------------------*/
void i2c_master_init(uint8_t sda_pin, uint8_t scl_pin, uint8_t i2c_addr, uint32_t speed)
{
	gpio_mux_ctl(sda_pin, 0);
	gpio_fun_inter(sda_pin, 0);
	gpio_mux_ctl(scl_pin, 0);
	gpio_fun_inter(scl_pin, 0);
	gpio_fun_sel(sda_pin, I2C_SDA);
	gpio_fun_sel(scl_pin, I2C_SCL);
	__ASSERT((speed == I2C_FAST_MODE) || (speed == I2C_STANDARD_MODE));
	writeReg32(CPR_CTLAPBCLKEN_GRCTL, 0x8000800);
	writeReg32(CPR_RSTCTL_SUBRST_SW, 0x400000);
	writeReg32(CPR_RSTCTL_SUBRST_SW, 0x400040);
	writeReg32(CPR_I2C_CLK_CTL, 0x110011);                //-> i2c_mclk = 16mhz.
	writeReg32(I2C_ENABLE, 0x0); 
	writeReg32(I2C_TAR, i2c_addr);
	writeReg32(I2C_RX_TL, 0x00);
	writeReg32(I2C_TX_TL, 0x00);
	writeReg32(I2C_INTR_EN, 0x00);  //����7λ��ַ
	if(speed == I2C_FAST_MODE) {
		writeReg32(I2C_CON, 0x65); 
		writeReg32(I2C_FS_SCL_HCNT, 12);
		writeReg32(I2C_FS_SCL_LCNT, 19); 
	} else if(speed == I2C_STANDARD_MODE) {
		writeReg32(I2C_CON, 0x63); 
		writeReg32(I2C_SS_SCL_HCNT, 72);
		writeReg32(I2C_SS_SCL_LCNT, 79); 
	}
	writeReg32(I2C_ENABLE, 0x1);
}

void i2c_master_set_addr(uint8_t i2c_addr)
{
    writeReg32(I2C_ENABLE, 0x0); 
    writeReg32(I2C_TAR, i2c_addr);
	writeReg32(I2C_ENABLE, 0x1);
}
/* ---------------------------------------------------------------------------------------------------
- ��������: i2c_master_write
- ��������: дһ���ֽ�����
- �������: reg: i2c�Ĵ�����ַ; idata: д����;
- ��������: 
----------------------------------------------------------------------------------------------------*/
void i2c_master_write(uint8_t reg, uint8_t idata)
{
	i2c_statuscheck(0, 0);
	writeReg32(I2C_DATA_CMD, reg); 
	writeReg32(I2C_DATA_CMD, (idata | 0x200));
}

/* ---------------------------------------------------------------------------------------------------
- ��������: i2c_master_writemulti
- ��������: д����ֽ�����
- �������: i2c�Ĵ�����ַ; д�����ݵĸ���; д���ݴ���׵�ַ; 
- ��������: 
----------------------------------------------------------------------------------------------------*/
void i2c_master_writemulti(uint8_t suba, uint8_t no, uint8_t *s)
{
	uint8_t i;
	i2c_statuscheck(0, 0);
	writeReg32(I2C_DATA_CMD, suba); 
	for(i = 0; i < (no - 1); i++) {
		i2c_statuscheck(1, 1);
		writeReg32(I2C_DATA_CMD, *s++); 
	}   
	i2c_statuscheck(1, 1);
	writeReg32(I2C_DATA_CMD, (*s++ | 0x200));  
}


/* ---------------------------------------------------------------------------------------------------
- ��������: i2c_master_read
- ��������: ��һ���ֽ�����
- �������: i2c�Ĵ�����ַ; �������ݴ���׵�ַ; 
- ��������: 
----------------------------------------------------------------------------------------------------*/
void i2c_master_read(uint8_t suba, uint8_t *s)
{
	uint8_t	val;
	i2c_statuscheck(0, 0);
	writeReg32(I2C_DATA_CMD, suba); 
	writeReg32(I2C_DATA_CMD, 0x300);
	i2c_statuscheck(3, 1);
	readReg32(I2C_DATA_CMD, val);
	*s = (uint8_t)val;
}

/* ---------------------------------------------------------------------------------------------------
- ��������: i2c_master_readmulti
- ��������: ������ֽ�����
- �������: i2c�Ĵ�����ַ; �������ݵĸ���(������ڵ���2,С��16) ; �������ݴ���׵�ַ; 
- ��������: 
----------------------------------------------------------------------------------------------------*/
void i2c_master_readmulti(uint8_t suba, uint8_t no, uint8_t *s)
{
	uint8_t val, i;
	i2c_statuscheck(0, 0);
	writeReg32(I2C_DATA_CMD, suba); 
	for(i = 0; i < (no - 1); i++) {
		i2c_statuscheck(1, 1);
		writeReg32(I2C_DATA_CMD, 0x100); 
	}   
	i2c_statuscheck(1, 1);
	writeReg32(I2C_DATA_CMD, 0x300);  
	for(i=0; i<no; i++) {
		i2c_statuscheck(3, 1);
		readReg32(I2C_DATA_CMD, val);
		*s++ = val;
	}						
}
//IIC_MASTER_END

//IIC_SLAVE_START
/* ---------------------------------------------------------------------------------------------------
- ��������: i2c_slave_init
- ��������: ��ʼ��i2c SLAVEģʽ
- �������: i2c������ַ
- ��������: 
----------------------------------------------------------------------------------------------------*/
void i2c_slave_init(uint8_t sda_pin, uint8_t scl_pin, uint16_t i2cAddR)
{
	gpio_mux_ctl(sda_pin, 0);
	gpio_fun_inter(sda_pin, 0);
	gpio_mux_ctl(scl_pin, 0);
	gpio_fun_inter(scl_pin, 0);
	gpio_fun_sel(sda_pin, I2C_SDA);
	gpio_fun_sel(scl_pin, I2C_SCL);
	writeReg32(CPR_CTLAPBCLKEN_GRCTL, 0x8000800);				//IIC_PCLKʱ��ʹ��
	writeReg32(CPR_RSTCTL_SUBRST_SW, 0x400000);
	writeReg32(CPR_RSTCTL_SUBRST_SW, 0x400040);					//IIC ģ�鸴λ��д1��λ��				
	writeReg32(CPR_I2C_CLK_CTL, 0x110011);                //-> i2c_mclk = 16mhz.   BIT4:IIC_CLKʱ��ʹ�ܣ�BIT3-0:IIC_CLKʱ�ӷ�Ƶϵ��    
    i2c_statuscheck(0, 0);				//״̬��⣬ѭ���ȴ�
	writeReg32(I2C_ENABLE, 0x00);				//IICʧ��  
	writeReg32(I2C_SAR, i2cAddR);				//���ñ���IIC��ַ		
	writeReg32(I2C_RX_TL, 0x00);				//����IIC_RX_FIFO��ֵ������������ > val ʱ����RX_FULL�жϣ����Ϊ16��(0~F)������Ϊ0ʱ����һ�������жϣ�
	writeReg32(I2C_TX_TL, 0x00);				//����IIC_TX_FIFO��ֵ������������ <= val ʱ����RX_FULL�ж�,����Ϊ1ʱ��������Ϊ1ʱ�����жϣ�		
	writeReg32(I2C_CON, 0x00);   
 	writeReg32(I2C_INTR_EN, (I2C_SLAVE_RX_DONE | I2C_SLAVE_RD_REQ | I2C_SLAVE_RX_FULL | I2C_SLAVE_STOP));     
    writeReg32(I2C_FS_SCL_HCNT, 12);
    writeReg32(I2C_FS_SCL_LCNT, 19); 
    writeReg32(I2C_SS_SCL_HCNT, 72);
    writeReg32(I2C_SS_SCL_LCNT, 79); 
    i2c_statuscheck(0, 0);
    writeReg32(I2C_ENABLE, 0x1);    
    NVIC_EnableIRQ(I2C_IRQn);

}

/* ---------------------------------------------------------------------------------------------------
- ��������: i2c_slave_register_callback
- ��������: Ϊi2c���豸ע���жϻص�����
- �������: �ж�״ֵ̬
- ��������: 
----------------------------------------------------------------------------------------------------*/
void i2c_slave_register_callback(i2cHandler_callback_t callback)
{
    i2cHandler_Callback = callback;
}

/* ---------------------------------------------------------------------------------------------------
- ��������: i2c_slave_write
- ��������: Ϊi2c���豸��������
- �������: ���͵�����
- ��������: 
----------------------------------------------------------------------------------------------------*/
void i2c_slave_write(uint8_t val)
{
    writeReg32(I2C_DATA_CMD, val);
}
/* ---------------------------------------------------------------------------------------------------
- ��������: i2c_slave_read
- ��������: Ϊi2c���豸��ȡ����
- �������: ��
- ��������: 
----------------------------------------------------------------------------------------------------*/
uint8_t i2c_slave_read(void)
{
    uint8_t val;
    readReg32(I2C_DATA_CMD, val); 
    return (val);
}
/* ---------------------------------------------------------------------------------------------------
- ��������: I2C_Handler
- ��������: I2C�жϴ�����
- �������: ��
- ��������: 
----------------------------------------------------------------------------------------------------*/
void I2C_Handler(void)
{
    uint32_t stat, value;   
    readReg32(I2C_INTR_STAT, stat);
    if(stat == 0) return;
    if(stat & 0x200) 
		readReg32(I2C_CLR_STOP_DET, value);    
    if(stat & 0x40)  
		readReg32(I2C_CLR_TX_ABRT, value);
        readReg32(I2C_CLR_INTR, value);      //- ����ж�    
	if (stat & 0x20) {
		if(i2cHandler_Callback != (i2cHandler_callback_t)0) {
			i2cHandler_Callback();
		}  
	}
}
//IIC_SLAVE_END
