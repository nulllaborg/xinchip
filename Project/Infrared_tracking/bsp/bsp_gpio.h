#ifndef		__BSP_GPIO_H_
#define		__BSP_GPIO_H_

#include	"xinc_m0.h"

#define    DETECT_INTER_NUM(num,val)                ((val>>num)&0x01)

#define    GPIO_OUTPUT_HIGH(num)                     gpio_output_high(num)
#define    GPIO_OUTPUT_LOW(num)                      gpio_output_low(num)
#define    GPIO_INPUT_VAL(num)                       gpio_input_val(num)

typedef enum{
 GPIO_Dx=0,//普通GPIO口功能
 UART0_TX,
 UART0_RX,
 UART0_CTS,
 UART0_RTS,
 I2C_SCL,
 I2C_SDA,
 UART1_RX,
 UART1_TX,
 SIM_IO,
 SIM_RST,
 SIM_CLK_OUT,
 PWM0,
 PWM1,
 SSI1_CLK,
 SSI1_SSN,
 SSI1_RX,
 SSI1_TX,
 PWM0_INV,  
 PWM1_INV,  

}GPIO_FUN_SEL_TypeDef;
typedef enum{
 NOT_INT=0,
 NA1_INT,
 NA2_INT,
 NA3_INT,
 NA4_INT,
 RIS_EDGE_INT,//上升沿中断
 NA6_INT,
 FAIL_EDGE_INT,//下降沿中断
 NA8_INT,   
 RIS_FAIL_EDGE_INT,//上升沿下降沿中断
    
}GPIO_INT_TypeDef;

typedef enum{
     GPIO_PIN_0=0,
     GPIO_PIN_1, 
     GPIO_PIN_2, 
     GPIO_PIN_3,
     GPIO_PIN_4, 
     GPIO_PIN_5, 
     GPIO_PIN_6, 
     GPIO_PIN_7, 
     GPIO_PIN_8, 
     GPIO_PIN_9, 
     GPIO_PIN_10,
     GPIO_PIN_11,
     GPIO_PIN_12,
     GPIO_PIN_13,
     GPIO_PIN_14,
     GPIO_PIN_15,
     GPIO_PIN_16,
     GPIO_PIN_17,
     GPIO_PIN_18,
     GPIO_PIN_19,
     GPIO_PIN_20,
     GPIO_PIN_21,
     GPIO_PIN_22,
     GPIO_PIN_23,
     GPIO_PIN_24,
     GPIO_PIN_25,
     GPIO_PIN_26,
     GPIO_PIN_27,
     GPIO_PIN_28,
     GPIO_PIN_29,
     GPIO_PIN_30,
     GPIO_PIN_31,
}GPIO_Pin_TypeDef;

typedef enum{
	GPIO_Mode_Input_Up=0, /*上拉输入*/
	GPIO_Mode_Input_Down, /*下拉输入*/
	GPIO_Mode_Input_Float,/*浮空输入*/
}GPIO_InputMode_TypeDef;



/* ---------------------------------------------------------------------------------------------------
- 	IO复用控制 -
----------------------------------------------------------------------------------------------------*/
typedef	union		gpio_fun_config {
        uint32_t    config;
        struct      bit {
            uint8_t     b0004  :5;     
            uint8_t     b0812  :5;
            uint8_t     b1620  :5;
            uint8_t     b2428  :5;
        }bits;
}gpio_fun_config_t;

typedef	struct	gpio_fun_sel_config{
		gpio_fun_config_t	fun_sel0;
		gpio_fun_config_t	fun_sel1;
		gpio_fun_config_t	fun_sel2;
		gpio_fun_config_t	fun_sel3;
		gpio_fun_config_t	fun_sel4;
		gpio_fun_config_t	fun_sel5;
		gpio_fun_config_t	fun_sel6;
		gpio_fun_config_t	fun_sel7;    
}gpio_fun_sel_config_t;


/* ---------------------------------------------------------------------------------------------------
- 	IO功能
----------------------------------------------------------------------------------------------------*/
typedef	union		mux1_config {
        uint32_t    muxctl1;
        struct	bit1	{
            uint8_t		gpio0 : 2;	uint8_t		gpio1	: 2;	uint8_t		gpio2 : 2;	uint8_t		gpio3	: 2;
            uint8_t		gpio4 : 2;	uint8_t		gpio5	: 2;	uint8_t		gpio6 : 2;	uint8_t		gpio7	: 2;
            uint8_t		gpio8 : 2;	uint8_t		gpio9	: 2;	uint8_t		gpio10: 2;	uint8_t		gpio11	: 2;
            uint8_t		gpio12: 2;	uint8_t		gpio13	: 2;	uint8_t		gpio14: 2;	uint8_t		gpio15	: 2;																
        }pad1;
}mux1_config_t;	

typedef	union		mux2_config {
        uint32_t    muxctl2;
        struct	bit2	{
            uint8_t		gpio16 : 2;	uint8_t		gpio17	: 2;	uint8_t		gpio18  : 2;	uint8_t		gpio19	: 2;
            uint8_t		gpio20 : 2;	uint8_t		gpio21	: 2;	uint8_t		gpio22  : 2;	uint8_t		gpio23	: 2;
            uint8_t		gpio24 : 2;	uint8_t		gpio25	: 2;	uint8_t		gpio26  : 2;	uint8_t		gpio27	: 2;
            uint8_t		gpio28 : 2;	uint8_t		bootctl	: 2;	uint16_t	swk : 2;		uint8_t     swd : 2;																				
        }pad2;
}mux2_config_t;	

