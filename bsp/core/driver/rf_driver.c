#include "stdint.h"
#include "string.h"
#include "system.h"

#define RF_BASE 0X4002F000

//#define QFN32
#define SSOP16
extern  void SER_WRITE(unsigned int regAddr,unsigned int regValue);
extern  unsigned int SER_READ(unsigned int regAddr,unsigned int *regValue);     
void ble_rccali(void);

typedef struct
{
    int chan;
    char* div_integer;
    char* div_fractional;    
}div_t;
// set bit
#define setbit(x,y) ((x) |= (1<<(y)))
#define clrbit(x,y) ((x) &= ~(1<<(y)))

#define SPI_CTL setbit(*(uint32_t volatile*)(0x40000000+0x130),0)
#define AHB_CTL clrbit(*(uint32_t volatile*)(0x40000000+0x130),0)

void wbit(uint16_t reg_addr,int start,int end,char* bit_str);
void set_ch(int ch);
                           
                            
uint16_t fpga_spi_read(uint32_t reg_addr)
{
    uint32_t spi_addr = reg_addr/4;   // read: MSB is 1
    uint32_t value; 
    //在这里添加spi读接口
    SER_READ(spi_addr, &value);
    
    return value;
}

int fpga_spi_write(uint32_t reg_addr, uint32_t reg_val)
{
    uint32_t spi_addr = reg_addr/4;             //write:MSB  is 0
    //在这个添加spi写接口  
    SER_WRITE(spi_addr, reg_val);
    
    return 0;
}
#define reversebit(x,y) (x)^=(1<<(y))
void Dcoc_Calib(uint16_t i,uint16_t q)
{
    uint16_t   dac_i;
    uint16_t   dac_q;
    uint16_t   calib_val;


    //求原码,高位取反
    reversebit(i,9);
    reversebit(q,9);


    float I = abs(i - 512);
    float Q = abs(q - 512);

    float adc_i = I*733/1024;
    float adc_q = Q*733/1024;
    dac_i = (uint16_t)(adc_i/13.6+0.5);
    dac_q = (uint16_t)(adc_q/13.6+0.5);

    if(i > 512)
    {
        dac_i = dac_i | 0x80;  //最高位符号1
    }

    if(q < 512)
    {
        dac_q = dac_q<<8 | 0x8000;   //最高位符号1,Q高16位
    }

    calib_val = dac_i | dac_q;

    fpga_spi_write(0x0098,calib_val);
}

