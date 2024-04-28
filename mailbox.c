/*Create a multitasking program on Keil MCB 1800 evaluation board to demonstrate IPC using mailbox. Create a task to read temperature value and send to another task executing concurrently through mailbox. Display temperature value on GLCD. Synchronize the execution of tasks. Use APIs of RL-ARM real time kernel. The following BSPs of sensor & GLCD can be used.
BSP of temperature sensor: TH_LM75.h and corresponding TH_LM75.c
 *-----------------------------------------------------------------------------
 *    TH_Init:  Initialize thermometer
 *  Parameter:  (none)
 *  Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
   //uint32_t TH_Init (void)
//*-----------------------------------------------------------------------------
 //*      TH_GetTemp:  Get temperature data from thermometer
 //*  Parameter:  (none)
 //*  Return:     digital equivalent of temperature 
 //*----------------------------------------------------------------------------*/
  //uint16_t TH_GetTemp (void) 


//Header files:  RTL.h (RL RTX specific header file) and LPC18xx.h (LPC 1857 definitions)

//BSP of GLCD: GLCD. h & Corresponding GLCD_LPC18xx.c

/***********************************************************
* Initialize the Graphic LCD controller                                        *
*   Parameter:                                                                 *
*   Return:                                                                    *
************************************************************
extern void GLCD_Init           (void);
	
/************************************************************
* Set foreground color                                                         *
*   Parameter:      color:    foreground color                                 *
*   Return:                                                                    *
************************************************************
extern void GLCD_SetTextColor   (unsigned short color);

/****************************************************
* Set background color                                                         *
*   Parameter:      color:    background color                                 *
*   Return:                                                                    *
***************************************************************/
//extern void GLCD_SetBackColor   (unsigned short color);

/********************************************************
* Clear display                                                                *
*   Parameter:      color:    display clearing color                           *
*   Return:                                                                    *
*********************************************************
extern void GLCD_Clear          (unsigned short color);

/**********************************************************
* Display string on given line                                                 *
*   Parameter:      ln:       line number                                      *
*                   col:      column number                                    *
*                   fi:       font index (0 = 6x8, 1 = 16x24)                  *
*                   s:        pointer to string                                *
*   Return:                                                                    *
**********************************************************
extern void GLCD_DisplayString  (unsigned int ln, unsigned int col, unsigned char fi, unsigned char *s); */

//main code starts here
#include <RTL.h>
#include <LPC18xx.h>                    /* LPC18xx Definitions               */
#include "GLCD.h"
#include "TH_75.h"
#include <stdio.h>

OS_TID t_temp;                        /* assigned task id of task: phase_a */
OS_TID t_lcd;                           /* assigned task id of task: lcd     */
OS_MBX mailbox;
os_mbx_declare(mailbox,2);   // Macro declare mailbox
int16_t  tempVal;

/*----------------------------------------------------------------------------
 *        Task 1 'ADCread': Phase A output
 *---------------------------------------------------------------------------*/
__task void TEMPread (void) {
	char StringBufSend[20];
  for (;;) {              
         tempVal = TH_GetTemp();
         strcpy(StringBufSend," ");
         sprintf(StringBufSend, "Digital Value:%d", tempVal);
         os_mbx_send (mailbox, (void *)StringBufSend, 100);
         os_dly_wait (200);  
  }
}

/*----------------------------------------------------------------------------
 *        Task 2 'lcd': LCD Control task
 *---------------------------------------------------------------------------*/
__task void lcd (void) {
   void *StringBufRecv;;
  for (;;) {
    os_mbx_wait (mailbox,&StringBufRecv,0xFFFF);
    GLCD_SetBackColor(Blue);                           /* Set the Text Color */
    GLCD_SetTextColor(White);                          /* Set the Text Color */
    GLCD_DisplayString (4, 1, 1, (unsigned char *)StringBufRecv);
    os_dly_wait (400); 
  }
}
/*----------------------------------------------------------------------------
 *        Task 3 'init': Initialize
 *---------------------------------------------------------------------------*/
__task void init (void) {
    t_temp = os_tsk_create (TEMPread, 0);  /* start task phaseA                */
    t_lcd    = os_tsk_create (lcd, 0);     /* start task lcd                   */
    os_tsk_delete_self ();
}
/*----------------------------------------------------------------------------
 *        Main: Initialize and start RTX Kernel
 *---------------------------------------------------------------------------*/
int main (void) {

  TH_Init();  
  GLCD_Init();                              /* Initialize the GLCD           */
  GLCD_Clear(White);                        /* Clear the GLCD                */
  os_mbx_init(&mailbox, 100);
  os_sys_init(init);                        /* Initialize RTX and start init */
}
