
#include "Platform.h"

uint8_t Rcv_Addr_OK;
uint8_t Flag_RcvOK = 0;
uint8_t addr;
uint8_t addr_val;

uint8_t IIC_REC_BUFFER[INFRARED_REG_MAX][0x02];




#if 1

/* ---------------------------------------------------------------------------------------------------
- 函数名称: i2c_master_StatusCheck
- 函数功能: 状态检测
- 输入参数: 第几bit; 对应bit上的值
- 创建日期: 
----------------------------------------------------------------------------------------------------*/
static	void	i2c_master_StatusCheck(uint8_t	nbit, uint8_t bitval)
{
	uint8_t	val;
	
	for(; ;)	{
		__read_hw_reg32(I2C_STATUS , val);
		if( bitval == ((val>>nbit)&0x01) ) break;
	}	

}


/* ---------------------------------------------------------------------------------------------------
- 函数名称: i2c_slave_init
- 函数功能: 初始化i2c SLAVE模式
- 输入参数: i2c器件地址
- 创建日期: 
----------------------------------------------------------------------------------------------------*/
extern	void	i2c_slave_init(uint16_t	i2cAddR)
{
	__write_hw_reg32(CPR_CTLAPBCLKEN_GRCTL, 0x8000800);				//IIC_PCLK时钟使能
	__write_hw_reg32(CPR_RSTCTL_SUBRST_SW, 0x400000);
	__write_hw_reg32(CPR_RSTCTL_SUBRST_SW, 0x400040);					//IIC 模块复位（写1复位）				
	__write_hw_reg32(CPR_I2C_CLK_CTL, 0x110011);                //-> i2c_mclk = 16mhz.   BIT4:IIC_CLK时钟使能，BIT3-0:IIC_CLK时钟分频系数
    
    i2c_master_StatusCheck(0, 0);				//状态检测，循环等待
	__write_hw_reg32(I2C_ENABLE, 0x00);				//IIC失能  
	__write_hw_reg32(I2C_SAR, i2cAddR);				//设置本机IIC地址		

	__write_hw_reg32(I2C_RX_TL, 0x00);				//设置IIC_RX_FIFO阈值，当接收数据 > val 时触发RX_FULL中断（最大为16个(0~F)，设置为0时接收一个发出中断）
	__write_hw_reg32(I2C_TX_TL, 0x00);				//设置IIC_TX_FIFO阈值，当接收数据 <= val 时触发RX_FULL中断,设置为1时，数据项为1时发出中断）
		
	__write_hw_reg32(I2C_CON, 0x00);
   
 	__write_hw_reg32(I2C_INTR_EN, (I2C_SLAVE_RX_DONE | I2C_SLAVE_RD_REQ | I2C_SLAVE_RX_FULL | I2C_SLAVE_STOP )); 
    
    __write_hw_reg32(I2C_FS_SCL_HCNT, 12);
    __write_hw_reg32(I2C_FS_SCL_LCNT, 19); 
    __write_hw_reg32(I2C_SS_SCL_HCNT, 72);
    __write_hw_reg32(I2C_SS_SCL_LCNT, 79); 

    i2c_master_StatusCheck(0, 0);
    __write_hw_reg32(I2C_ENABLE, 0x1);
    
    NVIC_EnableIRQ(I2C_IRQn);

}
#define SELVE_ADDRESS (0x50 << 1)
/* ---------------------------------------------------------------------------------------------------
- 函数名称: I2C_Handler
- 函数功能: I2C中断处理函数
- 输入参数: 无
- 创建日期: 
----------------------------------------------------------------------------------------------------*/
extern	void	I2C_Handler(void)
{
    uint32_t    stat, VAL;
    
     __read_hw_reg32(I2C_INTR_STAT, stat);
//     printf("stat=%#x =>",stat);                                                                                                    //if(stat == 0) return; //if(stat & 0x200) __read_hw_reg32(I2C_CLR_STOP_DET, VAL);//-读 此 寄 存 器 清 除 中 断STOP_DET    //if(stat & 0x40)  __read_hw_reg32(I2C_CLR_TX_ABRT, VAL); //- 读此寄存器清除中断 TX_ABRT
     __read_hw_reg32(I2C_CLR_INTR, VAL);//- 清除中断 读此寄存器清除组合中断，各个独立中断以及I2C_TX_ABRT_SOURCE 寄存器   
     if(stat & 0x80)//RX_DONE-作为 slave 发送数据时，如果master 没有响应的话，在发送最后一个字节后产生此中断，表示发送结束
     {                            
//         printf("RX_DONE--%#x\n",*(I2C_DATA_CMD));  
     }    
     if(stat & 0x200)//IIC 停止信号  --STOP_DET  结束条件标志 1：总线出现结束条件  0：无效
     {         
//         printf("STOP_DET--%#x\n",*(I2C_DATA_CMD));  
     } 
     if(stat & 0x4) //从站收到主站的发送的寄存器地址 (并应答主机的读或写的请求)--RX_FULL RX FIFO 满标志 1：RX FIFO 数据大于或者等于 RX FIFO 阈值。此比特随 FIFO状态自动更新  0：无效
     {
        uint8_t val= *(I2C_DATA_CMD);        
        if(val==(1)) //寄存器地址
        {
            *(I2C_DATA_CMD)=0xCC;
        }else if(val==0x02)  //寄存器地址
        {
//           printf("*****write - %#2x\n",*(I2C_DATA_CMD));
        }
        else
        {
        
        }            
//        printf("RX_FULL--%#2x\n",val); 
     } 
     if(stat & 0x20) //RD_REQ  //从站收到主站的再次读请求中断，    
     {   //作为 slave，当另外一个 master尝试从 slave 读数据时产生此中断。Slave 保持 I2C 总线为等待状态（SCL=0）直到中断服务被响应。
         //这表示 slave 被远端的master 寻址，并且要求其发送数据。处理器必须响应这个中断，并 将 待 发 送 的 数 据 写 入I2C_DATA_CMD 寄存器  
         static uint8_t count=1;
//         *(I2C_DATA_CMD)=count;
		 *(I2C_DATA_CMD)=IIC_REC_BUFFER[0x05][0x00];
//         printf("RD_REQ--%#x\n",count++); 
     } 
}

