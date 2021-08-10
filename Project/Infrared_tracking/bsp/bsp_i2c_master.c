
#include	"Platform.h"

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
	/*IIC״̬�Ĵ���:					bit4 		 |     bit3     |    bit2    |    bit1      |  bit0
													RX FIFO �� | RX FIFO ���� | RX FIFO �� | RX FIFO ���� | ����״̬
	*/
	for(; ;)	{
		__read_hw_reg32(I2C_STATUS , val);
		if( bitval == ((val>>nbit)&0x01) ) break;
	}	

}



/* ---------------------------------------------------------------------------------------------------
- ��������: i2c_master_init
- ��������: ��ʼ��i2c Masterģʽ
- �������: i2c������ַ
- ��������: 
----------------------------------------------------------------------------------------------------*/
extern	void	i2c_master_init(uint8_t	i2cAddR, uint32_t speed)
{
	uint32_t	i;
    
    __ASSERT((speed==I2C_FAST_MODE) || (speed = I2C_STANDARD_MODE));
    
	__write_hw_reg32(CPR_CTLAPBCLKEN_GRCTL, 0x8000800);

	__write_hw_reg32(CPR_RSTCTL_SUBRST_SW, 0x400000);
	__write_hw_reg32(CPR_RSTCTL_SUBRST_SW, 0x400040);

	__write_hw_reg32(CPR_I2C_CLK_CTL, 0x110011);                //-> i2c_mclk = 16mhz.
 						 
	__write_hw_reg32(I2C_ENABLE, 0x0); 

	__write_hw_reg32(I2C_TAR, i2cAddR);
	__write_hw_reg32(I2C_RX_TL, 0x00);
	__write_hw_reg32(I2C_TX_TL, 0x00);
	__write_hw_reg32(I2C_INTR_EN, 0x00);
    
    if(speed == I2C_FAST_MODE) {
        
        __write_hw_reg32(I2C_CON, 0x65); 
        __write_hw_reg32(I2C_FS_SCL_HCNT, 12);
        __write_hw_reg32(I2C_FS_SCL_LCNT, 19); 
    }
    else if(speed == I2C_STANDARD_MODE) {

        __write_hw_reg32(I2C_CON, 0x63); 
        __write_hw_reg32(I2C_SS_SCL_HCNT, 72);
        __write_hw_reg32(I2C_SS_SCL_LCNT, 79); 
    }
	__write_hw_reg32(I2C_ENABLE, 0x1);
}

/* ---------------------------------------------------------------------------------------------------
- ��������: i2c_master_WriteSingle
- ��������: дһ���ֽ�����
- �������: i2c�Ĵ�����ַ; д����;
- ��������: 
----------------------------------------------------------------------------------------------------*/
extern	void	i2c_master_WriteSingle(uint8_t	suba, uint8_t idata)
{
	i2c_master_StatusCheck(0, 0);

	__write_hw_reg32(I2C_DATA_CMD, suba); 
	__write_hw_reg32(I2C_DATA_CMD, (idata | 0x200));

	
}
/* ---------------------------------------------------------------------------------------------------
- ��������: i2c_master_WriteMulti
- ��������: д����ֽ�����
- �������: i2c�Ĵ�����ַ; д�����ݵĸ���; д���ݴ���׵�ַ; 
- ��������: 
----------------------------------------------------------------------------------------------------*/
extern	void	i2c_master_WriteMulti(uint8_t	suba, uint8_t no, uint8_t *s)
{
	uint8_t i;
  
	i2c_master_StatusCheck(0, 0);

 	__write_hw_reg32(I2C_DATA_CMD, suba); 

	for(i=0; i<(no-1); i++)
	{
	   	i2c_master_StatusCheck(1, 1);
	   	__write_hw_reg32(I2C_DATA_CMD, *s++); 
    }   
	i2c_master_StatusCheck(1, 1);
	__write_hw_reg32(I2C_DATA_CMD, (*s++ | 0x200));  
	 			
}


