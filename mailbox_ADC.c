/*----------------------------------------------------------------------------
 *      RL-ARM - RTX
 *----------------------------------------------------------------------------
 *      Name:    BLINKY.C
 *      Purpose: RTX example program
 *----------------------------------------------------------------------------
 *     Created by MGR,ECE,RVCE
 ---------------------------------------------------------------------------/

#include <RTL.h>
#include <LPC18xx.h>                    /* LPC18xx Definitions               */
#include "GLCD.h"
#include "ADC.h"
#include <stdio.h>
#include <string.h>



OS_TID t_adc;                        /* assigned task id of task: phase_a */
OS_TID t_lcd;                           /* assigned task id of task: lcd     */


OS_MBX mailbox;

os_mbx_declare(mailbox,2);   // Macro declare mailbox


#define __FI       1                    /* Font index 16x24                  */

int32_t  adcVal;

/*----------------------------------------------------------------------------
 *        Task 1 'ADCread': ADC Read digital data 
 ---------------------------------------------------------------------------/
__task void ADCread (void) {
	char StringBufSend[20];
  for (;;) {
        
         ADC_StartCnv();                  
         adcVal = ADC_GetVal();
		     strcpy(StringBufSend,"                ");
		     sprintf(StringBufSend, "Digital Value:%d", adcVal);
		     os_mbx_send (mailbox, (void *)StringBufSend, 100);
		     //os_dly_wait (50);  
  }
}
	

/*----------------------------------------------------------------------------
 *        Task 2 'lcd': LCD Control task
 ---------------------------------------------------------------------------/
__task void lcd (void) {
   void *StringBufRecv;;
  for (;;) {
    os_mbx_wait (mailbox,&StringBufRecv,0xFFFF);
    GLCD_SetBackColor(Blue);                           /* Set the Text Color */
    GLCD_SetTextColor(White);                          /* Set the Text Color */
		 GLCD_DisplayString (4, 1, 1, "                       ");
    GLCD_DisplayString (4, 1, 1, (unsigned char *)StringBufRecv);
    //os_dly_wait (50); 
  }
}

/*----------------------------------------------------------------------------
 *        Task 3 'init': Initialize
 ---------------------------------------------------------------------------/
__task void init (void) {

  
  t_adc = os_tsk_create (ADCread, 0);  /* start task phaseA                */
  t_lcd    = os_tsk_create (lcd, 0);     /* start task lcd                   */
  os_tsk_delete_self ();
}

/*----------------------------------------------------------------------------
 *        Main: Initialize and start RTX Kernel
 ---------------------------------------------------------------------------/
int main (void) {

  ADC_Init();  
  GLCD_Init();                              /* Initialize the GLCD           */
  GLCD_Clear(White);                        /* Clear the GLCD                */
	GLCD_SetBackColor(Blue);                           /* Set the Text Color */
  GLCD_SetTextColor(White);                          /* Set the Text Color */
	GLCD_DisplayString (0, 0, 1, "       ECE  RVCE        ");
  GLCD_DisplayString (1, 0, 1, "      Mailbox Demo      ");
  os_mbx_init(&mailbox, 100);
  os_sys_init(init);                        /* Initialize RTX and start init */
}

/*----------------------------------------------------------------------------
 * end of file
 ---------------------------------------------------------------------------/
