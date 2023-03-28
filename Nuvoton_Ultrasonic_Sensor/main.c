/**************************************************************************//**
 * @file     main.c
 * @version  V0.10
 * @brief    Show the usage of GPIO interrupt function.
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2019 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "NuMicro.h"


#define EchoPin PB11
#define TrigPin PB10

/**
 * @brief       GPIO PB IRQ
 *
 * @param       None
 *
 * @return      None
 *
 * @details     The PB default IRQ, declared in startup_M251.s.


*/

signed int  OlculenDeger=0;
 void TMR1_IRQHandler(void)
{
	
	TIMER_ClearIntFlag(TIMER1);  // Timer 1 kesme bayragi temizleniyor
}
 
 
void GPB_IRQHandler(void)
{
    /* To check if PB.2 interrupt occurred */
    if (GPIO_GET_INT_FLAG(PB, BIT11))
    {
        GPIO_CLR_INT_FLAG(PB, BIT11);
       if (EchoPin==1) {
				 TIMER_Start(TIMER1);
				  TIMER_ResetCounter(TIMER1);
					 }
				 else {
					
					 OlculenDeger=TIMER_GetCounter(TIMER1);
					 TIMER_Stop(TIMER1);
					 OlculenDeger=(OlculenDeger/465) / 2; // Buradaki 465 degeri ses hizinin ortak bölenidir. yaptigim testlerde bu deger ile gerçege en yakin ölçümleri vermektedir.						 
				  	printf("Mesafe %d cm\n", OlculenDeger);
			 }
		
			
			
			
			
    }
    else
    {
        uint32_t u32Status;
        u32Status = PB->INTSRC;
         PB->INTSRC = u32Status;
        
    }
}


void SYS_Init(void)
{

   
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));
    CLK_EnableModuleClock(UART0_MODULE);
    CLK_EnableModuleClock(GPB_MODULE);
      CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, CLK_CLKDIV0_UART0(1));
   

/* Enable IP clock */
  
    CLK_EnableModuleClock(TMR1_MODULE);






  Uart0DefaultMPF();

}


void UART0_Init()
{
 
    SYS_ResetModule(UART0_RST);
    UART_Open(UART0, 115200);
}

/*---------------------------------------------------------------------------------------------------------*/
/* MAIN function                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
int main(void)
{
  
    SYS_UnlockReg();
    SYS_Init();
    SYS_LockReg();
    UART0_Init();
    printf("EchoPin PB.11 TrigPin PB10\n");

    /* Configure PB.2 as Input mode and enable interrupt by rising edge trigger */
    GPIO_SetMode(PB, BIT11, GPIO_MODE_INPUT);
	  GPIO_SetMode(PB, BIT10, GPIO_MODE_OUTPUT);
    GPIO_EnableInt(PB, 11, GPIO_INT_BOTH_EDGE);
    NVIC_EnableIRQ(GPB_IRQn);
    TrigPin=0;
    GPIO_SET_DEBOUNCE_TIME(GPIO_DBCTL_DBCLKSRC_LIRC, GPIO_DBCTL_DBCLKSEL_1024);
    TIMER_Open(TIMER1, TIMER_PERIODIC_MODE, 1);  // Timer 1 yapilandirmasi
    TIMER_EnableInt(TIMER1);  // Timer 1 kesmelerini etkinlestirin
    NVIC_EnableIRQ(TMR1_IRQn);  // Timer 1 kesmesi için NVIC etkinlestirin
  



    /* Waiting for interrupts */
    while (1){
		 
		
	 	CLK_SysTickDelay(10);		
	  TrigPin=0;
		CLK_SysTickDelay(10);
    TrigPin=1;
    CLK_SysTickDelay(10);
    TrigPin=0;				
		
			 CLK_SysTickDelay(1000000);

			
		}
}

/*** (C) COPYRIGHT 2019 Nuvoton Technology Corp. ***/