/* ---------------------------------------------------------------------------------------------------
- ��������: i2c_master_ReadSingle
- ��������: ��һ���ֽ�����
- �������: i2c�Ĵ�����ַ; �������ݴ���׵�ַ; 
- ��������: 
----------------------------------------------------------------------------------------------------*/
extern	void	i2c_master_ReadSingle(uint8_t	suba, uint8_t *s)
{
	uint8_t	val;

	i2c_master_StatusCheck(0, 0);
	
	__write_hw_reg32(I2C_DATA_CMD, suba); 
	__write_hw_reg32(I2C_DATA_CMD, 0x300);

	i2c_master_StatusCheck(3, 1);

	__read_hw_reg32(I2C_DATA_CMD , val);
	*s = (uint8_t)val;
}


/* ---------------------------------------------------------------------------------------------------
- ��������: i2c_master_ReadMulti
- ��������: ������ֽ�����
- �������: i2c�Ĵ�����ַ; �������ݵĸ���(������ڵ���2,С��16) ; �������ݴ���׵�ַ; 
- ��������: 
----------------------------------------------------------------------------------------------------*/
extern	void	i2c_master_ReadMulti(uint8_t	suba, uint8_t no , uint8_t *s)
{
	uint8_t val, i;
  
	i2c_master_StatusCheck(0, 0);

 	__write_hw_reg32(I2C_DATA_CMD, suba); 
	for(i=0; i<(no-1); i++)
	{
	   	i2c_master_StatusCheck(1, 1);
	   	__write_hw_reg32(I2C_DATA_CMD, 0x100); 
    }   
	i2c_master_StatusCheck(1, 1);
	__write_hw_reg32(I2C_DATA_CMD, 0x300);  
	
  	for(i=0; i<no; i++)
	{
		i2c_master_StatusCheck(3, 1);
		__read_hw_reg32(I2C_DATA_CMD , val);
        *s++ = val;
	}						
}

	
///////////////////////////////////////////////////////////////////////////////////////////////


#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		908
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767  
#define EE_TYPE     AT24C08


uint8_t  at24cxx_read_one_byte(uint16_t read_addr)
{
    uint8_t val=0;
    i2c_master_StatusCheck(0, 0);
    //������������ʱ �ȷ��ʹӻ���ַȻ������������
    if(EE_TYPE>AT24C16){
        __write_hw_reg32(I2C_DATA_CMD, read_addr/256); //���͸ߵ�ַ
    
    }else {
        if(read_addr>255){
             __write_hw_reg32(I2C_ENABLE, 0x0); 
	         __write_hw_reg32(I2C_TAR, 0x50+(read_addr/256));
             __write_hw_reg32(I2C_ENABLE, 0x1); 
        }
    }
    __write_hw_reg32(I2C_DATA_CMD,(read_addr%256)); //���͵͵�ַ
    __write_hw_reg32(I2C_DATA_CMD,(0x300)); //
    i2c_master_StatusCheck(3, 1);
	__read_hw_reg32(I2C_DATA_CMD , val);//�ظ�Ӧ��ʱ�ӻ�Ҳ���Ȼظ��Լ��Ĵӻ���ַ Ȼ��������ǻظ�������
    return val;    
    
}
void  at24cxx_write_one_byte(uint16_t read_addr,uint8_t data)
{

    i2c_master_StatusCheck(0, 0);
    if(EE_TYPE>AT24C16){
        __write_hw_reg32(I2C_DATA_CMD, read_addr/256); //���͸ߵ�ַ
    
    }else{
        if(read_addr>255){
             __write_hw_reg32(I2C_ENABLE, 0x0); 
	         __write_hw_reg32(I2C_TAR, 0x50+(read_addr/256));
             __write_hw_reg32(I2C_ENABLE, 0x1); 
        }
    }
    __write_hw_reg32(I2C_DATA_CMD,(read_addr%256)); //���͵͵�ַ
    __write_hw_reg32(I2C_DATA_CMD,(data|0x200)); // 
    for(int i=0;i<8777;i++);//32M����  дEEPROM ʱҪ��ʱ�����д��ȥ
}


