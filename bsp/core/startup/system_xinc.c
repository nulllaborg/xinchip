/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system
 *
 */
 
extern	void 	retarget_init(void);
 
void SystemInit (void)
{
	//
	// SystemInit
	//
	retarget_init();
	//此寄存器设置0x2d，有助于提升adc采集的稳定性，建议必须采用
	//此寄存器的默认值是0x2c，设为0x2d是为了方便在低功耗和非低功耗中折中同时使用；
	//如果不使用低功耗不需要考虑功耗那么可以直接设置为0x2e;
	*((volatile unsigned *)(0x40002400 + 0x20)) = 0x2d;
}

