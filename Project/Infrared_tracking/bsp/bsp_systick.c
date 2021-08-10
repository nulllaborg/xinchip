
extern  void ble_tick_handler(void);
volatile  unsigned int GulSystickCount=0;
void	SysTick_Handler(void)
{
        ble_tick_handler();//mainloop超时计数函数
		GulSystickCount++;//计数一次10ms
}