void test_slave_i2c()
{
    //IIC 主机命令1 是读，0是写  空闲状态下IIC 的SCL SDA都为高电平 等待时擎住SCL线到低电平
    gpio_mux_ctl(4,0);gpio_fun_inter(4,0);gpio_fun_sel(4,5);//scl  5:I2C_SCL
    gpio_mux_ctl(5,0);gpio_fun_inter(5,0);gpio_fun_sel(5,6);//sda  6:I2C_SDA
    //i2c_slave_init(0x3D);//IIC上位机侦测的IIC从机地址为0x7A(等于0x3D<<1)
    i2c_slave_init((SELVE_ADDRESS>>1));
    
   // while(1);

}

#else


i2cHandler_callback_t   i2cHandler_Callback = (i2cHandler_callback_t)0;

/* ---------------------------------------------------------------------------------------------------
- 函数名称: i2c_slave_init
- 函数功能: 初始化i2c SLAVE模式
- 输入参数: i2c器件地址
- 创建日期: 
----------------------------------------------------------------------------------------------------*/
extern	void	i2c_slave_init(uint16_t	i2cAddR)
{
	__write_hw_reg32(CPR_CTLAPBCLKEN_GRCTL, 0x8000800);

	__write_hw_reg32(CPR_RSTCTL_SUBRST_SW, 0x400000);
	__write_hw_reg32(CPR_RSTCTL_SUBRST_SW, 0x400040);

	__write_hw_reg32(CPR_I2C_CLK_CTL, 0x110011);                //-> i2c_mclk = 16mhz.

	__write_hw_reg32(I2C_ENABLE, 0x00);  
	__write_hw_reg32(I2C_SAR, i2cAddR);		

	__write_hw_reg32(I2C_RX_TL, 0x00);
	__write_hw_reg32(I2C_TX_TL, 0x00);
		
	__write_hw_reg32(I2C_CON, 0x00);
   
 	__write_hw_reg32(I2C_INTR_EN, (I2C_SLAVE_RX_DONE | I2C_SLAVE_RD_REQ | I2C_SLAVE_RX_FULL | I2C_SLAVE_STOP )); 
    
    
    __write_hw_reg32(I2C_ENABLE, 0x1);
    
    NVIC_EnableIRQ(I2C_IRQn);

}

/* ---------------------------------------------------------------------------------------------------
- 函数名称: i2c_slave_Register_Callback
- 函数功能: 为i2c从设备注册中断回调函数
- 输入参数: 中断状态值
- 创建日期: 
----------------------------------------------------------------------------------------------------*/
extern	void	i2c_slave_Register_Callback(i2cHandler_callback_t callback)
{
    i2cHandler_Callback = callback;
}

/* ---------------------------------------------------------------------------------------------------
- 函数名称: i2c_slave_WriteSingle
- 函数功能: 为i2c从设备发送数据
- 输入参数: 发送的数据
- 创建日期: 
----------------------------------------------------------------------------------------------------*/
extern  void    i2c_slave_WriteSingle(uint8_t val)
{
    __write_hw_reg32(I2C_DATA_CMD, val);
}
/* ---------------------------------------------------------------------------------------------------
- 函数名称: i2c_slave_ReadSingle
- 函数功能: 为i2c从设备获取数据
- 输入参数: 无
- 创建日期: 
----------------------------------------------------------------------------------------------------*/
extern  uint8_t    i2c_slave_ReadSingle(void)
{
    uint8_t     val;
    __read_hw_reg32(I2C_DATA_CMD, val);
    
    return (val);
}
/* ---------------------------------------------------------------------------------------------------
- 函数名称: I2C_Handler
- 函数功能: I2C中断处理函数
- 输入参数: 无
- 创建日期: 
----------------------------------------------------------------------------------------------------*/
extern	void	I2C_Handler(void)
{
    uint32_t    stat, VAL;
    
    __read_hw_reg32(I2C_INTR_STAT, stat);
    if(stat == 0) return;
    
    if(stat & 0x200) __read_hw_reg32(I2C_CLR_STOP_DET, VAL);    
    if(stat & 0x40)  __read_hw_reg32(I2C_CLR_TX_ABRT, VAL);
    __read_hw_reg32(I2C_CLR_INTR, VAL);      //- 清除中断    
    
    
    if(i2cHandler_Callback != (i2cHandler_callback_t)0) {
        i2cHandler_Callback(stat);
    }
       
}
#endif