#ifndef _BSP_TIMER_H_
#define _BSP_TIMER_H_

typedef void (*fun)(void);

static void timer_init(uint8_t timer_num,  uint32_t us);
void init_timer(void);
void Timer0Set(unsigned long us, void (*f)());
void Timer0Start(void));
void Timer0Stop(void));

void Timer1Set(unsigned long us, void (*f)());
void Timer1Start(void));
void Timer1Stop(void));

void Timer2Set(unsigned long us, void (*f)());
void Timer2Start(void));
void Timer2Stop(void));

void Timer3Set(unsigned long us, void (*f)());
void Timer3Start(void));
void Timer3Stop(void));

#endif