void le_rf_init(void)
{
    //b400lite initialization,fvco=2437M,bbpll en
    wbit(0x0030,15,8,"11111111");//debug sx 26m en and sx trx ldo en
    wbit(0x0010,15,15,"1");// debug_LDO_DEBUG, ADDA_BBVCO cap-ldo enable
    wbit(0x0034,15,8,"11111111");//reg	sx 26m en and sx trx ldo en
    wbit(0x0014,15,15,"1");// reg_LDO_DEBUG, ADDA_BBVCO cap-ldo enable
    //wbit(0x0060,15,9,"1111111");//debug sx trx ldo fc
    //wbit(0x0064,15,9,"1111111");//reg sx trx ldo fc
    wbit(0x0018,15,8,"11011010");//adda bbpll ldo vout default 11101110
    wbit(0x016C,15,8,"00001000");//pllvcohllo ldo vout,default 01011001
    //wbit(0x0174,13,8,"100100");//sx fb dsm vddres,default 000000
    //wbit(0x017C,11,6,"100000");//sx pfd cp vddres,default 000000
    wbit(0x0254,12,2,"01101000110");//lobq2lnabias lnaldovout,def01101101101
    //wbit(0x0254,3,2,"10");//lna ldovout,default 01
    wbit(0x0244,15,12,"1010");//rx and tx ldovout,default 0101
    //wbit(0x0254,12,10,"010");//rx lo bias,default 011
    wbit(0x024C,15,2,"01011101010100");//bqvcmiobpfabbcc,def101010
    //wbit(0x024C,15,10,"010111");//rx bq vcmio and cz,default 101010
    wbit(0x02F4,15,6,"0100011010");//tiabq1bias tiavcmoi def0110111010
    //wbit(0x02F4,9,6,"1010");//rxtia vcmoi default1010
    wbit(0x0008,7,2,"010001");//ismsx and bbpll clkbuf,default 100100
    //wbit(0x024C,6,2,"10110");//abbcc default=01000,10001 to 11000
    wbit(0x040C,15,5,"10001110000");//sx
    //wbit(0x040C,15,15,"1");// SET 0x040C[15]=1，0x040C[9]=1); manual mode
    //wbit(0x040C,14,10,"00000");//manual mode, state control
    //wbit(0x040C,9,9,"1");// wifi/bt select maual control enable
    //wbit(0x040C,8,8,"0");// manual control wifi/bt selection, manual mode,1WF0BT
    //wbit(0x040C,7,7,"0");// wifi/bt selection polar for ISM DCOC function
    //wbit(0x040C,6,6,"0");// wifi/bt selection polar for ISM GAIN function
    //wbit(0x040C,5,5,"0");// wifi/bt selection polar for wifi/bt DAC data function
    //wbit(0x040C,14,10,"00011");// set state to WF_SX_CAL
    wbit(0x0068,15,13,"011");// debug_afcen,ISM_SX_AFC dsm_RESETN
    //wbit(0x0068,14,14,"1");// debug_DA_ISM_SX_AFC_RESETN
    wbit(0x006C,15,13,"011");// reg_afcen,ISM_SX_AFC dsm_RESETN
    //wbit(0x006C,14,14,"1");// reg_DA_ISM_SX_AFC_RESETN
    //wbit(0x0068,13,13,"1");// debug_DA_ISM_SX_dsm_RESETN
    //wbit(0x006C,13,13,"1");// reg_DA_ISM_SX_dsm_RESETN
    //wbit(0x01A4,6,5,"11");// BT_RG_ISM_SX_AFC_DELAY_CHARGING,default10
    wbit(0x01A4,15,5,"00000010011");// BT_RG_ISM_SX_AFC_DELAY_CHARGING,default10
    //	wbit(0x01A4,10,5,"010011");// BT_RG_ISM_SX_AFC_DELAY_CHARGING,default10
    wbit(0x01B4,15,15,"0");// BT_RG_ISM_SX_ATC_EN
    wbit(0x0194,7,5,"111");// kvco def010
    wbit(0x018C,13,11,"000");// vcobiasctat def001
    //wbit(0x01A4,15,11,"00000");// icp
    wbit(0x017C,5,1,"00101");// icpoff
    wbit(0x0184,15,3,"0101000111011");// sxlpf r2c1c2c3
    //wbit(0x015C,15,7,"100110000");// BT_RG_ISM_CAL_SX_DIVN,2437M
    //wbit(0x015C,6,3,"1010");//BT_RG_ISM_CAL_SX_DIVFRACH
    //wbit(0x0164,15,0,"0000000000000000");// BT_RG_ISM_CAL_SX_DIVFRACL
    wbit(0x0304,11,10,"00"); //BT_RG_WB_SX_VCO_LDO1P5_IBC default 10,min00
    wbit(0x01AC,11,3,"111000101");// pkdet dealy aacautoen pbrefbias,def001
    //wbit(0x01AC,11,10,"11");// pkdet dealy,11max,default00
    //wbit(0x01AC,8,6,"000");// pkdet pdbias
    //wbit(0x01AC,5,3,"011");// pkdet refbias
    //wbit(0x015C,15,7,"100101100"); //2402
    //wbit(0x015C,6,3,"0100");	  
    //wbit(0x0068,15,15,"1");// debug_DA_ISM_SX_AFC_STARTL2H
    //wbit(0x006C,15,15,"0");// reg_DA_ISM_SX_AFC_STARTL2H
    //wbit(0x006C,15,15,"1");// reg_DA_ISM_SX_AFC_STARTL2H,from 0to1 start AFC
    wbit(0x001C,15,8,"11101011");// debug BBPLL
    //wbit(0x001C,15,15,"1");// debug BBPLL reset ,low reset
    //wbit(0x001C,14,14,"1");// Enable for BBPLL1,high enable,also control ref clock
    //wbit(0x001C,13,13,"1");// debug_DA_BBPLL1_EN
    //wbit(0x001C,11,11,"1");// Enable for 64M clk output to digital module
    //wbit(0x001C,9,9,"1");// Enable for 24M clk output to digital module
    //wbit(0x001C,8,8,"1");// Enable for 104M clk output to digital module
    //wbit(0x0020,15,8,"11101011");// reg BBPLL
    //wbit(0x0020,15,15,"1");// reg  BBPLL reset ,low reset
    //wbit(0x0020,14,14,"1");// Enable for BBPLL1,high enable,also control ref clock
    //wbit(0x0020,13,13,"1");// debug_DA_BBPLL1_EN
    //wbit(0x0020,11,11,"1");// Enable for 64M clk output to digital module
    //wbit(0x0020,9,9,"1");// Enable for 24M clk output to digital module
    //wbit(0x0020,8,8,"1");// Enable for 104M clk output to digital module
    wbit(0x0024,9,4,"000110"); //default 001100, div12,32m 000110,div6
    wbit(0x002C,15,13,"000"); //default 001,bbpll vddres

    //wbit(0x03E4,7,7,"0");// En bbpll test
    wbit(0x03D4,13,3,"01101011111");// reserved0
    //wbit(0x03D4,13,11,"011");// vdd64m res
    //wbit(0x03D4,10,9,"01");// bbpll dacclk buf driver
    //wbit(0x07D4,15,15,"1");// debug_DA_ISM_RX_GLNA
    //wbit(0x07D4,14,14,"1");//debug_DA_ISM_RX_ABB_GC_BQ
//LILIN
#if  1
    wbit(0x06D0,15,13,"100");// lna gain map0
    wbit(0x06D4,15,13,"100");// lna gain map1
    wbit(0x06D8,15,13,"100");// lna gain map2
    wbit(0x06DC,15,13,"100");// lna gain map3
    wbit(0x06E0,15,13,"100");// lna gain map4
    wbit(0x06E4,15,13,"101");// lna gain map5
    wbit(0x06E8,15,13,"110");// lna gain map6
    wbit(0x06EC,15,13,"111");// lna gain map7
    wbit(0x06F0,15,10,"001100");// rxabb gain map0
    wbit(0x06F4,15,10,"001100");// rxabb gain map1
    wbit(0x06F8,15,10,"001100");// rxabb gain map2
    wbit(0x06FC,15,10,"001100");// rxabb gain map3
    wbit(0x0700,15,10,"001100");// rxabb gain map4
    wbit(0x0704,15,10,"001100");// rxabb gain map5
    wbit(0x0708,15,10,"001100");// rxabb gain map6
    wbit(0x070C,15,10,"010000");// rxabb gain map7
    wbit(0x0710,15,10,"010100");// rxabb gain map8
    wbit(0x0714,15,10,"011000");// rxabb gain map9
    wbit(0x0718,15,10,"011100");// rxabb gain map10
    wbit(0x071C,15,10,"100000");// rxabb gain map11
    wbit(0x0720,15,10,"100100");// rxabb gain map12
    wbit(0x0724,15,10,"101000");// rxabb gain map13
    wbit(0x0728,15,10,"101100");// rxabb gain map14
    wbit(0x072C,15,10,"110000");// rxabb gain map15 

//  wbit(0x06D0,15,13,"100");// lna gain map0
//  wbit(0x06D4,15,13,"100");// lna gain map1
//  wbit(0x06D8,15,13,"100");// lna gain map2
//  wbit(0x06DC,15,13,"100");// lna gain map3
//  wbit(0x06E0,15,13,"100");// lna gain map4
//  wbit(0x06E4,15,13,"101");// lna gain map5
//  wbit(0x06E8,15,13,"110");// lna gain map6
//  wbit(0x06EC,15,13,"111");// lna gain map7
//  wbit(0x06F0,15,10,"000100");// rxabb gain map0
//  wbit(0x06F4,15,10,"000100");// rxabb gain map1
//  wbit(0x06F8,15,10,"000100");// rxabb gain map2
//  wbit(0x06FC,15,10,"000100");// rxabb gain map3
//  wbit(0x0700,15,10,"000100");// rxabb gain map4
//  wbit(0x0704,15,10,"001000");// rxabb gain map5
//  wbit(0x0708,15,10,"001100");// rxabb gain map6
//  wbit(0x070C,15,10,"010000");// rxabb gain map7
//  wbit(0x0710,15,10,"010100");// rxabb gain map8
//  wbit(0x0714,15,10,"011000");// rxabb gain map9
//  wbit(0x0718,15,10,"011100");// rxabb gain map10
//  wbit(0x071C,15,10,"100000");// rxabb gain map11
//  wbit(0x0720,15,10,"100100");// rxabb gain map12
//  wbit(0x0724,15,10,"101000");// rxabb gain map13
//  wbit(0x0728,15,10,"101100");// rxabb gain map14
//  wbit(0x072C,15,10,"110000");// rxabb gain map15 

#endif
///
    wbit(0x07D0,15,13,"111");// debug_DA_ISM_TX_GC
    //wbit(0x07D0,15,15,"1");// debug_DA_ISM_TXDAC_GC
    //wbit(0x07D0,14,14,"1");// debug_DA_ISM_TXGM_GC
    //wbit(0x07D0,13,13,"1");// debug_DA_ISM_TXMOD_GC
    //wbit(0x0038,14,10,"11111");// debug En rx
    //wbit(0x0038,9,7,"111");// debug En tx

    wbit(0x005C,15,13,"111");// lna gc
    wbit(0x005C,12,9,"1100");// rxabb gc
#ifdef QFN32
    wbit(0x0054,12,7,"010100");// txgm gc qfn32
#elif defined SSOP16
    wbit(0x0054,12,7,"010100");// txgm gc ssop16
#endif
    //wbit(0x0054,15,4,"100010000000");// txdac3gm6 gc pacap lowbit3
    wbit(0x0214,15,15,"0");// txpacap highbit,default0 
    wbit(0x0054,6,4,"111");// txpacap lowbit,default111 

    wbit(0x0294,15,15,"1");// debug ISM ADC enable signal, high enable, Default 0
    //wbit(0x0294,14,14,"1");// Enable for WF/BT ADC clk
    wbit(0x0298,15,15,"1");// reg ISM ADC enable signal, high enable, Default 0
    wbit(0x060C,14,14,"1");// ad adc clk inv,1 inv,default 0
    //wbit(0x0298,14,14,"1");// Enable for WF/BT ADC clk
    //wbit(0x02B4,11,11,"0");// adc sel ckih,0 work,1 pd
    //wbit(0x03D4,8,7,"01");//adcbuf sel,reserved0<8:7>
    //wbit(0x03D4,6,5,"11");//rccalbuf sel,reserved0<6:5>
    //wbit(0x03D4,4,3,"00");//tia highres,reserved0<4:3>
    //wbit(0x0254,6,4,"001");//lna bias,default 011
    wbit(0x03DC,15,3,"0011100100100");//lna,reserved2 
    //wbit(0x03DC,15,12,"1100");//lna input match cap
    //wbit(0x02F4,15,13,"010");// rxtia bias default011,min001
    //wbit(0x02F4,12,10,"001");// rxabb bq1 bias default011,min001
    //wbit(0x0254,9,7,"010");// rxabb bq2 bias default011,min001
    //wbit(0x024C,9,9,"0");// default=0, 1 2mif,0 1mif
    //wbit(0x024C,8,8,"1");// default=1, 1 1mif,0 -1mif
    //wbit(0x024C,7,7,"0");// default=1, 1 lpf,0 complex filter
    //wbit(0x03DC,11,9,"011");//lna pcas
    //wbit(0x03DC,8,6,"011");//lna ncas
    //wbit(0x03DC,5,3,"011");//lna cmcode
    //wbit(0x025C,15,15,"1");// debug Powerdown DAC, 1,enable
    //wbit(0x025C,14,14,"1");// TX bias enable ,including DAC bias,0:pd 1en
    //wbit(0x025C,13,13,"1");// Enable for WF/BT DAC clk
    wbit(0x025C,11,11,"1");// A signal resets DAC digital part,0:(def) reset, 1

    //wbit(0x0260,15,15,"1");// reg Powerdown DAC, 1,enable
    //wbit(0x0260,14,14,"1");// TX bias enable ,including DAC bias,0:pd 1en
    //wbit(0x0260,13,13,"1");// Enable for WF/BT DAC clk
    wbit(0x0260,11,11,"1");// A signal resets DAC digital part,0:(def) reset, 1

    wbit(0x0284,14,14,"1");//dac clk inv,default 0,1 clk inv
    //wbit(0x03D8,15,15,"1");//bbpll ldo en,reserved1<15>
    //soc mode add it  
    //wbit(0x03D8,13,13,"1");//adcdac clk div2 en,default 16M
    wbit(0x03D8,15,0,"0010000011100000");//reserved1

    wbit(0x0054,15,13,"011");// txdac gc 
    wbit(0x020C,15,12,"0010");// txgm rc1 default 0110,111 maxres,000 minres,b12 nouse
    wbit(0x021C,15,10,"011000");// txgm gmop ibc
    //wbit(0x021C,15,13,"000");// txgm ibc,wf 100 40u,bt 011 35u
    //wbit(0x021C,12,10,"000");// txgmop ibc,wf 100 20u,bt 000 10u
    wbit(0x0224,15,4,"100111111111");// txgm mc4 cc1 2 rc2 3 cc2 3
    //wbit(0x0224,15,12,"1001");// txgm mc,1111 max
    //wbit(0x0224,9,7,"111");// tx gm rc2,16m 111,64m 011
    //wbit(0x0224,6,4,"111");// tx gm cc2,16m 111,64m 011
    //wbit(0x022C,10,5,"111000");// txlocm txcascm def010 010
    //wbit(0x07D8,14,13,"11");//debug dcoc en
    //en 48M
    //wbit(0x0020,15,8,"11101011");// reg BBPLL
    //wbit(0x03D8,15,15,"1");//bbpll ldo en,reserved1<15>
    //pd 48M
    //wbit(0x0020,15,8,"00000000");// reg BBPLL,default 000000000
    //wbit(0x03D8,15,15,"0");//bbpll ldo en,reserved1<15>,default 0

    ble_rccali(); // rc calibration	

    // wbit(0x0098,15,8,"00011100");//i dcoc dac   新板子 
    //wbit(0x0098,7,0, "10011101");//q dcoc dac
    //Dcoc_Calib_Dcoc_I();
    //Dcoc_Calib_Dcoc_Q();
#if  0
       wbit(0x03E4,6,3,"1111");// En testipinqpqn test
       wbit(0x03E4,15,15,"0");// En ismadc test
       wbit(0x03E4,14,14,"1");// En rxabb test
       wbit(0x03E4,13,13,"0");// En rssiadc test
       wbit(0x03E4,12,12,"0");// En rxtia test 
       wbit(0x03E4,11,11,"0");// En dac test
       wbit(0x03E4,10,10,"0");// En txabb test
#endif

#if 0
    //dcoc start
    wbit(0x03D8,2,0,"111");//rxtxsx auto en,reserved1
    wbit(0x0030,7,0,"11111111");//debug sx 26m en and sx trx ldo en
    wbit(0x0034,7,0,"11111111");//reg	sx 26m en and sx trx ldo en	
    wbit(0x015C,15,7,"010010101"); //2395
    wbit(0x015C,6,3,"1011");
    wbit(0x0038,14,11,"1111");// debug En rx
    wbit(0x003C,14,11,"1111");//reg En rx
    wbit(0x07D4,15,14,"11");// debug_DA_ISM_RX_GLNA_abb
    wbit(0x005C,15,13,"111");// lna gc
    wbit(0x005C,12,9,"1100");// rxabb gc
    //Calib_Delay();
    //ble_rccali(); // rc calibration	
    //Dcoc_Calib_Dcoc_I();
    //Dcoc_Calib_Dcoc_Q();
    //fpga_spi_write(0x0098,0xa9c);
    // wbit(0x0098,15,8,"00011100");//i dcoc dac   新板子 
    //wbit(0x0098,7,0, "10011101");//q dcoc dac
    Dcoc_Simple_Calib();

    wbit(0x03D8,2,0,"000");//rxtxsx auto en,reserved1
    wbit(0x0030,7,0,"00000000");//debug sx 26m en and sx trx ldo en
    wbit(0x0034,7,0,"00000000");//reg	sx 26m en and sx trx ldo en
    wbit(0x0038,14,11,"0000");// debug En rx
    wbit(0x003C,14,11,"0000");//reg En rx
    wbit(0x07D4,15,14,"00");// debug_DA_ISM_RX_GLNA_abb
#endif
}

