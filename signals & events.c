/*----------------------------------------------------------------------------
 *      RL-ARM - RTX
 *----------------------------------------------------------------------------
 *      Name:    BLINKY.C
 *      Purpose: RTX example program
 *----------------------------------------------------------------------------
 *      Edited by MGR,ECE,RVCE
 *      Demonstration of singals events 
 ---------------------------------------------------------------------------*/

#include <RTL.h>
#include <LPC18xx.h>                    /* LPC18xx Definitions               */
#include "GLCD.h"
#include "LED.h"

OS_TID t_phaseA;                        /* assigned task id of task: phase_a */
OS_TID t_phaseB;                        /* assigned task id of task: phase_b */
OS_TID t_phaseC;                        /* assigned task id of task: phase_c */
OS_TID t_phaseD;                        /* assigned task id of task: phase_d */                        
OS_TID t_lcd;                           /* assigned task id of task: lcd     */

OS_MUT mut_GLCD;                        /* Mutex to controll GLCD access     */

#define LED_A      2
#define LED_B      3
#define LED_C      4
#define LED_D      5

#define __FI       1                    /* Font index 16x24                  */

/*----------------------------------------------------------------------------
 *        Switch LED on
 ---------------------------------------------------------------------------*/
void Switch_On  (unsigned char led) {

  LED_On (led);
  os_mut_wait(&mut_GLCD, 0xffff);
  GLCD_SetBackColor(White);                         /* Set the Back Color */
  GLCD_SetTextColor(Green);                         /* Set the Text Color */
  GLCD_DisplayChar(4, 5+led, __FI, 0x80+1);         /* Circle Full        */
  os_mut_release(&mut_GLCD);
}

/*----------------------------------------------------------------------------
 *        Switch LED off
 ---------------------------------------------------------------------------*/
void Switch_Off (unsigned char led) {

  LED_Off (led);
  os_mut_wait(&mut_GLCD, 0xffff);
  GLCD_SetBackColor(White);                         /* Set the Back Color */
  GLCD_SetTextColor(Green);                         /* Set the Text Color */
  GLCD_DisplayChar(4, 5+led, __FI, 0x80+0);         /* Circle Empty       */
  os_mut_release(&mut_GLCD);
}
/*----------------------------------------------------------------------------
 *        Function 'signal_func' called from multiple tasks
 ---------------------------------------------------------------------------*/
void signal_func (OS_TID task)  {
  os_dly_wait (100); 
	os_evt_set (0x0001, task);             /* send event to task 'task'        */
  os_dly_wait (100);                      /* delay 50 clock ticks             */
}
/*----------------------------------------------------------------------------
 *        Task 1 'phaseA': Phase A output
 ---------------------------------------------------------------------------*/
__task void phaseA (void) {
  for (;;) {
    os_evt_wait_and (0x0001, 0xffff);    /* wait for an event flag 0x0001    */
    Switch_On (LED_A);
    signal_func (t_phaseB);              /* call common signal function      */
    Switch_Off(LED_A);
  }
}
/*----------------------------------------------------------------------------
 *        Task 2 'phaseB': Phase B output
 ---------------------------------------------------------------------------*/
__task void phaseB (void) {
  for (;;) {
    os_evt_wait_and (0x0001, 0xffff);    /* wait for an event flag 0x0001    */
    Switch_On (LED_B);
    signal_func (t_phaseC);              /* call common signal function      */
    Switch_Off(LED_B);
  }
}
/*----------------------------------------------------------------------------
 *        Task 3 'phaseC': Phase C output
 ---------------------------------------------------------------------------*/
__task void phaseC (void) {
  for (;;) {
    os_evt_wait_and (0x0001, 0xffff);    /* wait for an event flag 0x0001    */
    Switch_On (LED_C);
    signal_func (t_phaseD);              /* call common signal function      */
    Switch_Off(LED_C);
  }
}
/*----------------------------------------------------------------------------
 *        Task 4 'phaseD': Phase D output
 ---------------------------------------------------------------------------*/
__task void phaseD (void) {
  for (;;) {
    os_evt_wait_and (0x0001, 0xffff);    /* wait for an event flag 0x0001    */
    Switch_On (LED_D);
    signal_func (t_phaseA);              /* call common signal function      */
    Switch_Off(LED_D);
  }
}


/*----------------------------------------------------------------------------
 *        Task 5 'lcd': LCD Control task
 ---------------------------------------------------------------------------*/
__task void lcd (void) {

  for (;;) {
    os_mut_wait(&mut_GLCD, 0xffff);
    GLCD_SetBackColor(Blue);                           /* Set the Text Color */
    GLCD_SetTextColor(White);                          /* Set the Text Color */
    GLCD_DisplayString(0, 0, __FI, "       ECE          ");
    GLCD_DisplayString(1, 0, __FI, "       RVCE         ");
    GLCD_DisplayString(2, 0, __FI, "      Signals       ");
    os_mut_release(&mut_GLCD);
    os_dly_wait (100);

    os_mut_wait(&mut_GLCD, 0xffff);
    GLCD_SetBackColor(Blue);                           /* Set the Text Color */
    GLCD_SetTextColor(Red);                            /* Set the Text Color */
    GLCD_DisplayString(0, 0, __FI, "       ECE          ");
    GLCD_DisplayString(1, 0, __FI, "       RVCE         ");
    GLCD_DisplayString(2, 0, __FI, "       Events       ");
    os_mut_release(&mut_GLCD);
    os_dly_wait (100);
		
  }
}

/*----------------------------------------------------------------------------
 *         'init': Initialize
 ---------------------------------------------------------------------------*/
__task void init (void) {

  os_mut_init(&mut_GLCD);

  t_phaseA = os_tsk_create (phaseA, 0);  /* start task phaseA                */
  t_phaseB = os_tsk_create (phaseB, 0);  /* start task phaseB                */
  t_phaseC = os_tsk_create (phaseC, 0);  /* start task phaseC                */
  t_phaseD = os_tsk_create (phaseD, 0);  /* start task phaseD                */
  t_lcd    = os_tsk_create (lcd, 0);     /* start task lcd                   */
  os_evt_set (0x0001, t_phaseA);         /* send signal event to task phaseA */
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
