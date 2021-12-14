#include "board.h"
#include "timer.h"
#include "Arduino.h"

fun fun_timer0 = NULL, fun_timer1 = NULL, fun_timer2 = NULL, fun_timer3 = NULL;

static void timer_enable(uint8_t timer_num, uint8_t enable)
{
    if (enable == TRUE)
        writeReg32(TIMERx_TCR(timer_num), 0x3);//使能定时器timer_num
    else 
        writeReg32(TIMERx_TCR(timer_num), 0);
}

void timer_init(uint8_t timer_num, uint32_t us)
{
    writeReg32(CPR_CTLAPBCLKEN_GRCTL, 0x80008); //TIMER_PCLK 时钟使能
    writeReg32(CPR_TIMER_CLK_CTL(timer_num), 0x0F);//TIMERx_CLK时钟控制寄存器 mclk_in(32MHz)/2*(0x0F + 0x1) = 1MHZ
    writeReg32(TIMERx_TCR(timer_num), 0x0); //不屏蔽定时器中断，不使能定时器timer_num，
    writeReg32(TIMERx_TCR(timer_num), 0x2); //设置定时器工作在用户定义计数模式
    writeReg32(TIMERx_TLC(timer_num), us);  //载入计数器计数初值(32bits),该值应大于等于 0x4
    //timer_enable(timer_num, TRUE);
    //writeReg32(TIMERx_TCR(timer_num), 0x3);
    NVIC_EnableIRQ((IRQn_Type)(TIMER0_IRQn + timer_num));
}

void TIMER0_Handler(void)
{
    uint32_t val = 0;
    static uint8_t overflowing = 0;

    readReg32(TIMER0_TIC, val);
    if (!overflowing) {
        overflowing = 1;
        if (fun_timer0)
        (*fun_timer0)();
        overflowing = 0;
    }
}

void TIMER1_Handler(void)
{
    static uint8_t overflowing = 0;
    uint32_t val = 0;
    readReg32(TIMER1_TIC, val);
    if (!overflowing) {
        overflowing = 1;
        if (fun_timer1)
        (*fun_timer1)();
        overflowing = 0;
    }
}

void TIMER2_Handler(void)
{
    static uint8_t overflowing = 0;
    uint32_t val = 0;
    readReg32(TIMER2_TIC, val);
    if (!overflowing) {
        overflowing = 1;
        if (fun_timer2)
        (*fun_timer2)();
        overflowing = 0;
    }
}

void TIMER3_Handler(void)
{
    uint32_t val = 0;
    static uint8_t overflowing = 0;
    readReg32(TIMER3_TIC, val);
    if (!overflowing) {
        overflowing = 1;
        if (fun_timer3)
        (*fun_timer3)();
        overflowing = 0;
    }
}

void Timer0Set(unsigned long us, void (*f)()) {
    timer_init(0, us);
    if (f)
    fun_timer0 = f;
}
void Timer0Start(void) {
    timer_enable(0, TRUE);
}

void Timer0Stop(void) {
    timer_enable(0, FALSE);
}

void Timer1Set(unsigned long us, void (*f)()) {
    timer_init(1, us);
    if (f)
    fun_timer1 = f;
}

void Timer1Start(void) {
    timer_enable(1, TRUE);
}

void Timer1Stop(void) {
    timer_enable(1, FALSE);
}

void Timer2Set(unsigned long us, void (*f)()) {
    timer_init(2, us);
    if (f)
    fun_timer2 = f;
}

void Timer2Start(void) {
    timer_enable(2, TRUE);
}

void Timer2Stop(void) {
    timer_enable(2, FALSE);
}

void Timer3Set(unsigned long us, void (*f)()) {
    timer_init(3, us);
    if (f)
    fun_timer3 = f;
}

void Timer3Start(void) {
    timer_enable(3, TRUE);
}

void Timer3Stop(void) {
    timer_enable(3, FALSE);
}