// The format of the string is binary
void le_set_rf_tx_power(char* bit_str)
{
    wbit(0x0054,12,7,bit_str);
}

void wbit(uint16_t reg_addr,int end,int start,char* bit_str)
{
    uint16_t reg_val = 0;
    char ch = 0;
    int i = 0;

    if(strlen(bit_str) != (end-start+1))
    {
        while(1); //error
    }
    #ifdef APB_REG
    reg_val = _reg_read16(RF_BASE+reg_addr);
    #else
    reg_val = fpga_spi_read(reg_addr);
    #endif

    for(i=start;i<=end;i++)
    {
        int bit_idx = i-start;  //index of bit string
        ch = bit_str[(end-start)-bit_idx] - '0';
        if(ch==1)
        {
            setbit(reg_val,i);
        }
        else if(ch==0)
        {
            clrbit(reg_val,i);
        }
    }

    #ifdef APB_REG
    _reg_write16(RF_BASE+reg_addr,reg_val);
    #else
    fpga_spi_write(reg_addr,reg_val);
    #endif
}

void w_regbit(uint32_t reg_addr,int end,int start,char* bit_str)
{
    uint16_t reg_val = 0;
    char ch = 0;
    int i = 0;
    
    if(strlen(bit_str) != (end-start+1))
    {
        while(1); //error
    }
    
    reg_val = readReg16(RF_BASE+reg_addr);
    
    for(i=start;i<=end;i++)
    {
        
        int bit_idx = i-start;	//index of bit string
        
        ch = bit_str[(end-start)-bit_idx] - '0';
        if(ch==1)
        {
            setbit(reg_val,i);
        }
        else if(ch==0)
        {
            clrbit(reg_val,i);
        }
    }
    

    writeReg16(RF_BASE+reg_addr,reg_val);
}
//void xc_rf_ldo_on(void)
//{
//    AHB_CTL;
//    w_regbit(0x0034,15,8,"11111111");       
//    SPI_CTL;
//}
//void xc_rf_ldo_off(void)
//{
//    AHB_CTL;
//    w_regbit(0x0034,15,8,"00000000");
//    SPI_CTL;
//}
void ldo_on_rx(void)
{
    AHB_CTL;
    //ldoandsx en
    w_regbit(0x0034,15,0,"1111111111111111");//reg	sx 26m en and sx trx ldo en
    w_regbit(0x0064,15,9,"1111111");//reg sx trx ldo fc
    //en trx
    //	wbit(0x003C,14,10,"11110");// reg En rx
    //	wbit(0x003C,9,7,"111");// reg En tx
    w_regbit(0x003C,14,7,"11110000");// reg En rx

    //en adc
    //	wbit(0x0298,15,15,"1");// reg ISM ADC enable signal, high enable, Default 0
    //	wbit(0x0298,14,14,"1");// Enable for WF/BT ADC clk
    w_regbit(0x0298,15,14,"11");// reg ISM ADC enable signal, high enable, Default 0

    //en dac
    //	wbit(0x0260,15,15,"1");// reg Powerdown DAC, 1,enable
    //	wbit(0x0260,14,14,"1");// TX bias enable ,including DAC bias,0:pd 1en
    //	wbit(0x0260,13,13,"1");// Enable for WF/BT DAC clk
    w_regbit(0x0260,15,13,"111");
    w_regbit(0x0260,11,11,"1");// A signal resets DAC digital part,0:(def) reset, 1
    SPI_CTL;
}