typedef	struct	gpio_config {
		mux1_config_t		mux1_ctl;
		mux2_config_t		mux2_ctl;
}gpio_config_t;

/* ---------------------------------------------------------------------------------------------------
- 	IO方向
----------------------------------------------------------------------------------------------------*/
typedef	union		dir_config {
        uint32_t    dir;
        struct	bitd	{
					uint8_t		gpio0   : 1;		uint8_t		gpio1  	: 1;		uint8_t		gpio2	: 1;		uint8_t		gpio3  	: 1;
					uint8_t		gpio4	: 1;		uint8_t		gpio5 	: 1;		uint8_t		gpio6	: 1;		uint8_t		gpio7 	: 1;
					uint8_t		gpio8	: 1;		uint8_t		gpio9 	: 1;		uint8_t		gpio10	: 1;		uint8_t		gpio11 	: 1;		
					uint8_t		gpio12: 1;			uint8_t		gpio13 	: 1;		uint8_t		gpio14	: 1;		uint8_t		gpio15  : 1;			
					uint8_t		gpio16: 1;			uint8_t		gpio17 	: 1;		uint8_t		gpio18	: 1;		uint8_t		gpio19  : 1;			
					uint8_t		gpio20: 1;			uint8_t		gpio21 	: 1;		uint8_t		gpio22	: 1;		uint8_t		gpio23  : 1;			
					uint8_t		gpio24: 1;			uint8_t		gpio25  : 1;		uint8_t		gpio26	: 1;		uint8_t		gpio27  : 1;					
					uint8_t		gpio28: 1;			uint8_t		gpio29  : 1;		uint8_t		gpio30	: 1;		uint8_t		gpio31  : 1;						
        }pad;
}dir_config_t;	

/* ---------------------------------------------------------------------------------------------------
- 	IO上下拉
----------------------------------------------------------------------------------------------------*/
typedef	union		pupd1_config {
			uint32_t    pupd;
			struct		bita{
					uint8_t	GPIO0  : 2;			uint8_t	GPIO1 	: 2;	uint8_t	GPIO2 	: 2;	uint8_t	GPIO3 	: 2;
					uint8_t	GPIO4  : 2;			uint8_t	GPIO5 	: 2;	uint8_t	GPIO6 	: 2;	uint8_t	GPIO7 	: 2;
					uint8_t	GPIO8  : 2;			uint8_t	GPIO9 	: 2;	uint8_t	GPIO10 	: 2; 	uint8_t	GPIO11 	: 2;
					uint8_t	GPIO12 : 2; 		uint8_t	GPIO13 	: 2;	uint8_t	GPIO14 	: 2; 	uint8_t	GPIO15 	: 2;	
			}pad;
}pupd1_config_t;

typedef	union		pupd2_config {
			uint32_t    pupd;
			struct		bitb{
					uint8_t	GPIO16  : 2;			uint8_t	GPIO17 	: 2;	uint8_t	GPIO18 	: 2;	uint8_t	GPIO19 	: 2;
					uint8_t	GPIO20  : 2;			uint8_t	GPIO21 	: 2;	uint8_t	GPIO22 	: 2;	uint8_t	GPIO23 	: 2;
					uint8_t	GPIO24  : 2;			uint8_t	GPIO25 	: 2;	uint8_t	GPIO26 	: 2; 	uint8_t	GPIO27 	: 2;
					uint8_t	GPIO28  : 2; 		    uint8_t	BOOTCTL : 2;	uint8_t	SWCK 	: 2; 	uint8_t	SWD 	: 2;	
			}pad;
}pupd2_config_t;

typedef     struct  {
    pupd1_config_t  CTL_PECTL1;
    pupd2_config_t  CTL_PECTL2;
}pupd_config_t;

/* ---------------------------------------------------------------------------------------------------
- 	IO中断模式
----------------------------------------------------------------------------------------------------*/
typedef	union		int_config {
        uint32_t    interrupt;
				struct	bitt	{
					uint8_t		mode0  :	4;			uint8_t		mode1	: 4;	
					uint8_t		mode2  : 	4;			uint8_t		mode3	: 4;	
					uint16_t  reserved;						
				}pad;
}int_config_t;

typedef	struct	int_config_i {
		int_config_t		intr_ctl0;
		int_config_t		intr_ctl1;
		int_config_t		intr_ctl2;
		int_config_t		intr_ctl3;
		int_config_t		intr_ctl4;
		int_config_t		intr_ctl5;
		int_config_t		intr_ctl6;
		int_config_t		intr_ctl7;	
}interrupt_config_t;

typedef void	    (*IoHandler_callback)(uint32_t val);
extern	void	    Init_gpio(void);
extern	void	    gpio_Register_Callback(IoHandler_callback callback);
extern	void        gpio_direction_input(uint8_t num, uint8_t pull_up_type);
extern	uint8_t     gpio_input_val(uint8_t num);
extern	void	    gpio_output_low(uint8_t	num);
extern	void	    gpio_output_high(uint8_t num);
extern  void        gpio_mux_ctl(uint8_t num,uint8_t mux);
extern  void        gpio_fun_inter(uint8_t num,uint8_t inter);
extern  void        gpio_fun_sel(uint8_t num,uint8_t sel);
extern	void	    gpio_direction_output(uint8_t num);

#endif

