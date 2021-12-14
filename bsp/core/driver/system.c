/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system
 *
 */
 #include "system.h"
 #include "board.h"
 
volatile unsigned long int ms_ticks;

struct {
    void (*systick_callback)(void);
    unsigned int interval;
}sys_tick;

void register_systick_callback(void (*fun)(void), unsigned int time_interval)
{
    sys_tick.systick_callback = fun;
    sys_tick.interval = time_interval;
}
 
void SysTick_Handler(void)
{
    ms_ticks++;
    if (ms_ticks%sys_tick.interval == 0) {
        if (sys_tick.systick_callback) {
            sys_tick.systick_callback();
        }
    }
}

void systick_init(void)
{
    ms_ticks = 0;
    SysTick_Config(MS_TICKS);  // 1ms interrupte
}

#if 0
void delay_us(unsigned int us)
{
    uint32_t temp;
    SysTick->LOAD = us * US_TICKS;
    SysTick->VAL = 0x00;
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
    do {
        temp = SysTick->CTRL;
    }
    while(temp & 0x01&&!(temp&(1<<16)));
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0x00;
}

void delay_ms(unsigned int ms)
{
    uint32_t temp;
    SysTick->LOAD = ms * MS_TICKS;
    SysTick->VAL = 0x00;
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
    do {
        temp = SysTick->CTRL;
    }
    while( (temp & 0x01) && (!(temp & (1 << 16))) );
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0x00;
}
#else 
uint32_t micros(void)
{
    register uint32_t ms, cycle_cnt;
    do {
        ms = ms_ticks;
        cycle_cnt = SysTick->VAL;
    } while (ms != ms_ticks);
    return (ms * 1000) + (1000 - cycle_cnt/US_TICKS);
}

// Return system uptime in milliseconds (rollover in 49 days)
uint32_t millis(void)
{
    return ms_ticks;
}

void delay_us(unsigned int us)
{
    uint32_t t0 = micros();
    while(micros() - t0 < us);
}

void delay_ms(unsigned int ms)
{
    uint32_t t0 = millis();
    while(millis() - t0 < ms);
}

#endif

void CTL_APBPclkCmd(EM_APB_PCLK_Source source, uint8_t enable)
{
    if (enable) {
         setBitReg32(CPR_CTLAPBCLKEN_GRCTL, 1 << source);
    } else {
         clearBitReg32(CPR_CTLAPBCLKEN_GRCTL, 1 << source);
    }
}

void SystemInit(void)
{
    //
    // SystemInit
    //
    // retarget_init();
    //此寄存器设置0x2d，有助于提升adc采集的稳定性，建议必须采用
    //此寄存器的默认值是0x2c，设为0x2d是为了方便在低功耗和非低功耗中折中同时使用；
    //如果不使用低功耗不需要考虑功耗那么可以直接设置为0x2e;
    *((volatile unsigned *)(CPR_AO_BASE + 0x20)) = 0x2e;
}
