

Stack_Size      EQU     0x00000800

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB

; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
; ToDo:  Add here the vectors for the device specific external interrupts handler
                DCD  	BLE_Handler
                DCD  	DMAS_Handler
                DCD  	CPR_Handler
                DCD  	GPIO_Handler
                DCD  	RTC_Handler
                DCD  	TIMER0_Handler
                DCD  	TIMER1_Handler
                DCD  	TIMER2_Handler
                DCD  	TIMER3_Handler
                DCD  	WDT_Handler
                DCD  	I2C_Handler
                DCD  	UART0_Handler
                DCD  	UART1_Handler
                DCD  	SPI0_Handler
                DCD  	SPI1_Handler
                DCD  	KBS_Handler
                DCD  	QDEC_Handler
                DCD  	GADC_Handler
				DCD		SIM_Handler
				DCD		AES_Handler
       	     	              	              	              	              	  
__Vectors_End

__Vectors_Size 	EQU 	__Vectors_End - __Vectors



                AREA    |.text|, CODE, READONLY
; Reset Handler

Reset_Handler   PROC

                EXPORT  Reset_Handler           [WEAK]

                IMPORT  SystemInit        
                IMPORT  __main
          		
                LDR     r0, =0x4000013C         ; remap
                LDR     r1, =0x10000001
                STR     r1, [r0] 



                LDR     R0, =SystemInit
                BLX     R0
                
                LDR     R0, =__main
                BX      R0
        
                ENDP

; Dummy Exception Handlers (infinite loops which can be modified)
NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP


HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP

Default_Handler PROC
                EXPORT  	BLE_Handler          	    [WEAK]
                EXPORT  	DMAS_Handler    	        [WEAK]	   
                EXPORT  	CPR_Handler		    	    [WEAK]
                EXPORT  	GPIO_Handler	    	    [WEAK]
                EXPORT  	RTC_Handler	    	        [WEAK]
                EXPORT  	TIMER0_Handler 		        [WEAK]
                EXPORT  	TIMER1_Handler 		        [WEAK]
                EXPORT  	TIMER2_Handler 		        [WEAK]
                EXPORT  	TIMER3_Handler 		        [WEAK]
                EXPORT  	WDT_Handler	                [WEAK]
                EXPORT  	I2C_Handler    		        [WEAK]
                EXPORT  	UART0_Handler	            [WEAK]
                EXPORT  	UART1_Handler               [WEAK]
                EXPORT  	SPI0_Handler                [WEAK]
                EXPORT  	SPI1_Handler                [WEAK]
                EXPORT  	KBS_Handler                 [WEAK]
                EXPORT  	QDEC_Handler     	    	[WEAK]
                EXPORT  	GADC_Handler    	        [WEAK]
				EXPORT		SIM_Handler					[WEAK]
				EXPORT		AES_Handler					[WEAK]
                EXPORT		PendSV_Handler      	    [WEAK]
		        EXPORT		SysTick_Handler	            [WEAK]	
PendSV_Handler
SysTick_Handler
BLE_Handler
DMAS_Handler    	
CPR_Handler
GPIO_Handler
RTC_Handler
TIMER0_Handler
TIMER1_Handler
TIMER2_Handler
TIMER3_Handler
WDT_Handler
I2C_Handler
UART0_Handler
UART1_Handler
SPI0_Handler
SPI1_Handler
KBS_Handler
QDEC_Handler
GADC_Handler
SIM_Handler
AES_Handler
                B       .
                ENDP


                ALIGN

; User Initial Stack & Heap

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, = (Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                END

