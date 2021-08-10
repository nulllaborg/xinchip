#ifndef		__BSP_I2C_SLAVE_H_
#define		__BSP_I2C_SLAVE_H_

#include	"Platform.h"

extern uint8_t Flag_RcvOK;
extern uint8_t IIC_REC_BUFFER[INFRARED_REG_MAX][0x02];
extern	void	i2c_slave_init(uint16_t	i2cAddR);
extern  void    i2c_slave_WriteSingle(uint8_t val);
extern  uint8_t i2c_slave_ReadSingle(void);
typedef	void	(*i2cHandler_callback_t)(uint32_t stat);

#define     I2C_SLAVE_RX_DONE       (1<<7)
#define     I2C_SLAVE_RD_REQ        (1<<5)
#define     I2C_SLAVE_RX_FULL       (1<<2)
#define     I2C_SLAVE_STOP          (1<<9)



#endif