void ldo_on_tx(void)
{
    AHB_CTL;
    //ldoandsx en
    w_regbit(0x0034,15,0,"1111111111111111");//reg	sx 26m en and sx trx ldo en
    w_regbit(0x0064,15,9,"1111111");//reg sx trx ldo fc
    //en trx
    //  wbit(0x003C,14,10,"11110");// reg En rx
    //  wbit(0x003C,9,7,"111");// reg En tx
    w_regbit(0x003C,14,7,"00000111");// reg En rx

    //en adc
    //  wbit(0x0298,15,15,"1");// reg ISM ADC enable signal, high enable, Default 0
    //  wbit(0x0298,14,14,"1");// Enable for WF/BT ADC clk
    w_regbit(0x0298,15,14,"11");// reg ISM ADC enable signal, high enable, Default 0

    //en dac
    //  wbit(0x0260,15,15,"1");// reg Powerdown DAC, 1,enable
    //  wbit(0x0260,14,14,"1");// TX bias enable ,including DAC bias,0:pd 1en
    //  wbit(0x0260,13,13,"1");// Enable for WF/BT DAC clk
    w_regbit(0x0260,15,13,"111");
    w_regbit(0x0260,11,11,"1");// A signal resets DAC digital part,0:(def) reset, 1
    SPI_CTL;
}

