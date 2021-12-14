#include "i2c.h"

i2cHandler_callback_t i2cHandler_Callback = (i2cHandler_callback_t)0;

/* ---------------------------------------------------------------------------------------------------
- 函数名称: i2c_statuscheck
- 函数功能: 状态检测
- 输入参数: 第几bit; 对应bit上的值
- 创建日期: 
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
- 函数名称: i2c_master_init
- 函数功能: 初始化i2c Master模式
- 输入参数: i2c器件地址
- 创建日期: 
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
	writeReg32(I2C_INTR_EN, 0x00);  //主机7位地址
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
- 函数名称: i2c_master_write
- 函数功能: 写一个字节数据
- 输入参数: reg: i2c寄存器地址; idata: 写数据;
- 创建日期: 
----------------------------------------------------------------------------------------------------*/
void i2c_master_write(uint8_t reg, uint8_t idata)
{
	i2c_statuscheck(0, 0);
	writeReg32(I2C_DATA_CMD, reg); 
	writeReg32(I2C_DATA_CMD, (idata | 0x200));
}

/* ---------------------------------------------------------------------------------------------------
- 函数名称: i2c_master_writemulti
- 函数功能: 写多个字节数据
- 输入参数: i2c寄存器地址; 写入数据的个数; 写数据存放首地址; 
- 创建日期: 
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
- 函数名称: i2c_master_read
- 函数功能: 读一个字节数据
- 输入参数: i2c寄存器地址; 读出数据存放首地址; 
- 创建日期: 
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
- 函数名称: i2c_master_readmulti
- 函数功能: 读多个字节数据
- 输入参数: i2c寄存器地址; 读出数据的个数(必须大于等于2,小于16) ; 读出数据存放首地址; 
- 创建日期: 
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
- 函数名称: i2c_slave_init
- 函数功能: 初始化i2c SLAVE模式
- 输入参数: i2c器件地址
- 创建日期: 
----------------------------------------------------------------------------------------------------*/
void i2c_slave_init(uint8_t sda_pin, uint8_t scl_pin, uint16_t i2cAddR)
{
	gpio_mux_ctl(sda_pin, 0);
	gpio_fun_inter(sda_pin, 0);
	gpio_mux_ctl(scl_pin, 0);
	gpio_fun_inter(scl_pin, 0);
	gpio_fun_sel(sda_pin, I2C_SDA);
	gpio_fun_sel(scl_pin, I2C_SCL);
	writeReg32(CPR_CTLAPBCLKEN_GRCTL, 0x8000800);				//IIC_PCLK时钟使能
	writeReg32(CPR_RSTCTL_SUBRST_SW, 0x400000);
	writeReg32(CPR_RSTCTL_SUBRST_SW, 0x400040);					//IIC 模块复位（写1复位）				
	writeReg32(CPR_I2C_CLK_CTL, 0x110011);                //-> i2c_mclk = 16mhz.   BIT4:IIC_CLK时钟使能，BIT3-0:IIC_CLK时钟分频系数    
    i2c_statuscheck(0, 0);				//状态检测，循环等待
	writeReg32(I2C_ENABLE, 0x00);				//IIC失能  
	writeReg32(I2C_SAR, i2cAddR);				//设置本机IIC地址		
	writeReg32(I2C_RX_TL, 0x00);				//设置IIC_RX_FIFO阈值，当接收数据 > val 时触发RX_FULL中断（最大为16个(0~F)，设置为0时接收一个发出中断）
	writeReg32(I2C_TX_TL, 0x00);				//设置IIC_TX_FIFO阈值，当接收数据 <= val 时触发RX_FULL中断,设置为1时，数据项为1时发出中断）		
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
- 函数名称: i2c_slave_register_callback
- 函数功能: 为i2c从设备注册中断回调函数
- 输入参数: 中断状态值
- 创建日期: 
----------------------------------------------------------------------------------------------------*/
void i2c_slave_register_callback(i2cHandler_callback_t callback)
{
    i2cHandler_Callback = callback;
}

/* ---------------------------------------------------------------------------------------------------
- 函数名称: i2c_slave_write
- 函数功能: 为i2c从设备发送数据
- 输入参数: 发送的数据
- 创建日期: 
----------------------------------------------------------------------------------------------------*/
void i2c_slave_write(uint8_t val)
{
    writeReg32(I2C_DATA_CMD, val);
}
/* ---------------------------------------------------------------------------------------------------
- 函数名称: i2c_slave_read
- 函数功能: 为i2c从设备获取数据
- 输入参数: 无
- 创建日期: 
----------------------------------------------------------------------------------------------------*/
uint8_t i2c_slave_read(void)
{
    uint8_t val;
    readReg32(I2C_DATA_CMD, val); 
    return (val);
}
/* ---------------------------------------------------------------------------------------------------
- 函数名称: I2C_Handler
- 函数功能: I2C中断处理函数
- 输入参数: 无
- 创建日期: 
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
        readReg32(I2C_CLR_INTR, value);      //- 清除中断    
	if (stat & 0x20) {
		if(i2cHandler_Callback != (i2cHandler_callback_t)0) {
			i2cHandler_Callback();
		}  
	}
}
//IIC_SLAVE_END
