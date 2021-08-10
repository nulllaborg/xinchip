#ifndef		__BSP_UART_H_
#define		__BSP_UART_H_

#define		BAUD_1000000			0x00100011

#define		BAUD_921600				0x24002711
#define		BAUD_460800				0x12002711
#define		BAUD_256000				0x060007D3


#define		BAUD_230400				0x09002711
#define		BAUD_128000				0x030007D3
#define		BAUD_115200				0x04802711
#define		BAUD_57600				0x04802712
#define		BAUD_38400				0x04802713
#define		BAUD_23040				0x04802715
#define		BAUD_19200				0x04802716
#define		BAUD_14400				0x04802718
#define		BAUD_12800				0x04802719
#define		BAUD_9600				0x0480271C
#define		BAUD_4800				0x0240271C
#define		BAUD_2400				0x0120271C
typedef		uint32_t	(*uHandler_callback)(uint8_t ch);
extern	void	Init_uart(uint32_t	ch , uint32_t	baud);
extern	void	Uart_Register_Callback(uint32_t	no, uHandler_callback callback);
extern	void	Uart_Send_Char(uint32_t	ch, uint8_t	c);
extern	void	Uart_Send_String(uint32_t	ch, uint8_t	*s);
extern	void	Uart_Send_Buf(uint32_t	ch, uint8_t	*s,uint32_t length);
extern	void	Uart_DMA_Send(uint8_t   ch,uint8_t	*s,uint32_t length);

#define QUEUE_SIZE  (512)
typedef struct
{
    uint8_t  data[QUEUE_SIZE];
    uint16_t head;
    uint16_t tail;
} QUEUE_Init_TypeDef;


#endif