void ldo_on(void)
{
    AHB_CTL;
    //ldoandsx en
    w_regbit(0x0034,15,0,"1111111111111111");//reg  sx 26m en and sx trx ldo en
    w_regbit(0x0064,15,9,"1111111");//reg sx trx ldo fc
    //en trx
    //  wbit(0x003C,14,10,"11110");// reg En rx
    //  wbit(0x003C,9,7,"111");// reg En tx
    w_regbit(0x003C,14,7,"11110111");// reg En rx

    //en adc
    //  wbit(0x0298,15,15,"1");// reg ISM ADC enable signal, high enable, Default 0
    //  wbit(0x0298,14,14,"1");// Enable for WF/BT ADC clk
    w_regbit(0x0298,15,14,"11");// reg ISM ADC enable signal, high enable, Default 0

    //en dac
    //  wbit(0x0260,15,15,"1");// reg Powerdown DAC, 1,enable
    //  wbit(0x0260,14,14,"1");// TX bias enable ,including DAC bias,0:pd 1en
    //  wbit(0x0260,13,13,"1");// Enable for WF/BT DAC clk
    w_regbit(0x0260,15,13,"111");
    w_regbit(0x0260,11,11,"1");// A signal resets DAC digital part,0:(def) reset, 1
    SPI_CTL;
}

