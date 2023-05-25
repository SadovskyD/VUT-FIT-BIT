/* ###################################################################
**     Filename    : main.c
**     Project     : test
**     Processor   : MK60DN512VLQ10
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2022-12-04, 03:06, # CodeGen: 0
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
#include "FRTOS1.h"
#include "MCUC1.h"
#include "UTIL1.h"
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
#include "semphr.h"
/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int led=12;
SemaphoreHandle_t xSemaphore;
static void ledD9_task(void *pvParameters) {
  (void)pvParameters; /* parameter not used */
  for(;;)
  {
	  xSemaphoreTake( xSemaphore, portMAX_DELAY );
	  if(led==9)
	  {
		  FRTOS1_vTaskDelay(100);
		  Bit1_PutVal(FALSE);
		  FRTOS1_vTaskDelay(100);
		  Bit1_PutVal(TRUE);
	  }
	  else
	  {
		  Bit1_PutVal(TRUE);
	  }
	  xSemaphoreGive( xSemaphore );
  }
}
static void ledD12_task(void *pvParameters) {
  (void)pvParameters; /* parameter not used */
  for(;;)
    {
	  xSemaphoreTake( xSemaphore, portMAX_DELAY );
  	  if(led==12)
  	  {
  		  FRTOS1_vTaskDelay(100);
  		  Bit3_PutVal(FALSE);
  		  FRTOS1_vTaskDelay(100);
  		  Bit3_PutVal(TRUE);
  	  }
  	  else
  	  {
 		  Bit3_PutVal(TRUE);
  	  }
  	  xSemaphoreGive( xSemaphore );
    }
}
static void sw2_task(void *pvParameters) {
  (void)pvParameters; /* parameter not used */
  for(;;) {
    if(Bit2_GetVal() == FALSE) {
    	if (led==9) led = 12;
    	else led=9;
    	FRTOS1_vTaskDelay(30);
	}
  }
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
  //port E pin 10 pull up resistor initialization
  PORT_PDD_SetPinPullSelect(PORTE_BASE_PTR, 27, PORT_PDD_PULL_UP);
  PORT_PDD_SetPinPullEnable(PORTE_BASE_PTR, 27, PORT_PDD_PULL_ENABLE);

  xSemaphore = xSemaphoreCreateMutex();

  //creating a task for the OS
  FRTOS1_xTaskCreate(ledD9_task, "LED_9", configMINIMAL_STACK_SIZE, (void*)NULL, 1, (void*)NULL);
  FRTOS1_xTaskCreate(ledD12_task, "LED_12", configMINIMAL_STACK_SIZE, (void*)NULL, 1, (void*)NULL);
  FRTOS1_xTaskCreate(sw2_task, "SW_2", configMINIMAL_STACK_SIZE, (void*)NULL, 1, (void*)NULL);

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