uint8_t at24cxx_check(void)
{
	uint8_t temp;
	temp=at24cxx_read_one_byte(EE_TYPE);//����ÿ�ο�����дAT24CXX			   
	if(temp==0X55)return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		at24cxx_write_one_byte(EE_TYPE,0X55);
	    temp=at24cxx_read_one_byte(EE_TYPE);	  
		if(temp==0X55)return 0;
	}
	return 1;


    
}
void at24cxx_read(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead)
{    
    uint8_t val=0;
    i2c_master_StatusCheck(0, 0);
    //������������ʱ �ȷ��ʹӻ���ַȻ������ŵ�����
    if(ReadAddr>255&&(EE_TYPE<=AT24C16)){
         __write_hw_reg32(I2C_ENABLE, 0x0); 
         __write_hw_reg32(I2C_TAR, 0x50+(ReadAddr/256));
         __write_hw_reg32(I2C_ENABLE, 0x1); 
    }
    
    while(NumToRead)
	{
       if(EE_TYPE>AT24C16)  __write_hw_reg32(I2C_DATA_CMD, ReadAddr/256); //���͸ߵ�ַ
        __write_hw_reg32(I2C_DATA_CMD,(ReadAddr%256)); //���͵͵�ַ
        __write_hw_reg32(I2C_DATA_CMD,(0x300)); //
        i2c_master_StatusCheck(3, 1);
	    __read_hw_reg32(I2C_DATA_CMD , val);//�ظ�Ӧ��ʱ�ӻ�Ҳ���Ȼظ��Լ��Ĵӻ���ַ Ȼ��������ǻظ�������
    
		*pBuffer++=val;	
		NumToRead--;
        ReadAddr++;
	}

}
void at24cxx_write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite)
{
	
    
    i2c_master_StatusCheck(0, 0);
    //������������ʱ �ȷ��ʹӻ���ַȻ������ŵ�����
    if(WriteAddr>255&&(EE_TYPE<=AT24C16)){
         __write_hw_reg32(I2C_ENABLE, 0x0); 
         __write_hw_reg32(I2C_TAR, 0x50+(WriteAddr/256));
         __write_hw_reg32(I2C_ENABLE, 0x1); 
    }

    while(NumToWrite--)
	{
        if(EE_TYPE>AT24C16)  __write_hw_reg32(I2C_DATA_CMD, WriteAddr/256); //���͸ߵ�ַ
        __write_hw_reg32(I2C_DATA_CMD,(WriteAddr%256)); //���͵͵�ַ
        __write_hw_reg32(I2C_DATA_CMD,(*pBuffer|0x200)); // 
		WriteAddr++;
		pBuffer++;
        for(int i=0;i<8777;i++);//32M����  дEEPROM ʱҪ��ʱ�����д��ȥ
	}
}
 
const uint8_t TEXT_Buf[]={"AT24C08 IIC TEST..."};
#define SIZE sizeof(TEXT_Buf)
void test_master_at24cxx_i2c(void)
{ 
    //IIC ��������1 �Ƕ���0��д   ����״̬��IIC ��SCL SDA��Ϊ�ߵ�ƽ ���ȴ�ʱ��סSCL�ߵ��͵�ƽ
    uint8_t datatemp[SIZE]="";
    gpio_mux_ctl(4,0);gpio_fun_inter(4,0);gpio_fun_sel(4,6);//sda  6:I2C_SDA
    gpio_mux_ctl(5,0);gpio_fun_inter(5,0);gpio_fun_sel(5,5);//scl  5:I2C_SCL
    i2c_master_init(0x50, I2C_STANDARD_MODE);//0x50=0xA0>>1
    while(at24cxx_check());//24c08
    at24cxx_write(1001,(uint8_t*)TEXT_Buf,SIZE);
	at24cxx_read(1001,datatemp,SIZE);
	printf("%s",datatemp);
    while(1);
    
}


