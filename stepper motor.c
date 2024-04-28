/*----------------------------------------------------------------------------
 *      RL-ARM - RTX
 *----------------------------------------------------------------------------
 *      Name:    BLINKY.C
 *      Purpose: RTX example program
 *----------------------------------------------------------------------------
 *      Edited by MGR,ECE,RVCE.
 ---------------------------------------------------------------------------/

#include <RTL.h>
#include <LPC18xx.h>                    /* LPC18xx Definitions               */
#include "GLCD.h"
#include "LED.h"
#include "STEP.h"

OS_TID t_phaseA;                        /* assigned task id of task: phase_a */
OS_TID t_lcd;                           /* assigned task id of task: lcd     */
OS_TID t_step;

#define LED_A      2
#define __FI       1                    /* Font index 16x24                  */

/*----------------------------------------------------------------------------
 *        Switch LED on
 ---------------------------------------------------------------------------/
void Switch_On  (unsigned char led) {
  LED_On (led);
  
}

/*----------------------------------------------------------------------------
 *        Switch LED off
 ---------------------------------------------------------------------------/
void Switch_Off (unsigned char led) {
  LED_Off (led);
  
}

/*----------------------------------------------------------------------------
 *        Task 1 'phaseA': Phase A output
 ---------------------------------------------------------------------------/
__task void phaseA (void) {
  for (;;) {
   
    Switch_On (LED_A);
		os_dly_wait (100);
    Switch_Off(LED_A);
		os_dly_wait (100);
  }
}
/*----------------------------------------------------------------------------
 *        Task 2 'lcd': LCD Control task
 ---------------------------------------------------------------------------/
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
/*********************************
  stepper task
**********************************/
__task void stepper(void)
{
	for(;;){
	STEP_On(0);        
	STEP_Off(3);
	
	os_dly_wait(10);

	STEP_On(1);
	STEP_Off(0);
	
	os_dly_wait(10);
	
	STEP_On(2);
	STEP_Off(1);
	
	os_dly_wait(10);
	
	STEP_On(3);
	STEP_Off(2);
	
	os_dly_wait(10);
	}
}
	

/*----------------------------------------------------------------------------
 *        Task  'init': Initialize
 ---------------------------------------------------------------------------/
__task void init (void) {
  t_phaseA = os_tsk_create (phaseA, 0);  /* start task phaseA                */              
  t_lcd    = os_tsk_create (lcd, 0);     /* start task lcd                   */
	t_step=os_tsk_create(stepper,0);
  os_tsk_delete_self ();
}
/*----------------------------------------------------------------------------
 *        Main: Initialize and start RTX Kernel
 ---------------------------------------------------------------------------/
int main (void) {
  LED_Init();                               /* Initialize the LEDs           */  
  GLCD_Init();                              /* Initialize the GLCD           */
	STEP_Init();
  GLCD_Clear(White);                        /* Clear the GLCD                */
  os_sys_init(init);                        /* Initialize RTX and start init */
}

/*----------------------------------------------------------------------------
 * end of file
 ---------------------------------------------------------------------------/
