
extern  void ble_tick_handler(void);
volatile  unsigned int GulSystickCount=0;
void	SysTick_Handler(void)
{
        ble_tick_handler();//mainloop��ʱ��������
		GulSystickCount++;//����һ��10ms
}