#else
/* ---------------------------------------------------------------------------------------------------
- ��������: i2c_master_init
- ��������: ��ʼ��i2c Masterģʽ
- �������: i2c������ַ
- ��������: 
----------------------------------------------------------------------------------------------------*/
extern	void	i2c_master_init(uint8_t	i2cAddR, uint32_t speed)
{
	uint32_t	i;
    
    __ASSERT((speed==I2C_FAST_MODE) || (speed = I2C_STANDARD_MODE));
    
	__write_hw_reg32(CPR_CTLAPBCLKEN_GRCTL, 0x8000800);

	__write_hw_reg32(CPR_RSTCTL_SUBRST_SW, 0x400000);
	__write_hw_reg32(CPR_RSTCTL_SUBRST_SW, 0x400040);

	__write_hw_reg32(CPR_I2C_CLK_CTL, 0x110011);                //-> i2c_mclk = 16mhz.
 						 
	__write_hw_reg32(I2C_ENABLE, 0x0); 

	__write_hw_reg32(I2C_TAR, i2cAddR);
	__write_hw_reg32(I2C_RX_TL, 0x00);
	__write_hw_reg32(I2C_TX_TL, 0x00);
	__write_hw_reg32(I2C_INTR_EN, 0x00);
    
    if(speed == I2C_FAST_MODE) {
        
        __write_hw_reg32(I2C_CON, 0x65); 
        __write_hw_reg32(I2C_FS_SCL_HCNT, 12);
        __write_hw_reg32(I2C_FS_SCL_LCNT, 19); 
    }
    else if(speed == I2C_STANDARD_MODE) {

        __write_hw_reg32(I2C_CON, 0x63); 
        __write_hw_reg32(I2C_SS_SCL_HCNT, 72);
        __write_hw_reg32(I2C_SS_SCL_LCNT, 79); 
    }
	__write_hw_reg32(I2C_ENABLE, 0x1);
}

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
- ��������: i2c_master_WriteSingle
- ��������: дһ���ֽ�����
- �������: i2c�Ĵ�����ַ; д����;
- ��������: 
----------------------------------------------------------------------------------------------------*/
extern	void	i2c_master_WriteSingle(uint8_t	suba, uint8_t idata)
{
	i2c_master_StatusCheck(0, 0);

	__write_hw_reg32(I2C_DATA_CMD, suba); 
	__write_hw_reg32(I2C_DATA_CMD, (idata | 0x200));

	
}

/* ---------------------------------------------------------------------------------------------------
- ��������: i2c_master_WriteMulti
- ��������: д����ֽ�����
- �������: i2c�Ĵ�����ַ; д�����ݵĸ���; д���ݴ���׵�ַ; 
- ��������: 
----------------------------------------------------------------------------------------------------*/
extern	void	i2c_master_WriteMulti(uint8_t	suba, uint8_t no, uint8_t *s)
{
	uint8_t i;
  
	i2c_master_StatusCheck(0, 0);

 	__write_hw_reg32(I2C_DATA_CMD, suba); 

	for(i=0; i<(no-1); i++)
	{
	   	i2c_master_StatusCheck(1, 1);
	   	__write_hw_reg32(I2C_DATA_CMD, *s++); 
    }   
	i2c_master_StatusCheck(1, 1);
	__write_hw_reg32(I2C_DATA_CMD, (*s++ | 0x200));  
	 			
}


/* ---------------------------------------------------------------------------------------------------
- ��������: i2c_master_ReadSingle
- ��������: ��һ���ֽ�����
- �������: i2c�Ĵ�����ַ; �������ݴ���׵�ַ; 
- ��������: 
----------------------------------------------------------------------------------------------------*/
extern	void	i2c_master_ReadSingle(uint8_t	suba, uint8_t *s)
{
	uint8_t	val;

	i2c_master_StatusCheck(0, 0);
	
	__write_hw_reg32(I2C_DATA_CMD, suba); 
	__write_hw_reg32(I2C_DATA_CMD, 0x300);

	i2c_master_StatusCheck(3, 1);

	__read_hw_reg32(I2C_DATA_CMD , val);
	*s = (uint8_t)val;
}

/* ---------------------------------------------------------------------------------------------------
- ��������: i2c_master_ReadMulti
- ��������: ������ֽ�����
- �������: i2c�Ĵ�����ַ; �������ݵĸ���(������ڵ���2,С��16) ; �������ݴ���׵�ַ; 
- ��������: 
----------------------------------------------------------------------------------------------------*/
extern	void	i2c_master_ReadMulti(uint8_t	suba, uint8_t no , uint8_t *s)
{
	uint8_t val, i;
  
	i2c_master_StatusCheck(0, 0);

 	__write_hw_reg32(I2C_DATA_CMD, suba); 
	for(i=0; i<(no-1); i++)
	{
	   	i2c_master_StatusCheck(1, 1);
	   	__write_hw_reg32(I2C_DATA_CMD, 0x100); 
    }   
	i2c_master_StatusCheck(1, 1);
	__write_hw_reg32(I2C_DATA_CMD, 0x300);  
	
  	for(i=0; i<no; i++)
	{
		i2c_master_StatusCheck(3, 1);
		__read_hw_reg32(I2C_DATA_CMD , val);
        *s++ = val;
	}						
}
#endif
	
