#include "stdio.h"
#include "board.h"

#define DEBUG_OUT_PORT 1  //- 0:uart0 , 1:uart1  

void board_init(void)
{
    #if (XC_TIMER == 1)
    init_timer();
    #endif
    
    #if (XC_GPIO == 1)
    init_gpio();
    #endif
    
    #if (XC_GPADC == 1)
    init_adc();
    #endif
}