void ldo_on_debug(void)
{
    AHB_CTL;
    w_regbit(0x0034,15,8,"11111111");//reg  sx 26m en and sx trx ldo en wbit(0x0014,15,15,"1");// reg_LDO_DEBUG, ADDA_BBVCO cap-ldo enable
    w_regbit(0x0014,15,15,"1");// reg_LDO_DEBUG, ADDA_BBVCO cap-ldo enable
    //w_regbit(0x0298,15,15,"1");// reg ISM ADC enable signal, high enable, Default 0
    SPI_CTL;
}
void ldo_off_debug(void)
{
    AHB_CTL;
    w_regbit(0x0034,15,8,"00000000");//reg  sx 26m en and sx trx ldo en 
    w_regbit(0x0014,15,15,"0");// reg_LDO_DEBUG, ADDA_BBVCO cap-ldo enable
    //w_regbit(0x0298,15,15,"0");// reg ISM ADC enable signal, high enable, Default 0
    SPI_CTL;			
}


void ldo_off(void)
{
    AHB_CTL;

    //ldoandsx off
    w_regbit(0x0034,15,0,"0000000000000000");//reg	sx 26m en and sx trx ldo en
    w_regbit(0x0064,15,9,"0000000");//reg sx trx ldo fc
    //en trx
    //  wbit(0x003C,14,10,"11110");// reg En rx
    //  wbit(0x003C,9,7,"111");// reg En tx
    w_regbit(0x003C,14,7,"00000000");// reg En rx

    //en adc
    //  wbit(0x0298,15,15,"1");// reg ISM ADC enable signal, high enable, Default 0
    //  wbit(0x0298,14,14,"1");// Enable for WF/BT ADC clk
    w_regbit(0x0298,15,14,"00");// reg ISM ADC enable signal, high enable, Default 0

    //en dac
    //  wbit(0x0260,15,15,"1");// reg Powerdown DAC, 1,enable
    //  wbit(0x0260,14,14,"1");// TX bias enable ,including DAC bias,0:pd 1en
    //  wbit(0x0260,13,13,"1");// Enable for WF/BT DAC clk
    w_regbit(0x0260,15,13,"000");
    w_regbit(0x0260,11,11,"0");// A signal resets DAC digital part,0:(def) reset, 1
    SPI_CTL;
}

