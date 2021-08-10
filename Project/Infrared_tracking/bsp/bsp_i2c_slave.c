
#include "Platform.h"

uint8_t Rcv_Addr_OK;
uint8_t Flag_RcvOK = 0;
uint8_t addr;
uint8_t addr_val;

uint8_t IIC_REC_BUFFER[INFRARED_REG_MAX][0x02];




#if 1

/* ---------------------------------------------------------------------------------------------------
- ��������: i2c_master_StatusCheck
- ��������: ״̬���
- �������: �ڼ�bit; ��Ӧbit�ϵ�ֵ
- ��������: 
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
- ��������: i2c_slave_init
- ��������: ��ʼ��i2c SLAVEģʽ
- �������: i2c������ַ
- ��������: 
----------------------------------------------------------------------------------------------------*/
extern	void	i2c_slave_init(uint16_t	i2cAddR)
{
	__write_hw_reg32(CPR_CTLAPBCLKEN_GRCTL, 0x8000800);				//IIC_PCLKʱ��ʹ��
	__write_hw_reg32(CPR_RSTCTL_SUBRST_SW, 0x400000);
	__write_hw_reg32(CPR_RSTCTL_SUBRST_SW, 0x400040);					//IIC ģ�鸴λ��д1��λ��				
	__write_hw_reg32(CPR_I2C_CLK_CTL, 0x110011);                //-> i2c_mclk = 16mhz.   BIT4:IIC_CLKʱ��ʹ�ܣ�BIT3-0:IIC_CLKʱ�ӷ�Ƶϵ��
    
    i2c_master_StatusCheck(0, 0);				//״̬��⣬ѭ���ȴ�
	__write_hw_reg32(I2C_ENABLE, 0x00);				//IICʧ��  
	__write_hw_reg32(I2C_SAR, i2cAddR);				//���ñ���IIC��ַ		

	__write_hw_reg32(I2C_RX_TL, 0x00);				//����IIC_RX_FIFO��ֵ������������ > val ʱ����RX_FULL�жϣ����Ϊ16��(0~F)������Ϊ0ʱ����һ�������жϣ�
	__write_hw_reg32(I2C_TX_TL, 0x00);				//����IIC_TX_FIFO��ֵ������������ <= val ʱ����RX_FULL�ж�,����Ϊ1ʱ��������Ϊ1ʱ�����жϣ�
		
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
- ��������: I2C_Handler
- ��������: I2C�жϴ�����
- �������: ��
- ��������: 
----------------------------------------------------------------------------------------------------*/
extern	void	I2C_Handler(void)
{
    uint32_t    stat, VAL;
    
     __read_hw_reg32(I2C_INTR_STAT, stat);
//     printf("stat=%#x =>",stat);                                                                                                    //if(stat == 0) return; //if(stat & 0x200) __read_hw_reg32(I2C_CLR_STOP_DET, VAL);//-�� �� �� �� �� �� �� �� ��STOP_DET    //if(stat & 0x40)  __read_hw_reg32(I2C_CLR_TX_ABRT, VAL); //- ���˼Ĵ�������ж� TX_ABRT
     __read_hw_reg32(I2C_CLR_INTR, VAL);//- ����ж� ���˼Ĵ����������жϣ����������ж��Լ�I2C_TX_ABRT_SOURCE �Ĵ���   
     if(stat & 0x80)//RX_DONE-��Ϊ slave ��������ʱ�����master û����Ӧ�Ļ����ڷ������һ���ֽں�������жϣ���ʾ���ͽ���
     {                            
//         printf("RX_DONE--%#x\n",*(I2C_DATA_CMD));  
     }    
     if(stat & 0x200)//IIC ֹͣ�ź�  --STOP_DET  ����������־ 1�����߳��ֽ�������  0����Ч
     {         
//         printf("STOP_DET--%#x\n",*(I2C_DATA_CMD));  
     } 
     if(stat & 0x4) //��վ�յ���վ�ķ��͵ļĴ�����ַ (��Ӧ�������Ķ���д������)--RX_FULL RX FIFO ����־ 1��RX FIFO ���ݴ��ڻ��ߵ��� RX FIFO ��ֵ���˱����� FIFO״̬�Զ�����  0����Ч
     {
        uint8_t val= *(I2C_DATA_CMD);        
        if(val==(1)) //�Ĵ�����ַ
        {
            *(I2C_DATA_CMD)=0xCC;
        }else if(val==0x02)  //�Ĵ�����ַ
        {
//           printf("*****write - %#2x\n",*(I2C_DATA_CMD));
        }
        else
        {
        
        }            
//        printf("RX_FULL--%#2x\n",val); 
     } 
     if(stat & 0x20) //RD_REQ  //��վ�յ���վ���ٴζ������жϣ�    
     {   //��Ϊ slave��������һ�� master���Դ� slave ������ʱ�������жϡ�Slave ���� I2C ����Ϊ�ȴ�״̬��SCL=0��ֱ���жϷ�����Ӧ��
         //���ʾ slave ��Զ�˵�master Ѱַ������Ҫ���䷢�����ݡ�������������Ӧ����жϣ��� �� �� �� �� �� �� �� д ��I2C_DATA_CMD �Ĵ���  
         static uint8_t count=1;
//         *(I2C_DATA_CMD)=count;
		 *(I2C_DATA_CMD)=IIC_REC_BUFFER[0x05][0x00];
//         printf("RD_REQ--%#x\n",count++); 
     } 
}

void test_slave_i2c()
{
    //IIC ��������1 �Ƕ���0��д  ����״̬��IIC ��SCL SDA��Ϊ�ߵ�ƽ �ȴ�ʱ��סSCL�ߵ��͵�ƽ
    gpio_mux_ctl(4,0);gpio_fun_inter(4,0);gpio_fun_sel(4,5);//scl  5:I2C_SCL
    gpio_mux_ctl(5,0);gpio_fun_inter(5,0);gpio_fun_sel(5,6);//sda  6:I2C_SDA
    //i2c_slave_init(0x3D);//IIC��λ������IIC�ӻ���ַΪ0x7A(����0x3D<<1)
    i2c_slave_init((SELVE_ADDRESS>>1));
    
   // while(1);

}

#else


i2cHandler_callback_t   i2cHandler_Callback = (i2cHandler_callback_t)0;

/* ---------------------------------------------------------------------------------------------------
- ��������: i2c_slave_init
- ��������: ��ʼ��i2c SLAVEģʽ
- �������: i2c������ַ
- ��������: 
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
- ��������: i2c_slave_Register_Callback
- ��������: Ϊi2c���豸ע���жϻص�����
- �������: �ж�״ֵ̬
- ��������: 
----------------------------------------------------------------------------------------------------*/
extern	void	i2c_slave_Register_Callback(i2cHandler_callback_t callback)
{
    i2cHandler_Callback = callback;
}

/* ---------------------------------------------------------------------------------------------------
- ��������: i2c_slave_WriteSingle
- ��������: Ϊi2c���豸��������
- �������: ���͵�����
- ��������: 
----------------------------------------------------------------------------------------------------*/
extern  void    i2c_slave_WriteSingle(uint8_t val)
{
    __write_hw_reg32(I2C_DATA_CMD, val);
}
/* ---------------------------------------------------------------------------------------------------
- ��������: i2c_slave_ReadSingle
- ��������: Ϊi2c���豸��ȡ����
- �������: ��
- ��������: 
----------------------------------------------------------------------------------------------------*/
extern  uint8_t    i2c_slave_ReadSingle(void)
{
    uint8_t     val;
    __read_hw_reg32(I2C_DATA_CMD, val);
    
    return (val);
}
/* ---------------------------------------------------------------------------------------------------
- ��������: I2C_Handler
- ��������: I2C�жϴ�����
- �������: ��
- ��������: 
----------------------------------------------------------------------------------------------------*/
extern	void	I2C_Handler(void)
{
    uint32_t    stat, VAL;
    
    __read_hw_reg32(I2C_INTR_STAT, stat);
    if(stat == 0) return;
    
    if(stat & 0x200) __read_hw_reg32(I2C_CLR_STOP_DET, VAL);    
    if(stat & 0x40)  __read_hw_reg32(I2C_CLR_TX_ABRT, VAL);
    __read_hw_reg32(I2C_CLR_INTR, VAL);      //- ����ж�    
    
    
    if(i2cHandler_Callback != (i2cHandler_callback_t)0) {
        i2cHandler_Callback(stat);
    }
       
}
#endif