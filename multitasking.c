//Multitasking
/*----------------------------------------------------------------------------
 *      RL-ARM - RTX
 *----------------------------------------------------------------------------
 *      Name:    BLINKY.C
 *      Purpose: RTX example program
 *----------------------------------------------------------------------------
 *      Edited by MGR,ECE,RVCE.
 *      Demostration: Idle tasks-Basis of LOW POWER RTX
 ---------------------------------------------------------------------------*/

//main code 
#include <RTL.h>
#include <LPC18xx.h>                    /* LPC18xx Definitions               */
#include "GLCD.h"
#include "LED.h"

OS_TID t_led;                           /* assigned task id of task: led      */
OS_TID t_lcd;                           /* assigned task id of task: lcd     */

#define __FI       1                    /* Font index 16x24                  */


/*----------------------------------------------------------------------------
 *        Task 1 'LED': LED control task
 ---------------------------------------------------------------------------*/
__task void led (void) {
  for (;;) {
   
    LED_On(0);
		os_dly_wait (100);
    LED_Off(0);
		os_dly_wait (100);
  }
}
/*----------------------------------------------------------------------------
 *        Task 2 'lcd': LCD Control task
 ---------------------------------------------------------------------------*/
__task void lcd (void) {

  for (;;) {
    
    GLCD_SetBackColor(Blue);                           /* Set the Text Color */
    GLCD_SetTextColor(White);                          /* Set the Text Color */
    GLCD_DisplayString(0, 0, __FI, "        ECE         ");
    GLCD_DisplayString(1, 0, __FI, "        RVCE        ");
    GLCD_DisplayString(2, 0, __FI, "       MGRJH        ");
     os_dly_wait (100);
    GLCD_SetBackColor(Blue);                           /* Set the Text Color */
    GLCD_SetTextColor(Red);                            /* Set the Text Color */
    GLCD_DisplayString(0, 0, __FI, "        ECE         ");
    GLCD_DisplayString(1, 0, __FI, "        RVCE        ");
    GLCD_DisplayString(2, 0, __FI, "       MGRJH        ");
		 os_dly_wait (100);
   
  }
}

/*----------------------------------------------------------------------------
 *        Task  'init': Initialize
 ---------------------------------------------------------------------------*/
__task void init (void) {
	
  t_led = os_tsk_create (led, 0);           /* start task led               */              
  t_lcd    = os_tsk_create (lcd, 0);       /* start task lcd  */ 
  os_tsk_delete_self ();
	
}
/*----------------------------------------------------------------------------
 *        Main: Initialize and start RTX Kernel
 ---------------------------------------------------------------------------*/
int main (void) {
  LED_Init();                               /* Initialize the LEDs           */  
  GLCD_Init();                              /* Initialize the GLCD           */
  GLCD_Clear(White);                        /* Clear the GLCD                */
  os_sys_init(init);                        /* Initialize RTX and start init */
}

/*----------------------------------------------------------------------------
 * end of file
 ---------------------------------------------------------------------------*/