uint16_t rbit(uint16_t reg_addr)
{
    uint16_t reg_val = 0;
#ifdef APB_REG
    reg_val = _reg_read16(RF_BASE+reg_addr);
#else
    reg_val = fpga_spi_read(reg_addr);
#endif
    return reg_val;
}

void ble_rccali(void)
{
    uint16_t val=0;
    
    //wbit(0x0840,10,8,"101");//RG_RCCAL_CTRL(Rccal 输出码值控制) "101"
    wbit(0x0840,10,8,"111");//RG_RCCAL_CTRL(Rccal 输出码值控制) "101"
    
    wbit(0x0840,5,5,"1");//RG_RCCAL_RESETN=1
    wbit(0x0840,3,3,"0");//RG_RCCAL_SEL =0
    wbit(0x0840,2,2,"1");//RG_RCCAL_EN=1
    wbit(0x0840,4,4,"1");//RG_RCCAL_START=1
    
    while(!(rbit(0x0848)&0x8000));//等待AD_RCCAL_FINISH拉高 得出校准值 AD_RCCAL_CTRIM
    val=rbit(0x0848);
    val=(val&0x7FFF)>>10;// 将校准值AD_RCCAL_CTRIM保存
    
    wbit(0x0840,3,3,"1");//RG_RCCAL_SEL =1
    wbit(0x0840,2,2,"0");//RG_RCCAL_EN=0
    wbit(0x0840,4,4,"0");//RG_RCCAL_START=0

    //将保存的校准值转换成字符串
    char temp_buf[5]="";
    temp_buf[4]=((val&0x01) ? '1':'0');
    temp_buf[3]=((val&0x02) ? '1':'0');
    temp_buf[2]=((val&0x04) ? '1':'0');
    temp_buf[1]=((val&0x08) ? '1':'0');
    temp_buf[0]=((val&0x10) ? '1':'0');
    
    wbit(0x0840,15,11,temp_buf);
}
