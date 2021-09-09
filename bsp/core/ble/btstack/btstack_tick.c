
#include	"Includes.h"

#define		CORE_CLK			    32000000ul      /* Set Processor frequency */
#define     TICKS_PER_SEC           100ul           /* Set the number of ticks in one second  */
#define     TICK_PERIOD_IN_MS       (1000ul/TICKS_PER_SEC)


static void dummy_handler(void){};
static void (*tick_handler)(void) = &dummy_handler;


void hal_tick_init(void)
{
    SysTick_Config(CORE_CLK/TICKS_PER_SEC);
}

int  hal_tick_get_tick_period_in_ms(void){
    return  TICK_PERIOD_IN_MS;
}

void hal_tick_set_handler(void (*handler)(void)){
    if (handler == 0){
        tick_handler = &dummy_handler;
        return;
    }
    tick_handler = handler;
}

void ble_tick_handler(void){
	(*tick_handler)();
}

