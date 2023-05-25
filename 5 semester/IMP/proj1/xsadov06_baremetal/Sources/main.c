/* ###################################################################
**     Filename    : main.c
**     Project     : xsadov06_baremetal
**     Processor   : MK60DN512VLQ10
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2022-12-05, 21:49, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "Bit1.h"
#include "BitIoLdd1.h"
#include "Bit2.h"
#include "BitIoLdd2.h"
#include "Bit3.h"
#include "Bit3.h"
#include "BitIoLdd4.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */
#include "PORT_PDD.h"
/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
static void pause(unsigned int to)
{
	for (unsigned int i = 0; i<to; i++);
}
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */
  PORT_PDD_SetPinPullSelect(PORTE_BASE_PTR, 27, PORT_PDD_PULL_UP);
  PORT_PDD_SetPinPullEnable(PORTE_BASE_PTR, 27, PORT_PDD_PULL_ENABLE);
  int led=-1;
  for(;;)
  {
	  if(led==9)
	  {
		  pause(100000);
		  Bit1_PutVal(FALSE);
		  pause(100000);
		  Bit1_PutVal(TRUE);
	  }
	  else
	  {
	  	  Bit1_PutVal(TRUE);
	  }
	  if(led==12)
	  {
		  pause(100000);
	   	  Bit3_PutVal(FALSE);
	   	  pause(100000);
	   	  Bit3_PutVal(TRUE);
	  }
	  else
	  {
	  	  Bit3_PutVal(TRUE);
	  }
	  if(Bit2_GetVal() == FALSE)
	  {
		  if (led==9) led = 12;
		      else led=9;
		  pause(500000);
	  }
  }
  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
