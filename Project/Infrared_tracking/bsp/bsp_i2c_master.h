#ifndef		__BSP_I2C_MASTER_H_
#define		__BSP_I2C_MASTER_H_

#define     I2C_STANDARD_MODE     (1)           //- 100K bit/s
#define     I2C_FAST_MODE         (2)           //- 400K bit/s

extern	void	i2c_master_init(uint8_t	i2cAddR, uint32_t speed);
extern	void	i2c_master_WriteSingle(uint8_t	suba, uint8_t idata);
extern	void	i2c_master_WriteMulti(uint8_t	suba, uint8_t no, uint8_t *s);
extern	void	i2c_master_ReadSingle(uint8_t	suba, uint8_t *s);
extern	void	i2c_master_ReadMulti(uint8_t	suba, uint8_t no , uint8_t *s);


#endif
