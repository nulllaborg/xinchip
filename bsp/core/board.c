#include "stdio.h"
#include "board.h"

#define DEBUG_OUT_PORT 1  //- 0:uart0 , 1:uart1  

void board_init(void)
{
    #if (ENABLE_TIMER == 1)
    init_timer();
    #endif
    #if (ENABLE_GPIO == 1)
    init_gpio();
    #endif
	
    #if (ENABLE_I2C == 1)
    #endif  
	
    #if (ENABLE_GPADC == 1)
    init_adc();
    #endif
    
    #if (ENABL_UART0 == 1)
    #endif

    #if (ENABL_UART1 == 1)
    #endif
}
